#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>
#include "config.h"

// Button structure
struct Button {
    int16_t x;
    int16_t y;
    int16_t w;
    int16_t h;
    String label;
    uint16_t color;
    uint16_t textColor;
    bool visible;
    bool selected;
};

// Display Manager class
class DisplayManager {
public:
    DisplayManager();
    ~DisplayManager();
    
    // Initialization
    bool begin();
    
    // Screen management
    void clear();
    void drawTitle(const char* title);
    void drawMainMenu(int selectedIndex = 0);
    void drawBeaconMenu(int selectedIndex = 0);
    void drawAttackScreen(const char* attackName, uint32_t packets, uint8_t channel);
    void drawPortalScreen(int clients, int captured);
    
    // Console de Navegação (Inferior)
    void drawConsole();
    int checkConsolePress(); // Retorna 0: BACK, 1: SELECT, 2: NEXT
    
    // Button management
    void drawButton(const Button& btn);
    
    // Touch handling
    bool isTouched();
    void getTouch(int16_t& x, int16_t& y);
    
    // Text and graphics
    void printCentered(const char* text, int16_t y, uint8_t size = 2);
    
    // Utility
    TFT_eSPI* getTFT();
    
private:
    TFT_eSPI* tft;
    XPT2046_Touchscreen* touch;
    uint32_t lastTouchTime;
    
    // Botões do console inferior (fixos)
    Button consoleButtons[3];
    void initConsoleButtons();
};

// Global instance
extern DisplayManager display;

#endif // DISPLAY_H
