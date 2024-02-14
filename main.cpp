
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// conectado al puerto D4 de Wemos Lite
#define ONE_WIRE_BUS 2  

OneWire oneWire(ONE_WIRE_BUS); // inicia una instancia de OneWire

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire); 

// Update these with values suitable for your network.

const char* ssid = "FRANKFURT";       //Credenciales de Red
const char* password = "natalia-natalia";
const char* mqtt_server = "192.168.68.108"; 
String horas = "";      // Variable donde se guardara la hora recibida por MQTT
String minutos = "";    // Variable donde se guardara los minutos recibida por MQTT
bool flag = 0;


WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)  // tamaño del array de caractares que sera enviado como payload del mensaje MQTT
char msg[MSG_BUFFER_SIZE];  // array de caractares que sera enviado como payload del mensaje MQTT
int value = 0;

// Varialbe para guardar temporalmente las lecturas de los sensores 
// Convertir int --> String
String mensaje = "";        

void setup_wifi() {

  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
void callback(char* topic, byte* payload, unsigned int length) {
// Variable en las que ser guarda Tema MQTT recibido 
  String tema = ""; 
  horas = "";
  minutos = "";
  tema = String(topic);
//  Serial.println("minutos recibido");
  Serial.print ("Tema: ");
  Serial.println(tema);
// Si el tema es CASA/HORA/HORA; guarda el payload en la variable horas
  if (tema == "CASA/HORA/HORA")
  {
    for (int i = 0; i < length; i++) {
      horas = horas + (char)payload[i];
      }
    }
// Si el tema es CASA/HORA/MINUTOS; guarda el payload en la variable minutos

  if (tema == "CASA/HORA/MINUTOS")
  {
    for (int i = 0; i < length; i++) {
      minutos = minutos + (char)payload[i];
      }
    }

    // Setea la bandera que marca que recibimos un mensaje de tema CASA/HORA
    flag = 1;   
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");

      // call sensors.requestTemperatures() to issue a global temperature 
      // request to all devices on the bus
      Serial.print("Requesting temperatures...");
      sensors.requestTemperatures(); // Send the command to get temperatures
      Serial.println("DONE");
      // After we got the temperatures, we can print them here.
      // We use the function ByIndex, and as an example get the temperature from the first sensor only.
      float tempC = sensors.getTempCByIndex(0);
      // Check if reading was successful
      if(tempC != DEVICE_DISCONNECTED_C) 
        {
        Serial.print("Temperature for the device 1 (index 0) is: ");
        Serial.println(tempC);
        //snprintf (msg, MSG_BUFFER_SIZE, "hello world #%ld", value);
        //snprintf (msg, MSG_BUFFER_SIZE, "%ld", (char)tempC);
        mensaje =  String(tempC);
        mensaje.toCharArray(msg, MSG_BUFFER_SIZE);
        
        //msg = (char)tempC;
        Serial.print("Publish message: ");
        Serial.println(msg);
        client.publish("CASA/JARDIN/TEMPERATURA", msg);
        //    client.publish("CASA/JARDIN/TEMPERATURA", msg);

      } 
      else
      {
        Serial.println("Error: Could not read temperature data");
      }









//      client.subscribe("CASA/HORA/MINUTOS");
      client.subscribe("CASA/HORA/#");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  
  // Start up DS18b20 the library
  sensors.begin();
}



void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  if (flag == 1){
    Serial.print ("HORA: ");
    Serial.println( horas);
    Serial.print ("MINUTOS: ");
    Serial.println( minutos);
    flag = 0;
//    if (minutos == "0" || minutos == "10" || minutos =="20" || minutos == "30" || minutos == "40" || minutos == "50"){
    if (1){
      Serial.println ("alcoyana-alcoyana");
      // call sensors.requestTemperatures() to issue a global temperature 
      // request to all devices on the bus
      Serial.print("Requesting temperatures...");
      sensors.requestTemperatures(); // Send the command to get temperatures
      Serial.println("DONE");
      // After we got the temperatures, we can print them here.
      // We use the function ByIndex, and as an example get the temperature from the first sensor only.
      float tempC = sensors.getTempCByIndex(0);
      // Check if reading was successful
      if(tempC != DEVICE_DISCONNECTED_C) 
        {
        Serial.print("Temperature for the device 1 (index 0) is: ");
        Serial.println(tempC);
        //snprintf (msg, MSG_BUFFER_SIZE, "hello world #%ld", value);
        //snprintf (msg, MSG_BUFFER_SIZE, "%ld", (char)tempC);

        mensaje =  String(tempC);

        mensaje.toCharArray(msg, MSG_BUFFER_SIZE);

        
        //msg = (char)tempC;
        Serial.print("Publish message: ");
        Serial.println(msg);
        client.publish("CASA/JARDIN/TEMPERATURA", msg);
        //    client.publish("CASA/JARDIN/TEMPERATURA", msg);

      } 
      else
      {
        Serial.println("Error: Could not read temperature data");
      }

    }  
  }
}


/*
  unsigned long now = millis();
  if (now - lastMsg > 1000) {
    lastMsg = now;
    ++value;
    snprintf (msg, MSG_BUFFER_SIZE, "hello world #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("CASA/JARDIN/TEMPERATURA", msg);
  }
*/