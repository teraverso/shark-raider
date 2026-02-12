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

#include <Arduino.h>
#include "config.h"
#include "display.h"
#include "wifi_attack.h"
#include "evil_portal.h"

// Estados do Systema
enum AppState {
    DISCLAIMER,
    MAIN_MENU,
    BEACON_MENU,
    ATTACKING,
    PORTAL,
    CREDENTIALS
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
    Serial.println("SHARK RAIDER - Portal UI Upgrade");
    
    if (!display.begin()) {
        Serial.println("Display initialization failed!");
    }
    
    wifiAttack.begin();
    
    currentState = DISCLAIMER;
    updateDisplay = true;
}

void loop() {
    if (updateDisplay) {
        switch (currentState) {
            case DISCLAIMER:
                display.drawDisclaimer();
                break;
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
                display.updatePortalDisplay(
                    evilPortal.getUptimeSeconds(),
                    evilPortal.getTotalUniqueCount(),
                    evilPortal.getClientCount(),
                    evilPortal.getCapturedCredsCount(),
                    evilPortal.getSuccessRate(),
                    evilPortal.getAvgTimeToCatch(),
                    evilPortal.getAvgTimeToReject(),
                    evilPortal.getAvgTimeToLeft(),
                    evilPortal.getMemoryUsagePct(),
                    evilPortal.getRSSI(),
                    evilPortal.isActive()
                );
                break;
            case CREDENTIALS:
                display.drawCredentialsScreen(evilPortal.getCapturedCreds());
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
    if (currentState == DISCLAIMER) {
        if (display.isTouched()) {
            currentState = MAIN_MENU;
            updateDisplay = true;
            delay(300); // Debounce
        }
        return;
    }

    int press = display.checkConsolePress();
    if (press == -1) return;

    switch (press) {
        case 2: // BACK (índice 2 no consoleButtons) - Agora com lógica de movimentação NEXT
            if (currentState == MAIN_MENU) {
                selectedIndex++;
                if (selectedIndex > 2) selectedIndex = 0;
                updateDisplay = true;
            } else if (currentState == BEACON_MENU) {
                selectedIndex++;
                if (selectedIndex > 2) selectedIndex = 0;
                updateDisplay = true;
            } else if (currentState == PORTAL) {
                currentState = CREDENTIALS;
                updateDisplay = true;
            }
            break;
            
        case 1: // SELECT
            executeSelection();
            break;
            
        case 0: // NEXT - Agora com lógica de movimentação BACK
            goBack();
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
                wifiAttack.begin();
                currentState = ATTACKING;
                beaconType = BEACON_RICKROLL;
                wifiAttack.resetPacketCount();
            } else if (selectedIndex == 2) { // EVIL PORTAL
                currentState = PORTAL;
                evilPortal.begin(PORTAL_AP_NAME, PORTAL_CHANNEL);
            }
            break;

        case BEACON_MENU:
            wifiAttack.begin();
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
            break;
    }
    updateDisplay = true;
}

void goBack() {
    switch (currentState) {
        case MAIN_MENU:
            selectedIndex--;
            if (selectedIndex < 0) selectedIndex = 2;
            break;

        case BEACON_MENU:
            currentState = MAIN_MENU;
            selectedIndex = 0;
            break;
            
        case ATTACKING:
            wifiAttack.end();
            if (beaconType == BEACON_RICKROLL) {
                currentState = MAIN_MENU;
                selectedIndex = 1;
            } else {
                currentState = BEACON_MENU;
            }
            break;
            
        case PORTAL:
            evilPortal.end();
            currentState = MAIN_MENU;
            selectedIndex = 2;
            break;
            
        case CREDENTIALS:
            currentState = PORTAL;
            break;
    }
    updateDisplay = true;
}
