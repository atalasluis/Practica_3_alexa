# Practica_3_alexa

## 1. Requerimientos Funcionales y No Funcionales

### 1.1 Requerimientos Funcionales
- El sistema debe medir la distancia utilizando un sensor ultrasónico.
- El dispositivo debe reportar periódicamente la distancia a AWS IoT Core.
- El sistema debe permitir controlar LEDs mediante comandos remotos.
- El ESP32 debe recibir comandos desde AWS IoT Core utilizando Device Shadow.
- El sistema debe mostrar información en un display LCD (distancia, estado del LED y WiFi).
- Alexa debe permitir enviar comandos por voz para controlar los LEDs.
- El sistema debe mantener sincronizado el estado entre el dispositivo y la nube.

### 1.2 Requerimientos No Funcionales
- El sistema debe ser eficiente en el uso de mensajes MQTT (optimización de tráfico).
- Debe garantizar conexión segura mediante certificados TLS.
- Debe ser tolerante a fallos de red (reconexión WiFi).
- El sistema debe responder en tiempo casi real.
- El código debe ser modular y mantenible.

## 2. Diseño del Sistema
* Diagrama de bloques
* Diagrama de circuito
  
  ![Diagrama de circuito](/media/diagramaS.png)

* Diagrama de arquitectura del sistema
* Diagramas estructurales y de comportamiento
* Diseño de la skill de Alexa
## 3. Implementación
* Código fuente documentado
  Módulos principales:

    * AWSManager → conexión MQTT y Shadow
    * WiFiManager → manejo de red
    * UltrasonicSensor → medición de distancia
    * LedManager → control de LEDs
    * DisplayManager → interfaz visual

* Configuraciones en Alexa
  
    * Creación de Skill
    * Definición de intents
    * Integración con Lambda
  
  Publicación:

    $aws/things/thing_test/shadow/update

  Suscripción:

    $aws/things/thing_test/shadow/update/delta

  Seguridad

    * Certificados utilizados:
    * Root CA
    * Device Certificate
    * Private Key
  
  Protocolo:

    * MQTT sobre TLS (puerto 8883)

* Configuraciones en AWS

    * Creación de Thing
    * Configuración de Device Shadow
    * Generación de certificados
    * Creación de políticas (IoT Policy)
## 4. Pruebas y Validaciones

### 4.1 Pruebas realizadas
* Conectividad WiFi
* Publicación MQTT
* Recepción de comandos (delta)
* Control de LEDs
* Visualización en display
* Integración con AWS IoT Core
  
### 4.2 Herramientas utilizadas
* MQTT Test Client (AWS)
* Monitor Serial
* Device Shadow
  
### 4.3 Resultados de pruebas
* Comunicación bidireccional exitosa
* Latencia baja (<1s)
* Correcta sincronización del Shadow

## 5. Resultados
* Sistema IoT completamente funcional
* Control remoto de actuadores
* Monitoreo en tiempo real
* Integración exitosa con servicios cloud

## 6. Conclusiones
* AWS IoT Core permite una integración eficiente de dispositivos IoT.
* El uso de Device Shadow simplifica la sincronización de estados.
* MQTT es un protocolo adecuado para sistemas distribuidos.
* La modularidad del código facilita el mantenimiento y escalabilidad.
  
## 7. Recomendaciones
* Implementar control por cambios (no enviar datos constantemente).
* Manejar errores de sensor (valores -1).
* Optimizar uso del Shadow (limpieza de desired).
* Implementar autenticación adicional en Lambda.
* Añadir más sensores para ampliar funcionalidad.

## 8. Anexos