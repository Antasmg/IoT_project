# IoT Project: Video-to-Text with ESP8266 and MQTT

## Opis projektu
Ten projekt łączy przetwarzanie wideo na tekst przy użyciu modelu YOLOv8 z komunikacją w IoT przy pomocy mikrokontrolera ESP8266 oraz protokołu MQTT. Całość jest obsługiwana w środowisku Docker, co ułatwia wdrażanie i skalowanie projektu.

## Struktura projektu
TODO


## Wymagania
- Python 3.8+
- Docker i Docker Compose
- Mikrokontroler ESP8266
- Certyfikat SSL (`emqxsl-ca.crt`)

## Funkcjonalności
1. **Przetwarzanie wideo na tekst**:
   - Wykrywanie obiektów w czasie rzeczywistym za pomocą YOLOv8.
   - Wysyłanie informacji o wykrytych obiektach do brokera MQTT.
2. **Integracja IoT**:
   - Mikrokontroler ESP8266 odbiera dane i wykonuje odpowiednie akcje.

## Instrukcja uruchomienia

### 1. Konfiguracja środowiska Docker
1. Uzupełnij plik `.env` w katalogu `Docker_GRF_INFDB` odpowiednimi zmiennymi środowiskowymi (jeśli są wymagane).
2. Uruchom kontenery:
   ```bash
   cd Docker_GRF_INFDB
   docker-compose up -d
   
### 2. Uruchomienie skryptu Python
1. Przejdź do katalogu video-to-text.
2. Uruchom skrypt:
   ```bash
   python video-to-text.py

### 3. ESP8266
Otwórz plik `esp8266.ino` w środowisku Arduino IDE.
Wgraj kod na mikrokontroler.
