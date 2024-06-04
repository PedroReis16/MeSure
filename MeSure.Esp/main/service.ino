void checkTempStatus() {
  float currentTemp;
  
  if (currentPage == PAGE_TEMPERATURE) {
    lcd.setCursor(0, 1);
     currentTemp = readResistor();
    if (isCelsius) {
      lcd.print(currentTemp);
      lcd.print(" C ");
    } else {
      lcd.print(currentTemp);
      lcd.print(" F ");
    }
   
  }

  unsigned long currentMillis = millis();
  
  // Verifica se 15 segundos se passaram
  if (currentMillis - previousMillis >= 15000) {
    previousMillis = currentMillis;
    
   readings[indice] = readResistor();
    indice++;
    
//     Verifica se o array de leituras está cheio
    if (indice == 4) {
     float average = calculateAverage(readings, 4);
      
      handleDHT(average);
      indice = 0;
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
  isCelsius = true;

  writeToEEPROM("maxTempKey", maxTemp) ;
  writeToEEPROM("minTempKey", minTemp) ;
}
void convertFromCtoF() {
  maxTemp = (9.0 / 5.0) * maxTemp + 32;
  minTemp = (9.0 / 5.0) * minTemp + 32;
  isCelsius = false;

  writeToEEPROM("maxTempKey", maxTemp) ;
  writeToEEPROM("minTempKey", minTemp) ;
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

float readResistor() {
  int value = analogRead(34); 
  float result = ((float)value / 4095.0) * 3.3;
  float temp = (result * 100)/3.3;
  
  if(isCelsius)
    return temp;
  else
    return ((9.0 / 5.0) * temp) + 32;
}

float calculateAverage(float values[], int length) {
  float sum = 0;
  for (int i = 0; i < length; i++) {
    sum += values[i];
  }
  return sum / length;
}
