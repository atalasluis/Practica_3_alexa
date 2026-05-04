#pragma once
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

class AWSManager {
private:
    WiFiClientSecure net;
    PubSubClient client;

    const char* endpoint;
    const char* thingName;

    static void callbackStatic(char* topic, byte* payload, unsigned int length);
    void callback(char* topic, byte* payload, unsigned int length);

public:
    AWSManager(const char* endpoint, const char* thingName);

    void init();
    void connect();
    void loop();

    void publishState(long distancia, String led);

    String ledCommand;
};

/*
Device Shadow state
{
  "state": {
    "desired": {
      "led": "off"
    },
    "reported": {
      "distancia": 22,
      "led": "off"
    }
  }
}
Device Shadow metadata
{
  "metadata": {
    "desired": {
      "led": {
        "timestamp": 1777871239
      }
    },
    "reported": {
      "distancia": {
        "timestamp": 1777871364
      },
      "led": {
        "timestamp": 1777871364
      }
    }
  }
}

topic para publicar en mqtt test client

Topic name
$aws/things/thing_test/shadow/update

Message payload
{
  "state": {
    "desired": {
      "led": "off"
    }
  }
}

topic para suscribirse y ver el intercambio de mensajes

$aws/things/thing_test/shadow/#

las claves para que el esp32 se conecte a IoT core estan en:

ESP32_SMART_OBJECT/include/certs/

device_cert.h
private_key.h
root_ca.h

*/