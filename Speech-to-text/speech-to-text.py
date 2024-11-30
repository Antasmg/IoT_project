import pyaudio
import wave
import whisper
import os

# Konfiguracja audio
CHUNK = 1024  # Wielkość bufora
FORMAT = pyaudio.paInt16  # Format próbek audio
CHANNELS = 1  # Liczba kanałów (mono)
RATE = 16000  # Częstotliwość próbkowania
RECORD_SECONDS = 5  # Czas nagrania w sekundach
OUTPUT_FILENAME = "output.wav"  # Nazwa pliku wyjściowego

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
    with wave.open(OUTPUT_FILENAME, 'wb') as wf:
        wf.setnchannels(CHANNELS)
        wf.setsampwidth(audio.get_sample_size(FORMAT))
        wf.setframerate(RATE)
        wf.writeframes(b''.join(frames))

# Funkcja transkrypcji za pomocą Whisper
def transcribe_audio():
    model = whisper.load_model("medium")
    try:
         model = whisper.load_model("medium")
         if not os.path.exists(OUTPUT_FILENAME):
             print(f"Błąd: Plik {OUTPUT_FILENAME} nie istnieje")
             return
         result = model.transcribe(OUTPUT_FILENAME, language="pl")
         print("Transkrypcja:", result["text"])
         print("Transkrypcja:", result)
    except Exception as e:
         print(f"Wystąpił błąd podczas transkrypcji: {e}")

# Główna funkcja
if __name__ == "__main__":
    record_audio()
    transcribe_audio()
