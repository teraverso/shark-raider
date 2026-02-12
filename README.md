# 🦈 SHARK RAIDER

**Offensive Security Firmware for ESP32 CYD (Cheap Yellow Display)**

Version: 1.0.2

---

## ⚠️ DISCLAIMER

**This firmware is for EDUCATIONAL PURPOSES ONLY.**

- Use only on networks you own or have explicit permission to test
- Unauthorized access to computer networks is illegal
- The developers assume no liability for misuse of this software
- Always comply with local laws and regulations

---

## 📋 Features

### 🎯 Beacon Spam
- **Funny SSIDs**: Broadcast humorous network names
- **Random SSIDs**: Generate random network names
- Automatic channel rotation (1-13)
- Customizable packet rate

### 🎵 Rick Roll Attack
- Broadcast Rick Astley lyrics as SSIDs
- Sequential message display
- Multi-channel broadcast

### 🕸️ Evil Portal
- Captive portal for credential capture
- DNS hijacking
- Professional login page
- Real-time client monitoring
- Credential logging

---

## 🔧 Hardware Requirements

### Compatible Devices
- **ESP32-2432S028R** (2.4" Resistive Touch) - Recommended
- **ESP32-2432S024C** (2.4" Capacitive Touch)
- **ESP32-3248S035R** (3.5" Resistive Touch)
- **ESP32-3248S035C** (3.5" Capacitive Touch)

### Specifications
- ESP32 microcontroller
- ILI9341 or compatible TFT display (320x240)
- XPT2046 touch controller (resistive) or GT911/CST820 (capacitive)
- 4MB Flash minimum
- MicroUSB or USB-C for programming

---

## 🚀 Installation

### Method 1: PlatformIO (Recommended)

1. **Install PlatformIO**
   - Download and install [Visual Studio Code](https://code.visualstudio.com/)
   - Install PlatformIO extension from VS Code marketplace

2. **Open Project**
   ```bash
   cd SHARK_RAIDER
   code .
   ```

3. **Build and Upload**
   - Connect your CYD via USB
   - Press `Ctrl+Alt+U` or click "Upload" in PlatformIO
   - Wait for compilation and upload to complete

### Method 2: Arduino IDE

1. **Install Arduino IDE**
   - Download from [arduino.cc](https://www.arduino.cc/en/software)

2. **Install ESP32 Board Support**
   - File → Preferences
   - Add to Additional Board Manager URLs:
     ```
     https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
     ```
   - Tools → Board → Boards Manager
   - Search "ESP32" and install

3. **Install Libraries**
   - TFT_eSPI (2.5.43)
   - XPT2046_Touchscreen (1.4)
   - ESPAsyncWebServer (1.2.3)
   - AsyncTCP (1.1.1)
   - ArduinoJson (6.21.3)

4. **Configure TFT_eSPI**
   - Copy `platformio.ini` build flags to `User_Setup.h` in TFT_eSPI library

5. **Upload**
   - Tools → Board → ESP32 Dev Module
   - Tools → Upload Speed → 921600
   - Sketch → Upload

---

## 📖 Usage Guide

### Main Menu
After boot, you'll see the main menu with four options:

1. **BEACON SPAM** - WiFi beacon flooding attacks
2. **RICK ROLL** - Rick Astley lyric broadcast
3. **EVIL PORTAL** - Captive portal credential harvesting
4. **ABOUT** - Firmware information

### Beacon Spam
1. Select "BEACON SPAM" from main menu
2. Choose attack type:
   - **Funny SSIDs**: Pre-loaded humorous network names
   - **Random SSIDs**: Randomly generated names
3. Attack starts immediately
4. View real-time statistics:
   - Current channel
   - Packets sent
5. Tap "BACK" to end attack

### Rick Roll
1. Select "RICK ROLL" from main menu
2. Attack starts automatically
3. Broadcasts Rick Astley lyrics as SSIDs
4. Tap "BACK" to end attack

### Evil Portal
1. Select "EVIL PORTAL" from main menu
2. Portal starts with SSID "Free WiFi"
3. Victims connect and see login page
4. Monitor:
   - Connected clients
   - Captured credentials
5. Tap "BACK" to end portal
6. Tap "NEXT" to view credentials

---

## 🔍 Technical Details

### Architecture
```
SHARK_RAIDER/
├── include/
│   ├── config.h          # Global configuration
│   ├── display.h         # Display interface
│   ├── wifi_attack.h     # WiFi attack functions
│   └── evil_portal.h     # Evil portal implementation
├── src/
│   ├── main.cpp          # Main program logic
│   ├── display.cpp       # Display implementation
│   ├── wifi_attack.cpp   # WiFi attack implementation
│   └── evil_portal.cpp   # Evil portal implementation
├── lib/                  # External libraries
├── data/                 # Data files
└── platformio.ini        # PlatformIO configuration
```

### Memory Management
- Minimum free heap: 40KB
- Stack size per task: 4KB
- Packet buffer: 128 bytes (reusable)
- HTML buffer: Dynamic allocation
- PROGMEM for constant strings

### WiFi Configuration
- Mode: STA/AP/APSTA (dynamic)
- Channels: 1-13
- TX Power: 20 dBm (maximum)
- Protocol: 802.11 b/g/n

### Performance Metrics
- Beacon spam: ~100 packets/second
- Evil portal: 10 concurrent clients
- Memory usage: <200KB heap
- Boot time: <3 seconds

---


## 📚 Resources

### Inspiration & References
- [Bruce Predatory Firmware](https://github.com/BruceDevices/firmware)
- [ESP32 Marauder](https://github.com/justcallmekoko/ESP32Marauder)
- [CYD Marauder Port](https://github.com/Fr4nkFletcher/ESP32-Marauder-Cheap-Yellow-Display)

### Documentation
- [ESP32 Arduino Core](https://docs.espressif.com/projects/arduino-esp32/)
- [TFT_eSPI Library](https://github.com/Bodmer/TFT_eSPI)
- [ESP32 WiFi Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/network/esp_wifi.html)

### Community
- [ESP32 Reddit](https://www.reddit.com/r/esp32/)
- [PlatformIO Community](https://community.platformio.org/)

---

## 🙏 Acknowledgments

Special thanks to:
- **Bruce Firmware Team** - For the excellent Bruce Predatory Firmware
- **JustCallMeKoko** - For the ESP32 Marauder project
- **Fr4nkFletcher** - For the CYD Marauder port
- **ESP32 Community** - For continuous support and development

---

## 📧 Support

For issues, questions, or contributions:
- Open an issue on GitHub
- Check existing documentation
- Review troubleshooting section

---

**Remember: With great power comes great responsibility. Use ethically!** 🦈
