//Configuração de conexão do Esp32
char* default_SSID = "nome da rede"; // Nome da rede Wi-Fi
char* default_PASSWORD = "senha"; // Senha da rede Wi-Fi
char* default_BROKER_MQTT = "19.92.235.126"; // IP do Broker MQTT
int default_BROKER_PORT = 1883; // Porta do Broker MQTT
char* default_TOPICO_SUBSCRIBE = "/TEF/sensor001/cmd"; // Tópico MQTT de escuta
char* default_TOPICO_PUBLISH_1 = "/TEF/sensor001/attrs"; // Tópico MQTT de envio de informações para Broker
char* default_TOPICO_PUBLISH_2 = "/TEF/sensor001/attrs/l"; // Tópico MQTT de envio de informações para Broker
char* default_ID_MQTT = "fiware_001"; // ID MQTT
int default_D4 = 2; // Pino do LED onboard
// Declaração da variável para o prefixo do tópico
char* topicPrefix = "sensor001";

void setup(){
  initConnection(default_SSID,default_PASSWORD,default_BROKER_MQTT,default_BROKER_PORT,default_TOPICO_SUBSCRIBE,default_TOPICO_PUBLISH_1,default_TOPICO_PUBLISH_2,default_ID_MQTT,default_D4,topicPrefix);
}

void loop(){
  loopMethods();
}