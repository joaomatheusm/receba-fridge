// Libraries
#include <OneWire.h>
#include <DHT.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Directives
#define I2C_ADDR 0x27
#define LCD_COLS 16
#define LCD_ROWS 2

#define DHTPIN 2
#define DHTTYPE DHT11

#define BUZZER 13
#define GREEN 12
#define YELLOW 11
#define RED_1 10
#define RED_2 9
#define RED_3 8

// Object Instances
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLS, LCD_ROWS);

// Parameter Definition
const float MIN_TEMPERATURE = 6;
const float MEDIUM_TEMPERATURE = 9;
const float MAX_TEMPERATURE = 12;

int global = 0;

// Functions
void displayInfo(float temperature, float humidity) {
    lcd.setCursor(0, 0);
    printTemperature(temperature);
    lcd.setCursor(0, 1);
    printHumidity(humidity);
}

void printTemperature(float temp) {
    lcd.print("Temp: ");
    lcd.print(temp);
    lcd.print("C");
}

void printHumidity(float humidity) {
    lcd.print("Humidity: ");
    lcd.print(humidity);
    lcd.print("%");
}

int alertMin() {
    digitalWrite(GREEN, LOW);
    digitalWrite(YELLOW, HIGH);
    tone(BUZZER, 500);
    int ms = 3000;

    return ms;
}

int alertMed() {
    digitalWrite(GREEN, LOW);
    digitalWrite(YELLOW, LOW);
    digitalWrite(RED_1, HIGH);
    tone(BUZZER, 500);
    int ms = 2000;

    return ms;
}

void toggleMaxLeds() {
    if (global % 2) {
      digitalWrite(RED_2, HIGH);
      digitalWrite(RED_3, LOW);  
    } else {
      digitalWrite(RED_2, LOW);
      digitalWrite(RED_3, HIGH);
      tone(BUZZER, 500);
    }
}

int alertMax() {
    digitalWrite(GREEN, LOW);
    digitalWrite(YELLOW, LOW);
    digitalWrite(RED_1, LOW);

    toggleMaxLeds();
    
    int ms = 250;

    return ms;
}

void stopAlert() {
    digitalWrite(YELLOW, LOW);
    digitalWrite(RED_1, LOW);
    digitalWrite(RED_2, LOW);
    digitalWrite(RED_3, LOW);
    digitalWrite(GREEN, HIGH);
    noTone(BUZZER);  
}

void setup() {
    // LCD display setup
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);

    // Initialize sensor
    dht.begin();
    
    // Setup digital pins
    pinMode(BUZZER, OUTPUT);
    pinMode(GREEN, OUTPUT);
    pinMode(YELLOW, OUTPUT);
    pinMode(RED_1, OUTPUT);

}

// Loop to display information
void loop() {
    global++;

    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    int ms;

    lcd.clear();
    displayInfo(temperature, humidity);

    if (temperature > MAX_TEMPERATURE) {
      ms = alertMax();
    } else if (temperature > MEDIUM_TEMPERATURE) {
      ms = alertMed();
    } else if (temperature > MIN_TEMPERATURE) {
      ms = alertMin();
    } else {
      stopAlert();
    }
    delay(500);
    
    noTone(BUZZER);
    delay(ms);
}
