import pyaudio
import wave
import whisper
import os
import paho.mqtt.client as mqtt
import ssl

# Konfiguracja audio
CHUNK = 1024  # Wielkość bufora
FORMAT = pyaudio.paInt16  # Format próbek audio
CHANNELS = 1  # Liczba kanałów (mono)
RATE = 16000  # Częstotliwość próbkowania
RECORD_SECONDS = 3  # Czas nagrania w sekundach
OUTPUT_FILENAME = "output.wav"  # Nazwa pliku wyjściowego
OUTPUT_PATH = "/output.wav"  # Nazwa pliku wyjściowego

MQTT_BROKER = "n9ee4478.ala.eu-central-1.emqxsl.com"
MQTT_PORT = 8883
MQTT_TOPIC = "video_to_text"
MQTT_USERNAME = "laptop"
MQTT_PASS = "public"

def on_connect(client, userdata, flags, rc):
    print("Połączono z MQTT Brokerem z kodem: " + str(rc))

def send_message(client, message):
    result = client.publish(MQTT_TOPIC, message)

    status = result[0]
    if status == 0:
        print(f"Wysłano wiadomość do {MQTT_TOPIC}: {message}")
    else:
        print(f"Błąd podczas wysyłania wiadomości do {MQTT_TOPIC}")

# Funkcja nagrywania audio
def record_audio():
    audio = pyaudio.PyAudio()
    stream = audio.open(format=FORMAT, channels=CHANNELS,
                        rate=RATE, input=True,
                        frames_per_buffer=CHUNK)
    print("Nagrywanie...")
    frames = []

    for _ in range(0, int(RATE / CHUNK * RECORD_SECONDS)):
        data = stream.read(CHUNK)
        frames.append(data)

    print("Nagrywanie zakończone.")
    stream.stop_stream()
    stream.close()
    audio.terminate()

    # Zapis do pliku WAV
    output_file = os.path.join(os.getcwd(), "..", "output.wav")
    with wave.open(output_file, 'wb') as wf:
        wf.setnchannels(CHANNELS)
        wf.setsampwidth(audio.get_sample_size(FORMAT))
        wf.setframerate(RATE)
        wf.writeframes(b''.join(frames))

# Funkcja transkrypcji za pomocą Whisper
def transcribe_audio(client):
    model = whisper.load_model("tiny")
    try:
        output_file = os.path.join(os.getcwd(), "..", "output.wav")
        
        if not os.path.exists(output_file):
            print(f"Błąd: Plik {output_file} nie istnieje")
            return
        
        result = model.transcribe(output_file, language="pl")
        print("Transkrypcja:", result["text"])
        send_message(client, result["text"])
    except Exception as e:
        print(f"Wystąpił błąd podczas transkrypcji: {e}")

def connect_to_mqtt():
    print("Łączenie z brokerem MQTT...")
    client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION1)
    client.username_pw_set(MQTT_USERNAME, MQTT_PASS)
    client.on_connect = on_connect
    pwd = os.getcwd()
    print(pwd)
    cert = pwd + "/../emqxsl-ca.crt"
    client.tls_set(cert, tls_version=ssl.PROTOCOL_TLSv1_2)
    client.tls_insecure_set(True)
    client.connect(MQTT_BROKER, MQTT_PORT, 60)
    client.loop_start()

    return client

# Główna funkcja
if __name__ == "__main__":
    client = connect_to_mqtt()

    while True:
        record_audio()
        transcribe_audio(client)
