# Docker - Grafana oraz InfluxDB #

## Spis treści ##
* [Opis](#Opis)
* [Wymagania](#Wymagania)
  * [Biblioteki](#Biblioteki)
* [Instrukcja](#Instrukcja)

## Opis ## 
Plik docker-file służacy do wykorzystywania docker-compose pozwala na jednoczesne wdrożenie kontenerów z aplikacjami Grafana oraz InfluxDB. 
W projekcie kontener z InfluxDB służy do odbierania danych pochodzących z odczytów czujnika temperatury DHT11 podłączonego do ESP8266. Następnie Grafana pozwala nam na wizualizacje tych danych w formie przystępnych wykresów lub metryk.

Plik .env służy do przechowywania zmiennych środowiskowych, w normalnych okolicznościach nie byłby on przesyłany do repozytorium i byłby zawarty w pliku .gitignore ze względów bezpieczeństwa. Plik te zawiera m.in. hasła oraz loginy do aplikacji. 

Plik docker-compose.yml definiuje również woluminy, sieci oraz stałe adresy IP aplikacji. Woluminy służą do stałego przechowywania danych i w tym przypadku są one tworzone z przestrzeni dyskowej hosta kontenerów. Adresy IP zostały dodane w celu dodatkowej możliwości sprawdzania łączności pomiędzy kontenerami. 
Sieci nie są odseparowane od sieci zewnętrznych, lecz są w trybie bridge, ponieważ do obu tych kontenerów użytkownik musi uzyskiwać dostęp za pomocą przeglądarki web i protokołu HTTP. 

Połączenie pomędzy kontenerami jest realizowane w sieci dockera, więc nie wychodzi ono poza sieć kontenerową. 

## Wymagania ## 
Do wykonania tej części projektu będzie wymagane: 

* Aplikacja Docker Desktop
* Wstępnie utworzona sieć w środowisku Docker cdv_grafana

W środowisku linux możemy osiągnąć to samo, lecz nie musimy nawet instalować aplikacji docker Desktop jeśli uznamy, że interfejs GUI nie jest nam potrzebny. 

### Obrazy ### 
Obrazy z których korzystaliśmy w tym projekcie to
* Oficjalny obraz Grafana:latest
* Oficjalny obraz Influxdb:latest

## Instrukcja ##
