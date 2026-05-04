#include <Arduino.h>
#include "DisplayManager.h"
#include "LedManager.h"
#include "UltrasonicSensor.h"
#include "WiFiManager.h"
#include "AWSManager.h"

// ==== variables ====
unsigned long lastPublish = 0;
const int publishInterval = 5000; // 5 segundos

// ==== CONFIG ====
#define TRIG 18
#define ECHO 19

const char* ssid = "Redmi 10 2022";
const char* password = "ulnt0462";

// ==== AWS ====
const char* endpoint = "a22gvicr7s9bow-ats.iot.us-east-1.amazonaws.com";
AWSManager aws(endpoint, "thing_test");

// ==== OBJETOS ====
DisplayManager display(0x27, 16, 2);
LedManager leds(17, 16, 32);
UltrasonicSensor sensor(TRIG, ECHO);
WiFiManager wifi(ssid, password);

// ==== CONTROL EXTERNO (AWS luego) ====
//String ledCommand = "danger";  // <- esto luego vendrá del Shadow

void aplicarComando(String cmd) {

  if (cmd == "green") {
    leds.setState(GREEN);
    display.setAlerta("GOOD");

  } else if (cmd == "yellow") {
    leds.setState(YELLOW);
    display.setAlerta("GOOD");

  } else if (cmd == "red") {
    leds.setState(RED);
    display.setAlerta("GOOD");

  } else if (cmd == "danger") {
    leds.setState(DANGER);
    display.setAlerta("DANGER");

  } else {
    leds.setState(OFF);
    display.setAlerta("GOOD");
  }
}

void setup() {
  Serial.begin(115200);

  display.init(21, 22);
  leds.init();
  sensor.init();

  wifi.connect();

  aws.init();
}

void loop() {

  // ==== WIFI ====
  wifi.reconnect();
  display.setWifi(wifi.isConnected());

  // ==== SENSOR ====
  long distancia = sensor.medir();

  Serial.print("Distancia: ");
  Serial.println(distancia);

  // ==== AWS ====
  if (wifi.isConnected()) {
    aws.loop();  // 1. recibir comandos
  }

  // ==== LEDS ====
  aplicarComando(aws.ledCommand);  // 2. aplicar comando recibido
  leds.update();

  // ==== AWS (PUBLICAR DESPUÉS) ====
  if (wifi.isConnected()) {
    if (millis() - lastPublish > publishInterval) {
        aws.publishState(distancia, aws.ledCommand); // 3. publicar estado actualizado
        lastPublish = millis();
    }
  }

  // ==== DISPLAY ====
  String msg = "Distancia: " + String(distancia) + " cm | LED: " + aws.ledCommand;
  display.setMensaje(msg);
  display.update();

  delay(500);
}
