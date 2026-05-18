# Reporte_Tecnico_FluxGuard

# 1. Introduccón 
El prototipo FluxGuard fue desarrollado como una solución basada en el Internet de las Cosas (IoT) para el monitoreo de condiciones eléctricas en casa habitación. Su función principal consiste en supervisar variables críticas como corriente eléctrica, temperatura del conductor y condiciones ambientales, con la finalidad de identificar posibles anomalías dentro de una instalación eléctrica residencial.

El sistema emplea tecnologías inalámbricas para la transmisión y supervisión remota de la información obtenida por los sensores. Para ello, se utilizan módulos Heltec ESP32 LoRa operando a 915 MHz, permitiendo la comunicación de largo alcance entre el nodo transmisor y el gateway receptor. Además, la información es enviada mediante protocolo MQTT hacia plataformas de monitoreo remoto.

FluxGuard integra sensores SCT-013-030 para la medición de corriente eléctrica, sensores DS18B20 para temperatura de conductores y sensores DHT11 para variables ambientales. Los datos adquiridos son procesados por el microcontrolador ESP32 y posteriormente visualizados mediante Node-RED y Grafana, facilitando el monitoreo en tiempo real del sistema eléctrico.

El desarrollo de este prototipo permite aplicar conceptos relacionados con tecnologías inalámbricas, protocolos de comunicación IoT y transmisión de datos en redes de monitoreo remoto. Asimismo, demuestra la importancia de las comunicaciones inalámbricas en aplicaciones orientadas a la supervisión y prevención de riesgos eléctricos en entornos residenciales.

# 2. Justificación


# 3. Objetivos

## Objetivo general
Desarrollar un prototipo IoT basado en tecnologías inalámbricas LoRa, Wi-Fi y GPRS para el monitoreo en tiempo real de variables eléctricas en instalaciones de casa habitación, mediante sensores de corriente y temperatura integrados a una plataforma de supervisión remota, con la finalidad de analizar parámetros característicos de los sistemas inalámbricos, como la confiabilidad, interoperabilidad y consumo energético en la transmisión de datos, así como contribuir a la prevención de riesgos eléctricos y reducir daños en electrodomésticos.

## Objetivos especificos

- Implementar comunicación inalámbrica mediante módulos Heltec ESP32 LoRa y conectividad Wi-Fi/GPRS para la transmisión de datos entre los nodos del sistema, evaluando parámetros como alcance, estabilidad e interferencia en la comunicación.

- Desarrollar una plataforma de supervisión remota utilizando protocolo MQTT, Node-RED, InfluxDB y Grafana para visualizar en tiempo real las variables eléctricas obtenidas por los sensores y analizar el comportamiento del sistema.


