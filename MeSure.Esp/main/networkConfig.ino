#include <WiFi.h>
#include <PubSubClient.h>

char* SSID;
char* PASSWORD;
char* BROKER_MQTT;
int BROKER_PORT = 1883;
char* TOPICO_SUBSCRIBE;
char* TOPICO_PUBLISH_1;
char* TOPICO_PUBLISH_2;
char* ID_MQTT = const_cast<char*>("fiware_001");
int D4 = 2;
char* topicPrefix;

WiFiClient espClient;
PubSubClient MQTT(espClient);
char EstadoSaida = '0';


void initConnection(char* defaultSSID, char* password, char* broker, char* topicoSubscribe, char* topicoPublish1, char* topicoPublish2,
                    char* sensorPrefix) {
  SSID = const_cast<char*>(defaultSSID);
  PASSWORD = const_cast<char*>(password);
  BROKER_MQTT = const_cast<char*>(broker);
  TOPICO_SUBSCRIBE = const_cast<char*>(topicoSubscribe);
  TOPICO_PUBLISH_1 = const_cast<char*>(topicoPublish1);
  TOPICO_PUBLISH_2 = const_cast<char*>(topicoPublish2);
  topicPrefix = const_cast<char*>(sensorPrefix);

  InitOutput();
  initWiFi();
  initMQTT();
  delay(5000);
  MQTT.publish(TOPICO_PUBLISH_1, "s|on");

  xTaskCreate(TaskMQTT, "TaskMQTT", 4096, NULL, 1, NULL);
}


void TaskMQTT(void* pvParameters) {
  while (true) {
    VerificaConexoesWiFIEMQTT();
    EnviaEstadoOutputMQTT();
    MQTT.loop();
    vTaskDelay(pdMS_TO_TICKS(1000));  // Verifica a cada 1 segundo
  }
}


void loopMethods() {
  VerificaConexoesWiFIEMQTT();
  EnviaEstadoOutputMQTT();
  MQTT.loop();
  // handleDHT();
  // handleLuminosity();
}

void initWiFi() {
  delay(10);
  Serial.println("------Conexao WI-FI------");
  Serial.print("Conectando-se na rede: ");
  Serial.println(SSID);
  Serial.println("Aguarde");
  reconectWiFi();
}

void initMQTT() {
  MQTT.setServer(BROKER_MQTT, BROKER_PORT);
  MQTT.setCallback(mqtt_callback);
}

void reconectWiFi() {
  if (WiFi.status() == WL_CONNECTED)
    return;
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Conectado com sucesso na rede ");
  Serial.print(SSID);
  Serial.println("IP obtido: ");
  Serial.println(WiFi.localIP());

  // Garantir que o LED inicie desligado
  digitalWrite(D4, LOW);
}

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  String msg;
  int property = 0;
  for (int i = 0; i < length; i++) {
    char c = (char)payload[i];
    msg += c;
  }
  Serial.print("- Mensagem recebida: ");
  Serial.println(msg);

  // Forma o padrão de tópico para comparação
  String onTopic = String(topicPrefix) + "@on|";
  String offTopic = String(topicPrefix) + "@off|";

  // Compara com o tópico recebido
  if (msg.equals(offTopic)) {
    digitalWrite(D4, LOW);
    EstadoSaida = '0';
  }
  if (msg.equals(onTopic)) {
    digitalWrite(D4, HIGH);
    EstadoSaida = '1';
  }
  if (msg.indexOf("Fahrenheit") != -1) {
    EstadoSaida = '2';

    int pipeIndex = msg.indexOf("|");

    tempUnity = msg.substring(pipeIndex + 1);

    writeToEEPROM("tempUnityKey", tempUnity.c_str());

    convertFromCtoF();
    property = 1;
  }
  if (msg.indexOf("Celsius") != -1) {
    EstadoSaida = '3';

    int pipeIndex = msg.indexOf("|");

    tempUnity = msg.substring(pipeIndex + 1);

    writeToEEPROM("tempUnityKey", tempUnity.c_str());

    convertFromFtoC();
    property = 1;
  }
  if (msg.indexOf("maxTemp") != -1) {
    EstadoSaida = '4';

    int pipeIndex = msg.indexOf("|");

    String value = msg.substring(pipeIndex + 1);

    maxTemp = value.toInt();
    writeToEEPROM("maxTempKey", maxTemp);
    property = 2;
  }
  if (msg.indexOf("minTemp") != -1) {
    EstadoSaida = '5';

    int pipeIndex = msg.indexOf("|");

    String value = msg.substring(pipeIndex + 1);
    minTemp = value.toInt();
    writeToEEPROM("minTempKey", minTemp);
    property = 3;
  }
  updateLCD(property);
}

void VerificaConexoesWiFIEMQTT() {
  if (!MQTT.connected())
    reconnectMQTT();
  reconectWiFi();
}

void EnviaEstadoOutputMQTT() {
   if (EstadoSaida == '0') {
    MQTT.publish(TOPICO_PUBLISH_1, "s|off");
  }
  
  if (EstadoSaida == '1') {
    MQTT.publish(TOPICO_PUBLISH_1, "s|on");
  }

  if(EstadoSaida == '2'){
    MQTT.publish(TOPICO_PUBLISH_1, "u|Fahrenheit");
  }
  if(EstadoSaida == '3'){
    MQTT.publish(TOPICO_PUBLISH_1, "u|Celsius");
  }
  
  delay(1000);
}

void InitOutput() {
  pinMode(D4, OUTPUT);
  digitalWrite(D4, HIGH);
  boolean toggle = false;

  for (int i = 0; i <= 10; i++) {
    toggle = !toggle;
    digitalWrite(D4, toggle);
    delay(200);
  }
}

void reconnectMQTT() {
  while (!MQTT.connected()) {
    Serial.print("* Tentando se conectar ao Broker MQTT: ");
    Serial.println(BROKER_MQTT);
    if (MQTT.connect(ID_MQTT)) {
      Serial.println("Conectado com sucesso ao broker MQTT!");
      MQTT.subscribe(TOPICO_SUBSCRIBE);
    } else {
      Serial.println("Falha ao reconectar no broker.");
      Serial.println("Haverá nova tentativa de conexão em 2s");
      delay(2000);
    }
  }
}

void handleDHT(float value) {
  String message = String (value);
  MQTT.publish(TOPICO_PUBLISH_2, message.c_str());
}
