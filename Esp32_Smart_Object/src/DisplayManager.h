#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

class DisplayManager {
  private:
    LiquidCrystal_I2C lcd;

    String mensaje;
    int posicion;
    unsigned long lastScroll;
    int scrollDelay;

    bool wifiConectado;
    String estadoAlerta;

    void scrollTexto();
    void mostrarEstado();

  public:
    DisplayManager(uint8_t address, uint8_t cols, uint8_t rows);

    void init(uint8_t sda, uint8_t scl);
    void setMensaje(String msg);
    void setWifi(bool estado);
    void setAlerta(String estado);

    void update();
};

#endif