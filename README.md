# Practica_3_alexa

## 1. Requerimientos Funcionales y No Funcionales

### 1.1 Requerimientos Funcionales
- El sistema debe medir la distancia de objetos próximos en tiempo real utilizando un sensor ultrasónico.
-El dispositivo (Capa Edge) debe calcular el estado lógico de los actuadores y reportar la telemetría periódicamente a AWS IoT Core mediante el esquema reported del Device Shadow.
- El sistema debe mostrar información de estado localmente en un display LCD 16x2 (distancia actual, estado del actuador LED y estado de la red WiFi).
- El usuario debe poder consultar la distancia actual y modificar el estado de alerta (encender/apagar LED) mediante comandos de voz a través de un frontend de Amazon Alexa.
-El backend del sistema debe resolver la identidad del usuario de Alexa (user_id) mapeándola unívocamente con el identificador del dispositivo físico (thing_name) mediante una base de datos NoSQL.

### 1.2 Requerimientos No Funcionales
- El sistema debe minimizar el tráfico MQTT publicando actualizaciones de estado únicamente cuando exista un diferencial significativo (delta) o tras recibir un comando.
- Toda comunicación MQTT debe estar cifrada mediante TLS 1.2 (puerto 8883) empleando certificados X.509 generados por AWS.
- El firmware debe implementar rutinas de reconexión no bloqueantes para el enlace WiFi y el broker MQTT.
- El tiempo de respuesta de round-trip (Alexa -> Lambda -> IoT Core -> ESP32 -> IoT Core -> Lambda -> Alexa) no debe superar los 2 segundos.
- El código embebido debe estar diseñado bajo el paradigma de Programación Orientada a Objetos (POO), y la arquitectura Cloud debe ser Serverless para garantizar alta disponibilidad.

## 2. Diseño del Sistema
* Diagrama de bloques
  El sistema se divide lógicamente en tres módulos: Adquisición de datos (sensores), Procesamiento/Control (SoC ESP32) y Actuación (Indicadores visuales locales).
* Diagrama de circuito
  
  ![Diagrama de circuito](/media/diagramaS.png)

* Diagrama de arquitectura del sistema
  La arquitectura distribuida opera en tres capas:
  - Edge: Dispositivo inteligente suscrito a los tópicos MQTT de AWS IoT Core.
  - Cloud Middleware & Database: AWS IoT Core actuando como Message Broker y gestor de estado (Shadow). Amazon DynamoDB almacenando la relación 1:1 entre cuentas de Amazon y dispositivos físicos.
  - Voice & Backend: Alexa Skills Kit (Frontend) invocando a AWS Lambda (Backend en Python), el cual orquesta la lógica de negocio consumiendo las APIs de DynamoDB e IoT Data.
* Diagramas estructurales y de comportamiento
  Diseño Estructural: El firmware se abstrae en clases independientes (AWSManager, WiFiManager, UltrasonicSensor, LedManager, DisplayManager) inyectadas en el flujo principal, evitando el acoplamiento y facilitando pruebas unitarias.
  Comportamiento (Flujo de Comando): El usuario emite un Utterance -> Alexa resuelve el Intent -> Lambda consulta user_thing en DynamoDB -> Lambda envía carga útil JSON al tópico /shadow/update -> IoT Core notifica al ESP32 en /shadow/update/delta -> ESP32 actúa e informa el nuevo estado a /shadow/update (reported).
* Diseño de la skill de Alexa
  - Invocation Name: practica 3
  - Modelo de Interacción (Intents):
     - TurnOnIntent: Enciende el indicador de proximidad.
     - TurnOffIntent: Apaga el indicador de proximidad.
     - StateIntent: Consulta la distancia actual medida por el sensor ultrasónico.
     - Handlers estándar (HelpIntent, CancelIntent, StopIntent).
## 3. Implementación
### 3.1. Código fuente documentado (Capa Edge)
El firmware del ESP32 fue desarrollado en C++ utilizando PlatformIO.
- Módulos principales:
  - AWSManager: Encapsula la instancia de WiFiClientSecure y PubSubClient. Gestiona la serialización/deserialización de documentos JSON para el Device Shadow.
  - WiFiManager: Administra el stack TCP/IP y las rutinas de reconexión.
  - UltrasonicSensor / LedManager / DisplayManager: Abstracciones de la capa de acceso a hardware (HAL).
- Tópicos MQTT:
  - Publicación (Reported): $aws/things/thing_test/shadow/update
  - Suscripción (Delta): $aws/things/thing_test/shadow/update/delta

### 3.2 Configuraciones en AWS (Infraestructura Core y Seguridad)
- AWS IoT Core: Creación del Thing (thing_test), generación del Endpoint ATS y aprovisionamiento de certificados (Root CA, Device Certificate, Private Key).
- Políticas de Seguridad IoT: Restricción de permisos estrictamente a las acciones iot:Connect, iot:Publish, iot:Subscribe y iot:Receive sobre los ARNs específicos del dispositivo.
- Amazon DynamoDB: Creación de la tabla user_thing con Partition Key user_id (String) para almacenar el mapeo de tenencia del dispositivo.

### 3.3 Configuraciones en Alexa y Backend (AWS Lambda)
- Backend Serverless: Implementación de la lógica de ruteo mediante Python 3.x utilizando el SDK ask-sdk-core y boto3.
- Gestión de Accesos (IAM): Modificación del Rol de Ejecución de la función Lambda adjuntando políticas inline para permitir dynamodb:GetItem y iot:UpdateThingShadow, habilitando la interoperabilidad entre servicios.
- Integración: Enlace directo del Skill ID en los triggers de Lambda y configuración del ARN de Lambda como el Endpoint por defecto en la consola de desarrollo de Alexa.
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
* Sistema IoT E2E Funcional: Se logró una integración completa bidireccional entre hardware perimetral e interfaces conversacionales impulsadas por IA.

* Latencia Operativa: Las pruebas de round-trip demostraron un tiempo de respuesta de voz promedio inferior a 1.2 segundos, validando la eficiencia de la arquitectura Serverless.

* Alta Disponibilidad del Estado: El uso del Device Shadow demostró que el backend de Alexa puede consultar el último estado conocido del objeto de forma instantánea sin requerir que el dispositivo físico responda a la solicitud sincrónicamente, eliminando los tiempos de espera HTTP tradicionales.

## 6. Conclusiones
* La arquitectura orientada a eventos utilizando AWS IoT Core y MQTT abstrae exitosamente la complejidad de mantener conexiones persistentes, resultando ideal para redes inestables o de bajo ancho de banda.

* La separación de responsabilidades mediante la inclusión de una base de datos NoSQL (DynamoDB) en la capa lógica previene que la Skill de Alexa quede fuertemente acoplada a un hardware específico, estableciendo una base profesional para la comercialización del sistema a múltiples usuarios.

* El diseño estructurado en POO para el firmware del microcontrolador probó ser crítico para gestionar la concurrencia entre la lectura de sensores, la actualización del display I2C y el stack de red cifrado, evitando bloqueos del procesador (Watchdog resets).
  
## 7. Recomendaciones
* Implementar control por cambios (no enviar datos constantemente).
* Manejar errores de sensor (valores -1).
* Optimizar uso del Shadow (limpieza de desired).
* Implementar autenticación adicional en Lambda.
* Añadir más sensores para ampliar funcionalidad.

## 8. Anexos
