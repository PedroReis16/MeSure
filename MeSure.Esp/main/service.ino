void checkTempStatus() {
  float currentTemp;
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).

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
  maxTemp = (maxTemp - 32) * (5 / 9);
  minTemp = (minTemp - 32) * (5 / 9);
}
void convertFromCtoF() {
  maxTemp = (9.0 / 5.0) * maxTemp + 32;
  minTemp = (9.0 / 5.0) * minTemp + 32;
}