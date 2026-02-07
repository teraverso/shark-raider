#ifndef WIFI_ATTACK_H
#define WIFI_ATTACK_H

#include <Arduino.h>
#include <WiFi.h>
#include "esp_wifi.h"
#include "esp_wifi_types.h"
#include "config.h"

// WiFi packet structures
#define BEACON_PKT_LEN 128

// 802.11 Beacon frame structure
typedef struct {
    uint8_t frame_ctrl[2];
    uint8_t duration[2];
    uint8_t destination[6];
    uint8_t source[6];
    uint8_t bssid[6];
    uint8_t seq_ctrl[2];
    uint8_t timestamp[8];
    uint8_t beacon_interval[2];
    uint8_t capability[2];
    uint8_t tag_number;
    uint8_t tag_length;
    uint8_t ssid[32];
} __attribute__((packed)) BeaconFrame;

// WiFi Attack Manager class
class WiFiAttackManager {
public:
    WiFiAttackManager();
    ~WiFiAttackManager();
    
    // Initialization
    bool begin();
    void end();
    
    // Channel management
    void setChannel(uint8_t channel);
    uint8_t getChannel();
    void nextChannel();
    
    // Packet generation
    void generateRandomMac(uint8_t* mac);
    void prepareBeaconPacket(uint8_t* packet, const uint8_t* mac, 
                            const char* ssid, uint8_t ssidLen, 
                            uint8_t channel);
    
    // Beacon spam functions
    void beaconSpamFunny();
    void beaconSpamRandom();
    void beaconSpamRickRoll();
    void beaconSpamCustom(const char* ssid);
    
    // Packet sending
    bool sendBeaconPacket(uint8_t* packet, size_t len);
    
    // Status
    bool isActive();
    uint32_t getPacketCount();
    void resetPacketCount();
    
private:
    uint8_t currentChannel;
    bool active;
    uint32_t packetCount;
    uint8_t packetBuffer[BEACON_PKT_LEN];
    
    // Helper functions
    void initWiFi();
    void stopWiFi();
};

// Global instance
extern WiFiAttackManager wifiAttack;

// Rick Roll SSIDs
extern const char* RICKROLL_SSIDS[RICKROLL_SSID_COUNT];

// Funny SSIDs
extern const char* FUNNY_SSIDS[];
extern const int FUNNY_SSIDS_COUNT;

#endif // WIFI_ATTACK_H
