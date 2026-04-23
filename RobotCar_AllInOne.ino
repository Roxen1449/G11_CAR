#include <WiFi.h>
#include <WebSocketsServer.h>

// 🔥 MUST MATCH YOUR PHONE HOTSPOT
const char* ssid = "Piyush's A26";
const char* password = "easy1234";

WebSocketsServer webSocket(81);

// Motor Pins
#define IN1 27
#define IN2 26
#define IN3 25
#define IN4 33
#define ENA 14
#define ENB 32

int speedVal = 180;

// ================= MOTOR =================
void move(char c){
  switch(c){
    case 'F':
      ledcWrite(ENA, speedVal);
      ledcWrite(ENB, speedVal);
      digitalWrite(IN1,HIGH); digitalWrite(IN2,LOW);
      digitalWrite(IN3,HIGH); digitalWrite(IN4,LOW);
      break;

    case 'B':
      digitalWrite(IN1,LOW); digitalWrite(IN2,HIGH);
      digitalWrite(IN3,LOW); digitalWrite(IN4,HIGH);
      break;

    case 'L':
      digitalWrite(IN1,LOW); digitalWrite(IN2,HIGH);
      digitalWrite(IN3,HIGH); digitalWrite(IN4,LOW);
      break;

    case 'R':
      digitalWrite(IN1,HIGH); digitalWrite(IN2,LOW);
      digitalWrite(IN3,LOW); digitalWrite(IN4,HIGH);
      break;

    case 'S':
      digitalWrite(IN1,LOW); digitalWrite(IN2,LOW);
      digitalWrite(IN3,LOW); digitalWrite(IN4,LOW);
      break;
  }
}

// ================= WEBSOCKET =================
void onWS(uint8_t num, WStype_t type, uint8_t * payload, size_t length){
  if(type == WStype_TEXT){
    char cmd = payload[0];
    move(cmd);
    Serial.print("CMD: ");
    Serial.println(cmd);
  }
}

// ================= SETUP =================
void setup(){
  Serial.begin(115200);

  pinMode(IN1,OUTPUT); pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT); pinMode(IN4,OUTPUT);

  ledcAttach(ENA,1000,8);
  ledcAttach(ENB,1000,8);

  // 🔥 CONNECT TO PHONE HOTSPOT
  WiFi.begin(ssid, password);

  Serial.print("Connecting to Hotspot");

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  Serial.println("\n✅ Connected!");
  Serial.print("ESP32 IP: ");
  Serial.println(WiFi.localIP());  // IMPORTANT

  webSocket.begin();
  webSocket.onEvent(onWS);
}

// ================= LOOP =================
void loop(){
  webSocket.loop();
}
