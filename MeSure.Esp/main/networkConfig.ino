//Autor: Fábio Henrique Cabrini
//Resumo: Esse programa possibilita ligar e desligar o led onboard, além de mandar o status para o Broker MQTT possibilitando o Helix saber
//se o led está ligado ou desligado.
//Revisões:
//Rev1: 26-08-2023 Código portado para o ESP32 e para realizar a leitura de luminosidade e publicar o valor em um tópico aprorpiado do broker
//Autor Rev1: Lucas Demetrius Augusto
//Rev2: 28-08-2023 Ajustes para o funcionamento no FIWARE Descomplicado
//Autor Rev2: Fábio Henrique Cabrini
//Rev3: 1-11-2023 Refinamento do código e ajustes para o funcionamento no FIWARE Descomplicado
//Autor Rev3: Fábio Henrique Cabrini
#include <WiFi.h>
#include <PubSubClient.h>

// Configurações - variáveis editáveis

// const char* default_SSID = "sua_rede_wifi"; // Nome da rede Wi-Fi
// const char* default_PASSWORD = "sua_senha_wifi"; // Senha da rede Wi-Fi
// const char* default_BROKER_MQTT = "ip_host_fiware"; // IP do Broker MQTT
// const int default_BROKER_PORT = 1883; // Porta do Broker MQTT
// const char* default_TOPICO_SUBSCRIBE = "/TEF/lamp001/cmd"; // Tópico MQTT de escuta
// const char* default_TOPICO_PUBLISH_1 = "/TEF/lamp001/attrs"; // Tópico MQTT de envio de informações para Broker
// const char* default_TOPICO_PUBLISH_2 = "/TEF/lamp001/attrs/l"; // Tópico MQTT de envio de informações para Broker
// const char* default_ID_MQTT = "fiware_001"; // ID MQTT
// const int default_D4 = 2; // Pino do LED onboard
// // Declaração da variável para o prefixo do tópico
// const char* topicPrefix = "lamp001";

// Variáveis para configurações editáveis

// char* SSID = const_cast<char*>(default_SSID);
// char* PASSWORD = const_cast<char*>(default_PASSWORD);
// char* BROKER_MQTT = const_cast<char*>(default_BROKER_MQTT);
// int BROKER_PORT = default_BROKER_PORT;
// char* TOPICO_SUBSCRIBE = const_cast<char*>(default_TOPICO_SUBSCRIBE);
// char* TOPICO_PUBLISH_1 = const_cast<char*>(default_TOPICO_PUBLISH_1);
// char* TOPICO_PUBLISH_2 = const_cast<char*>(default_TOPICO_PUBLISH_2);
// char* ID_MQTT = const_cast<char*>(default_ID_MQTT);
// int D4 = default_D4;
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
  // BROKER_PORT = brokerPort;
  TOPICO_SUBSCRIBE = const_cast<char*>(topicoSubscribe);
  TOPICO_PUBLISH_1 = const_cast<char*>(topicoPublish1);
  TOPICO_PUBLISH_2 = const_cast<char*>(topicoPublish2);
  // ID_MQTT = const_cast<char*>(idMQQT);
  // D4 = defaultD4;
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
    // Serial.println("- Led Desligado");
  }
  
  if (EstadoSaida == '1') {
    MQTT.publish(TOPICO_PUBLISH_1, "s|on");
    // Serial.println("- Led Ligado");
  }

  if(EstadoSaida == '2'){
    MQTT.publish(TOPICO_PUBLISH_1, "u|Fahrenheit");
  }
  if(EstadoSaida == '3'){
    MQTT.publish(TOPICO_PUBLISH_1, "u|Celsius");
  }
  
  // Serial.println("- Estado do LED onboard enviado ao broker!");
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

void handleDHT() {
  // float humidity = dht.readHumidity();
  double temperature = (double)(dht.readTemperature());

  // Verifica se a leitura falhou
  if (isnan(temperature)) {
    Serial.println("Falha ao ler do sensor DHT!");
    return;
  }

  String mensagem = String(temperature);
  Serial.println(mensagem.c_str());
  MQTT.publish(TOPICO_PUBLISH_2, mensagem.c_str());
}