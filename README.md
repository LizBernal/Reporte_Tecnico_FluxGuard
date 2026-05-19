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


