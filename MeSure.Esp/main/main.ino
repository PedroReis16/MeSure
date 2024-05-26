// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

#include "DHT.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Preferences.h>
#include <ezButton.h>

#define DHTPIN 4

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT11  // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

//Analog read
#define ANALOGICAL_TEMP_PIN 34

//LEDs
#define GREEN_LED 19
#define YELLOW_LED 18
#define RED_LED 5

//Button
#define BUTTON_CANCEL_PIN 26
#define BUTTON_PREVIOUS_PIN 25
#define BUTTON_NEXT_PIN 33
#define BUTTON_CONFIRM_PIN 32

#define DEBOUCE_TIME 50

unsigned long changeTime;
unsigned long altTime;
unsigned long choiceTime;

Preferences preferences;
ezButton cancelButton(BUTTON_CANCEL_PIN);
ezButton prevButton(BUTTON_PREVIOUS_PIN);
ezButton nextButton(BUTTON_NEXT_PIN);
ezButton confirmButton(BUTTON_CONFIRM_PIN);

// Variáveis de controle
const int pageCount = 5;
int tempLimits[2] = { 0, 60 };
int timeoutLimits[2] = { 0, 3600 };

bool isCelsius = true;
bool editingMode = false;
int currentSelection = 1;

//Variáveis configuraveis
String tempUnity;
int maxTemp;
int minTemp;
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
  Serial.begin(115200);

  dht.begin();

  //LED
  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  //Buttons
  pinMode(BUTTON_PREVIOUS_PIN, INPUT_PULLUP);
  pinMode(BUTTON_NEXT_PIN, INPUT_PULLUP);

  cancelButton.setDebounceTime(DEBOUCE_TIME);
  confirmButton.setDebounceTime(DEBOUCE_TIME);

  preferences.begin("me-sure", false);

  //Inicialização da EEPROM
  setVariables();

  lcd.init();
  lcd.backlight();
  intro();

  setCurrentPage();
  initConnection("Telenew Jose", "Jepema141609", "10.0.0.145", "/TEF/sensor001/cmd", "/TEF/sensor001/atts", "/TEF/sensor001/atts/t", "sensor001");
}

void loop() {
  checkTempStatus();

  cancelButton.loop();
  confirmButton.loop();

  if (confirmButton.isReleased()) {
    if (currentPage == PAGE_TEMPERATURE)
      return;

    if (editingMode)
      editingMode = false;
    else
      editingMode = true;

    setCurrentPage();
    delay(200);
  }

  if (editingMode && cancelButton.isReleased()) {
    editingMode = false;
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
  loopMethods();
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
