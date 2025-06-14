#include <WiFi.h>
#include <PubSubClient.h>

// Wi-Fi credentials
const char* ssid = "TP-LINK_EA32";
const char* password = "11642251";

// MQTT broker info
const char* mqtt_server = "192.168.1.95"; // You can use test.mosquitto.org, etc.
const int mqtt_port = 1883;
const char* mqtt_topic = "esp32/test";

WiFiClient espClient;
PubSubClient client(espClient);

// Function to connect to WiFi
void setup_wifi() {
  delay(10);
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to WiFi. IP: ");
  Serial.println(WiFi.localIP());
}

// Function to connect to MQTT broker
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP32Client-" + String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // client.subscribe("your/topic"); // optional
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" retrying in 5 seconds...");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Simulate data (e.g. temperature)
  float temperature = random(20, 30) + random(0, 100) / 100.0;

  // Convert to string and publish
  char msg[50];
  snprintf(msg, 50, "Temperature: %.2f C", temperature);
  Serial.print("Publishing message: ");
  Serial.println(msg);

  client.publish(mqtt_topic, msg);

  delay(5000); // Wait 5 seconds
}
