# Projekt IoT # 

## Autorzy projektu ## 
* Antoni Marcinek 
* Michał Piechowski

## Opis projektu ##

Projekt skupia się na wykorzystaniu mikrokontrolera ESP3866 do następujących zadań:

* Odczytywanie temperatury z czujnika,
* Wyświetlanie odczytu na ekranie LCD,
* Przesyłałnie komunikatów o odczytach za pomocą protokołu MQTTs do brokera,
* Przechowywanie danych za pomocą InnfluxDB stojącego jako kontener Docker,
* Wizualizacja danych za pomocą aplikacji Grafana również stojącej w kontenerze Docker,
* Wykorzystanie modułu ESP8266 w celu wykonywania operacji Speech-to-text za pomocą modelu językowego Whisper,
* Wykorzystanie modułu ESP8266 w celu rozpoznawania obrazu za pomocą modelu językowego YOLO.

### ESP32 Temperatura oraz Ekran ###

ESP8266 dokonuje odczytów z czujnika DHT11 za pomocą dowolnego pinu cyfrowego ( w naszym przypadku pin 11 ), z ESP do modułu jest dostarczane zasilanie na linii 3.3V oraz lini GND. 

### Docker - Grafana oraz InfluxDB ###

### Speech-to-text ###

### Video-to-text ###

