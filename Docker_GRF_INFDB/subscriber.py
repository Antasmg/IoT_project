import paho.mqtt.client as mqtt
from influxdb_client import InfluxDBClient, Point
from influxdb_client.client.write_api import SYNCHRONOUS
import json
import time
import os
from datetime import datetime
import ssl

# Konfiguracja MQTT
MQTT_BROKER = "n9ee4478.ala.eu-central-1.emqxsl.com"
MQTT_PORT = 8883
MQTT_TOPIC = "temperature"
MQTT_USERNAME = "laptop"
MQTT_PASS = "public"

# Konfiguracja InfluxDB
INFLUXDB_URL = "http://localhost:8086"
INFLUXDB_TOKEN = "wPN8tQSMj-zwc5j9NDevqHRvLoBXwa6uXrA21Wx0zf--BE1YNJlLXpPD7mdlsfvzJkXRAy8RJ13Y8P52A-pG6g=="  # Token do autoryzacji
INFLUXDB_ORG = "cdv"          # Nazwa organizacji
INFLUXDB_BUCKET = "cdv"  # Nazwa bucketa

# Inicjalizacja klienta InfluxDB
influx_client = InfluxDBClient(
    url=INFLUXDB_URL,
    token=INFLUXDB_TOKEN,
    org=INFLUXDB_ORG
)
write_api = influx_client.write_api(write_options=SYNCHRONOUS)

def on_connect(client, userdata, flags, rc):
    print("Połączono z MQTT Brokerem z kodem: " + str(rc))
    client.subscribe(MQTT_TOPIC)
    print(f"Zasubskrybowano topic: {MQTT_TOPIC}")

def on_message(client, userdata, msg):
    """Callback wywoływany po otrzymaniu wiadomości MQTT"""
    print("Receive a message")
    try:
        # Próba parsowania otrzymanych danych jako JSON
        data = json.loads(msg.payload.decode())
        
        # Tworzenie punktu danych dla InfluxDB
        point = Point("temperature_measurement")\
            .field("temperature", float(data.get('temperature', 0)))

        # Zapis do InfluxDB
        write_api.write(bucket=INFLUXDB_BUCKET, record=point)
        print(f"Zapisano dane: {data}")

    except Exception as e:
        print(f"Błąd podczas przetwarzania wiadomości: {e}")

def connect_to_mqtt():
    print("Łączenie z brokerem MQTT...")
    client = mqtt.Client()
    client.username_pw_set(MQTT_USERNAME, MQTT_PASS)
    client.on_connect = on_connect
    pwd = os.getcwd()
    print(pwd)
    cert = pwd + "/emqxsl-ca.crt"
    client.tls_set(cert, tls_version=ssl.PROTOCOL_TLSv1_2)
    client.tls_insecure_set(True)
    client.on_message = on_message
    try:
        # Połączenie z brokerem MQTT
        client.connect(MQTT_BROKER, MQTT_PORT, 60)
        
        # Uruchomienie pętli obsługi wiadomości
        client.loop_forever()

    except KeyboardInterrupt:
        print("\nZatrzymywanie programu...")
        client.disconnect()
        influx_client.close()

def main():
    connect_to_mqtt()

if __name__ == "__main__":
    main()