# 🦈 SHARK RAIDER

**Offensive Security Firmware for ESP32 CYD (Cheap Yellow Display)**

Version: 1.0.0

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
5. Tap "STOP" to end attack

### Rick Roll
1. Select "RICK ROLL" from main menu
2. Attack starts automatically
3. Broadcasts Rick Astley lyrics as SSIDs
4. Tap "STOP" to end attack

### Evil Portal
1. Select "EVIL PORTAL" from main menu
2. Portal starts with SSID "Free WiFi"
3. Victims connect and see login page
4. Monitor:
   - Connected clients
   - Captured credentials
5. Tap "STOP" to end portal
6. Credentials are logged to Serial (115200 baud)

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

## 🛠️ Customization

### Modify SSIDs
Edit `src/wifi_attack.cpp`:
```cpp
const char* FUNNY_SSIDS[] = {
    "Your Custom SSID 1",
    "Your Custom SSID 2",
    // Add more...
};
```

### Change Portal SSID
Edit `include/config.h`:
```cpp
#define PORTAL_AP_NAME "Your Portal Name"
```

### Adjust Colors
Edit `include/config.h`:
```cpp
#define BACKGROUND_COLOR TFT_BLACK
#define TEXT_COLOR TFT_WHITE
#define ACCENT_COLOR TFT_CYAN
#define TITLE_COLOR TFT_RED
```

### Touch Calibration
If touch is inaccurate, adjust in `src/display.cpp`:
```cpp
x = map(p.x, 200, 3700, 0, SCREEN_WIDTH);
y = map(p.y, 240, 3800, 0, SCREEN_HEIGHT);
```

---

## 🐛 Troubleshooting

### Display Not Working
- Check TFT_eSPI configuration matches your hardware
- Verify SPI pins in `platformio.ini`
- Try different rotation: `tft->setRotation(1)` to `(3)`

### Touch Not Responding
- Calibrate touch coordinates
- Check TOUCH_CS pin definition
- Verify touch controller type (XPT2046 vs GT911)

### WiFi Not Working
- Ensure ESP32 WiFi is enabled
- Check antenna connection (if external)
- Verify channel range (1-13 for most regions)

### Upload Fails
- Hold BOOT button during upload
- Try lower upload speed (115200)
- Check USB cable (must support data)
- Press RST button after upload

### Low Memory Warnings
- Reduce BEACON_RATE_LIMIT in config.h
- Disable debug output (set DEBUG_MODE to 0)
- Use shorter SSID lists

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

## 📝 License

This project is released under the **MIT License**.

```
MIT License

Copyright (c) 2026 SHARK RAIDER

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

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
