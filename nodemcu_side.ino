#include <ESP8266WiFi.h>

#ifndef STASSID
#define STASSID "****"
#define STAPSK  "****************"
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

int AApin = D0;
int ABpin = D1;
int BApin = D2;
int BBpin = D3;

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

    if (RPWM > 255) {
      Serial.println("ERROR: maximum PWM range is 255!");
      RPWM = 255;
    }
    if (LPWM > 255) {
      Serial.println("ERROR: maximum PWM range is 255!");
      LPWM = 255;
    }
  }
  else {
    Serial.println("ERROR, incorect command");
  }
  return RPWM;
  return LPWM;
  Serial.println("RPWM = "+ String(RPWM) +"  "+"LPWM = "+ String(LPWM));  
}

void movment(int Rspeed, int Lspeed){
  if (Rspeed >= 0){
    analogWrite(AApin,Rspeed);
    analogWrite(ABpin,0);
    Serial.println("R_forward");
  }
  else{
    analogWrite(ABpin,Rspeed);
    analogWrite(AApin,0);
    Serial.println("R_backward");
  }

  if (Lspeed >= 0){
    analogWrite(BApin,Lspeed);
    analogWrite(BBpin,0);
    Serial.println("L_forward");
  }
  else{
    analogWrite(BBpin,Lspeed);
    analogWrite(BApin,0);
    Serial.println("L_backward");
  }
  
}
