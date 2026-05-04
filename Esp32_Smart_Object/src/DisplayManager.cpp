#include "DisplayManager.h"
#include <Wire.h>

DisplayManager::DisplayManager(uint8_t address, uint8_t cols, uint8_t rows)
  : lcd(address, cols, rows) {
  posicion = 0;
  lastScroll = 0;
  scrollDelay = 300;
  wifiConectado = false;
  estadoAlerta = "GOOD";
}

void DisplayManager::init(uint8_t sda, uint8_t scl) {
  Wire.begin(sda, scl);
  lcd.init();
  lcd.backlight();

  // Icono WiFi
  byte wifiIcon[8] = {
    B00000,
    B01110,
    B10001,
    B00100,
    B01010,
    B00000,
    B00100,
    B00000
  };

  // Icono alerta
  byte alertIcon[8] = {
    B00100,
    B00100,
    B00100,
    B00100,
    B00100,
    B00000,
    B00100,
    B00000
  };

  lcd.createChar(0, wifiIcon);
  lcd.createChar(1, alertIcon);
}

void DisplayManager::setMensaje(String msg) {
  mensaje = msg + "   ";
}

void DisplayManager::setWifi(bool estado) {
  wifiConectado = estado;
}

void DisplayManager::setAlerta(String estado) {
  estadoAlerta = estado;
}

void DisplayManager::update() {
  scrollTexto();
  mostrarEstado();
}

void DisplayManager::scrollTexto() {
  if (millis() - lastScroll > scrollDelay) {
    lastScroll = millis();

    lcd.setCursor(0, 0);

    String visible = mensaje.substring(posicion, posicion + 16);
    lcd.print(visible);

    posicion++;
    if (posicion > mensaje.length() - 16) {
      posicion = 0;
    }
  }
}

void DisplayManager::mostrarEstado() {
  lcd.setCursor(0, 1);

  lcd.write(byte(0)); // WiFi icono
  if (wifiConectado) {
    lcd.print(" ON ");
  } else {
    lcd.print(" OFF");
  }

  lcd.print(" ");

  lcd.write(byte(1)); // alerta icono
  lcd.print(" ");
  lcd.print(estadoAlerta);
}