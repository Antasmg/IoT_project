from ultralytics import YOLO
import cv2
import time
import ssl
import paho.mqtt.client as mqtt
import os

# Konfiguracja MQTT
MQTT_BROKER = "n9ee4478.ala.eu-central-1.emqxsl.com"
MQTT_PORT = 8883
MQTT_TOPIC = "video_to_text"
MQTT_USERNAME = "laptop"
MQTT_PASS = "public"

previous_message = ""


def on_connect(client, userdata, flags, rc):
    print("Połączono z MQTT Brokerem z kodem: " + str(rc))


def send_message(client, detected_objects):
    sleep_time = 0.25
    message = f"{', '.join(detected_objects)}"
    
    global previous_message
    if previous_message != detected_objects:
        result = client.publish(MQTT_TOPIC, message)
        sleep_time = 1

    previous_message = detect_objects
    status = result[0]
    if status == 0:
        print(f"Wysłano wiadomość do {MQTT_TOPIC}: {message}")
    else:
        print(f"Błąd podczas wysyłania wiadomości do {MQTT_TOPIC}")
    time.sleep(sleep_time)

def detect_objects(client):
    model = YOLO("yolov8n.pt")

    cap = cv2.VideoCapture(0)
    if not cap.isOpened():
        print("Nie można otworzyć kamery.")
        return

    print("Rozpoczynam wykrywanie obiektów. Wciśnij 'q', aby zakończyć.")

    while True:
        ret, frame = cap.read()
        if not ret:
            print("Nie udało się odczytać klatki.")
            break

        results = model(frame)

        detected_objects = []
        for result in results:
            for box in result.boxes:
                label = model.names[int(box.cls)]
                detected_objects.append(label)

        if detected_objects:
            print(f"Wykryte obiekty: {', '.join(detected_objects)}")
            send_message(client, detected_objects)
        else:
            print("Nie wykryto obiektów.")

        annotated_frame = results[0].plot()
        cv2.imshow("YOLOv8 Detection", annotated_frame)

        if cv2.waitKey(1) & 0xFF == ord("q"):
            break

    cap.release()
    cv2.destroyAllWindows()


def connect_to_mqtt():
    print("Łączenie z brokerem MQTT...")
    client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION1)
    client.username_pw_set(MQTT_USERNAME, MQTT_PASS)
    client.on_connect = on_connect
    pwd = os.getcwd()
    print(pwd)
    cert = pwd + "/emqxsl-ca.crt"
    client.tls_set(cert, tls_version=ssl.PROTOCOL_TLSv1_2)
    client.tls_insecure_set(True)
    client.connect(MQTT_BROKER, MQTT_PORT, 60)
    client.loop_start()

    return client


def main():
    print("Start programu...")
    clinet = connect_to_mqtt()
    detect_objects(clinet)


if __name__ == "__main__":
    main()
