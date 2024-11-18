#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <time.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>

#define DHTPIN 14     // Digital pin connected to the DHT sensor

#define DHTTYPE    DHT11     // DHT 11
unsigned long lastDHTCheck = 0;      // Czas ostatniego odczytu DHT
const unsigned long contactCheckInterval = 3000; // Czas w ms pomiędzy odczytami kontaktrona
const unsigned long dhtCheckInterval = 6000;    // Czas w ms pomiędzy odczytami DHT

DHT dht(DHTPIN, DHTTYPE);
float t = 0.0;
float h = 0.0;

// WiFi credentials
const char *ssid = "A12";             // Replace with your WiFi name
const char *password = "bgzg2418";   // Replace with your WiFi password

// MQTT Broker settings
const int mqtt_port = 8883;  // MQTT port (TLS)
const char *mqtt_broker = "n9ee4478.ala.eu-central-1.emqxsl.com";  // EMQX broker endpoint
const char *mqtt_speech_to_text_topic = "speech_to_text";     // MQTT topic
const char *mqtt_video_to_text_topic = "video_to_text";     // MQTT topic
const char *mqtt_temperature_topic = "temperature";     // MQTT topic
const char *mqtt_username = "esp8266";  // MQTT username for authentication
const char *mqtt_password = "public";  // MQTT password for authentication

// NTP Server settings
const char *ntp_server = "pool.ntp.org";     // Default NTP server
const long gmt_offset_sec = 0;            // GMT offset in seconds (adjust for your time zone)
const int daylight_offset_sec = 0;        // Daylight saving time offset in seconds

// WiFi and MQTT client initialization
BearSSL::WiFiClientSecure espClient;
PubSubClient mqtt_client(espClient);

// SSL certificate for MQTT broker
// Load DigiCert Global Root G2, which is used by EMQX Public Broker: broker.emqx.io
static const char ca_cert[]
PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDrzCCApegAwIBAgIQCDvgVpBCRrGhdWrJWZHHSjANBgkqhkiG9w0BAQUFADBh
MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3
d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBD
QTAeFw0wNjExMTAwMDAwMDBaFw0zMTExMTAwMDAwMDBaMGExCzAJBgNVBAYTAlVT
MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j
b20xIDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IENBMIIBIjANBgkqhkiG
9w0BAQEFAAOCAQ8AMIIBCgKCAQEA4jvhEXLeqKTTo1eqUKKPC3eQyaKl7hLOllsB
CSDMAZOnTjC3U/dDxGkAV53ijSLdhwZAAIEJzs4bg7/fzTtxRuLWZscFs3YnFo97
nh6Vfe63SKMI2tavegw5BmV/Sl0fvBf4q77uKNd0f3p4mVmFaG5cIzJLv07A6Fpt
43C/dxC//AH2hdmoRBBYMql1GNXRor5H4idq9Joz+EkIYIvUX7Q6hL+hqkpMfT7P
T19sdl6gSzeRntwi5m3OFBqOasv+zbMUZBfHWymeMr/y7vrTC0LUq7dBMtoM1O/4
gdW7jVg/tRvoSSiicNoxBN33shbyTApOB6jtSj1etX+jkMOvJwIDAQABo2MwYTAO
BgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4EFgQUA95QNVbR
TLtm8KPiGxvDl7I90VUwHwYDVR0jBBgwFoAUA95QNVbRTLtm8KPiGxvDl7I90VUw
DQYJKoZIhvcNAQEFBQADggEBAMucN6pIExIK+t1EnE9SsPTfrgT1eXkIoyQY/Esr
hMAtudXH/vTBH1jLuG2cenTnmCmrEbXjcKChzUyImZOMkXDiqw8cvpOp/2PV5Adg
06O/nVsJ8dWO41P0jmP6P6fbtGbfYmbW0W5BjfIttep3Sp+dWOIrWcBAI+0tKIJF
PnlUkiaY4IBIqDfv8NZ5YBberOgOzW6sRBc4L0na4UU+Krk2U886UAb3LujEV0ls
YSEY1QSteDwsOoBrp+uvFRTp2InBuThs4pFsiv9kuXclVzDAGySj4dzp30d8tbQk
CAUw7C29C79Fv1C5qfPrmAESrciIxpg0X40KPMbp1ZWVbd4=
-----END CERTIFICATE-----
)EOF";

// Load DigiCert Global Root CA ca_cert, which is used by EMQX Serverless Deployment
/*
static const char ca_cert[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDrzCCApegAwIBAgIQCDvgVpBCRrGhdWrJWZHHSjANBgkqhkiG9w0BAQUFADBh
MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3
d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBD
QTAeFw0wNjExMTAwMDAwMDBaFw0zMTExMTAwMDAwMDBaMGExCzAJBgNVBAYTAlVT
MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j
b20xIDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IENBMIIBIjANBgkqhkiG
9w0BAQEFAAOCAQ8AMIIBCgKCAQEA4jvhEXLeqKTTo1eqUKKPC3eQyaKl7hLOllsB
CSDMAZOnTjC3U/dDxGkAV53ijSLdhwZAAIEJzs4bg7/fzTtxRuLWZscFs3YnFo97
nh6Vfe63SKMI2tavegw5BmV/Sl0fvBf4q77uKNd0f3p4mVmFaG5cIzJLv07A6Fpt
43C/dxC//AH2hdmoRBBYMql1GNXRor5H4idq9Joz+EkIYIvUX7Q6hL+hqkpMfT7P
T19sdl6gSzeRntwi5m3OFBqOasv+zbMUZBfHWymeMr/y7vrTC0LUq7dBMtoM1O/4
gdW7jVg/tRvoSSiicNoxBN33shbyTApOB6jtSj1etX+jkMOvJwIDAQABo2MwYTAO
BgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4EFgQUA95QNVbR
TLtm8KPiGxvDl7I90VUwHwYDVR0jBBgwFoAUA95QNVbRTLtm8KPiGxvDl7I90VUw
DQYJKoZIhvcNAQEFBQADggEBAMucN6pIExIK+t1EnE9SsPTfrgT1eXkIoyQY/Esr
hMAtudXH/vTBH1jLuG2cenTnmCmrEbXjcKChzUyImZOMkXDiqw8cvpOp/2PV5Adg
06O/nVsJ8dWO41P0jmP6P6fbtGbfYmbW0W5BjfIttep3Sp+dWOIrWcBAI+0tKIJF
PnlUkiaY4IBIqDfv8NZ5YBberOgOzW6sRBc4L0na4UU+Krk2U886UAb3LujEV0ls
YSEY1QSteDwsOoBrp+uvFRTp2InBuThs4pFsiv9kuXclVzDAGySj4dzp30d8tbQk
CAUw7C29C79Fv1C5qfPrmAESrciIxpg0X40KPMbp1ZWVbd4=
-----END CERTIFICATE-----
)EOF";
*/

// LCD number if columns and rows
int lcdColumns = 16;
int lcdRows = 2;

// set LCD address, number of columns and rows
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

// Function declarations
void connectToWiFi();

void connectToMQTT();

void syncTime();

void mqttCallback(char *topic, byte *payload, unsigned int length);

void setup() {
    Serial.begin(115200);
    lcd.init();
    lcd.backlight();
    connectToWiFi();
    syncTime();  // X.509 validation requires synchronization time
    mqtt_client.setServer(mqtt_broker, mqtt_port);
    mqtt_client.setCallback(mqttCallback);
    connectToMQTT();
    dht.begin();
}

void connectToWiFi() {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");
}

void syncTime() {
    configTime(gmt_offset_sec, daylight_offset_sec, ntp_server);
    Serial.print("Waiting for NTP time sync: ");
    while (time(nullptr) < 8 * 3600 * 2) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("Time synchronized");
    struct tm timeinfo;
    if (getLocalTime(&timeinfo)) {
        Serial.print("Current time: ");
        Serial.println(asctime(&timeinfo));
    } else {
        Serial.println("Failed to obtain local time");
    }
}

void measure(){
  unsigned long currentMillis = millis();
  if (currentMillis - lastDHTCheck >= dhtCheckInterval) {
    lastDHTCheck = currentMillis; // Zaktualizuj czas ostatniego odczytu

    float h = dht.readHumidity();
    float t = dht.readTemperature();

    if (isnan(h) || isnan(t)) {
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
    }

    // Tworzenie JSON-a z danymi
    String payload = String(t);

    mqtt_client.publish(mqtt_temperature_topic, payload.c_str());

    Serial.print(F("Wilgotność: "));
    Serial.print(h);
    Serial.print(F("%  Temperatura: "));
    Serial.print(t);
    Serial.print(F("°C \n"));
  }
}

void connectToMQTT() {
    BearSSL::X509List serverTrustedCA(ca_cert);
    espClient.setTrustAnchors(&serverTrustedCA);
    while (!mqtt_client.connected()) {
        String client_id = "esp8266-client-" + String(WiFi.macAddress());
        Serial.printf("Connecting to MQTT Broker as %s.....\n", client_id.c_str());
        if (mqtt_client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
            Serial.println("Connected to MQTT broker");
            mqtt_client.subscribe(mqtt_speech_to_text_topic);
            mqtt_client.subscribe(mqtt_video_to_text_topic);
            // Publish message upon successful connection
            mqtt_client.publish(mqtt_speech_to_text_topic, "Hi EMQX I'm ESP8266 ^^");
            mqtt_client.publish(mqtt_video_to_text_topic, "Hi EMQX I'm ESP8266 ^^");
            mqtt_client.publish(mqtt_temperature_topic, "Hi EMQX I'm ESP8266 ^^");
        } else {
            char err_buf[128];
            espClient.getLastSSLError(err_buf, sizeof(err_buf));
            Serial.print("Failed to connect to MQTT broker, rc=");
            Serial.println(mqtt_client.state());
            Serial.print("SSL error: ");
            Serial.println(err_buf);
            delay(5000);
        }
    }
}

void mqttCallback(char *topic, byte *payload, unsigned int length) {
    String message = "";

    int cursorPosition = (strcmp(topic, "speech_to_text") == 0) ? 0 : 1;

    for (unsigned int i = 0; i < length; i++) {
        message += (char)payload[i];
    }

    if (length > 16){
      for (unsigned int start = 0; start < length; start += 16) {
            String fragment = message.substring(start, start + 16);
            lcd.setCursor(0, cursorPosition);
            lcd.print("                "); // Czyszczenie linii
            lcd.setCursor(0, cursorPosition);
            lcd.print(fragment);

            // Czekaj chwilę (np. 1 sekunda)
            delay(250);
        }
    }
    else{
      lcd.setCursor(0, cursorPosition);
      lcd.print("                ");
      lcd.setCursor(0, cursorPosition);

      lcd.print(message);
    }
    
    Serial.print("Message received on topic ");
    Serial.print(topic);
    Serial.print(": ");
    Serial.println(message);
}

void loop() {
    lcd.setCursor(0, 0);
    if (!mqtt_client.connected()) {
        connectToMQTT();
    }
    mqtt_client.loop();

    measure();
}
