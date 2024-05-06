// //Configuração de conexão do Esp32
// char* default_SSID = "nome da rede"; // Nome da rede Wi-Fi
// char* default_PASSWORD = "senha"; // Senha da rede Wi-Fi
// char* default_BROKER_MQTT = "19.92.235.126"; // IP do Broker MQTT
// int default_BROKER_PORT = 1883; // Porta do Broker MQTT
// char* default_TOPICO_SUBSCRIBE = "/TEF/sensor001/cmd"; // Tópico MQTT de escuta
// char* default_TOPICO_PUBLISH_1 = "/TEF/sensor001/attrs"; // Tópico MQTT de envio de informações para Broker
// char* default_TOPICO_PUBLISH_2 = "/TEF/sensor001/attrs/l"; // Tópico MQTT de envio de informações para Broker
// char* default_ID_MQTT = "fiware_001"; // ID MQTT
// int default_D4 = 2; // Pino do LED onboard
// // Declaração da variável para o prefixo do tópico
// char* topicPrefix = "sensor001";

// void setup(){
//   initConnection(default_SSID,default_PASSWORD,default_BROKER_MQTT,default_BROKER_PORT,default_TOPICO_SUBSCRIBE,default_TOPICO_PUBLISH_1,default_TOPICO_PUBLISH_2,default_ID_MQTT,default_D4,topicPrefix);
// }

// void loop(){
//   loopMethods();
// }

// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

#include "DHT.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Ticker.h>

#define DHTPIN 4

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT11   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

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

// Variáveis de controle
int currentPage = 1;
int pageCount = 5;
bool editingMode = false;
int currentSelection = 1;
int maxSelections = 5; // Define o número máximo de itens editáveis
bool blockLoop = false;

//Variáveis configuraveis
String tempUnity = "Celsius";
int maxTemp = 60;
int minTemp = 0;
int timeoutSend = 60;

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

  lcd.init();
  lcd.backlight();
  setCurrentPage();
}

void loop() {
  if (digitalRead(BUTTON_CANCEL_PIN) == LOW) {
    editingMode = false;
    setCurrentPage();
    delay(200);
  }

  if (digitalRead(BUTTON_CONFIRM_PIN) == LOW) {
    if (currentPage == 1)
      return;
    editingMode = true;
    setCurrentPage();
    delay(200); // Delay para evitar detecção múltipla
  }
  checkTempStatus();
  // Se estiver no modo de edição, permite a navegação entre os itens
  if (!editingMode) {
    if (digitalRead(BUTTON_PREVIOUS_PIN) == LOW) {
      currentPage--;
      if (currentPage < 1) {
        currentPage = 1;
        return;
      }
      delay(200);
      setCurrentPage();
    }
    else if (digitalRead(BUTTON_NEXT_PIN) == LOW) {
      currentPage++;
      if (currentPage > pageCount) {
        currentPage = pageCount;
        return;
      }
      delay(200);
      setCurrentPage();
    }
  }
  else {
    ///Deixar sempre essa verificação no final do bloco
    if (blockLoop)
      return;

    setCurrentPage();
  }
}

void setCurrentPage() {
  switch (currentPage) {
    case 1:
      firstPage();
      break;
    case 2:
      configTempUnity();
      break;
    case 3:
      configMaxTempLimit() ;
      break;
    case 4:
      configMinTempLimit();
      break;
    case 5:
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

void configTempUnity() {
  static bool tempUnityChanged = false;
  static String originalUnity = "";

  if (!editingMode) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Unidade: ");
    lcd.setCursor(0, 1);
    lcd.print(tempUnity);
  }
  else {
    if (!tempUnityChanged) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Unidade: ");
      tempUnityChanged = true;
      lcd.setCursor(0, 1);
      lcd.print(tempUnity);
      originalUnity = tempUnity;
    }

    if (digitalRead(BUTTON_PREVIOUS_PIN) == LOW || digitalRead(BUTTON_NEXT_PIN) == LOW) {
      if (tempUnity == "Celsius") {
        tempUnity = "Fahrenheit";
      } else {
        tempUnity = "Celsius";
      }
      lcd.setCursor(0, 1);
      lcd.print(tempUnity + "   ");
      delay(200);
    }

    if (digitalRead(BUTTON_CONFIRM_PIN) == LOW) {
      editingMode = false;
      tempUnityChanged = false;
      originalUnity = "";
    }
    else if (digitalRead(BUTTON_CANCEL_PIN) == LOW ) {
      editingMode = false;
      tempUnityChanged = false;
      tempUnity = originalUnity;
    }
  }

}

void configMaxTempLimit() {
  static bool tempLimitChanged = false;
  static int originalMaxTempLimit = 0;
  static String unity = tempUnity == "Celsius" ? " C " : " F " ;

  if (!editingMode) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temperatura max:");
    lcd.setCursor(0, 1);
    lcd.print(maxTemp);
    lcd.print(unity);
  }
  else {
    if (!tempLimitChanged) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Temperatura max:");
      lcd.setCursor(0, 1);
      lcd.print(maxTemp);
      lcd.print(unity);
      originalMaxTempLimit = maxTemp;
      tempLimitChanged = true;
    }

    if (digitalRead(BUTTON_PREVIOUS_PIN) == LOW) {
      if (tempUnity == "Celsius" && maxTemp > 0) {
        maxTemp--;
      } else if (tempUnity == "Fahrenheit" && maxTemp > 32) {
        maxTemp--;
      }
      lcd.setCursor(0, 1);
      lcd.print(maxTemp);
      lcd.print(unity);
      delay(200);
    }
    if (digitalRead(BUTTON_NEXT_PIN) == LOW) {
      if (tempUnity == "Celsius" && maxTemp < 80) {
        maxTemp++;
      } else if (tempUnity == "Fahrenheit" && maxTemp < 176) {
        maxTemp++;
      }
      lcd.setCursor(0, 1);
      lcd.print(maxTemp);
      lcd.print(unity);
      delay(200);
    }

    if (digitalRead(BUTTON_CONFIRM_PIN) == LOW) {
      editingMode = false;
      tempLimitChanged = false;
      originalMaxTempLimit = 0;
    } else if (digitalRead(BUTTON_CANCEL_PIN) == LOW) {
      editingMode = false;
      tempLimitChanged = false;
      maxTemp = originalMaxTempLimit;
    }
  }
}

void configMinTempLimit() {
  static bool tempLimitChanged = false;
  static int originalMinTempLimit = 0;
  static String unity = tempUnity == "Celsius" ? " C " : " F " ;

  if (!editingMode) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temperatura min:");
    lcd.setCursor(0, 1);
    lcd.print(minTemp);
    lcd.print(unity);
  }
  else {
    if (!tempLimitChanged) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Temperatura min:");
      lcd.setCursor(0, 1);
      lcd.print(minTemp);
      lcd.print(unity);
      originalMinTempLimit = maxTemp;
      tempLimitChanged = true;
    }

    if (digitalRead(BUTTON_PREVIOUS_PIN) == LOW) {
      if (tempUnity == "Celsius" && maxTemp > 0) {
        minTemp--;
      } else if (tempUnity == "Fahrenheit" && minTemp > 32) {
        minTemp--;
      }
      lcd.setCursor(0, 1);
      lcd.print(minTemp);
      lcd.print(unity);
      delay(200);
    }
    if (digitalRead(BUTTON_NEXT_PIN) == LOW) {
      if (tempUnity == "Celsius" && minTemp < 80) {
        minTemp++;
      } else if (tempUnity == "Fahrenheit" && minTemp < 176) {
        minTemp++;
      }
      lcd.setCursor(0, 1);
      lcd.print(minTemp);
      lcd.print(unity);
      delay(200);
    }

    if (digitalRead(BUTTON_CONFIRM_PIN) == LOW) {
      editingMode = false;
      tempLimitChanged = false;
      originalMinTempLimit = 0;
    } else if (digitalRead(BUTTON_CANCEL_PIN) == LOW) {
      editingMode = false;
      tempLimitChanged = false;
      minTemp = originalMinTempLimit;
    }
  }
}

void configSendTimeout() {
  static bool timeoutSendChanged = false;
  static int originalTimeoutSend = 0;

  if (!editingMode) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Send Timeout: ");
    lcd.setCursor(0, 1);
    lcd.print(timeoutSend);
    lcd.print(" s ");
  } else {
    if (!timeoutSendChanged) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Send Timeout: ");
      lcd.setCursor(0, 1);
      lcd.print(timeoutSend);
      lcd.print(" s ");
      timeoutSendChanged = true;
      originalTimeoutSend = timeoutSend;
    }

    if (digitalRead(BUTTON_PREVIOUS_PIN) == LOW) {
      if (timeoutSend > 0) {
        timeoutSend = timeoutSend - 2;
      }

      lcd.setCursor(0, 1);
      lcd.print(timeoutSend);
      lcd.print(" s ");
    }
    if (digitalRead(BUTTON_NEXT_PIN) == LOW) {
      if (timeoutSend < 3600) {
        timeoutSend = timeoutSend + 2;
      }
      lcd.setCursor(0, 1);
      lcd.print(timeoutSend);
      lcd.print(" s ");
    }

    if (digitalRead(BUTTON_CONFIRM_PIN) == LOW) {
      editingMode = false;
      timeoutSendChanged = false;
      originalTimeoutSend = 0;
    } else if (digitalRead(BUTTON_CANCEL_PIN) == LOW) {
      editingMode = false;
      timeoutSendChanged = false;
      timeoutSend = originalTimeoutSend;
    }
  }
}

bool blinkYellow = false;
bool blinkRed = false;

void checkTempStatus() {
  float currentTemp;
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).

  if (isnan(t) || isnan(f)) {
    if (currentPage == 1) {
      lcd.clear();
      lcd.setCursor(6, 0);
      lcd.print("ERROR");
      return;
    }
    else {
      digitalWrite(GREEN_LED, HIGH);
      digitalWrite(YELLOW_LED, HIGH);
      digitalWrite(RED_LED, HIGH);
    }
  }
  if (currentPage == 1) {
    lcd.setCursor(0, 1);
    if (tempUnity == "Celsius") {
      lcd.print(t);
      lcd.print(" C ");
      currentTemp = t;
    }
    else {
      lcd.print(f);
      lcd.print(" F ");
      currentTemp = f;
    }
  }

  float maxTempPercent = (currentTemp / maxTemp) * 100;

  if (currentTemp > minTemp + 3.0 && currentTemp <= minTemp + 7.0) {
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(YELLOW_LED, HIGH);
    digitalWrite(RED_LED, LOW);
    return;
  } else if (currentTemp <= minTemp + 3.0) {
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(RED_LED, HIGH);
    return;
  }

  if (maxTempPercent < 65.00) {
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(RED_LED, LOW);
  }
  else if (maxTempPercent >= 65.00 && maxTempPercent < 85.00) {
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(YELLOW_LED, HIGH);
    digitalWrite(RED_LED, LOW);
  }
  else if (maxTempPercent >= 85.00) {
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(YELLOW_LED, HIGH);
    digitalWrite(RED_LED, HIGH);
  }



}
