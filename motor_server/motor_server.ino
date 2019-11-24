#include <LedControl.h>

#include <CertStoreBearSSL.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiType.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiClientSecureAxTLS.h>
#include <WiFiClientSecureBearSSL.h>
#include <WiFiServer.h>
#include <WiFiServerSecure.h>
#include <WiFiServerSecureAxTLS.h>
#include <WiFiServerSecureBearSSL.h>
#include <WiFiUdp.h>
#include "LedControl.h"

#define trig 2 //D4
#define echo 0 //D3
long duration;
int distance;

int fast=200;
int slow = 80;
const char* ssid = "SYNETWORK";
const char* password = "12345678";

WiFiServer server(80);
// 8X8로 X자를 표시할 수 있는 배열을 생성합니다.(1은 켜짐, 0은 꺼짐)

void setup() {
  pinMode(5,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(0,OUTPUT);
  pinMode(2,OUTPUT);
  
  
  digitalWrite(5,LOW);
  digitalWrite(4,LOW);
  digitalWrite(0,LOW);
  digitalWrite(2,LOW);
  

  pinMode(trig,OUTPUT);
  pinMode(echo,INPUT);

  Serial.begin(115200);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connecting to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  server.begin();
  Serial.println("Server started");
}
 
void loop() {
  WiFiClient client = server.available();
  if(!client) return;
  
  Serial.println("새로운 클라이언트");
  client.setTimeout(5000);
  
  String request = client.readStringUntil('\r');
  Serial.println("request: ");
  Serial.println(request);
  /*초음파 센서*/
  
  Serial.begin(9600);
 
  digitalWrite(trig,LOW);
  delayMicroseconds(2);

  digitalWrite(trig,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig,LOW);


  if(request.indexOf("/stop") != -1) {
    
    digitalWrite(5,LOW);
    digitalWrite(4,LOW);
    digitalWrite(0,HIGH);
    digitalWrite(2,HIGH);
    
    delay(1500);
  }
  else if(request.indexOf("/start") != -1) {
    duration = pulseIn(echo,HIGH);
    distance = duration*0.034/2;
    if(distance <20){
      
      digitalWrite(5,LOW);
      digitalWrite(4,LOW);
      digitalWrite(0,HIGH);
      digitalWrite(2,HIGH);
      delay(2000);
    }
    else{
      
      digitalWrite(5,HIGH);
      digitalWrite(4,HIGH);
      digitalWrite(0,HIGH);
      digitalWrite(2,HIGH);
      
      
    }

  }
  else {
    Serial.println("invalid request");
    
  }
 
  while(client.available()) {
    client.read();
  }
 
  client.print(
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Connection: close\r\n"  // the connection will be closed after completion of the response
            "Refresh: 1\r\n"        // refresh the page automatically every 20 sec
            "\r\n");
  client.print("<!DOCTYPE HTML>");
  client.print("<html>");
  client.print("<head>"); 
  client.print("<meta&nbsp;charset=\"UTF-8\">");
  client.print("<title>세타 Control Webpage</title>");
  client.print("</head>");
  client.print("<style>.title {background-color:#c4dff6; color:gray;}body {padding:20px;}</style>");
  client.print("<body>");
  client.print("<h1 class='title'>Get Safe Your Board! SAE THA</h1>");                                                                  
  client.print("<h5>Auto Kick Board System</h5>");
  client.print("<a href='/start'>Start</a>");
  client.print("&nbsp");
  client.print("<a href='/stop'>Stop</a>");
  client.print("</h2>");
  client.print("<br>");
  client.print("</body>");
  client.print("</html>");
 
  Serial.println("클라이언트 연결 해제");
}
