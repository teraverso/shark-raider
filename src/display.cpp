#include "display.h"

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
    
    // Usamos cores transparentes/pretas para o fundo e vermelho para o texto
    consoleButtons[0] = {10, by, bw, bh, "BACK", BACKGROUND_COLOR, CONSOLE_COLOR, true, false};
    consoleButtons[1] = {110, by, bw, bh, "SELECT", BACKGROUND_COLOR, CONSOLE_COLOR, true, false};
    consoleButtons[2] = {210, by, bw, bh, "NEXT", BACKGROUND_COLOR, CONSOLE_COLOR, true, false};
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
    tft->setTextSize(3);
    tft->drawString("SHARK RAIDER", 160, 120, 2); // Centralizado com fonte maior
    
    delay(2000);
    return true;
}

void DisplayManager::clear() {
    tft->fillScreen(TFT_BLACK);
}

void DisplayManager::drawTitle(const char* title) {
    // Topo da tela: Fundo preto, Texto "SELECT OPITION" em verde neon
    tft->fillRect(0, 0, 320, 40, TFT_BLACK);
    tft->setTextColor(NEON_GREEN, TFT_BLACK);
    tft->setTextSize(2);
    tft->setTextDatum(MC_DATUM);
    tft->drawString("SELECT OPITION", 160, 20, 2);
}

void DisplayManager::drawConsole() {
    // Desenha apenas o texto dos botões do console (invisíveis visualmente)
    for (int i = 0; i < 3; i++) {
        tft->setTextColor(consoleButtons[i].textColor, BACKGROUND_COLOR);
        tft->setTextSize(2);
        tft->setTextDatum(MC_DATUM);
        tft->drawString(consoleButtons[i].label, consoleButtons[i].x + consoleButtons[i].w/2, consoleButtons[i].y + consoleButtons[i].h/2, 2);
    }
}

void DisplayManager::drawMainMenu(int selectedIndex) {
    clear();
    drawTitle("SELECT OPITION");
    
    const char* labels[] = {"BEACON SPAM", "RICK ROLL", "EVIL PORTAL"};
    
    for (int i = 0; i < 3; i++) {
        uint16_t y = 55 + (i * 45);
        bool isSelected = (i == selectedIndex);
        
        if (isSelected) {
            // Botão selecionado: Preenchido com verde neon, Letra preta (vazada)
            tft->fillRoundRect(30, y, 260, 35, 2, NEON_GREEN);
            tft->setTextColor(TFT_BLACK, NEON_GREEN);
        } else {
            // Botão não selecionado: Apenas moldura verde neon, Letra verde neon
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
    drawTitle("SELECT OPITION");
    
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
    clear();
    drawTitle("PORTAL STATUS");
    
    tft->setTextDatum(TL_DATUM);
    tft->setTextColor(NEON_GREEN, TFT_BLACK);
    tft->setTextSize(2);
    
    char buf[32];
    sprintf(buf, "CLIENTS: %d", clients);
    tft->drawString(buf, 20, 60, 2);
    
    sprintf(buf, "CAPTURED: %d", captured);
    tft->drawString(buf, 20, 95, 2);
    
    tft->setTextColor(NEON_RED, TFT_BLACK);
    tft->drawString("PORTAL ACTIVE", 20, 140, 2);
    
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
            return i; // 0: BACK, 1: SELECT, 2: NEXT
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
    tft->drawString(text, 160, y, 2);
}
TFT_eSPI* DisplayManager::getTFT() { return tft; }
