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

#### Odczyt temperatury z czujnika DHT11 #### 
Moduł ESP32 (lub ESP8266) współpracuje z cyfrowym czujnikiem temperatury i wilgotności DHT11. Odczyt odbywa się przez dowolny pin cyfrowy, w naszym przypadku jest to pin 11. Czujnik ten dostarcza zarówno dane o temperaturze, jak i wilgotności w formacie cyfrowym, co eliminuje konieczność stosowania przetworników analogowo-cyfrowych.

* Zasilanie czujnika:
  Czujnik DHT11 wymaga napięcia zasilania w zakresie od 3.3V do 5V. W naszym przypadku zasilanie jest dostarczane przez linię 3.3V z ESP. Linia GND zapewnia masę do układu.

* Komunikacja:
  Komunikacja między ESP a czujnikiem odbywa się w protokole jednoliniowym. Odczyt danych wymaga odpowiedniego oprogramowania, które inicjuje transmisję i interpretuje dane o temperaturze oraz wilgotności. Popularne biblioteki, takie jak Adafruit DHT Library, upraszczają implementację.
  
```
void readDHT(float &humidity, float &temperature, float &fahrenheit) {
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();
  fahrenheit = dht.readTemperature(true);

  if (isnan(humidity) || isnan(temperature) || isnan(fahrenheit)) {
    display.println(F("Failed to read from DHT sensor!"));
  }
}
```

#### Obsługa wyświetlacza przez I2C ####
Wyświetlacz LCD został podłączony do ESP za pomocą protokołu I2C, co znacząco redukuje liczbę wymaganych linii połączeń. Wyświetlacz pełni funkcję prezentacji danych, takich jak odczyty z czujnika temperatury i inne komunikaty systemowe.

* Zasilanie wyświetlacza:
  Wyświetlacz wymaga zasilania w postaci napięcia 5V, które jest dostarczane przez ESP. Linie GND obu urządzeń są wspólne i umożliwiają prawidłowe działanie układu.

* Linie sygnałowe I2C:
  Standardowe połączenie I2C wykorzystuje dwie linie sygnałowe:

  **SCL (Serial Clock):** Linia zegara, synchronizująca komunikację.
  
  **SDA (Serial Data):** Linia danych, przesyłająca informacje między ESP a wyświetlaczem.

```
void displayData(float humidity, float temperature) {
  display.clearDisplay();
  display.setCursor(0,0);
  display.print(F("Wilgotnosc: "));
  display.print(humidity);
  display.print(F("%"));
  display.setCursor(0,10);
  display.print(F("Temperatura: "));
  display.print(temperature);
  display.print(F("C"));
  display.display();
}
```

### Docker - Grafana oraz InfluxDB ###

### Speech-to-text ###

### Video-to-text ###

