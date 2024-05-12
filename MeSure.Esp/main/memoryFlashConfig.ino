void setVariables() {
  tempUnity = preferences.getString("tempUnityKey", "Celsius");
  maxTemp = preferences.getInt("maxTempKey", 80);
  minTemp = preferences.getInt("minTempKey", 0);
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