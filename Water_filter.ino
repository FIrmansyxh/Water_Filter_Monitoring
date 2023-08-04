#include <LiquidCrystal_I2C.h>
#include <NewPing.h>

#define TRIGGER_PIN 5 
#define ECHO_PIN 4   
#define MAX_DISTANCE 200

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

const int sensorPin = A0;
const int clearThreshold = 20;
const int cloudyThreshold = 50;
const int relayPinA = 14;
const int relayPinB = 12;

bool turbidityActivated = false;

LiquidCrystal_I2C lcd(0x27, 4, 16); 

void setup() {
  Serial.begin(9600); 
  lcd.begin(16, 4);
  lcd.backlight(); 
  pinMode(relayPinA, OUTPUT); 
  pinMode(relayPinB, OUTPUT);
  digitalWrite(relayPinA, LOW);
  digitalWrite(relayPinB, LOW); 
}

void loop() {
  delay(500);
  unsigned int distance = sonar.ping_cm();
  if (distance == 0) {
    Serial.println("Jarak terlalu jauh atau sensor tidak terdeteksi.");
  } else {
    Serial.print("Jarak: ");
    Serial.print(distance);
    Serial.println(" cm");

    if (distance < 10) {
      turbidityActivated = true;
      Serial.println("Turbidity is activated!");
      lcd.setCursor(0, 1);
      lcd.print("Turbidity: ON ");
      lcd.print("         ");
    } else {
      turbidityActivated = false;
      Serial.println("Turbidity is not activated.");
      lcd.setCursor(0, 1);
      lcd.print("Turbidity: OFF");
      lcd.print("         "); 
      digitalWrite(relayPinA, LOW); 
      digitalWrite(relayPinB, LOW);
    }
  }

  if (turbidityActivated) {
    int sensorValue = analogRead(sensorPin);
    Serial.println(sensorValue);

    int turbidity = map(sensorValue, 0, 750, 0, 100);
    delay(100);

    if (turbidity < clearThreshold) {
      Serial.println("It's CLEAR");
      lcd.setCursor(0, 0);
      lcd.print("Status: CLEAR   ");
      digitalWrite(relayPinA, HIGH);
      digitalWrite(relayPinB, LOW);
    } else if (turbidity < cloudyThreshold) {
      Serial.println("It's CLOUDY");
      lcd.setCursor(0, 0);
      lcd.print("Status: CLOUDY  ");
      digitalWrite(relayPinA, LOW);
      digitalWrite(relayPinB, HIGH);
    } else {
      Serial.println("It's between CLEAR and CLOUDY");
      lcd.setCursor(0, 0);
      lcd.print("Status: BETWEEN ");
      digitalWrite(relayPinA, LOW);
      digitalWrite(relayPinB, LOW);
    }
  }
}
