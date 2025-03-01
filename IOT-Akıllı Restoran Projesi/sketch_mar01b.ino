#define BLYNK_TEMPLATE_ID "TMPdL6pFHHKS"
#define BLYNK_TEMPLATE_NAME "1ESP8266"
#define BLYNK_DEVICE_NAME "2ESP8266"
#define BLYNK_AUTH_TOKEN "lU6JAKaA16_xnTqG9khJ7Jt2-Q0AE50G"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Wi-Fi Bilgileri
char ssid[] = "x";
char pass[] = "x";

// Pin Tanımları
#define BUTTON_PIN D1
#define LED_PIN D7

unsigned long startTime = 0;
unsigned long elapsedTime = 0;
bool isButtonPressed = false;

BlynkTimer timer;

// Işık Verisi ve LED Kontrolü
void manageLDRAndLED() {
    int isik = analogRead(A0); // LDR'den ışık değerini oku
    Serial.print("Işık Değeri: ");
    Serial.println(isik);

    // LDR değerine göre LED kontrolü
    if (isik > 300) { 
        digitalWrite(LED_PIN, LOW); // LED kapalı
        Blynk.virtualWrite(V0, 0); // Blynk LDR kapalı bilgisi
    } else if (isik <= 300) { 
        digitalWrite(LED_PIN, HIGH); // LED açık
        Blynk.virtualWrite(V0, 1); // Blynk LDR açık bilgisi
    }

    int buttonState = digitalRead(BUTTON_PIN);
    
    if (buttonState == LOW) { // Buton basılıysa
        if (!isButtonPressed) {
            startTime = millis();
            isButtonPressed = true;
            Blynk.virtualWrite(V1, "Müşteri Geldi!");
        }
    } else { // Buton bırakıldıysa
        if (isButtonPressed) {
            elapsedTime = millis() - startTime;
            Blynk.virtualWrite(V1, "Geçen Süre: " + String(elapsedTime / 1000) + " saniye");
            isButtonPressed = false;
        }
    }
}

// Buton durumunu periyodik olarak kontrol eden fonksiyon
void checkButtonState() {
    int buttonState = digitalRead(BUTTON_PIN);
    if (buttonState == HIGH && isButtonPressed) {
        elapsedTime = millis() - startTime;
        Blynk.virtualWrite(V1, "Müşteri Gitti! Geçen Süre: " + String(elapsedTime / 1000) + " saniye");
        isButtonPressed = false;
    }
}

void setup() {
    Serial.begin(9600);
    Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(LED_PIN, OUTPUT);

    // Zamanlayıcıları başlat
    timer.setInterval(1000L, manageLDRAndLED); // 1 saniyede bir LDR kontrolu
    timer.setInterval(100L, checkButtonState); // 100 ms’de bir buton durumu kontrolu
}

void loop() {
    Blynk.run();
    timer.run();
}
