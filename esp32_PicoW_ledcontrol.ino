#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "YOUR_SSID"; 
const char* password = "YOUR_PASSWORD"; 
const char* mqtt_server = "XXX.XXX.X.XX";

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(100);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP32Publisher-";
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim(); // Remove any trailing newline characters

    if (input.equalsIgnoreCase("On") || input.equalsIgnoreCase("Off")) {
      Serial.print("Publishing message: ");
      Serial.println(input);
      client.publish("/home/led0", input.c_str());
    } else {
      Serial.println("Invalid input. Please enter 'On' or 'Off'.");
    }
  }
}

