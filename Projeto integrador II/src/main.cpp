#include <Arduino.h>
#include <IRremote.h> //INCLUSÃO DE BIBLIOTECA 
#include <ESP8266WiFi.h>

//config Rede
const char* ssid     = "PROF.RAFAEL"; //Roteador
const char* password = ""; 
IPAddress ip(192,168,1,131); //ip do dispositivo 
IPAddress gateway(192,168,1,1); // gateway
IPAddress subnet(255,255,255,0); //mascara



WiFiServer server(80);


String header;

String Sligar = "Desligado";

String Sumidificar = "Desligado";

String Svelocidade = "Desativado";

String Soscilacao = "Desativado";

String Sresfriar = "Desativado";

String Stimer = "Desativado";




int IRSenderPin = 5;//D1

unsigned long currentTime = millis();

unsigned long previousTime = 0; 

const long timeoutTime = 2000;

 

 

//Pro controle do alexandre




 
void setup(){
  
  Serial.begin(9600); //INICIALIZA A SERIAL
  IrSender.begin(IRSenderPin);
  
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  WiFi.config(ip, gateway, subnet);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}
 
void loop(){


 WiFiClient client = server.available();   

  if (client) {                             
    Serial.println("New Client.");          
    String currentLine = "";                
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { 
      currentTime = millis();         
      if (client.available()) {             
        char c = client.read();            
        Serial.write(c);                    
        header += c;
        if (c == '\n') {                    
         
          if (currentLine.length() == 0) {
            
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
           
            if (header.indexOf("GET /AR/Ligado") >= 0) {
              Serial.println("on");
              IrSender.sendNEC(0x80, 0x12, 2);
              Sligar = "Ligado";
              
            } else if (header.indexOf("GET /AR/Desligado") >= 0) {
              Serial.println("off");
              IrSender.sendNEC(0x80, 0x12, 2);
              Sligar = "Desligado";
              
            }

              if (header.indexOf("GET /UMIDIFICAR/Ligado") >= 0) {
              Serial.println("on");
              IrSender.sendNEC(0x80, 0x4, 2);
              Sumidificar = "Ligado";
              
            } else if (header.indexOf("GET /UMIDIFICAR/Desligado") >= 0) {
              Serial.println("off");
              IrSender.sendNEC(0x80, 0x4, 2);
              Sumidificar= "Desligado";
              
            }

               if (header.indexOf("GET /VELOCIDADE/Ativado") >= 0) {
              Serial.println("on");
              IrSender.sendNEC(0x80, 0x5 , 2);
              Svelocidade = "Ativado";
              
            } else if (header.indexOf("GET /VELOCIDADE/Desligado") >= 0) {
              Serial.println("off");
              IrSender.sendNEC(0x80, 0x5, 2);
              Svelocidade = "Desativado";
              
            }

              if (header.indexOf("GET /OSCILACAO/Ativado") >= 0) {
              Serial.println("on");
              IrSender.sendNEC(0x80, 0x6, 2);
              Soscilacao = "Ativado";
              
            } else if (header.indexOf("GET /OSCILACAO/Desligado") >= 0) {
              Serial.println("off");
              IrSender.sendNEC(0x80, 0x6, 2);
              Soscilacao = "Desativado";
              
            }

             if (header.indexOf("GET /RESFRIAR/Ativado") >= 0) {
              Serial.println("on");
              IrSender.sendNEC(0x80, 0xA, 2);
              Sresfriar = "Ativado";
              
            } else if (header.indexOf("GET /RESFRIAR/Desligado") >= 0) {
              Serial.println("off");
              IrSender.sendNEC(0x80, 0xA, 2);
              Sresfriar = "Desativado";
              
            }

               if (header.indexOf("GET /TIMER/Ativado") >= 0) {
              Serial.println("on");
              IrSender.sendNEC(0x80, 0x1F, 2);
              Stimer = "Ativado";
              
            } else if (header.indexOf("GET /TIMER/Desligado") >= 0) {
              Serial.println("off");
              IrSender.sendNEC(0x80, 0x1F, 2);
              Stimer = "Desativado";
              
            }
            
          
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
          
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color:  #ffcbdb; border: none; color: black; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color:  #ffcbdb;}</style></head>");
           


            client.println("<body><h1>ESP8266 Web Server</h1>");
            client.println("<body><h2>Projeto Ar Condicionado</h2>");
            
             
            client.println("<p>Ar Condicionado " + Sligar + "</p>");
              
            if (Sligar=="Desligado") {
              client.println("<p><a href=\"/AR/Ligado\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/AR/Desligado\"><button class=\"button button2\">OFF</button></a></p>");
            } 
             client.println("<p>Umidificador " + Sumidificar + "</p>");
              
            if (Sumidificar=="Desligado") {
              client.println("<p><a href=\"/UMIDIFICAR/Ligado\"><button class=\"button\">Umidificar</button></a></p>");
            } else {
              client.println("<p><a href=\"/UMIDIFICAR/Desligado\"><button class=\"button button2\">Umidificar</button></a></p>");
            } 
            client.println("<p>Velocidade " + Svelocidade + "</p>");
             if (Svelocidade=="Desativado") {
              client.println("<p><a href=\"/VELOCIDADE/Ativado\"><button class=\"button\">Velocidade</button></a></p>");
            } else {
              client.println("<p><a href=\"/VELOCIDADE/Desativado\"><button class=\"button button2\">Velocidade</button></a></p>");
            }
            client.println("<p>Oscilar " + Soscilacao + "</p>"); 
            if (Soscilacao=="Desativado") {
              client.println("<p><a href=\"/OSCILACAO/Ativado\"><button class=\"button\">Oscilar</button></a></p>");
            } else {
              client.println("<p><a href=\"OSCILACAO/Desativado\"><button class=\"button button2\">Oscilar</button></a></p>");
            }
            client.println("<p>Ar Condicionado " + Sresfriar + "</p>");
            if (Sresfriar=="Desativado") {
              client.println("<p><a href=\"/RESFRIAR/Ativado\"><button class=\"button\">Resfriar</button></a></p>");
            } else {
              client.println("<p><a href=\"RESFRIAR/Desativado\"><button class=\"button button2\">Resfriar</button></a></p>");
            }
            client.println("<p>Ar Condicionado " + Stimer + "</p>");
            if (Stimer=="Desativado") {
              client.println("<p><a href=\"/TIMER/Ativado\"><button class=\"button\">Timer</button></a></p>");
            } else {
              client.println("<p><a href=\"TIMER/Desativado\"><button class=\"button button2\">Timer</button></a></p>");
            }

            client.println("</body></html>");
            
   
            client.println();
       
            break;
          } else { 
            currentLine = "";
          }
        } else if (c != '\r') {  
          currentLine += c;      
        }
      }
    }
    
    header = "";
    
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
