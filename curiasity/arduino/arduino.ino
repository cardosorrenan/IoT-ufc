#include <Servo.h>
#include <SPI.h>
#include "DHT.h"
#include "nRF24L01.h"
#include "RF24.h"
#include <string.h>
#include <NewPing.h>
#include <Adafruit_Sensor.h>

 
// Objetos
NewPing sonar(4, 5, 400); // Ultrassonico
Servo myservo; // Servo Motor
DHT dht(8, DHT11); // Sensor DHT11
RF24 radio(10, 3); // Radio


// Var's necessárias para o envio e recebimento de mensagens
// Configurações do rádio:
byte enderecos[][6] = {"1node","2node"};
unsigned int timeProx = 750;
unsigned long timeToSend;


// Envio:
char sendMessage[37];
char prox_str[4];
char temp_str[16];
char umid_str[16];
int proximity = 0;
float t = 0;
float h = 0;
float t_prev = 0;
float h_prev = 0;

// Recebimento
char receiveMessage[7];
char *search = "-";
int servo = 90;
int motor = 0;
int switch_motor;

void setup() {
   
  // put your setup code here, to run once:
  Serial.begin(9600);
  timeToSend = millis();
  dht.begin();
  radio.begin();
  radio.setChannel(100);
  radio.openWritingPipe(enderecos[1]);	
  radio.openReadingPipe(1, enderecos[0]);
  radio.startListening();
  myservo.attach(9);
  myservo.write(servo);
  pinMode(2, OUTPUT);
}

 
void loop() {
		
  // Testa se o intervalo de envio (700ms) estourou, se sim, envia.
  if (millis() >= timeToSend) {
    radio.stopListening(); // Pare de ouvir
    timeToSend += timeProx;
    proximity = sonar.ping_cm(); 
    sprintf(prox_str, "%d", proximity);  
    t = dht.readTemperature();
    h = dht.readHumidity();
    if (isnan(h) || isnan(t)) {
      t = t_prev;
      h = h_prev;
    } else {
      t_prev = t; 
      h_prev = h;
    }
    dtostrf(t, 2, 2, temp_str);
    dtostrf(h, 2, 2, umid_str);
    sprintf(sendMessage, "%s-%s-%s", prox_str, temp_str, umid_str);  // Concantena os dados para envio em uma string
    
    
    // Testa se a mensagem foi enviada com sucesso
    if (radio.write( &sendMessage, sizeof(sendMessage) )) { 
      Serial.print("Envia Pacote: Sensor: ");
      Serial.print(proximity);
      Serial.print(", Temperatura: ");
      Serial.print(t);
      Serial.print(", Umidade: ");
      Serial.println(h);  
    } else {
      Serial.println("Error to send!");  
    }
 
    radio.startListening(); // Volte a ouvir o canal
    
  }


  // Testa se há mensagens novas
  if (radio.available()) {
    radio.read(&receiveMessage, sizeof(receiveMessage)); // Recebe a mensagem, e 'destrincha' em comandos do servo e para o motor DC
    char *servo_str = strtok(receiveMessage, search);
    char *motor_str = strtok(NULL, search);
    servo = atoi(servo_str);
    motor = atoi(motor_str);
    switch_motor = motor;
    Serial.print("Recebe Pacote: Servo: ");
    Serial.print(servo);
    myservo.write(servo);
    Serial.print(", Motor: ");
    Serial.println(motor);
  }
    
  if(switch_motor) {
    analogWrite(2, 255);
  } else {
    analogWrite(2, 0);
  }
    
 
  delay(50);
}
