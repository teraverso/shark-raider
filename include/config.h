#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// Firmware version
#define FIRMWARE_VERSION "1.9.1"
#define FIRMWARE_NAME "SHARK RAIDER"

// Cores Personalizadas (Verde Neon)
#define NEON_GREEN 0x07E0  // RGB565: 0, 255, 0 (Puro) ou algo mais específico se necessário
#define NEON_RED   0xF800  // Vermelho puro

// Display settings (CYD 2.4")
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define BACKGROUND_COLOR TFT_BLACK
#define TEXT_COLOR NEON_GREEN
#define ACCENT_COLOR NEON_GREEN
#define TITLE_COLOR NEON_GREEN
#define CONSOLE_COLOR NEON_RED

// Touch calibration (CYD Standard)
#define TS_MINX 200
#define TS_MAXX 3700
#define TS_MINY 240
#define TS_MAXY 3800

// Touch settings
#define TOUCH_THRESHOLD 600
#define TOUCH_DEBOUNCE 300

// WiFi settings
#define DEFAULT_CHANNEL 6
#define MAX_CHANNEL 13
#define MIN_CHANNEL 1
#define WIFI_TX_POWER 20  // dBm

// Beacon spam settings
#define BEACON_PACKET_SIZE 128
#define BEACON_INTERVAL_MS 100
#define MAX_SSID_LENGTH 32
#define BEACON_RATE_LIMIT 10  // packets per iteration

// Evil portal settings
#define PORTAL_AP_NAME "Shark Raider"
#define PORTAL_PASSWORD "Kib5Kib5"  // Open network
#define PORTAL_CHANNEL 6
#define PORTAL_MAX_CONNECTIONS 10
#define PORTAL_TIMEOUT_MS 1800000  // 30 minutes
#define DNS_PORT 53
#define WEB_SERVER_PORT 80

// Memory management
#define MIN_FREE_HEAP 40960  // 40KB minimum
#define STACK_SIZE 4096
#define PACKET_BUFFER_SIZE 128

// Rick Roll SSIDs
#define RICKROLL_SSID_COUNT 8

// Debug settings
#define DEBUG_MODE 1
#if DEBUG_MODE
  #define DEBUG_PRINT(x) Serial.print(x)
  #define DEBUG_PRINTLN(x) Serial.println(x)
#else
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINTLN(x)
#endif

// Attack modes
enum AttackMode {
    MODE_IDLE = 0,
    MODE_BEACON_SPAM,
    MODE_RICK_ROLL,
    MODE_EVIL_PORTAL
};

// Beacon spam types
enum BeaconSpamType {
    BEACON_FUNNY = 0,
    BEACON_RANDOM,
    BEACON_RICKROLL,
    BEACON_CUSTOM
};

#endif // CONFIG_H
