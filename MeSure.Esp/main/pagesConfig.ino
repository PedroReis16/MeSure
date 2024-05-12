void intro() {
  int i = 4;
  int line = 0;

  byte regua[] = {
    B10101,
    B11101,
    B11111,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000
  };

  byte reguaBaixa[] = {
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B10101,
    B11101,
    B11111
  };

  lcd.createChar(0, reguaBaixa);
  lcd.createChar(1, regua);

  for (int i = 0; i < 6; i++) {
    lcd.clear();
    lcd.setCursor(i, 0);
    lcd.print("MeSure");
    delay(250);
  }

  while (i < 13) {
    lcd.setCursor(i, 1);
    lcd.write(byte(line));

    i++;

    if (i == 12 && line == 1) {
      break;
    } else if (i == 12) {
      delay(250);
      i = 4;
      line++;
    }
  }

  delay(1000);
  lcd.clear();
  lcd.print("CARREGANDO...");
  delay(1000);
  lcd.clear();
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
  } else {
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
      if (isCelsius) {
        tempUnity = "Fahrenheit";
        isCelsius = false;
      } else {
        tempUnity = "Celsius";
        isCelsius = true;
      }
      lcd.setCursor(0, 1);
      lcd.print(tempUnity + "   ");
      delay(200);
    }

    if (confirmButton.isPressed()) {
      writeToEEPROM("tempUnityKey", tempUnity.c_str());
      if (isCelsius)
        convertFromFtoC();
      else
        convertFromCtoF();

      tempUnityChanged = false;
      originalUnity = "";

      // editingMode = false;
    } else if (cancelButton.isPressed()) {
      // editingMode = false;
      tempUnityChanged = false;
      tempUnity = originalUnity;
      isCelsius = originalUnity == "Celsius";
      return;
    }
  }
}

void configMaxTempLimit() {
  static bool tempLimitChanged = false;
  static float originalMaxTempLimit = 0;
  static String unity = tempUnity == "Celsius" ? " C " : " F ";

  if (!editingMode) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temperatura max:");
    lcd.setCursor(0, 1);
    lcd.print(maxTemp);
    lcd.print(unity);
  } else {
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
      if (isCelsius && maxTemp > 0) {
        maxTemp--;
      } else if (!isCelsius && maxTemp > 32) {
        maxTemp--;
      }
      lcd.setCursor(0, 1);
      lcd.print(maxTemp);
      lcd.print(unity);
      delay(200);
    }
    if (digitalRead(BUTTON_NEXT_PIN) == LOW) {
      if (isCelsius && maxTemp < 80) {
        maxTemp++;
      } else if (!isCelsius && maxTemp < 176) {
        maxTemp++;
      }
      lcd.setCursor(0, 1);
      lcd.print(maxTemp);
      lcd.print(unity);
      delay(200);
    }

    if (confirmButton.isPressed()) {
      writeToEEPROM("maxTempKey", maxTemp);

      tempLimitChanged = false;
      originalMaxTempLimit = 0;
      
      // editingMode = false;
    } else if (cancelButton.isPressed()) {
      // editingMode = false;
      tempLimitChanged = false;
      maxTemp = originalMaxTempLimit;
    }
  }
}

void configMinTempLimit() {
  static bool tempLimitChanged = false;
  static float originalMinTempLimit = 0;
  static String unity = isCelsius ? " C " : " F ";

  if (!editingMode) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temperatura min:");
    lcd.setCursor(0, 1);
    lcd.print(minTemp);
    lcd.print(unity);
  } else {
    if (!tempLimitChanged) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Temperatura min:");
      lcd.setCursor(0, 1);
      lcd.print(minTemp);
      lcd.print(unity);
      originalMinTempLimit = minTemp;
      tempLimitChanged = true;
    }

    if (digitalRead(BUTTON_PREVIOUS_PIN) == LOW) {
      if (isCelsius && minTemp > 0) {
        minTemp--;
      } else if (!isCelsius && minTemp > 32) {
        minTemp--;
      }
      lcd.setCursor(0, 1);
      lcd.print(minTemp);
      lcd.print(unity);
      delay(200);
    }
    if (digitalRead(BUTTON_NEXT_PIN) == LOW) {
      if (isCelsius && minTemp < 80) {
        minTemp++;
      } else if (!isCelsius && minTemp < 176) {
        minTemp++;
      }
      lcd.setCursor(0, 1);
      lcd.print(minTemp);
      lcd.print(unity);
      delay(200);
    }

    if (confirmButton.isPressed()) {
      writeToEEPROM("minTempKey", minTemp);

      tempLimitChanged = false;
      originalMinTempLimit = 0;

      // editingMode = false;
    } else if (cancelButton.isPressed()) {
      //editingMode = false;
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

    if (confirmButton.isPressed()) {
      writeToEEPROM("timeoutSendKey", timeoutSend);

      originalTimeoutSend = 0;
      timeoutSendChanged = false;
      
      // editingMode = false;
    } else if (cancelButton.isPressed()) {
      // editingMode = false;
      timeoutSendChanged = false;
      timeoutSend = originalTimeoutSend;
    }
  }
}