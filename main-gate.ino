#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#define LED 2
#define RELAY 0 // relay connected to  GPIO0
const char* ssid = "jasio"; // fill in here your router or wifi SSID
const char* password = "Polinezyjska7/6"; // fill in here your router or wifi password
const char* mqtt_broker = "192.168.1.3";
const char* topic = "GATE/MAIN";
const char *mqtt_username = "lkulis";
const char *mqtt_password = "qwer12";
const int mqtt_port = 1883;

bool ledState = false;

WiFiClient espClient;
PubSubClient client(espClient);

char msg[50];
int value = 0;
const char* device_name = "ESP-01S";

void setup() {

  Serial.begin(115200);

  pinMode(RELAY,OUTPUT);
  digitalWrite(RELAY, LOW);
 
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: "); 
  Serial.println(WiFi.localIP ());


// Setting LED pin as output
    pinMode(LED, OUTPUT);
    digitalWrite(LED, HIGH);  // Turn off the LED initially

    // Connecting to an MQTT broker
    client.setServer(mqtt_broker, mqtt_port);
    client.setCallback(callback);
    while (!client.connected()) {
        String client_id = "esp8266-client-";
        client_id += String(WiFi.macAddress());
        Serial.printf("The client %s connects to the public MQTT broker\n", client_id.c_str());
        if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
            Serial.println("");
            Serial.println("HOME ASSISTANT MQTT broker connected");
        } else {
            Serial.print("Failed with state ");
            Serial.print(client.state());
            delay(2000);
        }
    }
    client.subscribe(topic);
}

void callback(char *topic, byte *payload, unsigned int length) {
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("Message: ");
    String message;
    for (int i = 0; i < length; i++) {
        message += (char) payload[i];  // Convert *byte to string
    }
    Serial.print(message);
    if (message == "OPEN" && !ledState) {
        digitalWrite(LED, LOW);  // Turn on the LED
        digitalWrite(RELAY, HIGH);
        delay(1000);
        digitalWrite(LED, HIGH);  // Turn off the LED
        digitalWrite(RELAY, LOW);

//        ledState = true;
    }
    if (message == "off" && ledState) {
        digitalWrite(LED, HIGH); // Turn off the LED
        ledState = false;
    }
    Serial.println();
    Serial.println("-----------------------");
}

void loop() {
    client.loop();
    delay(100); // Delay for a short period in each loop iteration
}

