#include <SPI.h>          
#include <ESP8266WiFiMulti.h>
#include <BlynkSimpleEsp8266.h>
#include "RF24.h"
#include "nRF24L01.h"    

// Objetos
WiFiClient client; // Modo cliente
ESP8266WiFiMulti wifiMulti; // Modo servidor
RF24 radio(D4, D8); // Rádio

// Var's necessárias para o envio e recebimento de mensagens
// Configuração do rádio:
byte enderecos[][6] = {"1node","2node"};

// Envio:
char sendMessage[7];
char *search = "-";
char servo_str[4];
char motor_str[4];
int servo = 90;
int motor = 0;

// Recebimento:
char receiveMessage[37];
char prox_str[4];
char temp_str[16];
char umid_str[16];
int proximity = 0;
int temperature = 0;
int humidity = 0;

boolean changes = true;

// Cliente Wifi
const int httpPort = 80;
const char* host = "curiasity.000webhostapp.com";

// Quando houver modificação no joystick virtual
BLYNK_WRITE(V0){
   servo = param[0].asInt();
   motor = param[1].asInt();
   if (motor <= 0) {
    motor = 0;
   } else {
    motor = 1;
   }
   changes = true;
}

// Quando botão de envio para banco de dados for pressionado
BLYNK_WRITE(V1){
      digitalWrite(LED_BUILTIN, LOW);
      delay(300);
      String url = "/api/insert.php?temp=" + String(temperature) + "&umid="+ String(humidity) + "&prox="+ String(proximity);
      if (!client.connect(host, httpPort)) {
        Serial.println("connection failed");
        return;
      }
      Serial.println(url);
      client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" +  "Connection: close\r\n\r\n");
      delay(300);
      digitalWrite(LED_BUILTIN, HIGH);
      Blynk.notify("Disponível em:\n\n bit.ly/curiasity19");
}

// Quando botão de reset for pressionado
BLYNK_WRITE(V5){
   ESP.reset();
}

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(9600);  
    start_wifi();
    radio.begin();
    radio.setChannel(100);
    radio.openWritingPipe(enderecos[0]); 
    radio.openReadingPipe(1, enderecos[1]);
    radio.startListening(); 
    pinMode(D3, OUTPUT);
}

void loop() {
  Blynk.run();

  // Se houver alguma alteração no joystick, os comandos serão coletados para envio ao Arduíno
  if (changes) {
    digitalWrite(D2, HIGH);
    radio.stopListening(); // Pare de ouvir
    sprintf(servo_str, "%d", servo);
    sprintf(motor_str, "%d", motor);
    sprintf(sendMessage, "%s-%s", servo_str, motor_str);
    if (radio.write( &sendMessage, sizeof(sendMessage) )) { // Concantena os dados para envio em uma string
      Serial.print("Envia Pacote: Servo: ");
      Serial.print(servo);
      Serial.print(", Motor: ");
      Serial.println(motor); 
    } else {
      Serial.println("Error to send! :(");  
    }
    changes = false;
    radio.startListening(); // Volte a ouvir o canal
    digitalWrite(D2, LOW);
  }

  // Testa se há mensagens novas
  if (radio.available()) {
    radio.read(&receiveMessage, sizeof(receiveMessage)); // Recebe a mensagem, e 'destrincha' nas medidas dos sensores
    digitalWrite(D3, LOW); 
    char *prox_str = strtok(receiveMessage, search);
    char *temp_str = strtok(NULL, search);
    char *umid_str = strtok(NULL, search);
    proximity = atof(prox_str);
    temperature = atof(temp_str);
    humidity = atof(umid_str);
    Serial.print("Recebe Pacote: Ultrassonico: ");
    Serial.print(proximity);
    Serial.print(", Temp: ");
    Serial.print(temperature); 
    Serial.print(", Umid: ");
    Serial.println(humidity);  


    // Envio para o blynk
    Blynk.virtualWrite(V2,proximity);
    Blynk.virtualWrite(V3,temperature);
    Blynk.virtualWrite(V4,humidity);
    
    digitalWrite(D3, HIGH);   
  }
}

void start_wifi() {
  char ssid[] = "YOUR_SSID_WIFI";
  char pass[] = "YOUR_PASSWORD_WIFI";
  char auth[] = "YOUR_KEY_AUTH_BLINK";
  Serial.println("Conectando");
  wifiMulti.addAP(ssid, pass);
  
  while (wifiMulti.run() != WL_CONNECTED) {  
    Serial.print(".");
    delay(300);
  }
  
  Serial.println("\r\n");
  Serial.print("Conectado a rede ");
  Serial.println(WiFi.SSID());
  Serial.print("Endereco IP: ");
  Serial.print(WiFi.localIP());
  Serial.println("\r\n");
  
  Blynk.begin(auth, ssid, pass);
}
