#include           
#include  //Sensor Suhu Air
#include  //Sensor Suhu Air
#include     

#define WIFI_AP_NAME "xxxxxxxxxxx"
#define WIFI_PASSWORD "xxxxxxxxxxx"
#define TOKEN "xxxxxxxxxxx"
#define THINGSBOARD_SERVER "demo.thingsboard.io"
#define SERIAL_DEBUG_BAUD 115200

#define ONE_WIRE_BUS 13 //Sensor Suhu Air

OneWire oneWire(ONE_WIRE_BUS); //Sensor Suhu Air
DallasTemperature sensors(&oneWire); //Sensor Suhu Air

WiFiClient espClient;

ThingsBoard tb(espClient);

int status = WL_IDLE_STATUS;
int quant = 20;
int send_passed = 0;
int send_delay = 2000;

void setup() {
// Initialize serial for debugging
Serial.begin(SERIAL_DEBUG_BAUD);
WiFi.begin(WIFI_AP_NAME, WIFI_PASSWORD); //ssid dan pw
InitWiFi();

 pinMode(14, OUTPUT); //Bazzer dan LED

}
void loop() {
delay(quant);
send_passed += quant;

 // Reconnect to ThingsBoard, if needed
if (!tb.connected()) {
// Connect to the ThingsBoard
Serial.print("Connecting to: ");
Serial.print(THINGSBOARD_SERVER);
Serial.print(" with token ");
Serial.println(TOKEN);
if (!tb.connect(THINGSBOARD_SERVER, TOKEN)) {
Serial.println("Failed to connect");
return;
}
}

if (send_passed > send_delay) {
Serial.println("[DATA DIKIRIM KE THINGBOARD]");

 float ntu1 = (analogRead(34) *(-0.067)) + 146.65;
float ntu = constrain(ntu1, 1, 100);

 sensors.requestTemperatures();
float tempC = sensors.getTempCByIndex(0);
if (isnan(tempC) || isnan(ntu)){
Serial.println("failed to read sensor");
} else {
tb.sendTelemetryFloat("temperature", tempC);
tb.sendTelemetryFloat("turbidity", ntu);
}

 Serial.print("- Kekeruhan : ");
Serial.print(ntu);
Serial.println(" NTU");
Serial.print("- Suhu : ");
Serial.print(tempC);
Serial.println(" *C"); 

//if ((tempC >= 35){
if ((tempC >= 35) || (ntu >= 27)) {
digitalWrite(14, HIGH);
}
else{
digitalWrite(14, LOW);
}

delay(2000);

 send_passed = 0;
}

 // Process messages
tb.loop();
}

void InitWiFi()
{
Serial.println("Connecting to AP ...");
// attempt to connect to WiFi network

 WiFi.begin(WIFI_AP_NAME, WIFI_PASSWORD);
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
Serial.println("Connected to AP");
}

void reconnect() {
// Loop until we're reconnected
status = WiFi.status();
if ( status != WL_CONNECTED) {
WiFi.begin(WIFI_AP_NAME, WIFI_PASSWORD);
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
Serial.println("Connected to AP");
}
}
