# Projekt IoT # 

## Autorzy projektu ## 
* Antoni Marcinek 
* Michał Piechowski

## Spis Treści ## 

1. [Projekt IoT](#projekt-iot)
   - [Autorzy projektu](#autorzy-projektu)
   - [Opis projektu](#opis-projektu)

2. [ESP32 Temperatura oraz Ekran](#esp32-temperatura-oraz-ekran)
   - [Odczyt temperatury z czujnika DHT11](#odczyt-temperatury-z-czujnika-dht11)
   - [Obsługa wyświetlacza przez I2C](#obsługa-wyświetlacza-przez-i2c)

3. [Docker - Grafana oraz InfluxDB](#docker---grafana-oraz-influxdb)
   - [InfluxDB: Przechowywanie danych pomiarowych](#influxdb-przechowywanie-danych-pomiarowych)
   - [Grafana: Wizualizacja danych](#grafana-wizualizacja-danych)

4. [Speech-to-Text](#speech-to-text)
   - [Ogólny opis technologii](#ogólny-opis-technologii-1)
   - [Działanie w projekcie](#działanie-w-projekcie)
   - [Zastosowane technologie](#zastosowane-technologie)

5. [Video-to-Text](#video-to-text)
   - [Ogólny opis technologii](#ogólny-opis-technologii-2)
   - [Działanie w projekcie](#działanie-w-projekcie-1)
   - [Zastosowane technologie](#zastosowane-technologie-1)

## Opis projektu ##

Projekt skupia się na wykorzystaniu mikrokontrolera ESP3866 do następujących zadań:

* Odczytywanie temperatury z czujnika,
* Wyświetlanie odczytu na ekranie LCD,
* Przesyłałnie komunikatów o odczytach za pomocą protokołu MQTTs do brokera,
* Przechowywanie danych za pomocą InnfluxDB stojącego jako kontener Docker,
* Wizualizacja danych za pomocą aplikacji Grafana również stojącej w kontenerze Docker,
* Wykorzystanie modułu ESP8266 w celu wykonywania operacji Speech-to-text za pomocą modelu językowego Whisper,
* Wykorzystanie modułu ESP8266 w celu rozpoznawania obrazu za pomocą modelu językowego YOLO.

Dokładniejsze opisy każdego z zastosowanych elementów projektów znajduje się w podkatalogach. Opis na tej stronie jest oględny i ma za zadanie ogólne pokazanie wykorzystanych technologii. 

### ESP32 Temperatura oraz Ekran ###

#### Odczyt temperatury z czujnika DHT11 #### 
Moduł ESP32 (lub ESP8266) współpracuje z cyfrowym czujnikiem temperatury i wilgotności DHT11. Odczyt odbywa się przez dowolny pin cyfrowy, w naszym przypadku jest to pin 11. Czujnik ten dostarcza zarówno dane o temperaturze, jak i wilgotności w formacie cyfrowym, co eliminuje konieczność stosowania przetworników analogowo-cyfrowych.

* Zasilanie czujnika:
  Czujnik DHT11 wymaga napięcia zasilania w zakresie od 3.3V do 5V. W naszym przypadku zasilanie jest dostarczane przez linię 3.3V z ESP. Linia GND zapewnia masę do układu.

* Komunikacja:
  Komunikacja między ESP a czujnikiem odbywa się w protokole jednoliniowym. Odczyt danych wymaga odpowiedniego oprogramowania, które inicjuje transmisję i interpretuje dane o temperaturze oraz wilgotności. Popularne biblioteki, takie jak Adafruit DHT Library, upraszczają implementację.
  
#### Obsługa wyświetlacza przez I2C ####
Wyświetlacz LCD został podłączony do ESP za pomocą protokołu I2C, co znacząco redukuje liczbę wymaganych linii połączeń. Wyświetlacz pełni funkcję prezentacji danych, takich jak odczyty z czujnika temperatury i inne komunikaty systemowe.

* Zasilanie wyświetlacza:
  Wyświetlacz wymaga zasilania w postaci napięcia 5V, które jest dostarczane przez ESP. Linie GND obu urządzeń są wspólne i umożliwiają prawidłowe działanie układu.

* Linie sygnałowe I2C:
  Standardowe połączenie I2C wykorzystuje dwie linie sygnałowe:

  **SCL (Serial Clock):** Linia zegara, synchronizująca komunikację. <br>
  **SDA (Serial Data):** Linia danych, przesyłająca informacje między ESP a wyświetlaczem.

### Docker - Grafana oraz InfluxDB ###

#### InfluxDB: Przechowywanie danych pomiarowych ####

InfluxDB jest bazą danych zaprojektowaną do przechowywania i analizy danych szeregów czasowych, takich jak odczyty temperatury z czujnika DHT11. W projekcie InfluxDB działa jako kontener Docker, co upraszcza proces instalacji, konfiguracji i utrzymania systemu.

* Funkcjonalność:

  Przechowywanie danych w formacie szeregów czasowych (timestamp + wartość).
  Możliwość szybkiego wyszukiwania i agregacji danych historycznych.
  Integracja z narzędziami wizualizacyjnymi, takimi jak Grafana.

* **Konfiguracja kontenera:** Kontener InfluxDB jest zarządzany za pomocą pliku docker-compose.yml. W pliku określono:

  Mapowanie portów, aby baza była dostępna na hoście.
  Wolumeny do przechowywania danych w sposób trwały.
  Ustawienia uwierzytelniania (opcjonalnie).

#### Grafana: Wizualizacja danych ####
Grafana jest narzędziem do tworzenia zaawansowanych wykresów i dashboardów, które w projekcie służą do prezentowania danych z InfluxDB. Dzięki intuicyjnemu interfejsowi użytkownika, Grafana umożliwia:

Tworzenie dynamicznych wykresów temperatury w czasie rzeczywistym.

Monitorowanie historii odczytów oraz trendów.

Konfigurację alertów w przypadku przekroczenia określonych wartości.

**Konfiguracja kontenera:** Grafana działa również w kontenerze Docker, co umożliwia łatwe wdrożenie. Połączono ją z InfluxDB, konfigurując odpowiednie źródło danych.

**Przykładowy dashboard:** W Grafanie można utworzyć dashboard wyświetlający dane z InfluxDB. Wykres temperatury w czasie prezentuje dane pobrane z MQTT, zapisane w bazie, co pozwala na:
Analizę odczytów w różnych przedziałach czasowych.
Wykorzystanie statystyk (np. średnia temperatura, maksymalna wartość).
Wizualizację danych w formie tabel, wykresów liniowych, słupkowych czy wskaźników.

### Speech-to-text ###

#### Ogólny opis technologii #### 
Moduł Speech-to-Text jest odpowiedzialny za przekształcenie mowy na tekst, wykorzystując model językowy Whisper opracowany przez OpenAI. Whisper to zaawansowany system transkrypcji mowy, który cechuje się wysoką dokładnością oraz wsparciem dla wielu języków, w tym języka polskiego. Model został zaprojektowany do analizy strumieni audio i generowania tekstu w czasie rzeczywistym lub na podstawie nagrań.

* **Działanie w projekcie:**

*System nasłuchuje strumień dźwiękowy lub przetwarza plik audio.
*Po zidentyfikowaniu wypowiedzianych słów przekształca je w tekst, który jest przesyłany za pomocą protokołu MQTT do brokera.
*ESP8266 odbiera tekst i wyświetla go na LCD, prezentując wyniki na górnej linii wyświetlacza.

### Video-to-text ###

Ogólny opis technologii
Moduł Video-to-Text zajmuje się przetwarzaniem obrazu w czasie rzeczywistym w celu identyfikacji obiektów oraz ich opisywania w postaci tekstowej. W projekcie wykorzystano model YOLO (You Only Look Once) do analizy obrazu. YOLO jest zaawansowaną siecią neuronową zdolną do jednoczesnej detekcji i klasyfikacji obiektów na obrazach lub w strumieniach wideo.

#### Działanie w projekcie: ####

*System analizuje obrazy z kamery wideo w czasie rzeczywistym lub przetwarza zapisane materiały.
*Model YOLO identyfikuje obiekty w kadrze, przypisuje im nazwy (np. "cat", "car", "person") i generuje odpowiedni tekst.
*Wynikowa informacja tekstowa jest przesyłana za pomocą MQTT do brokera.
*ESP8266 odbiera tekst i wyświetla go na LCD, prezentując nazwy obiektów na dolnej linii wyświetlacza.

* **Zastosowane technologie:**

*YOLO: Model wyróżnia się szybkością działania, co czyni go idealnym rozwiązaniem dla systemów pracujących w czasie rzeczywistym.
*Python i OpenCV: Do obsługi strumieni wideo oraz integracji z modelem YOLO.
*MQTT: Zapewnia komunikację między modułem analizy obrazu a ESP8266.

