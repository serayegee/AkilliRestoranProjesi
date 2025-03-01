#define BLYNK_TEMPLATE_ID "TMPL6u6PfHHKS"
#define BLYNK_DEVICE_NAME "ESP8266-1"
#define BLYNK_AUTH_TOKEN "DljEdGkrAWdGFpg-kuMH5sMSrb3Zhk1U"
#define BLYNK_TEMPLATE_NAME "1ESP8266"

#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>


// Wi-Fi Bilgileri
char ssid[] = "x";
char pass[] = "x";

// Pin Tanımları
int sensor = A0; 
#define SS_PIN D4
#define RST_PIN D3 
#include <SPI.h>
#include <MFRC522.h>
MFRC522 mfrc522(SS_PIN, RST_PIN);

// RFID LED'ler
int yesilLed = D1;
int kirmiziLed = D2;

#define R1 10000.0 // Referans direnç
#define c1 1.009249522e-03
#define c2 2.378405444e-04
#define c3 2.019202697e-07

float readTemperature(int Vo) {
  float R2, logR2, T;

  R2 = R1 * (1023.0 / (float)Vo - 1.0); 
  logR2 = log(R2);                      
  T = (1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2)); // Kelvin sıcaklık
  T = T - 184.15;                      
  return T;
}

BlynkTimer timer;

void sendTemperature() {

   int sensorValue = analogRead(sensor);        // Analog sıcaklık verisini oku
  float temperature = readTemperature(sensorValue); // Sıcaklık hesapla
  Blynk.virtualWrite(V0, temperature);
}

void setup() {
  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  pinMode(yesilLed, OUTPUT);
  pinMode(kirmiziLed, OUTPUT);
  SPI.begin();
  mfrc522.PCD_Init();

  timer.setInterval(5000L, sendTemperature); // Sıcaklık verisini her 5 saniyede
}

void loop() {
  Blynk.run();
  timer.run();

  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    String content = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      content.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    content.toUpperCase();

    // RFID kartı kontrol et
    if (content == "73CC9511") { 
      Blynk.virtualWrite(V1, "Giriş Başarılı!");
      digitalWrite(yesilLed, HIGH);
      delay(500);
      digitalWrite(yesilLed, LOW);
    } else {
      Blynk.virtualWrite(V1, "Kart Tanımlanmadı!");
      digitalWrite(kirmiziLed, HIGH);
      delay(500);
      digitalWrite(kirmiziLed, LOW);
    }
  }
}
