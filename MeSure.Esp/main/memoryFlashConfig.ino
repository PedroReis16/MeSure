void setVariables() {
  tempUnity = preferences.getString("tempUnityKey", "Celsius");
  maxTemp = preferences.getFloat("maxTempKey", 80.00);
  minTemp = preferences.getFloat("minTempKey", 0.00);
  timeoutSend = preferences.getInt("timeoutSendKey", 0);
}

void writeToEEPROM(const char* key, const char* value) {
  preferences.putString(key, value);
}

void writeToEEPROM(const char* key, float value) {
  preferences.putFloat(key, value);
}

void writeToEEPROM(const char* key, int value) {
  preferences.putInt(key, value);
}