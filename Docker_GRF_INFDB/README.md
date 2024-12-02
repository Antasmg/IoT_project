# Docker - Grafana oraz InfluxDB #

## Spis treści ##
* [Opis](#Opis)
* [Wymagania](#Wymagania)
  * [Biblioteki](#Biblioteki)
* [Instrukcja](#Instrukcja)

## Opis ## 
Plik docker-file służacy do wykorzystywania docker-compose pozwala na jednoczesne wdrożenie kontenerów z aplikacjami Grafana, InfluxDB oraz `subscriber.py`. 
Program `subscriber.py` pełni funkcję subskrybenta MQTT i jest uruchamiany jako osobny kontener Dockera, który zapisuje dane przesłane przez ESP8266 do InfluxDB. Następnie Grafana pozwala nam na wizualizacje tych danych w formie przystępnych wykresów lub metryk.

Plik .env służy do przechowywania zmiennych środowiskowych, w normalnych okolicznościach nie byłby on przesyłany do repozytorium i byłby zawarty w pliku .gitignore ze względów bezpieczeństwa. Plik te zawiera m.in. hasła oraz loginy do aplikacji. 

Plik docker-compose.yml definiuje również woluminy, sieci oraz stałe adresy IP aplikacji. Woluminy służą do stałego przechowywania danych i w tym przypadku są one tworzone z przestrzeni dyskowej hosta kontenerów. Adresy IP zostały dodane w celu dodatkowej możliwości sprawdzania łączności pomiędzy kontenerami. 
Sieci nie są odseparowane od sieci zewnętrznych, lecz są w trybie bridge, ponieważ do obu tych kontenerów użytkownik musi uzyskiwać dostęp za pomocą przeglądarki web i protokołu HTTP. 

Połączenie pomędzy kontenerami jest realizowane w sieci dockera, więc nie wychodzi ono poza sieć kontenerową. 

## Wymagania ## 
Do wykonania tej części projektu będzie wymagane: 

* Aplikacja Docker Desktop
* Wstępnie utworzona sieć w środowisku Docker cdv_grafana
* Certyfikat `emqxsl-ca.crt` do szyfrowanego połączenia z brokerem MQTT.

W środowisku linux możemy osiągnąć to samo, lecz nie musimy nawet instalować aplikacji docker Desktop jeśli uznamy, że interfejs GUI nie jest nam potrzebny. 

### Obrazy ### 
Obrazy z których korzystaliśmy w tym projekcie to
* Oficjalny obraz Grafana:latest
* Oficjalny obraz Influxdb:latest
* Niestandardowy obraz oparty na python:3.9-slim dla subskrybenta MQTT.

## Instrukcja ##
Aby uruchomić kontenery należy:
 1. Zainstalować oraz uruchomić aplikację Docker 
 2. Utworzyć sieć wirtualną cdv_grafana za pomocą `docker network create -d bridge cdv_grafana`
 3. Wykonać plik docker-compose - znajdując się w katalogu pliku wykonać polecenie: `docker-compose up -d`
 4. Zalogować się do maszyn przez przeglądarkę, wykonać wstępną konfiguracje 
    * Grafana - `localhost:3000`
    * InfluxDB - `localhost:8086`
 5. Dodać połączenie do nowego źródła danych w Grafanie
 6. W ustawieniach połączenia:
    * Język zapytań - Flux
    * URL - http://influxdb:8086
    * Authentyfikacja - Basic Auth - Zostawiamy puste pola
    * Organisation - Podajemy nazwę skonfigurowanej organizacji 
    * Token - Podajemy wygenerowany token, w razie czego możemy wygenerować nowy w Influx w zakładce Load Data --> API Tokens
    * Default Bucket - Podajemy nazwę naszego bucketa
 7. Wykonujemy test połączenia i zapisujemy.
 8. Od tego momentu możemy tworzyć dashboard ze źródłem danych wykresów w postaci InfluxDB

## Subskrybent MQTT ##
Plik `subscriber.py` działa jako osobny kontener Dockera, który subskrybuje temat MQTT i zapisuje dane do InfluxDB.
Kontener `subscriber` subskrybuje temat `temperature` z brokera MQTT i zapisuje dane w buckecie w InfluxDB.