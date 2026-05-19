# FluxGuard
Sistema IoT para monitoreo eléctrico en casa habitación mediante tecnologías inalámbricas LoRa, Wi-Fi y GPRS.
![Prototipo FluxGuard](img/prototipo.jpeg)

# Introduccón 
FluxGuard es un prototipo basado en IoT diseñado para el monitoreo remoto de instalaciones eléctricas residenciales con el fin de prevenir riesgos. El sistema utiliza un microcontrolador ESP32 y sensores para medir corriente (SCT-013-030), temperatura de conductores (DS18B20) y variables ambientales (DHT11). Para la transmisión de datos a largo alcance, emplea módulos Heltec ESP32 LoRa (915 MHz) y el protocolo MQTT, permitiendo la visualización de la información en tiempo real mediante Node-RED y Grafana. Este desarrollo demuestra la importancia y aplicación de las tecnologías inalámbricas y protocolos IoT en la supervisión y seguridad del hogar.

# Justificación
El desarrollo del prototipo FluxGuard surge ante la falta de sistemas de supervisión en tiempo real en las viviendas, lo que incrementa el riesgo de sobrecargas, cortocircuitos e incendios. Para solucionar esto, el proyecto integra sensores de corriente y temperatura con tecnologías inalámbricas (LoRa, Wi-Fi y GPRS), permitiendo la prevención de riesgos mediante el monitoreo remoto en plataformas IoT. De esta manera, el sistema aporta un impacto tecnológico y académico al evaluar la estabilidad y confiabilidad de la transmisión de datos, ofreciendo una solución funcional para la seguridad eléctrica residencial.

# Objetivos

## Objetivo general
Desarrollar un prototipo IoT basado en tecnologías inalámbricas LoRa, Wi-Fi y GPRS para el monitoreo en tiempo real de variables eléctricas en instalaciones de casa habitación, mediante sensores de corriente y temperatura integrados a una plataforma de supervisión remota, con la finalidad de analizar parámetros característicos de los sistemas inalámbricos, como la confiabilidad, interoperabilidad y consumo energético en la transmisión de datos, así como contribuir a la prevención de riesgos eléctricos y reducir daños en electrodomésticos.

## Objetivos especificos

- Implementar comunicación inalámbrica mediante módulos Heltec ESP32 LoRa y conectividad Wi-Fi/GPRS para la transmisión de datos entre los nodos del sistema, evaluando parámetros como alcance, estabilidad e interferencia en la comunicación.

- Desarrollar una plataforma de supervisión remota utilizando protocolo MQTT, Node-RED, InfluxDB y Grafana para visualizar en tiempo real las variables eléctricas obtenidas por los sensores y analizar el comportamiento del sistema.

# Requerimientos de Hardware y Software

## 🛠 Hardware

| Componente | Descripción |
|---|---|
| Heltec ESP32 LoRa V3 | Comunicación LoRa |
| SCT-013-030 | Sensor de corriente |
| DS18B20 | Sensor de temperatura |
| DHT11 | Sensor ambiental |
| LilyGO T-SIM7070G | Respaldo GPRS |
| Módulo GPS | Geolocalización del sistema |

## 💻 Software

| Software | Función |
|---|---|
| Arduino IDE | Programación |
| Node-RED | Dashboard |
| MQTT Broker | Comunicación |
| InfluxDB | Base de datos |
| Grafana | Visualización |

# Tabla de conexiones 

## 🔹 Nodo Emisor (TX)

| Componente | Pin ESP32 | Descripción |
|---|---|---|
| DS18B20 | GPIO4 | Sensor de temperatura del conductor |
| DHT11 | GPIO5 | Sensor ambiental |
| GPS RX/TX | GPIO7 / GPIO6 | Comunicación GPS |
| ADS1115 SDA | GPIO41 | Comunicación I2C |
| ADS1115 SCL | GPIO42 | Comunicación I2C |
| OLED SDA | GPIO21 | Pantalla OLED |
| OLED SCL | GPIO18 | Pantalla OLED |
| LoRa SX1262 | SPI personalizado | Comunicación inalámbrica |

## 🔹 Nodo Receptor (RX)

| Componente | Pin ESP32 | Descripción |
|---|---|---|
| OLED SDA | GPIO21 | Pantalla OLED |
| OLED SCL | GPIO18 | Pantalla OLED |
| LoRa SX1262 | SPI personalizado | Recepción LoRa |

## 🔸 Gateway LilyGO T-SIM7070G

| Componente | Pin | Descripción |
|---|---|---|
| SIM7070 TX | GPIO27 | Comunicación módem |
| SIM7070 RX | GPIO26 | Comunicación módem |
| I2C SDA | GPIO21 | Comunicación I2C |
| I2C SCL | GPIO22 | Comunicación I2C |
| PWR MODEM | GPIO4 | Encendido del módem |

# Tabla de direccionamiento

| Elemento | Configuración |
|---|---|
| Frecuencia LoRa | 915 MHz |
| Canal ESP-NOW | 7 |
| Broker MQTT | broker.hivemq.com |
| Puerto MQTT | 1883 |
| Topic MQTT | itsoeh/mary_gress/fluxguard/datos |
| Protocolo de transmisión | MQTT |
| Tipo de comunicación local | LoRa |
| Tipo de comunicación Gateway | ESP-NOW |

# Esquema de Funcionamiento

![Diagrma/Esquema Fluxguard](img/Diagrama_FluxGuard.jpg)

# Códigos del proyecto

## 🟦 Nodo Emisor (TX)
[Ver código TX](NodoEmisor_TX/NodoEmisor_TX.ino)

## 🟩 Nodo receptor (RX)
[Ver código RX](NodoReceptor_RX/NodoReceptor_RX.ino)

## 🟧 LilyGO T-SIM7070G
[Ver código Gateway](Lilygo/Lilygo.ino)


