#include <Arduino.h>
#include "config.h"
#include "display.h"
#include "wifi_attack.h"
#include "evil_portal.h"

// Estados do Sistema
enum AppState {
    MAIN_MENU,
    BEACON_MENU,
    ATTACKING,
    PORTAL
};

AppState currentState = MAIN_MENU;
AppState previousState = MAIN_MENU;
int selectedIndex = 0;
bool updateDisplay = true;

// Variável de tipo de beacon
BeaconSpamType beaconType = BEACON_FUNNY;

// Protótipos de funções
void handleNavigation();
void executeSelection();
void goBack();

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("SHARK RAIDER v1.9.1 - Navigation Fix");
    
    if (!display.begin()) {
        Serial.println("Display initialization failed!");
    }
    
    wifiAttack.begin();
    
    currentState = MAIN_MENU;
    updateDisplay = true;
}

void loop() {
    if (updateDisplay) {
        switch (currentState) {
            case MAIN_MENU:
                display.drawMainMenu(selectedIndex);
                break;
            case BEACON_MENU:
                display.drawBeaconMenu(selectedIndex);
                break;
            case ATTACKING:
                display.drawAttackScreen(
                    (beaconType == BEACON_RICKROLL) ? "RICK ROLL" : "BEACON SPAM", 
                    wifiAttack.getPacketCount(), 
                    wifiAttack.getChannel()
                );
                break;
            case PORTAL:
                display.drawPortalScreen(evilPortal.getClientCount(), evilPortal.getCapturedCredsCount());
                break;
        }
        updateDisplay = false;
    }

    handleNavigation();

    if (currentState == ATTACKING) {
        switch (beaconType) {
            case BEACON_FUNNY: wifiAttack.beaconSpamFunny(); break;
            case BEACON_RANDOM: wifiAttack.beaconSpamRandom(); break;
            case BEACON_RICKROLL: wifiAttack.beaconSpamRickRoll(); break;
            default: break;
        }
        
        static uint32_t lastUpdate = 0;
        if (millis() - lastUpdate > 1000) {
            updateDisplay = true; 
            lastUpdate = millis();
        }
    } else if (currentState == PORTAL) {
        evilPortal.loop();
        static uint32_t lastUpdate = 0;
        if (millis() - lastUpdate > 1000) {
            updateDisplay = true;
            lastUpdate = millis();
        }
    }
    
    delay(10);
}

void handleNavigation() {
    int press = display.checkConsolePress();
    if (press == -1) return;

    switch (press) {
        case 0: // BACK - Retorna à tela anterior
            goBack();
            break;
            
        case 1: // SELECT - Executa a seleção atual
            executeSelection();
            break;
            
        case 2: // NEXT - Rotaciona as opções (Lógica movida do BACK anterior)
            selectedIndex++;
            if (selectedIndex > 2) selectedIndex = 0; 
            updateDisplay = true;
            break;
    }
}

void executeSelection() {
    switch (currentState) {
        case MAIN_MENU:
            if (selectedIndex == 0) {
                previousState = MAIN_MENU;
                currentState = BEACON_MENU;
                selectedIndex = 0;
            } else if (selectedIndex == 1) {
                previousState = MAIN_MENU;
                currentState = ATTACKING;
                beaconType = BEACON_RICKROLL;
                wifiAttack.resetPacketCount();
            } else if (selectedIndex == 2) {
                previousState = MAIN_MENU;
                currentState = PORTAL;
                evilPortal.begin(PORTAL_AP_NAME, PORTAL_CHANNEL);
            }
            break;

        case BEACON_MENU:
            previousState = BEACON_MENU;
            currentState = ATTACKING;
            if (selectedIndex == 0) beaconType = BEACON_FUNNY;
            else if (selectedIndex == 1) beaconType = BEACON_RANDOM;
            else if (selectedIndex == 2) beaconType = BEACON_FUNNY; 
            wifiAttack.resetPacketCount();
            break;

        case ATTACKING:
            // Em ataque, SELECT para o ataque
            wifiAttack.end();
            currentState = previousState;
            break;

        case PORTAL:
            // Em portal, SELECT para o portal
            evilPortal.end();
            currentState = MAIN_MENU;
            break;
    }
    updateDisplay = true;
}

void goBack() {
    switch (currentState) {
        case BEACON_MENU:
            currentState = MAIN_MENU;
            selectedIndex = 0;
            break;
            
        case ATTACKING:
            wifiAttack.end();
            currentState = previousState;
            break;
            
        case PORTAL:
            evilPortal.end();
            currentState = MAIN_MENU;
            break;
            
        case MAIN_MENU:
            // Já está no menu principal, não faz nada ou reseta seleção
            selectedIndex = 0;
            break;
    }
    updateDisplay = true;
}
