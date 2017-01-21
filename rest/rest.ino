/*
  This a simple example of the aREST Library for the ESP8266 WiFi chip.
  See the README file for more details.

  Written in 2015 by Marco Schwartz under a GPL license.
*/

// Import required libraries
#include <ESP8266WiFi.h>
#include <aREST.h>
#include <SevenSegment.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

// Create aREST instance
aREST rest = aREST();

sevenSegment ss(14, 12);
// WiFi parameters
const char* ssid = "DZ";
const char* password = "**********";

// The port to listen for incoming TCP connections
#define LISTEN_PORT           80
#define LED_PIN               0
#define BUZZER_PIN            1
#define DHTPIN                2 
#define DHTTYPE               DHT11


// Create an instance of the server
WiFiServer server(LISTEN_PORT);

DHT_Unified dht(DHTPIN, DHTTYPE);

// Variables to be exposed to the API
int temperature;
int humidity;


// Declare functions to be exposed to the API
int ledControl(String command);

void setup(void)
{
  // Start Serial
  Serial.begin(115200);

  dht.begin();
  sensor_t sensor;
  // Init variables and expose them to REST API
  

  // Function to be exposed
  rest.function("blink",ledBlink);
  rest.function("beep",beep);
  rest.function("seven",seven);
  rest.function("temp",temp);

  // variables exposed
  rest.variable("temperature",&temperature);
  rest.variable("humidity",&humidity);

  // Give name & ID to the device (ID should be 6 characters long)
  rest.set_id("7bf7b8");
  rest.set_name("NodeMCU_Mohsens_room");

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
}

void loop() {

  // Handle REST calls
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  while(!client.available()){
    delay(1);
  }
  rest.handle(client);

}

// Custom function accessible by the API
int ledBlink(String number) {

  // Get state from command
  int num = number.toInt();

  for(int i = 0; i < num; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(40);
    digitalWrite(LED_PIN, LOW);
    delay(80);
  }
  
  return 0;
}

int beep(String number) {

  // Get state from command
  int num = number.toInt();

  for(int i = 0; i < num; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(40);
    digitalWrite(BUZZER_PIN, LOW);
    delay(80);
  }
  
  return 0;
}

int seven(String command) {

  // Get state from command
  int num = command.toInt();

  ss.showHex(num);
  return num;
}

int temp(String command) {
 sensors_event_t event; 
 if (isnan(event.temperature)) {
    Serial.println("Error reading temperature!");
    return -1;
  }
  else {
    Serial.print("Temperature: ");
    Serial.print(event.temperature);
    Serial.println(" *C");
    temperature = event.temperature;
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println("Error reading humidity!");
    return -1;
  }
  else {
    Serial.print("Humidity: ");
    Serial.print(event.relative_humidity);
    Serial.println("%");
    humidity = event.relative_humidity;
  }
}

