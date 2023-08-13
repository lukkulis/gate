#include <ESP8266WiFi.h>
const char* ssid = "jasio"; // Enter your router or WiFi SSID here
const char* password = "Polinezyjska7/6"; // Enter your router or WiFi password here
#define RELAY 0 // connected to GPIO0
WiFiServer server(80);
void setup(){
 Serial.begin(115200); // must have the same baud rate as the serial monitor
 pinMode(RELAY,OUTPUT);
 digitalWrite(RELAY, LOW);
 // Connect to the WiFi network
 Serial.println();
 Serial.println();
 Serial.print("Connecting to ");
 Serial.println(ssid);
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED){
 delay(500);
 Serial.print(".");
 }
 Serial.println("");
 Serial.println("WiFi connected");
 // Start the Server
 server.begin();
 Serial.println("Server started");
 // Output of the IP address
 Serial.print("Use this URL to connect: ");
 Serial.print("https://");
 Serial.print(WiFi.localIP());
 Serial.println("/");
}
void loop(){
 // Check whether a user has established a connection
 WiFiClient client = server.available();
 if (!client){
 return;
 }
 // Wait until the user sends data
 Serial.println("new client");
 while(!client.available()){
 delay(1);
 }
 // Reading the first line of the request
 String request = client.readStringUntil('\r');
 Serial.println(request);
 client.flush();

 // Assignment of the request
 int value = LOW;
 if (request.indexOf("/RELAY=ON") != -1){
 Serial.println("RELAY=ON");
 digitalWrite(RELAY,LOW);
 value = LOW;
 }
 if (request.indexOf("/RELAY=OFF") != -1){
 Serial.println("RELAY=OFF");
 digitalWrite(RELAY,HIGH);
 value = HIGH;
 }
// Return the answer
 client.println("HTTP/1.1 200 OK");
 client.println("Content-Type: text/html");
 client.println(""); // this is necessary
 client.println("<!DOCTYPE HTML>");
 client.println("<html>");
 client.println("<meta name='viewport' content='width=device-width, initial-scale=1'>");
 client.println("<head><title>ESP8266 RELAY Control</title></head>");
 client.println("<style>");
 client.println("* {text-align: center; font-family: Consolas;}");
 client.println("");
 client.println(".container {position: absolute; top:50%;");
 client.println("left: 50%; transform: translateX(-50%)translateY(-50%);}");
 client.println("");
 client.println("a {display: inline-block; width: 50px;");
 client.println("color: #FFFFFF; background-color:#275C6B;");
 client.println("margin-top: 15px; padding: 10px 20px;");
 client.println("text-decoration: none; border-radius:5px;");
 client.println("font-weight: 800;}");
 client.println("</style>");
 client.println("<div class='container'>");
 client.println("<div class='inner'>");
 client.print("Relay is now: ");
 if(value == HIGH){client.print("OFF");}
 else{client.print("ON");}
 client.println("<br>");
 client.println("<a href=\"/RELAY=OFF\">OFF</a>");
 client.println("<a href=\"/RELAY=ON\">ON</a>");
 client.println("</div>");
 client.println("</div>");
 client.println("</html>");
 Serial.println("");
}
