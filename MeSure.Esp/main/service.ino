void checkTempStatus() {
  float currentTemp;
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  if (isnan(t) || isnan(f)) {
    if (currentPage == PAGE_TEMPERATURE) {
      lcd.clear();
      lcd.setCursor(6, 0);
      lcd.print("ERROR");
      return;
    } else {
      digitalWrite(GREEN_LED, HIGH);
      digitalWrite(YELLOW_LED, HIGH);
      digitalWrite(RED_LED, HIGH);
    }
  }
  if (currentPage == PAGE_TEMPERATURE) {
    lcd.setCursor(0, 1);
    if (isCelsius) {
      lcd.print(t);
      lcd.print(" C ");
      currentTemp = t;
    } else {
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
  } else if (maxTempPercent >= 65.00 && maxTempPercent < 85.00) {
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(YELLOW_LED, HIGH);
    digitalWrite(RED_LED, LOW);
  } else if (maxTempPercent >= 85.00) {
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(YELLOW_LED, HIGH);
    digitalWrite(RED_LED, HIGH);
  }
}

void convertFromFtoC() {
  maxTemp = (maxTemp - 32) / 1.8;
  minTemp = (minTemp - 32) / 1.8;
}
void convertFromCtoF() {
  maxTemp = (9.0 / 5.0) * maxTemp + 32;
  minTemp = (9.0 / 5.0) * minTemp + 32;
}

void updateLCD(int propertyChanged) {
  switch (currentPage) {
    case PAGE_TEMP_UNITY:
      if (propertyChanged == 1) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Unidade: ");
        lcd.setCursor(0, 1);
        lcd.print(tempUnity);
      }
      break;
    case PAGE_MAX_TEMP_LIMIT:
      if (propertyChanged == 2) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Temperatura max:");
        lcd.setCursor(0, 1);
        lcd.print(maxTemp);
        lcd.print(isCelsius ? " C":" F");
      }
      break;
    case PAGE_MIN_TEMP_LIMIT:
      if (propertyChanged == 3) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Temperatura min:");
        lcd.setCursor(0, 1);
        lcd.print(minTemp);
        lcd.print(isCelsius ? " C":" F");
      }
      break;
  }
}

void convertFromBitsToTemp() {
  // Entrada máxima do sistema = 10V
  // A cada 1V representa, aproximadamente, 10ºC
  // Convertendo para o esp
  // Tensão de entrada máxima 3.3V
  // A cada 0.33V representa, aproximadamente, 10º

  float value = analogRead(ANALOGICAL_TEMP_PIN);

  float tensor = map(value, 0, 4095, 0, 10.0);

  float temp = tensor * 10;
}