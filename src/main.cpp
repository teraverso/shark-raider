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
    Serial.println("SHARK RAIDER - Navigation Fix");
    
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
        case 0: // BACK
            goBack();
            break;
            
        case 1: // SELECT
            executeSelection();
            break;
            
        case 2: // NEXT
            if (currentState == MAIN_MENU) {
                // Tela 1: RICK ROLL(1) -> EVIL PORTAL(2) -> BEACON SPAM(0)
                selectedIndex++;
                if (selectedIndex > 2) selectedIndex = 0;
                updateDisplay = true;
            } else if (currentState == BEACON_MENU) {
                // Tela 2: RANDOM SSIDS(1) -> CUSTOM SSIDS(2) -> FUNNY SSIDS(0)
                selectedIndex++;
                if (selectedIndex > 2) selectedIndex = 0;
                updateDisplay = true;
            }
            break;
    }
}

void executeSelection() {
    switch (currentState) {
        case MAIN_MENU:
            if (selectedIndex == 0) { // BEACON SPAM
                currentState = BEACON_MENU;
                selectedIndex = 0; // FUNNY SSIDS
            } else if (selectedIndex == 1) { // RICK ROLL
                wifiAttack.begin(); // Ensure WiFi is active
                currentState = ATTACKING;
                beaconType = BEACON_RICKROLL;
                wifiAttack.resetPacketCount();
            } else if (selectedIndex == 2) { // EVIL PORTAL
                currentState = PORTAL;
                evilPortal.begin(PORTAL_AP_NAME, PORTAL_CHANNEL);
            }
            break;

        case BEACON_MENU:
            wifiAttack.begin(); // Ensure WiFi is active for any beacon attack
            if (selectedIndex == 0) { // FUNNY SSID
                currentState = ATTACKING;
                beaconType = BEACON_FUNNY;
                wifiAttack.resetPacketCount();
            } else if (selectedIndex == 1) {
                currentState = ATTACKING;
                beaconType = BEACON_RANDOM;
                wifiAttack.resetPacketCount();
            } else if (selectedIndex == 2) {
                currentState = ATTACKING;
                beaconType = BEACON_FUNNY; // Placeholder
                wifiAttack.resetPacketCount();
            }
            break;

        case ATTACKING:
        case PORTAL:
            // SELECT não faz nada na Tela 3
            break;
    }
    updateDisplay = true;
}

void goBack() {
    switch (currentState) {
        case MAIN_MENU:
            // Tela 1: BACK vai para EVIL PORTAL(2) -> RICK ROLL(1) -> BEACON SPAM(0)
            selectedIndex--;
            if (selectedIndex < 0) selectedIndex = 2;
            break;

        case BEACON_MENU:
            // Tela 2: BACK volta para a Tela 1
            currentState = MAIN_MENU;
            selectedIndex = 0; // Opcional: manter seleção no BEACON SPAM
            break;
            
        case ATTACKING:
            // Tela 3: BACK encerra ataque e volta para Tela 2
            wifiAttack.end();
            if (beaconType == BEACON_RICKROLL) {
                currentState = MAIN_MENU;
                selectedIndex = 1;
            } else {
                currentState = BEACON_MENU;
            }
            break;
            
        case PORTAL:
            // Tela 3: BACK encerra portal e retorna para Tela 1
            evilPortal.end();
            currentState = MAIN_MENU;
            selectedIndex = 2;
            break;
    }
    updateDisplay = true;
}
