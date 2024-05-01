

//Configuração de conexão do Esp32
const char* default_SSID = "sua_rede_wifi"; // Nome da rede Wi-Fi
const char* default_PASSWORD = "sua_senha_wifi"; // Senha da rede Wi-Fi
const char* default_BROKER_MQTT = "ip_host_fiware"; // IP do Broker MQTT
const int default_BROKER_PORT = 1883; // Porta do Broker MQTT
const char* default_TOPICO_SUBSCRIBE = "/TEF/lamp001/cmd"; // Tópico MQTT de escuta
const char* default_TOPICO_PUBLISH_1 = "/TEF/lamp001/attrs"; // Tópico MQTT de envio de informações para Broker
const char* default_TOPICO_PUBLISH_2 = "/TEF/lamp001/attrs/l"; // Tópico MQTT de envio de informações para Broker
const char* default_ID_MQTT = "fiware_001"; // ID MQTT
const int default_D4 = 2; // Pino do LED onboard
// Declaração da variável para o prefixo do tópico
const char* topicPrefix = "lamp001";

void setup(){
  initConnection(default_SSID,default_PASSWORD,default_BROKER_MQTT,default_BROKER_PORT,default_TOPICO_SUBSCRIBE,default_TOPICO_PUBLISH_1,default_TOPICO_PUBLISH_2,default_ID_MQTT,default_D4,topicPrefix);
}

void loop(){
  loopMethods();
}