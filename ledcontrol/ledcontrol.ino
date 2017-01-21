#include <ESP8266mDNS.h>

#include <SevenSegment.h>

#include <ESP8266WiFi.h>


const char* ssid = "DZ";
const char* password = "dastjerdiz";
 
int ledPin = 13; // GPIO13 - d7
String get_speed = "";
int speed = 1000;

int ledState = LOW;
unsigned long previousMillis = 0;  

WiFiServer server(80);
sevenSegment ss(14, 12);
void setup() {
  Serial.begin(115200);
  delay(10);
 
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
 
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
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
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
 if (!MDNS.begin("board")) {
    Serial.println("Error setting up MDNS responder!");
    while(1) { 
      delay(1000);
    }
  }
}
 
void loop() {

  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
 
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
 
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
 
  // Match the request
 
  int value = LOW;
  if (request.indexOf("?speed=") != -1)  {
    int index = request.indexOf("?speed=");
    index += 7;
    
    get_speed = request.substring(index);
    speed = get_speed.toInt();
    analogWrite(ledPin, speed);
    ss.showHex(speed);
  }
  if (request.indexOf("/LED=OFF") != -1)  {
    digitalWrite(ledPin, LOW);
    value = LOW;
  }
 
// Set ledPin according to the request
//digitalWrite(ledPin, value);
 
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html><head><title>NodeMCU</title></head><body>");
  client.println("<meta name='viewport' content='width=device-width, initial-scale=1'>");
  client.print("Led intensity is now: ");
  client.println(speed);
  client.print("<br>led pin number:");
  client.print(ledPin);
  client.println("<br>");
  client.println("<form method='get' action='/'>");
  client.println("intensity (0 - 1024): <br><input type='text' name='speed'>");
  client.println("<input type='submit' value='Submit'>");
  client.println("</form>");
  client.println("<br>");
  client.print("Uptime: ");
  client.print(millis() / 1000);
  client.print("s");
  client.println("</body></html>");
 
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");

}
 
