#include "AWSManager.h"
#include <ArduinoJson.h>
#include "certs/root_ca.h"
#include "certs/device_cert.h"
#include "certs/private_key.h"

static AWSManager* instance = nullptr;

AWSManager::AWSManager(const char* endpoint, const char* thingName)
    : client(net), endpoint(endpoint), thingName(thingName) {
    instance = this;
    ledCommand = "off";
}

void AWSManager::init() {
    //net.setInsecure(); // luego usamos certificados reales
    net.setCACert(AWS_ROOT_CA);
    net.setCertificate(DEVICE_CERT);
    net.setPrivateKey(PRIVATE_KEY);
    client.setServer(endpoint, 8883);
    client.setCallback(callbackStatic);
}

void AWSManager::callbackStatic(char* topic, byte* payload, unsigned int length) {
    if (instance) {
        instance->callback(topic, payload, length);
    }
}

void AWSManager::callback(char* topic, byte* payload, unsigned int length) {

    StaticJsonDocument<200> doc;
    deserializeJson(doc, payload, length);

    // IMPORTANTE: leer delta correctamente
    if (doc["state"]["led"]) {
        ledCommand = (const char*)doc["state"]["led"];
        
        Serial.print("Nuevo comando recibido: ");
        Serial.println(ledCommand);
    }
}

void AWSManager::connect() {
    if (client.connected()) return;

    if (client.connect("ESP32Client")) {
        client.subscribe("$aws/things/thing_test/shadow/update/delta");
    }
}

void AWSManager::loop() {
    if (!client.connected()) {
        connect();
    }
    client.loop();
}

void AWSManager::publishState(long distancia, String led) {

    StaticJsonDocument<200> doc;

    doc["state"]["reported"]["distancia"] = distancia;
    doc["state"]["reported"]["led"] = led;
    //doc["state"]["reported"]["serie"] = serie;

    char buffer[256];
    serializeJson(doc, buffer);

    client.publish("$aws/things/thing_test/shadow/update", buffer);
}