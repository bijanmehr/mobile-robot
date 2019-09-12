#include <ESP8266WiFi.h>

#ifndef STASSID
#define STASSID "****"
#define STAPSK  "*****************"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

WiFiServer server(80);

IPAddress staticIP(192,168,1,100);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

String req;
int RPWM = 0;
int LPWM = 0;

int AApin = 4;  //D2 right
int ABpin = 14;  //D5 right
int BApin = 12; //D6 left
int BBpin = 15; //D8 left

void setup() {

  Serial.begin(115200);

  pinMode(AApin,OUTPUT);
  pinMode(ABpin,OUTPUT);
  pinMode(BApin,OUTPUT);
  pinMode(BBpin,OUTPUT);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print(F("Connecting to "));
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  WiFi.config(staticIP, gateway, subnet);

  Serial.print("Connecting.");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }

  Serial.println();
  Serial.println(F("WiFi connected"));

  server.begin();
  Serial.println(F("Server started"));

  Serial.println(WiFi.localIP());
}

void loop() {

  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  Serial.println(F("new client"));

  client.setTimeout(500); // default is 1000

  // Read the first line of the request
  req = client.readStringUntil('\r');
  Serial.println(F("request: "));
  Serial.println(req);

  reqhandler();
  Serial.print("RPWM = ");
  Serial.print(RPWM);
  Serial.print("  LPWM = ");
  Serial.println(LPWM);

  movment(RPWM,LPWM);

  // read/ignore the rest of the request
  // do not client.flush(): it is for output only, see below
  while (client.available()) {
    // byte by byte is not very efficient
    client.read();
  }

  Serial.println(F("Disconnecting from client"));

}

int reqhandler() {
  
  if (req.endsWith("*")) {
    // commands should be this form "MRxLy*", M = motor command, R = right, L = left, x = right PWM, y = left PWM, * = end of command.
    RPWM = req.substring(req.indexOf("R") + 1, req.indexOf("L")).toInt();
    LPWM = req.substring(req.indexOf("L") + 1, req.indexOf("*")).toInt();
    
  return RPWM;
  return LPWM;
  Serial.println("RPWM = "+ String(RPWM) +"  "+"LPWM = "+ String(LPWM));  
}
}
void movment(int Rspeed, int Lspeed){
  if(Rspeed > 0 && Lspeed > 0){
    analogWrite(AApin,Rspeed);
    analogWrite(ABpin,0);
    analogWrite(BApin,Lspeed);
    analogWrite(BBpin,0);
  }
  else if(Rspeed > 0 && Lspeed < 0){
    analogWrite(AApin,Rspeed);
    analogWrite(ABpin,0);
    analogWrite(BApin,0);
    analogWrite(BBpin,Lspeed * (-1));
  }
  else if(Rspeed < 0 && Lspeed > 0){
    analogWrite(AApin,0);
    analogWrite(ABpin,Rspeed * (-1));
    analogWrite(BApin,Lspeed);
    analogWrite(BBpin,0);
  }
  else if(Rspeed < 0 && Lspeed < 0){
    analogWrite(AApin,0);
    analogWrite(ABpin,Rspeed * (-1));
    analogWrite(BApin,0);
    analogWrite(BBpin,Lspeed * (-1));
  }
  else{
    analogWrite(AApin,0);
    analogWrite(ABpin,0);
    analogWrite(BApin,0);
    analogWrite(BBpin,0);
  }
}
