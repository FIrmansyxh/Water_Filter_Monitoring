#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 2, 16);

const int sensorPin = A0; // A0 FOR ARDUINO/ 36 FOR ESP
const int clearThreshold = 20;
const int cloudyThreshold = 50;
const int relayPin = 12; // Pin relay dihubungkan
const unsigned long relayDelay = 5000; // Waktu delay relay (dalam milidetik)

bool isRelayOn = false;
unsigned long relayStartTime = 0;

void setup() {
  Serial.begin(9600);
  // Initialize LCD
  lcd.init();
  // Turn on LCD backlight                      
  lcd.backlight();
  
  pinMode(relayPin, OUTPUT); // Mengatur pin relay sebagai output
  digitalWrite(relayPin, LOW); // Mematikan relay saat pertama kali dijalankan
}

void loop() {
  int sensorValue = analogRead(sensorPin);
  Serial.println(sensorValue);

  int turbidity = map(sensorValue, 0, 750, 0, 100);
  delay(100);

  lcd.setCursor(0, 0);
  lcd.print("Turbidity: ");
  lcd.print(turbidity);
  lcd.print("   ");
  
  delay(100);
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
}
