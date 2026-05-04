#include "WiFiManager.h"

WiFiManager::WiFiManager(const char* ssid, const char* password)
    : ssid(ssid), password(password) {}

/* void WiFiManager::connect() {
    WiFi.begin(ssid, password);
    Serial.print("Conectando WiFi");

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nWiFi conectado");
} */

void WiFiManager::connect() {
    WiFi.begin(ssid, password);
    Serial.println("Intentando conectar WiFi...");
}

/* void WiFiManager::reconnect() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("Reconectando WiFi...");
        connect();
    }
} */

void WiFiManager::reconnect() {
    static unsigned long lastAttempt = 0;

    if (WiFi.status() == WL_CONNECTED) return;

    if (millis() - lastAttempt > 5000) {
        Serial.println("Reintentando WiFi...");
        WiFi.begin(ssid, password);
        lastAttempt = millis();
    }
}

bool WiFiManager::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}