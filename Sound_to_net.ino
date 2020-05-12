#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <Hash.h>

const char* ssid = "Embeded113";
const char* password = "Embeded113";
int previous = 0;
unsigned long previousMillis = 0;
const long interval = 10;

String WebPage = "<!DOCTYPE html><html><style>input[type=\"button\"]{width: 20%;height: 3.6vh;}.rxd{height: 90vh;}textarea {width: 100%;height: 20%;font-size: 18px;text-align-last: center;}</style><script>var Socket;function start() \n{Socket = new WebSocket('ws://' + window.location.hostname + ':81/');Socket.onmessage = function(evt) \n{var data = evt.data.toString();\n data = data.substring(0, (data.length-1)); \n var msg = JSON.parse(data); \nswitch(msg.type) {case \"on\":document.getElementById(\"rxConsole\").value = msg.value; break;case \"off\": document.getElementById(\"rxConsole\").value = msg.value; break; case \"temp\": document.getElementById(\"temp\").value = msg.value; break; case \"humi\": document.getElementById(\"humi\").value = msg.value; break; }}} function temperature() { Socket.send('z');} function humidity() {Socket.send('x');}function on() {Socket.send('c');} function off() {Socket.send('b');}</script><body onload=\"javascript:start();\"><br><div class=\"rxd\"><font size=\"6\"><center>Temperatura</center></font><div><textarea id=\"temp\" readonly></textarea><br><input type=\"button\" onclick=\"temperature();\" value=\"Odczyt temperatury\" ></div><br><font size=\"6\"><center>Wilgotnosc</center></font><div><textarea id=\"humi\" readonly></textarea><br><input type=\"button\" onclick=\"humidity();\" value=\"Odczyt wilgotnosci\" ></div><br><font size=\"6\"><center>Oswietlenie</center></font><div><textarea id=\"rxConsole\" readonly></textarea><br><input type=\"button\" onclick=\"on();\" value=\"Zapal\" ><input type=\"button\" onclick=\"off();\" value=\"Zgas\" ></div></div></body></html>";
//document.getElementById(\"txbuff\").value
WebSocketsServer webSocket = WebSocketsServer(81);
ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
   }
    
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    
    server.on("/", [](){
    server.send(200, "text/html", WebPage);
    });
    
    server.begin();
    
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
}

void loop() {
    webSocket.loop();
    server.handleClient();
    //unsigned long currentMillis = millis();
    //if (currentMillis - previousMillis >= interval) {
        //previousMillis = currentMillis;
        if (Serial.available()) {
          String str = Serial.readStringUntil('\n');
          int str_len = str.length(); 
          char c [str_len];
          str.toCharArray(c, str_len);
          Serial.println(c);
          webSocket.broadcastTXT(c, sizeof(c));
        }
    //}
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length){
   for(int i = 0; i < length; i++) Serial.print((char) payload[i]);
   Serial.println();
}

  /*char c[] = {(char)ad_value};
  webSocket.broadcastTXT(c, sizeof(c));
  bool val = 0;
  val=digitalRead(sensorPinDIG);
  char c2[] = {(char)val};
  webSocket.broadcastTXT(c2, sizeof(c2));*/
