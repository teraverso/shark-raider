# 🚀 SHARK RAIDER - Quick Start Guide

## 📦 What You Need

1. **ESP32 CYD Board** (Cheap Yellow Display)
   - ESP32-2432S028R recommended
   - 2.4" or 3.5" display variants supported

2. **USB Cable** (data-capable)
   - MicroUSB or USB-C depending on your board

3. **Computer** with:
   - Visual Studio Code + PlatformIO, OR
   - Arduino IDE

---

## ⚡ 5-Minute Setup (PlatformIO)

### Step 1: Install Software
```bash
# Download VS Code from https://code.visualstudio.com/
# Install PlatformIO extension from VS Code marketplace
```

### Step 2: Open Project
```bash
# Extract SHARK_RAIDER.zip
# Open folder in VS Code
```

### Step 3: Connect & Upload
1. Connect ESP32 CYD via USB
2. Press `Ctrl+Alt+U` (or click Upload button)
3. Wait for upload to complete (~2 minutes)

### Step 4: Done!
- Device will reboot automatically
- You'll see the SHARK RAIDER splash screen
- Main menu appears after 2 seconds

---

## 🎮 Using the Firmware

### Main Menu Options

#### 1️⃣ BEACON SPAM
**What it does:** Floods WiFi with fake network names

**How to use:**
1. Tap "BEACON SPAM"
2. Choose "FUNNY SSIDs" or "RANDOM SSIDs"
3. Attack starts immediately
4. Watch packet count increase
5. Tap "STOP" to end

**What you'll see:**
- Networks like "FBI Surveillance Van", "Pretty Fly for a WiFi"
- Channel rotation every few seconds
- Real-time packet counter

---

#### 2️⃣ RICK ROLL
**What it does:** Broadcasts Rick Astley lyrics as WiFi networks

**How to use:**
1. Tap "RICK ROLL"
2. Attack starts automatically
3. Tap "STOP" to end

**What victims see:**
```
01 Never gonna give you up
02 Never gonna let you down
03 Never gonna run around
04 and desert you
05 Never gonna make you cry
06 Never gonna say goodbye
07 Never gonna tell a lie
08 and hurt you
```

---

#### 3️⃣ EVIL PORTAL
**What it does:** Creates fake WiFi hotspot to capture credentials

**How to use:**
1. Tap "EVIL PORTAL"
2. Portal starts with SSID "Free WiFi"
3. Victims connect and see login page
4. Monitor captured credentials
5. Tap "STOP" to end

**Monitoring:**
- **Clients:** Number of connected devices
- **Captured:** Number of credential submissions
- View credentials via Serial Monitor (115200 baud)

**What victims see:**
- Professional-looking WiFi login page
- Fields for SSID and password
- "Successfully Connected" message after submission

---

## 🔧 Troubleshooting

### ❌ Upload Failed
**Solution:**
1. Hold BOOT button on CYD
2. Click Upload in PlatformIO
3. Release BOOT when upload starts
4. Press RST button after upload completes

### ❌ Screen is Blank
**Solution:**
1. Press RST button on CYD
2. Check USB power (try different cable/port)
3. Verify board variant in platformio.ini

### ❌ Touch Not Working
**Solution:**
1. Tap harder (resistive touch requires pressure)
2. Calibrate touch in display.cpp if needed
3. Check if your board has capacitive touch

### ❌ WiFi Not Working
**Solution:**
1. Check antenna connection (if external)
2. Move away from strong WiFi sources
3. Try different channel in config.h

---

## 📊 Serial Monitor

View detailed logs and captured credentials:

1. **PlatformIO:**
   - Click "Serial Monitor" icon
   - Baud rate: 115200

2. **Arduino IDE:**
   - Tools → Serial Monitor
   - Set baud rate to 115200

**What you'll see:**
```
=================================
   SHARK RAIDER FIRMWARE
   Version: 1.0.0
=================================

[Display] Initializing...
[Display] Initialized successfully
[System] Ready!

[Portal] Credentials captured:
SSID: MyNetwork | Pass: password123
```

---

## ⚙️ Customization

### Change Portal Name
Edit `include/config.h`:
```cpp
#define PORTAL_AP_NAME "Your Name Here"
```

### Add Custom SSIDs
Edit `src/wifi_attack.cpp`:
```cpp
const char* FUNNY_SSIDS[] = {
    "Your Custom SSID",
    "Another SSID",
    // Add more...
};
```

### Adjust Colors
Edit `include/config.h`:
```cpp
#define ACCENT_COLOR TFT_CYAN  // Change to TFT_GREEN, TFT_BLUE, etc.
```

---

## 🎯 Tips & Tricks

### Maximizing Beacon Spam Impact
- Use in crowded areas for maximum confusion
- Funny SSIDs work best in public spaces
- Random SSIDs overwhelm WiFi scanners

### Evil Portal Best Practices
- Use believable SSID names ("Starbucks WiFi", "Airport WiFi")
- Position near target area
- Monitor Serial output for credentials

### Battery Life
- CYD draws ~200mA during attacks
- Use power bank for portable operation
- Beacon spam uses less power than Evil Portal

---

## ⚠️ Legal Reminder

**ONLY USE ON YOUR OWN NETWORKS OR WITH PERMISSION!**

- Unauthorized network access is illegal
- Beacon spam may violate FCC regulations
- Evil portal is for testing YOUR security only
- You are responsible for your actions

---

## 🆘 Need Help?

1. Check README.md for detailed documentation
2. Review troubleshooting section above
3. Verify hardware compatibility
4. Check Serial Monitor for error messages

---

## 🎉 You're Ready!

Have fun, stay legal, and remember:
**With great power comes great responsibility!** 🦈

---

**Quick Reference:**
- Main Menu: Touch screen to select
- Stop Attack: Tap "STOP" button
- Serial Monitor: 115200 baud
- Reset Device: Press RST button
