#include <dht11.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define DHT11PIN 4
#define SENSOR_PIN A0
#define LED 2
#define BUZZ 7
#define LCD_COLS 16
#define LCD_ROWS 2
#define ADDRESS 0x27

dht11 DHT11;
LiquidCrystal_I2C lcd(ADDRESS, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

String condition = "Normal";
int threshold = 700; // Set the threshold for CO concentration

void setup() {
  Serial.begin(9600);
  pinMode(SENSOR_PIN, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(BUZZ, OUTPUT);

  lcd.begin(LCD_COLS, LCD_ROWS);
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Condition: ");
  lcd.print(condition);
}

void loop() {
  int chk = DHT11.read(DHT11PIN);
  float temperature = (float)DHT11.temperature;
  float humidity = (float)DHT11.humidity;
  int sensorData = analogRead(SENSOR_PIN);
  float sensorDataPPM = map(sensorData, 0, 1023, 0, 1000); // Adjust the range if needed

  if (chk == DHTLIB_OK) {
    if (sensorDataPPM > threshold) {
      condition = "Warning";
      digitalWrite(LED, HIGH);
      tone(BUZZ, 1000); // Buzzer beeps at 1000 Hz frequency
    } else {
      condition = "Normal";
      digitalWrite(LED, LOW);
      noTone(BUZZ); // Stop the buzzer from beeping
    }

    // Display the updated condition on the first row
    lcd.setCursor(10, 0);
    lcd.print(condition);

    // Scroll the second row with sensor data
    String sensorInfo = "Temp: " + String(temperature) + "C, Humid: " + String(humidity) + "%, CO: " + String(sensorDataPPM) + "ppm ";
    for (int i = 0; i < sensorInfo.length(); i++) {
      lcd.setCursor(0, 1);
      for (int j = i; j < i + LCD_COLS && j < sensorInfo.length(); j++) {
        lcd.print(sensorInfo[j]);
      }
      delay(300);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Condition: ");
      lcd.print(condition);
    }
  } else {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
}
