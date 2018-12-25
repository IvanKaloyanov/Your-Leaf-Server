#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <dht.h>

#define HEATING_PIN 3 
#define TEMPERATURE_SENSOR_PIN 4 
#define LIGHTNING_PIN 6 
#define LIGHTNING_SENSOR_PIN 1  
#define WATERING_SENSOR_PIN 0  
#define WATERING_PIN 5 

// Router credentials
const char* ROUTER_NAME = "Milanelo";
const char* ROUTER_PASSWORD = "Rossonero";

// Leaf password
const char* PASSWORD = "1001100001011110000110011";

// routes
const char* LOGIN_ROUTE = "/login";
const char* WATERING_ROUTE = "/watering";
const char* LIGHTNING_ROUTE = "/lightning";
const char* TEMPERATURE_ROUTE = "/temperature";

// request consts
const char* TEXT_PLAIN = "text/plain";
const char* APPLICATION_JSON = "application/json";
const char* WATERING_ARGUMENT = "amount";

// status codes consts
const int STATUS_OKAY = 200;
const int STATUS_NOT_FOUNT = 404;
const int STATUS_UNAUTHORIZED = 401;

// server
ESP8266WebServer server(80);

// temperature sensor
dht DHT;

// temperature manage
float temperatureSensorValue = 0;
bool heatingState = false;

// watering manage
float soilSensorValue = 0;  
float soilSensorOptimal = 13; // set it yourself based on your plant. 

// lightning manage
float lightningSensorValue = 0;
bool lightningState = false;


void handleWateringGet(){
  // Check the credentials
  if (server.authenticate("", PASSWORD)) {
    
    // Get the SEN-13322 sensor data and retrive it
    int soilSensorCrn = analogRead(WATERING_SENSOR_PIN);
    
    // Converting with 512 because the values are relatively small
    soilSensorValue = (soilSensorCrn / 512) * 100;
    String response = "{ value:" + String(soilSensorValue,2) + ", optimal:" +
    String(soilSensorOptimal,2) + " }";
    server.send(STATUS_OKAY, APPLICATION_JSON, response);
  }
  else{
    // Send 401 Unauthorized if the credentials are wrong
    server.send(STATUS_UNAUTHORIZED, APPLICATION_JSON, "");
  }
}

void handleWateringPost(){
  // Check the credentials 
  if (server.authenticate("", PASSWORD) && server.arg(WATERING_ARGUMENT)!= "") {
    server.send(STATUS_OKAY, APPLICATION_JSON, "");
    // Response and do the watering
    int wateringAmount = server.arg(WATERING_ARGUMENT).toInt();
    digitalWrite(WATERING_PIN, HIGH);
    delay(1000 * wateringAmount);
    digitalWrite(WATERING_PIN, LOW);
  }
  else{
    // Send 401 Unauthorized if the credentials are wrong
    server.send(STATUS_UNAUTHORIZED, APPLICATION_JSON, "");
  }
}

void handleLightningGet(){
  // Check the credentials 
  if (server.authenticate("", PASSWORD)) {
    
    // Get the photoresistor data and retrive it
    int lightSensorCrn = analogRead(LIGHTNING_SENSOR_PIN);
    lightningSensorValue = (lightSensorCrn / 1023) * 100;
    String response = "{ value:" + String(lightningSensorValue,2) 
    + ", state:" + boolToString(lightningState) + " }";
    server.send(STATUS_OKAY, APPLICATION_JSON, response);
  }
  else{
    // Send 401 Unauthorized if the credentials are wrong
    server.send(STATUS_UNAUTHORIZED, APPLICATION_JSON, "");
  }
}

void handleLightningPost(){
  // Check the credentials 
  if (server.authenticate("", PASSWORD)) {
    // Turn the lightning on and off 
    lightningState = !lightningState;
    if(lightningState){
       digitalWrite(LIGHTNING_PIN, HIGH);
    }
    else{
      digitalWrite(LIGHTNING_PIN, LOW);
    }
    String response = "{ value:" + String(lightningSensorValue,2) 
    + ", state:" + boolToString(lightningState) + " }";
   
    server.send(STATUS_OKAY, APPLICATION_JSON, response);
  }
  else{
     // Send 401 Unauthorized if the credentials are wrong
    server.send(STATUS_UNAUTHORIZED, APPLICATION_JSON, "");
  }
}

void handleTemperatureGet(){
  // Check the credentials 
  if (server.authenticate("", PASSWORD)) {
    // Get the DHT sensor data and retrive it
    DHT.read11(TEMPERATURE_SENSOR_PIN);
    temperatureSensorValue = DHT.temperature;
    String response = "{ value:"+ String(temperatureSensorValue, 2) 
    + ", state:" + boolToString(heatingState) + " }";
    Serial.println("TEST: GET temperature " + response);
    server.send(STATUS_OKAY, APPLICATION_JSON, response);
  }
  else{
    // Send 401 Unauthorized if the credentials are wrong
    server.send(STATUS_UNAUTHORIZED, APPLICATION_JSON, "");
  }
}

void handleTemperaturePost(){
  if (server.authenticate("", PASSWORD)) {
    // Turn the heating on and off
    heatingState = !heatingState;
    if(heatingState){
      digitalWrite(HEATING_PIN, HIGH);
    }
    else{
      digitalWrite(HEATING_PIN, LOW);
    }
    String response = "{ value:"+ String(temperatureSensorValue, 2) 
    + ", state:" + boolToString(heatingState) + " }";
    server.send(STATUS_OKAY, APPLICATION_JSON, response);
  }
  else{
    // Send 401 Unauthorized if the credentials are wrong
    server.send(STATUS_UNAUTHORIZED, APPLICATION_JSON, "");
  }
}

void handleLogin(){
  // Used for login / setup from the Android App
  if (server.authenticate("", PASSWORD)) {
    Serial.print("Loged in");
    server.send(STATUS_OKAY, APPLICATION_JSON, "");
  }
  else{
    Serial.print("Wrong password in");
    server.send(STATUS_UNAUTHORIZED, APPLICATION_JSON, "");
  }
}

 // Wrong request handler (for debug mainly)
void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(STATUS_NOT_FOUNT, TEXT_PLAIN, message);
  Serial.print(message);
}

// Quick bool to String parser
String boolToString(bool state){
  return state ? "true" : "false";
}
 
void setup(void){
  
  // Setup the port communication
  Serial.begin(115200);

  // Setup the pins
  pinMode(HEATING_PIN, OUTPUT);
  pinMode(WATERING_PIN, OUTPUT);
  pinMode(LIGHTNING_PIN, OUTPUT);
  pinMode(LIGHTNING_SENSOR_PIN, INPUT);
  pinMode(WATERING_SENSOR_PIN, INPUT);
  
  // Starting the connection
  WiFi.begin(ROUTER_NAME, ROUTER_PASSWORD);
  
  Serial.print("Establishing the connection.");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ROUTER_NAME);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  ArduinoOTA.begin();
  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }
  
  // Setup the server routes
  server.on(WATERING_ROUTE, HTTP_GET, handleWateringGet);
  server.on(WATERING_ROUTE, HTTP_POST, handleWateringPost);

  server.on(LIGHTNING_ROUTE, HTTP_GET, handleLightningGet);
  server.on(LIGHTNING_ROUTE, HTTP_POST, handleLightningPost);

  server.on(TEMPERATURE_ROUTE, HTTP_GET, handleTemperatureGet);
  server.on(TEMPERATURE_ROUTE, HTTP_POST, handleTemperaturePost);

  server.on(LOGIN_ROUTE, HTTP_POST, handleLogin);
 
  server.onNotFound(handleNotFound);
  
  // Start the server
  server.begin();
}
 
void loop(void){
  // Handle requests
  ArduinoOTA.handle();
  server.handleClient();
}
