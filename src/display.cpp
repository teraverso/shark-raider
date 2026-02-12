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

#include "display.h"
#include "evil_portal.h"

// Global instance
DisplayManager display;

// Definições para o barramento SPI do Touch no CYD
#define CYD_TOUCH_CLK  25
#define CYD_TOUCH_MISO 39
#define CYD_TOUCH_MOSI 32
#define CYD_TOUCH_CS   33
#define CYD_TOUCH_IRQ  36

// Constructor
DisplayManager::DisplayManager() 
    : tft(nullptr), touch(nullptr), lastTouchTime(0) {
    initConsoleButtons();
}

// Destructor
DisplayManager::~DisplayManager() {
    if (tft) delete tft;
    if (touch) delete touch;
}

void DisplayManager::initConsoleButtons() {
    // Console BACK, SELECT, NEXT (Botões invisíveis visualmente, apenas texto)
    int bw = 100;
    int bh = 30;
    int by = 210; // Posição Y na parte inferior
    
    // Ordem visual: BACK, SELECT, NEXT
    consoleButtons[2] = {10, by, bw, bh, "BACK", BACKGROUND_COLOR, CONSOLE_COLOR, true, false};
    consoleButtons[1] = {110, by, bw, bh, "SELECT", BACKGROUND_COLOR, CONSOLE_COLOR, true, false};
    consoleButtons[0] = {210, by, bw, bh, "NEXT", BACKGROUND_COLOR, CONSOLE_COLOR, true, false};
}

// Initialize display
bool DisplayManager::begin() {
    pinMode(TFT_CS, OUTPUT);
    pinMode(CYD_TOUCH_CS, OUTPUT);
    digitalWrite(TFT_CS, HIGH);
    digitalWrite(CYD_TOUCH_CS, HIGH);
    
    tft = new TFT_eSPI();
    tft->init();
    tft->setRotation(1);
    tft->fillScreen(TFT_BLACK);
    
    SPI.begin(CYD_TOUCH_CLK, CYD_TOUCH_MISO, CYD_TOUCH_MOSI, CYD_TOUCH_CS);
    touch = new XPT2046_Touchscreen(CYD_TOUCH_CS, CYD_TOUCH_IRQ); 
    touch->begin();
    touch->setRotation(1);
    
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH); 
    
    clear();
    
    // Splash Screen
    tft->setTextColor(NEON_GREEN, TFT_BLACK);
    tft->setTextDatum(MC_DATUM);
    tft->setTextSize(1);
    tft->drawString("SHARK RAIDER", 160, 120, 2); 
    
    delay(2000);
    return true;
}

void DisplayManager::clear() {
    tft->fillScreen(TFT_BLACK);
}

void DisplayManager::drawTitle(const char* title) {
    tft->fillRect(0, 0, 320, 40, BACKGROUND_COLOR);
    tft->setTextColor(NEON_GREEN, BACKGROUND_COLOR);
    tft->setTextSize(1);
    tft->setTextDatum(TL_DATUM);
    
    // Título à esquerda
    tft->drawString(title, 10, 10, 2);
    
    // M e RSSI à direita
    char infoStr[32];
    float mem = evilPortal.getMemoryUsagePct();
    int8_t rssi = evilPortal.getRSSI();
    snprintf(infoStr, sizeof(infoStr), "M: %.1f%% RSSI: %d", mem, rssi);
    tft->drawRightString(infoStr, 310, 10, 2);
    
    // Linha horizontal verde
    tft->drawLine(0, 35, 320, 35, NEON_GREEN);
}

void DisplayManager::drawDisclaimer() {
    clear();
    tft->setTextColor(NEON_RED, TFT_BLACK);
    tft->setTextSize(2);
    tft->setTextDatum(MC_DATUM);
    tft->drawString("WARNING", 160, 30, 2);
    
    tft->setTextColor(TFT_WHITE, TFT_BLACK);
    tft->setTextSize(1);
    tft->setTextDatum(MC_DATUM);
    
    int yStart = 70;
    int lineSpacing = 15;
    tft->drawString("Shark Raider has an educational, training and", 160, yStart, 2);
    tft->drawString("research purpose in controlled environments.", 160, yStart + lineSpacing, 2);
    tft->drawString("Access to devices and data collection without", 160, yStart + (lineSpacing * 3), 2);
    tft->drawString("prior authorization constitute crimes provided", 160, yStart + (lineSpacing * 4), 2);
    tft->drawString("for by law. By using Shark Raider, you declare", 160, yStart + (lineSpacing * 5), 2);
    tft->drawString("knowledge of current legislation and take full", 160, yStart + (lineSpacing * 6), 2);
    tft->drawString("responsibility for your actions.", 160, yStart + (lineSpacing * 7), 2);
    
    tft->setTextColor(NEON_GREEN, TFT_BLACK);
    tft->setTextSize(1);
    tft->setTextDatum(MC_DATUM);
    tft->drawString("Click to accept and continue", 160, 220, 2);
}

void DisplayManager::drawConsole() {
    for (int i = 0; i < 3; i++) {
        tft->setTextColor(consoleButtons[i].textColor, BACKGROUND_COLOR);
        tft->setTextSize(2);
        tft->setTextDatum(MC_DATUM);
        tft->drawString(consoleButtons[i].label, consoleButtons[i].x + consoleButtons[i].w/2, consoleButtons[i].y + consoleButtons[i].h/2, 2);
    }
}

void DisplayManager::drawMainMenu(int selectedIndex) {
    clear();
    drawTitle("SELECT OPTION");
    const char* labels[] = {"BEACON SPAM", "RICK ROLL", "EVIL PORTAL"};
    for (int i = 0; i < 3; i++) {
        uint16_t y = 55 + (i * 45);
        bool isSelected = (i == selectedIndex);
        if (isSelected) {
            tft->fillRoundRect(30, y, 260, 35, 2, NEON_GREEN);
            tft->setTextColor(TFT_BLACK, NEON_GREEN);
        } else {
            tft->drawRoundRect(30, y, 260, 35, 2, NEON_GREEN);
            tft->setTextColor(NEON_GREEN, TFT_BLACK);
        }
        tft->setTextSize(2);
        tft->setTextDatum(MC_DATUM);
        tft->drawString(labels[i], 160, y + 17, 2);
    }
    drawConsole();
}

void DisplayManager::drawBeaconMenu(int selectedIndex) {
    clear();
    drawTitle("SELECT OPTION");
    const char* labels[] = {"FUNNY SSIDs", "RANDOM SSIDs", "CUSTOM SSIDs"};
    for (int i = 0; i < 3; i++) {
        uint16_t y = 55 + (i * 45);
        bool isSelected = (i == selectedIndex);
        if (isSelected) {
            tft->fillRoundRect(30, y, 260, 35, 2, NEON_GREEN);
            tft->setTextColor(TFT_BLACK, NEON_GREEN);
        } else {
            tft->drawRoundRect(30, y, 260, 35, 2, NEON_GREEN);
            tft->setTextColor(NEON_GREEN, TFT_BLACK);
        }
        tft->setTextSize(2);
        tft->setTextDatum(MC_DATUM);
        tft->drawString(labels[i], 160, y + 17, 2);
    }
    drawConsole();
}

void DisplayManager::drawAttackScreen(const char* attackName, uint32_t packets, uint8_t channel) {
    clear();
    drawTitle("ATTACK STATUS");
    tft->setTextDatum(TL_DATUM);
    tft->setTextColor(NEON_GREEN, TFT_BLACK);
    tft->setTextSize(2);
    char buf[32];
    sprintf(buf, "CHANNEL: %d", channel);
    tft->drawString(buf, 20, 60, 2);
    sprintf(buf, "PACKETS: %u", packets);
    tft->drawString(buf, 20, 95, 2);
    tft->setTextColor(NEON_RED, TFT_BLACK);
    tft->drawString("RUNNING...", 20, 140, 2);
    drawConsole();
}

void DisplayManager::drawPortalScreen(int clients, int captured) {
    // Mantido por compatibilidade
}

void DisplayManager::drawCredentialsScreen(const std::vector<String>& creds) {
    clear();
    drawTitle("CAPTURED DATA");
    tft->setTextSize(1);
    tft->setTextDatum(TL_DATUM);
    int y = 40;
    int count = 0;
    for (int i = 0; i < creds.size() && count < 10; i++) {
        String entry = creds[i];
        String user = ""; String pass = "";
        int sepIdx = entry.indexOf('|');
        if (sepIdx != -1) {
            user = entry.substring(6, sepIdx); 
            pass = entry.substring(sepIdx + 8); 
        } else { user = entry; }
        user.trim(); pass.trim();
        tft->setTextColor(NEON_GREEN, TFT_BLACK);
        tft->drawString("USER: ", 10, y, 2);
        tft->setTextColor(TFT_WHITE, TFT_BLACK);
        tft->drawString(user.c_str(), 60, y, 2);
        y += 15;
        tft->setTextColor(NEON_RED, TFT_BLACK);
        tft->drawString("PASS: ", 10, y, 2);
        tft->setTextColor(TFT_WHITE, TFT_BLACK);
        tft->drawString(pass.c_str(), 60, y, 2);
        y += 18; count++;
        if (count < 10 && count < creds.size()) {
            tft->drawLine(10, y-2, 310, y-2, 0x3186); 
        }
    }
    if (creds.empty()) {
        tft->setTextColor(TFT_WHITE, TFT_BLACK);
        tft->setTextDatum(MC_DATUM);
        tft->drawString("No data captured yet", 160, 120, 2);
    }
    drawConsole();
}

void DisplayManager::drawButton(const Button& btn) {
    if (!btn.visible) return;
}

int DisplayManager::checkConsolePress() {
    digitalWrite(TFT_CS, HIGH);
    if (!touch->touched()) return -1;
    TS_Point p = touch->getPoint();
    if (p.z < 200) return -1;
    int16_t tx = 320 - map(p.x, 200, 3800, 0, 320);
    int16_t ty = map(p.y, 200, 3800, 0, 240);
    for (int i = 0; i < 3; i++) {
        if (tx >= consoleButtons[i].x && tx <= consoleButtons[i].x + consoleButtons[i].w &&
            ty >= consoleButtons[i].y && ty <= consoleButtons[i].y + consoleButtons[i].h) {
            delay(100);
            return i; 
        }
    }
    return -1;
}

bool DisplayManager::isTouched() { return touch->touched(); }
void DisplayManager::getTouch(int16_t& x, int16_t& y) {
    TS_Point p = touch->getPoint();
    x = 320 - map(p.x, 200, 3800, 0, 320);
    y = map(p.y, 200, 3800, 0, 240);
}
void DisplayManager::printCentered(const char* text, int16_t y, uint8_t size) {
    tft->setTextDatum(MC_DATUM);
    tft->drawString(text, 160, y, size);
}
TFT_eSPI* DisplayManager::getTFT() { return tft; }

void DisplayManager::updatePortalDisplay(uint32_t uptime, int tot, int act, int cap, float sr, float ttc, float ttr, float ttl, float mem, int8_t rssi, bool running) {
    tft->fillRect(0, 0, 320, 205, BACKGROUND_COLOR);
    
    // --- Header ---
    char upStr[32];
    uint32_t h = uptime / 3600;
    uint32_t m = (uptime % 3600) / 60;
    uint32_t s = uptime % 60;
    snprintf(upStr, sizeof(upStr), "UP: %02u:%02u:%02u", h, m, s);
    drawTitle(upStr);
    
    // --- Main Grid (5 Lines x 2 Columns) ---
    int startY = 45;
    int rowHeight = 30;
    int col1X = 10;
    int col2X = 165;
    int valOffset = 60;
    
    tft->setTextSize(1);
    tft->setTextDatum(TL_DATUM);
    
    // L1: TOT | ACT
    tft->setTextColor(NEON_GREEN, BACKGROUND_COLOR);
    tft->drawString("TOT:", col1X, startY, 2);
    tft->setTextColor(TFT_WHITE, BACKGROUND_COLOR);
    tft->drawString(String(tot).c_str(), col1X + valOffset, startY, 2);
    
    tft->setTextColor(NEON_GREEN, BACKGROUND_COLOR);
    tft->drawString("ACT:", col2X, startY, 2);
    tft->setTextColor(TFT_WHITE, BACKGROUND_COLOR);
    tft->drawString(String(act).c_str(), col2X + valOffset, startY, 2);
    
    // L2: CAP | SR%
    startY += rowHeight;
    tft->setTextColor(NEON_GREEN, BACKGROUND_COLOR);
    tft->drawString("CAP:", col1X, startY, 2);
    tft->setTextColor(TFT_WHITE, BACKGROUND_COLOR);
    tft->drawString(String(cap).c_str(), col1X + valOffset, startY, 2);
    
    tft->setTextColor(NEON_GREEN, BACKGROUND_COLOR);
    tft->drawString("SR%:", col2X, startY, 2);
    tft->setTextColor(TFT_WHITE, BACKGROUND_COLOR);
    char srStr[16]; snprintf(srStr, sizeof(srStr), "%.1f%%", sr);
    tft->drawString(srStr, col2X + valOffset, startY, 2);
    
    // L3: TTC | TTR
    startY += rowHeight;
    tft->setTextColor(NEON_GREEN, BACKGROUND_COLOR);
    tft->drawString("TTC:", col1X, startY, 2);
    tft->setTextColor(TFT_WHITE, BACKGROUND_COLOR);
    char ttcStr[16]; snprintf(ttcStr, sizeof(ttcStr), "%.1fs", ttc);
    tft->drawString(ttcStr, col1X + valOffset, startY, 2);
    
    tft->setTextColor(NEON_GREEN, BACKGROUND_COLOR);
    tft->drawString("TTR:", col2X, startY, 2);
    tft->setTextColor(TFT_WHITE, BACKGROUND_COLOR);
    char ttrStr[16]; snprintf(ttrStr, sizeof(ttrStr), "%.1fs", ttr);
    tft->drawString(ttrStr, col2X + valOffset, startY, 2);
    
    // L4: TTL | MEM
    startY += rowHeight;
    tft->setTextColor(NEON_GREEN, BACKGROUND_COLOR);
    tft->drawString("TTL:", col1X, startY, 2);
    tft->setTextColor(TFT_WHITE, BACKGROUND_COLOR);
    char ttlStr[16]; snprintf(ttlStr, sizeof(ttlStr), "%.1fs", ttl);
    tft->drawString(ttlStr, col1X + valOffset, startY, 2);
    
    tft->setTextColor(NEON_GREEN, BACKGROUND_COLOR);
    tft->drawString("MEM:", col2X, startY, 2);
    tft->setTextColor(TFT_WHITE, BACKGROUND_COLOR);
    char memStr[16]; snprintf(memStr, sizeof(memStr), "%.1f%%", mem);
    tft->drawString(memStr, col2X + valOffset, startY, 2);
    
    // L5: Reserva de espaço (vazia)
    
    // Status Row
    tft->setTextDatum(BC_DATUM);
    tft->setTextColor(running ? NEON_GREEN : NEON_RED, BACKGROUND_COLOR);
    tft->drawString(running ? "STATUS: RUNNING" : "STATUS: IDLE", 160, 200, 2);
    
    drawConsole();
}
