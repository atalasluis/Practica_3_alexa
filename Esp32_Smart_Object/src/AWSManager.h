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
{
  "state": {
    "reported": {
      "distancia": 25,
      "led": "green"
    }
  }
}
*/