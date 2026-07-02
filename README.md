# Análisis de la Botnet Mirai (TFM)

Trabajo de Fin de Máster (UNIR, 2023) centrado en el estudio del código fuente
filtrado de Mirai para entender sus vectores de propagación y sus tipos de ataque.

## Objetivo
El proyecto tiene fines exclusivamente analíticos y defensivos. El objetivo es
estudiar el comportamiento de la botnet en un entorno de laboratorio aislado para:
- Entender los vectores de infección típicos en dispositivos IoT
- Identificar patrones de tráfico C2 (command & control)
- Analizar los diferentes tipos de ataques que puede ejecutar (DDoS, telnet
  scanning, credential brute-forcing, etc.)

## Entorno
- Ejecutado exclusivamente en máquinas virtuales aisladas / red de laboratorio
  sin salida a internet
- Herramientas: Wireshark, Vagrant, Snort, etc.

## Disclaimer
Este repositorio tiene fines exclusivamente académicos y de investigación en
un entorno controlado.
