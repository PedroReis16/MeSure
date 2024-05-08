// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

#include "DHT.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Preferences.h>

#define DHTPIN 4

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT11  // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// //Endereços de memória paras as variáveis
// #define EEPROM_ADDR_TEMP_UNITY 0
// #define EEPROM_ADDR_MAX_TEMP 1
// #define EEPROM_ADDR_MIN_TEMP 3
// #define EEPROM_ADDR_TIMEOUT_SEND 5


DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

//LEDs
#define GREEN_LED 19
#define YELLOW_LED 18
#define RED_LED 5

//Button
#define BUTTON_CANCEL_PIN 26
#define BUTTON_PREVIOUS_PIN 25
#define BUTTON_NEXT_PIN 33
#define BUTTON_CONFIRM_PIN 32

unsigned long changeTime;
unsigned long altTime;
unsigned long choiceTime;

Preferences preferences;

// Variáveis de controle
const int pageCount = 5;
int tempLimits[2] = { 0, 60 };
int timeoutLimits[2] = { 0, 3600 };

bool isCelsius = true;
bool editingMode = false;
int currentSelection = 1;

//Variáveis configuraveis
String tempUnity;
float maxTemp;
float minTemp;
int timeoutSend;

int inicio = 0;

enum Page {
  PAGE_TEMPERATURE,
  PAGE_TEMP_UNITY,
  PAGE_MAX_TEMP_LIMIT,
  PAGE_MIN_TEMP_LIMIT,
  PAGE_SEND_TIMEOUT
};

Page currentPage = PAGE_TEMPERATURE;

void setup() {
  Serial.begin(9600);

  dht.begin();

  //LED
  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  //Buttons
  pinMode(BUTTON_CANCEL_PIN, INPUT_PULLUP);
  pinMode(BUTTON_PREVIOUS_PIN, INPUT_PULLUP);
  pinMode(BUTTON_NEXT_PIN, INPUT_PULLUP);
  pinMode(BUTTON_CONFIRM_PIN, INPUT_PULLUP);

  preferences.begin("me-sure", false);

  //Inicialização da EEPROM
  setVariables();

  lcd.init();
  lcd.backlight();
  intro();

  setCurrentPage();
}

void loop() {
  checkTempStatus();

  if (digitalRead(BUTTON_CANCEL_PIN) == LOW) {
    editingMode = false;
    setCurrentPage();
    delay(200);
  }

  if (digitalRead(BUTTON_CONFIRM_PIN) == LOW) {
    if (currentPage == PAGE_TEMPERATURE)
      return;
    editingMode = true;
    setCurrentPage();
    delay(200);
  }

  if (!editingMode) {
    if (digitalRead(BUTTON_PREVIOUS_PIN) == LOW) {
      currentPage = static_cast<Page>((currentPage - 1 + pageCount) % pageCount);
      setCurrentPage();
      delay(200);
    } else if (digitalRead(BUTTON_NEXT_PIN) == LOW) {
      currentPage = static_cast<Page>((currentPage + 1) % pageCount);
      setCurrentPage();
      delay(200);
    }
  } else {
    setCurrentPage();
  }
}

void setVariables() {
  tempUnity = preferences.getString("tempUnityKey", "Celsius");
  maxTemp = preferences.getFloat("maxTempKey", 80.00);
  minTemp = preferences.getFloat("minTempKey", 0.00);
  timeoutSend = preferences.getInt("timeoutSendKey", 0);
}

void setCurrentPage() {
  switch (currentPage) {
    case PAGE_TEMPERATURE:
      firstPage();
      break;
    case PAGE_TEMP_UNITY:
      configTempUnity();
      break;
    case PAGE_MAX_TEMP_LIMIT:
      configMaxTempLimit();
      break;
    case PAGE_MIN_TEMP_LIMIT:
      configMinTempLimit();
      break;
    case PAGE_SEND_TIMEOUT:
      configSendTimeout();
      break;
  }
}
// LCD Pages
void firstPage() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temperature: ");
}

void checkTempStatus()
{
    float currentTemp;
    float t = dht.readTemperature();
    // Read temperature as Fahrenheit (isFahrenheit = true)
    float f = dht.readTemperature(true);

    // Check if any reads failed and exit early (to try again).

    if (isnan(t) || isnan(f))
    {
        if (currentPage == PAGE_TEMPERATURE)
        {
            lcd.clear();
            lcd.setCursor(6, 0);
            lcd.print("ERROR");
            return;
        }
        else
        {
            digitalWrite(GREEN_LED, HIGH);
            digitalWrite(YELLOW_LED, HIGH);
            digitalWrite(RED_LED, HIGH);
        }
    }
    if (currentPage == PAGE_TEMPERATURE)
    {
        lcd.setCursor(0, 1);
        if (isCelsius)
        {
            lcd.print(t);
            lcd.print(" C ");
            currentTemp = t;
        }
        else
        {
            lcd.print(f);
            lcd.print(" F ");
            currentTemp = f;
        }
    }

    float maxTempPercent = (currentTemp / maxTemp) * 100;

    if (currentTemp > minTemp + 3.0 && currentTemp <= minTemp + 7.0)
    {
        digitalWrite(GREEN_LED, LOW);
        digitalWrite(YELLOW_LED, HIGH);
        digitalWrite(RED_LED, LOW);
        return;
    }
    else if (currentTemp <= minTemp + 3.0)
    {
        digitalWrite(GREEN_LED, LOW);
        digitalWrite(YELLOW_LED, LOW);
        digitalWrite(RED_LED, HIGH);
        return;
    }

    if (maxTempPercent < 65.00)
    {
        digitalWrite(GREEN_LED, HIGH);
        digitalWrite(YELLOW_LED, LOW);
        digitalWrite(RED_LED, LOW);
    }
    else if (maxTempPercent >= 65.00 && maxTempPercent < 85.00)
    {
        digitalWrite(GREEN_LED, HIGH);
        digitalWrite(YELLOW_LED, HIGH);
        digitalWrite(RED_LED, LOW);
    }
    else if (maxTempPercent >= 85.00)
    {
        digitalWrite(GREEN_LED, HIGH);
        digitalWrite(YELLOW_LED, HIGH);
        digitalWrite(RED_LED, HIGH);
    }
}

void convertFromFtoC() {
  // currentTemp = (currentTemp - 32) * (5 / 9);
  maxTemp = (maxTemp - 32) * (5 / 9);
  minTemp = (minTemp - 32) * (5 / 9);
}
void convertFromCtoF() {
  // currentTemp = (currentTemp * 9 / 5) + 32;
  maxTemp = (9.0 / 5.0) * maxTemp + 32;
  minTemp = (9.0 / 5.0) * minTemp + 32;
}

void writeToEEPROM(const char* key, const char* value)
{
    preferences.putString(key, value);
}

void writeToEEPROM(const char* key, float value)
{
    preferences.putFloat(key, value);
}

void writeToEEPROM(const char* key, int value)
{
    preferences.putInt(key, value);
}

// float readFromEEPROM(int address) {
//   float value;
//   EEPROM.get(address, value);
//   return value;
// }