# ESP8266 - Moduł komunikacyjny MQTT

## Spis treści
- [Opis](#opis)
- [Wymagania](#wymagania)
- [Schemat połączeń](#schemat-połączeń)
- [Konfiguracja](#konfiguracja)
- [Funkcjonalność](#funkcjonalność)
- [Użycie](#użycie)
- [Struktura kodu](#struktura-kodu)
- [Rozwiązywanie problemów](#rozwiązywanie-problemów)

## Opis
Moduł ESP8266 służy jako urządzenie IoT, które:
- Odczytuje dane z czujnika temperatury DHT11
- Wyświetla informacje na wyświetlaczu LCD I2C
- Komunikuje się poprzez protokół MQTT z brokerem
- Obsługuje bezpieczną komunikację poprzez SSL/TLS

## Wymagania
### Sprzęt
- ESP8266 (NodeMCU lub podobny)
- Czujnik DHT11
- Wyświetlacz LCD I2C (16x2)
- Przewody połączeniowe

### Biblioteki
```cpp
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <time.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>
```

## Schemat połączeń
- DHT11:
  - PIN danych: GPIO14 (D5)
  - VCC: 3.3V
  - GND: GND
- LCD I2C:
  - SDA: GPIO4 (D2)
  - SCL: GPIO5 (D1)
  - VCC: 3.3V
  - GND: GND

## Konfiguracja
### Parametry WiFi
```cpp
const char *ssid = "TWÓJ_SSID";
const char *password = "TWOJE_HASŁO";
```

### Parametry MQTT
```cpp
const int mqtt_port = 8883;
const char *mqtt_broker = "n9ee4478.ala.eu-central-1.emqxsl.com";
const char *mqtt_speech_to_text_topic = "speech_to_text";
const char *mqtt_video_to_text_topic = "video_to_text";
const char *mqtt_temperature_topic = "temperature";
const char *mqtt_username = "esp8266";
const char *mqtt_password = "public";
```

## Użycie
Po uruchomieniu, urządzenie:
1. Łączy się z siecią WiFi
2. Synchronizuje czas z serwerem NTP
3. Nawiązuje połączenie z brokerem MQTT
4. Rozpoczyna:
   - Odczyt temperatury
   - Nasłuchiwanie wiadomości MQTT
   - Wyświetlanie informacji na LCD

## Struktura kodu
Główne funkcje:
```cpp
void connectToWiFi()      // Połączenie z WiFi
void syncTime()           // Synchronizacja czasu
void measure()            // Pomiar temperatury
void connectToMQTT()      // Połączenie z brokerem MQTT
void mqttCallback()       // Obsługa przychodzących wiadomości
```

## Rozwiązywanie problemów
1. Problem z połączeniem WiFi:
   - Sprawdź poprawność SSID i hasła
   - Upewnij się, że sieć jest w zasięgu

2. Problem z MQTT:
   - Sprawdź poprawność certyfikatu SSL
   - Zweryfikuj dane dostępowe do brokera
   - Sprawdź logi Serial Monitor

3. Problem z czujnikiem DHT11:
   - Sprawdź połączenia
   - Zweryfikuj numer pinu w kodzie
   - Upewnij się, że biblioteka jest poprawnie zainstalowana

4. Problem z wyświetlaczem LCD:
   - Sprawdź adres I2C (domyślnie 0x27)
   - Zweryfikuj połączenia SDA i SCL
