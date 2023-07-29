#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define triggerPin  15
#define echoPin     13
#define relayPin    12 // Ganti dengan pin yang sesuai untuk mengontrol relay

LiquidCrystal_I2C lcd(0x27, 16, 2); // Ganti alamat 0x27 dengan alamat I2C LCD Anda, sesuaikan jumlah kolom dan baris LCD jika berbeda

const int sensorPin = A0; // A0 FOR ARDUINO/ 36 FOR ESP
const int clearThreshold = 20;
const int cloudyThreshold = 50;
const unsigned long relayDelay = 5000; // Waktu delay relay (dalam milidetik)

bool isRelayOn = false;
unsigned long relayStartTime = 0;

void setup() {
  Serial.begin(9600);
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(relayPin, OUTPUT);
  
  lcd.init(); // Initialize LCD
  lcd.backlight(); // Turn on LCD backlight

  lcd.begin(16, 2); // Ganti ukuran kolom dan baris sesuai dengan layar LCD Anda
  lcd.print("Jarak: "); // Menampilkan teks awal di layar LCD
}

void loop() {
  // Kode untuk mengukur jarak
  long duration, jarak;
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2); 
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10); 
  digitalWrite(triggerPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  jarak = (duration / 2) / 29.1;

  Serial.println("jarak :");
  Serial.print(jarak);
  Serial.println(" cm");

  lcd.setCursor(7, 0); // Set posisi kursor di layar LCD untuk menampilkan jarak
  lcd.print(jarak);
  lcd.print(" cm   "); // Untuk membersihkan tampilan lama

  // Kontrol relay berdasarkan jarak
  if (jarak <= 30) {
    digitalWrite(relayPin, LOW); // Matikan relay jika jarak <= 30 cm
  } else if (jarak >= 50) {
    digitalWrite(relayPin, HIGH); // Aktifkan relay jika jarak >= 50 cm
  }

  // Kode untuk membaca nilai turbidity dan menampilkan di LCD
  int sensorValue = analogRead(sensorPin);
  int turbidity = map(sensorValue, 0, 750, 0, 100);

  lcd.setCursor(0, 1);
  if (turbidity < clearThreshold) {
    lcd.print("It's CLEAR    ");
    Serial.println("It's CLEAR");
  } else if (turbidity < cloudyThreshold) {
    lcd.print("It's CLOUDY   ");
    Serial.println("It's CLOUDY");
  } else {
    lcd.print("It's DIRTY    ");
    Serial.println("It's DIRTY");
  }

  // Menyalakan relay jika kondisi adalah "Cloudy" atau "Dirty"
  if (turbidity >= cloudyThreshold) {
    if (!isRelayOn) {
      isRelayOn = true;
      relayStartTime = millis(); // Catat waktu relay dinyalakan
      digitalWrite(relayPin, HIGH); // Nyalakan relay
    }
  } else {
    isRelayOn = false;
  }

  // Mematikan relay setelah relayDelay (5 detik) menyala
  if (isRelayOn && millis() - relayStartTime >= relayDelay) {
    isRelayOn = false;
    digitalWrite(relayPin, LOW); // Matikan relay
  }

  delay(100);
}
