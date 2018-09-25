#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServerSecure.h>

ESP8266WebServerSecure server(80);

static const uint8_t x509[] PROGMEM = {
  //Extract make-self-signed-cert & run the file it will create two files x509.h & key.h 
  //Copy all hexa code from x509.h and paste here
};

static const uint8_t rsakey[] PROGMEM = {
  //Copy all hexa code from key.h and paste here
};

bool offLineSwitch1 = false;
bool onLineSwitch1 = false;
bool serverSwitch1 = false;
bool sw1BothOnState = false;

bool offLineSwitch2 = false;
bool onLineSwitch2 = false;
bool serverSwitch2 = false;
bool sw2BothOnState = false;

bool offLineSwitch3 = false;
bool onLineSwitch3 = false;
bool serverSwitch3 = false;
bool sw3BothOnState = false;

bool offLineSwitch4 = false;
bool onLineSwitch4 = false;
bool serverSwitch4 = false;
bool sw4BothOnState = false;

int inpu1 = 4;
int inpu2 = 5;
int inpu3 = 12;
int inpu4 = 13;

int outpu1 = 0;
int outpu2 = 14;
int outpu3 = 15;
int outpu4 = 16;

//the below two things are used for connecting to wifi module
const char* stat_ssid_def = "Your SSID";
const char* stat_password_def = "Your Password";

//the below two things are used for login to server when accessing through any browser & use chrome browser
const String username = "server username";
const String password = "server password";

bool is_authentified() {
  if (server.hasHeader("Cookie")) {
    String cookie = server.header("Cookie");
    if (cookie.indexOf("ESPSESSIONID=1") != -1) {
      return true;
    }
  }
  return false;
}

void setup() {
  delay(1000);
  pinMode(outpu1, OUTPUT);
  pinMode(outpu2, OUTPUT);
  pinMode(outpu3, OUTPUT);
  pinMode(outpu4, OUTPUT);
  pinMode(inpu1, INPUT);
  pinMode(inpu2, INPUT);
  pinMode(inpu3, INPUT);
  pinMode(inpu4, INPUT);
  digitalWrite(outpu1, LOW);
  digitalWrite(outpu2, LOW);
  digitalWrite(outpu3, LOW);
  digitalWrite(outpu4, LOW);
  WiFi.mode(WIFI_STA);
  write_Station_info_to_Memory(stat_ssid_def, stat_password_def);
  server.setServerKeyAndCert_P(rsakey, sizeof(rsakey), x509, sizeof(x509));
  server.on("/", handleRootStart);
  server.on("/signin", handleSignIn);
  server.on("/switches", handleSwitches);
  server.onNotFound(handleNotFound);
  const char* headerkeys[] = {"User-Agent", "Cookie"} ;
  size_t headerkeyssize = sizeof(headerkeys) / sizeof(char*);
  server.collectHeaders(headerkeys, headerkeyssize);
  server.begin();
  String sw1BoState = read_Sw1_Both_on_state_from_Memory();
  if (sw1BoState == "1") {
    sw1BothOnState = true;
    String sSwitch1 = read_Server_Switch1_state_from_Memory();
    if (sSwitch1 == "1") {
     serverSwitch1 = true;
    } else {
     serverSwitch1 = false; 
    }
    String oSwitch1 = read_Online_Switch1_state_from_Memory();
    if (oSwitch1 == "1") {
      onLineSwitch1 = true;
    } else {
     onLineSwitch1 = false; 
    }
  }
  String sw1State = read_Offline_Switch1_state_from_Memory();
  if (sw1State == "1" && !sw1BothOnState) {
    offLineSwitch1 = true;
  } else if (sw1State == "0") {
    offLineSwitch1 = false;
    
  }

  String sw2BoState = read_Sw2_Both_on_state_from_Memory();
  if (sw2BoState == "1") {
    sw2BothOnState = true;
    String sSwitch2 = read_Server_Switch2_state_from_Memory();
    if (sSwitch2 == "1") {
     serverSwitch2 = true;
    } else {
     serverSwitch2 = false; 
    }
    String oSwitch2 = read_Online_Switch2_state_from_Memory();
    if (oSwitch2 == "1") {
      onLineSwitch2 = true;
    } else {
     onLineSwitch2 = false; 
    }
  }
  String sw2State = read_Offline_Switch2_state_from_Memory();
  if (sw2State == "1" && !sw2BothOnState) {
    offLineSwitch2 = true;
  } else if (sw2State == "0") {
    offLineSwitch2 = false;
  }


  String sw3BoState = read_Sw3_Both_on_state_from_Memory();
  if (sw3BoState == "1") {
    sw3BothOnState = true;
    String sSwitch3 = read_Server_Switch3_state_from_Memory();
    if (sSwitch3 == "1") {
     serverSwitch3 = true;
    } else {
     serverSwitch3 = false; 
    }
    String oSwitch3 = read_Online_Switch3_state_from_Memory();
    if (oSwitch3 == "1") {
      onLineSwitch3 = true;
    } else {
     onLineSwitch3 = false; 
    }
  }
  String sw3State = read_Offline_Switch3_state_from_Memory();
  if (sw3State == "1" && !sw3BothOnState) {
    offLineSwitch3 = true;
  } else if (sw3State == "0") {
    offLineSwitch3 = false;
  }

  String sw4BoState = read_Sw4_Both_on_state_from_Memory();
  if (sw4BoState == "1") {
    sw4BothOnState = true;
    String sSwitch4 = read_Server_Switch4_state_from_Memory();
    if (sSwitch4 == "1") {
     serverSwitch4 = true;
    } else {
     serverSwitch4 = false; 
    }
    String oSwitch4 = read_Online_Switch4_state_from_Memory();
    if (oSwitch4 == "1") {
      onLineSwitch4 = true;
    } else {
     onLineSwitch4 = false; 
    }
  }
  String sw4State = read_Offline_Switch4_state_from_Memory();
  if (sw4State == "1" && !sw4BothOnState) {
    offLineSwitch4 = true;
  } else if (sw4State == "0") {
    offLineSwitch4 = false;
  }
}

void loop() {
  int inPut1 = digitalRead(inpu1);
  int inPut2 = digitalRead(inpu2);
  int inPut3 = digitalRead(inpu3);
  int inPut4 = digitalRead(inpu4);
  
  if(inPut1 == LOW) {
    if (sw1BothOnState) {
      sw1BothOnState = false;
      onLineSwitch1 = false;
      serverSwitch1 = false;
      write_Sw1_Both_on_state_to_Memory("0");
    }
    offLineSwitch1 = false;
    write_Offline_Switch1_state_to_Memory("0");
  } else if (inPut1 == HIGH) {
    offLineSwitch1 = true;
    write_Offline_Switch1_state_to_Memory("1");
  }
  if ((offLineSwitch1 && (onLineSwitch1 || serverSwitch1)) || ((offLineSwitch1 || onLineSwitch1) && serverSwitch1)) {
    controlSwitch1("ON");
    sw1BothOnState = true;
    write_Sw1_Both_on_state_to_Memory("1");
  } else if (offLineSwitch1 || onLineSwitch1 || serverSwitch1) {
    if (!sw1BothOnState) {
      controlSwitch1("ON");
    } else {
      controlSwitch1("OFF");
    }
  } else {
    controlSwitch1("OFF");
  }

  if(inPut2 == LOW) {
    if (sw2BothOnState) {
      sw2BothOnState = false;
      onLineSwitch2 = false;
      serverSwitch2 = false;
      write_Sw2_Both_on_state_to_Memory("0");
    }
    offLineSwitch2 = false;
    write_Offline_Switch2_state_to_Memory("0");
  } else if (inPut2 == HIGH) {
    offLineSwitch2 = true;
    write_Offline_Switch2_state_to_Memory("1");
  }
  if ((offLineSwitch2 && (onLineSwitch2 || serverSwitch2)) || ((offLineSwitch2 || onLineSwitch2) && serverSwitch2)) {
    controlSwitch2("ON");
    sw2BothOnState = true;
    write_Sw2_Both_on_state_to_Memory("1");
  } else if (offLineSwitch2 || onLineSwitch2 || serverSwitch2) {
    if (!sw2BothOnState) {
      controlSwitch2("ON");
    } else {
      controlSwitch2("OFF");
    }
  } else {
    controlSwitch2("OFF");
  }
  
  if(inPut3 == LOW) {
    if (sw3BothOnState) {
      sw3BothOnState = false;
      onLineSwitch3 = false;
      serverSwitch3 = false;
      write_Sw3_Both_on_state_to_Memory("0");
    }
    offLineSwitch3 = false;
    write_Offline_Switch3_state_to_Memory("0");
  } else if (inPut3 == HIGH) {
    offLineSwitch3 = true;
    write_Offline_Switch3_state_to_Memory("1");
  }
  if ((offLineSwitch3 && (onLineSwitch3 || serverSwitch3)) || ((offLineSwitch3 || onLineSwitch3) && serverSwitch3)) {
    controlSwitch3("ON");
    sw3BothOnState = true;
    write_Sw3_Both_on_state_to_Memory("1");
  } else if (offLineSwitch3 || onLineSwitch3 || serverSwitch3) {
    if (!sw3BothOnState) {
      controlSwitch3("ON");
    } else {
      controlSwitch3("OFF");
    }
  } else {
    controlSwitch3("OFF");
  }
  
  if(inPut4 == LOW) {
    if (sw4BothOnState) {
      sw4BothOnState = false;
      onLineSwitch4 = false;
      serverSwitch4 = false;
      write_Sw4_Both_on_state_to_Memory("0");
    }
    offLineSwitch4 = false;
    write_Offline_Switch4_state_to_Memory("0");
  } else if (inPut4 == HIGH) {
    offLineSwitch4 = true;
    write_Offline_Switch4_state_to_Memory("1");
  }
  if ((offLineSwitch4 && (onLineSwitch4 || serverSwitch4)) || ((offLineSwitch4 || onLineSwitch4) && serverSwitch4)) {
    controlSwitch4("ON");
    sw4BothOnState = true;
    write_Sw4_Both_on_state_to_Memory("1");
  } else if (offLineSwitch4 || onLineSwitch4 || serverSwitch4) {
    if (!sw4BothOnState) {
      controlSwitch4("ON");
    } else {
      controlSwitch4("OFF");
    }
  } else {
    controlSwitch4("OFF");
  }
  
  server.handleClient();
}

void handleRootStart() {
  String header;
  if (!is_authentified()) {
    server.sendHeader("Location", "/signin");
    server.sendHeader("Cache-Control", "no-cache");
    server.send(301);
    return;
  }
  server.sendHeader("Location", "/switches");
  server.sendHeader("Cache-Control", "no-cache");
  server.sendHeader("Set-Cookie", "ESPSESSIONID=1");
  server.send(301);
}

void handleSignIn() {
  String msg = "";
  if (server.hasHeader("Cookie")) {
    String cookie = server.header("Cookie");
  }
  if (server.hasArg("logout") && server.arg("logout") == "1") {
    server.sendHeader("Location", "/signin");
    server.sendHeader("Cache-Control", "no-cache");
    server.sendHeader("Set-Cookie", "ESPSESSIONID=0");
    server.send(301);
    return;
  }
  if (server.hasArg("username") && server.hasArg("password")) {
    if (server.arg("username") == username &&  server.arg("password") == password) {
      server.sendHeader("Location", "/switches");
      server.sendHeader("Cache-Control", "no-cache");
      server.sendHeader("Set-Cookie", "ESPSESSIONID=1");
      server.send(301);
      return;
    }
    msg = "Wrong username/password! try again.";
  }
  server.send(200, "text/html", createResponse(0, msg, -1, -1, -1, -1));
}

void handleSwitches() {

   int swit1, swit2, swit3, swit4;
   
   if (!is_authentified()) {
     server.sendHeader("Location", "/signin");
     server.sendHeader("Cache-Control", "no-cache");
     server.send(301);
     return;
   }
   
   if (offLineSwitch1 || onLineSwitch1 || serverSwitch1) {
    if (server.hasArg("sw") && server.arg("sw").toInt() == 0) {
      serverSwitch1 = false;
      write_Server_Switch1_state_to_Memory("0");
      swit1 = 0;
    } else {
      serverSwitch1 = true;
      swit1 = 1;
    }
   } else { 
    if (server.hasArg("sw") && server.arg("sw").toInt() == 1) {
      serverSwitch1 = true;
      write_Server_Switch1_state_to_Memory("1");
      swit1 = 1;
    } else {
      serverSwitch1 = false;
      swit1 = 0;
    }
   }

   if (offLineSwitch2 || onLineSwitch2 || serverSwitch2) {
    if (server.hasArg("sw") && server.arg("sw").toInt() == 0) {
      serverSwitch2 = false;
      write_Server_Switch2_state_to_Memory("0");
      swit2 = 0;
    } else {
      serverSwitch2 = true;
      swit2 = 1;
    }
   } else { 
    if (server.hasArg("sw") && server.arg("sw").toInt() == 1) {
      serverSwitch2 = true;
      write_Server_Switch2_state_to_Memory("1");
      swit2 = 1;
    } else {
      serverSwitch2 = false;
      swit2 = 0;
    }
   }
   
   if (offLineSwitch3 || onLineSwitch3 || serverSwitch3) {
    if (server.hasArg("sw") && server.arg("sw").toInt() == 0) {
      serverSwitch3 = false;
      write_Server_Switch3_state_to_Memory("0");
      swit3 = 0;
    } else {
      serverSwitch3 = true;
      swit3 = 1;
    }
   } else { 
    if (server.hasArg("sw") && server.arg("sw").toInt() == 1) {
      serverSwitch3 = true;
      write_Server_Switch3_state_to_Memory("1");
      swit3 = 1;
    } else {
      serverSwitch3 = false;
      swit3 = 0;
    }
   }

   if (offLineSwitch4 || onLineSwitch4 || serverSwitch4) {
    if (server.hasArg("sw") && server.arg("sw").toInt() == 0) {
      serverSwitch4 = false;
      write_Server_Switch4_state_to_Memory("0");
      swit4 = 0;
    } else {
      serverSwitch4 = true;
      swit4 = 1;
    }
   } else { 
    if (server.hasArg("sw") && server.arg("sw").toInt() == 1) {
      serverSwitch4 = true;
      write_Server_Switch4_state_to_Memory("1");
      swit4 = 1;
    } else {
      serverSwitch4 = false;
      swit4 = 0;
    }
   }
   server.send(200, "text/html", createResponse(2, "", swit1, swit2, swit3, swit4));
}

void controlSwitch1(String sw) {
  if (sw == "ON") {
    digitalWrite(outpu1, HIGH);
  } else {
    digitalWrite(outpu1, LOW);
  }
}

void controlSwitch2(String sw) {
  if (sw == "ON") {
    digitalWrite(outpu2, HIGH);
  } else {
    digitalWrite(outpu2, LOW);
  }
}

void controlSwitch3(String sw) {
  if (sw == "ON") {
    digitalWrite(outpu3, HIGH);
  } else {
    digitalWrite(outpu3, LOW);
  }
}

void controlSwitch4(String sw) {
  if (sw == "ON") {
    digitalWrite(outpu4, HIGH);
  } else {
    digitalWrite(outpu4, LOW);
  }
}

void write_Offline_Switch1_state_to_Memory(String s){
  EEPROM.begin(512);
  s+=";";
  write_EEPROM(s,0);
  EEPROM.commit();
  EEPROM.end();
}

void write_Server_Switch1_state_to_Memory(String s){
  EEPROM.begin(512);
  s+=";";
  write_EEPROM(s,3);
  EEPROM.commit();
  EEPROM.end();
}

void write_Online_Switch1_state_to_Memory(String s){
  EEPROM.begin(512);
  s+=";";
  write_EEPROM(s,6);
  EEPROM.commit();
  EEPROM.end();
}

void write_Sw1_Both_on_state_to_Memory(String s){
  EEPROM.begin(512);
  s+=";";
  write_EEPROM(s,9);
  EEPROM.commit();
  EEPROM.end();
}

void write_Offline_Switch2_state_to_Memory(String s){
  EEPROM.begin(512);
  s+=";";
  write_EEPROM(s,12);
  EEPROM.commit();
  EEPROM.end();
}

void write_Server_Switch2_state_to_Memory(String s){
  EEPROM.begin(512);
  s+=";";
  write_EEPROM(s,15);
  EEPROM.commit();
  EEPROM.end();
}

void write_Online_Switch2_state_to_Memory(String s){
  EEPROM.begin(512);
  s+=";";
  write_EEPROM(s,18);
  EEPROM.commit();
  EEPROM.end();
}

void write_Sw2_Both_on_state_to_Memory(String s){
  EEPROM.begin(512);
  s+=";";
  write_EEPROM(s,21);
  EEPROM.commit();
  EEPROM.end();
}

void write_Offline_Switch3_state_to_Memory(String s){
  EEPROM.begin(512);
  s+=";";
  write_EEPROM(s,24);
  EEPROM.commit();
  EEPROM.end();
}

void write_Server_Switch3_state_to_Memory(String s){
  EEPROM.begin(512);
  s+=";";
  write_EEPROM(s,27);
  EEPROM.commit();
  EEPROM.end();
}

void write_Online_Switch3_state_to_Memory(String s){
  EEPROM.begin(512);
  s+=";";
  write_EEPROM(s,30);
  EEPROM.commit();
  EEPROM.end();
}

void write_Sw3_Both_on_state_to_Memory(String s){
  EEPROM.begin(512);
  s+=";";
  write_EEPROM(s,33);
  EEPROM.commit();
  EEPROM.end();
}
void write_Offline_Switch4_state_to_Memory(String s){
  EEPROM.begin(512);
  s+=";";
  write_EEPROM(s,36);
  EEPROM.commit();
  EEPROM.end();
}

void write_Server_Switch4_state_to_Memory(String s){
  EEPROM.begin(512);
  s+=";";
  write_EEPROM(s,39);
  EEPROM.commit();
  EEPROM.end();
}

void write_Online_Switch4_state_to_Memory(String s){
  EEPROM.begin(512);
  s+=";";
  write_EEPROM(s,42);
  EEPROM.commit();
  EEPROM.end();
}

void write_Sw4_Both_on_state_to_Memory(String s){
  EEPROM.begin(512);
  s+=";";
  write_EEPROM(s,45);
  EEPROM.commit();
  EEPROM.end();
}

void write_Station_info_to_Memory(String s,String p){
  EEPROM.begin(512);
  s+=";";
  write_EEPROM(s,49);
  p+=";";
  write_EEPROM(p,193);
  EEPROM.commit();
  EEPROM.end();
  startStation(s, p);
}

void startStation(String s, String p) {
  if(WiFi.status() == WL_CONNECTED) {
    WiFi.disconnect();
  }
  char s_array[s.length()];
  s.toCharArray(s_array, s.length());
  char p_array[p.length()];
  p.toCharArray(p_array, p.length());
  WiFi.begin(s_array, p_array);
}

String getWifiIP() {
  return WiFi.localIP().toString();
}

void write_EEPROM(String x, int pos) {
  for(int n = pos; n < x.length() + pos; n++){
     EEPROM.write(n,x[n-pos]);
  }
}

String read_Offline_Switch1_state_from_Memory(){
  return read_EEPROM(2, 0);
}

String read_Server_Switch1_state_from_Memory(){
  return read_EEPROM(2, 3);
}

String read_Online_Switch1_state_from_Memory(){
  return read_EEPROM(2, 6);
}

String read_Sw1_Both_on_state_from_Memory(){
  return read_EEPROM(2, 9);
}

String read_Offline_Switch2_state_from_Memory(){
  return read_EEPROM(2, 12);
}

String read_Server_Switch2_state_from_Memory(){
  return read_EEPROM(2, 15);
}

String read_Online_Switch2_state_from_Memory(){
  return read_EEPROM(2, 18);
}

String read_Sw2_Both_on_state_from_Memory(){
  return read_EEPROM(2, 21);
}
String read_Offline_Switch3_state_from_Memory(){
  return read_EEPROM(2, 24);
}

String read_Server_Switch3_state_from_Memory(){
  return read_EEPROM(2, 27);
}

String read_Online_Switch3_state_from_Memory(){
  return read_EEPROM(2, 30);
}

String read_Sw3_Both_on_state_from_Memory(){
  return read_EEPROM(2, 33);
}
String read_Offline_Switch4_state_from_Memory(){
  return read_EEPROM(2, 36);
}

String read_Server_Switch4_state_from_Memory(){
  return read_EEPROM(2, 39);
}

String read_Online_Switch4_state_from_Memory(){
  return read_EEPROM(2, 42);
}

String read_Sw4_Both_on_state_from_Memory(){
  return read_EEPROM(2, 45);
}

String read_Station_ssid_from_Memory(){
  return read_EEPROM(70, 49);
}

String read_Station_pass_from_Memory(){
  return read_EEPROM(40, 193);
}

String read_EEPROM(int len, int pos) {
  EEPROM.begin(512);
  String string_Value = "";
  for (int n = pos; n < len; ++n) {
   if(char(EEPROM.read(n))!=';'){
     string_Value += String(char(EEPROM.read(n)));
   } else {
     n=len+1;
   }
  }
  EEPROM.end();
  return string_Value;
}

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
  message +="<H2><a href=\"/\">go home</a></H2><br>";
  server.send(404, "text/plain", message);
}

String createResponse(int i, String msg, int sw1, int sw2, int sw3, int sw4) {
  String HTML_CODE = "<!DOCTYPE HTML><html><head><meta content=\"text/html; charset=ISO-8859-1\" http-equiv=\"content-type\" />"
      "<meta name = \"viewport\" content = \"width = device-width, initial-scale = 1.0, maximum-scale = 1.0, user-scalable=0\" />"
      "<meta name = \"viewport\" content = \"height = device-height, initial-scale = 1.0, maximum-scale = 1.0, user-scalable=0\" />";
   if(i == 0) {
    HTML_CODE += "<style>body, html { background-color: #808080; color : #000000;font-family: \"SIMPSON\"; height: 100%;width: 100%;margin: 0; padding: 0}"
      ".center { height: 100%;position: relative;width: 100%;background-color: #b4e4f5;overflow: auto;}"
      ".center form {margin: 0;position: absolute;top: 50%;left: 50%;-ms-transform: translate(-50%, -50%);transform: translate(-50%, -50%);background-color: #fff0bc;width:85%;border-bottom-left-radius: 10px;border-bottom-style:  solid;border-width:  5px;border-bottom-right-radius: 10px;border-top:  5px;border-top-color: #969696;border-top-style: dashed;border-bottom-color:  #808080;text-align:  center;}"
      ".center form input { margin-bottom:10px; }* {box-sizing: border-box;}input[type=text], input[type=password] {padding: 8px;border: 1px solid #ccc;border-radius: 4px;resize: vertical;}"
      "label {padding: 12px 12px 12px 0;display: inline-block;}input[type=submit], input[type=reset] {margin-right: 5px;background-color: #4CAF50;color: white;padding: 12px 20px;border: none;border-radius: 4px;cursor: pointer;}"
      "input[type=submit]:hover, button:hover, input[type=reset]:hover{background-color: #45a049;box-shadow: 0 12px 16px 0 rgba(0,0,0,0.24), 0 17px 50px 0 rgba(0,0,0,0.19);}.cr {width: 40%;margin-top: 0;}"
      ".cr {  display: inline-block;  border-radius: 4px;  background-color: #ff4747;  border: none;  color: #FFFFFF;  text-align: center;  padding: 20px;  transition: all 0.5s;  cursor: pointer;  margin: 5px;}"
      ".cr span {  cursor: pointer;  display: inline-block;  position: relative;  transition: 0.5s;}.cr span:after {  content: '\\00bb';  position: absolute;  opacity: 0;  top: 0;  right: -20px;  transition: 0.5s;}"
      ".cr:hover span {  padding-right: 25px;}.cr:hover span:after {  opacity: 1;  right: 0;}#uname {margin-left: 6px;}#pass {margin-left: 9px;}.noti {color: red; font-size: small; font-weight: bold;}"
      "@media screen and (max-width: 600px) {input[type=submit], input[type=reset] {width: 80%;margin-top: 0;padding: 8px;}button {margin-bottom:10px;}.cr {width: 80%;margin: 0;padding: 8px;}"
      "input[type=submit] { margin-left: 3px;}}@media screen and (max-height: 650px) {.center form {-ms-transform: translate(-50%, -40%);transform: translate(-50%, -40%);}}"
      "@media screen and (max-height: 550px) {.center form {-ms-transform: translate(-50%, -30%);transform: translate(-50%, -30%);}}@media screen and (max-height: 390px) {"
      ".center form {-ms-transform: translate(-50%, -15%);transform: translate(-50%, -15%);}}@media screen and (max-width: 312px) {#uname {margin-left: 0;}#pass {margin-left: 0;}}</style>"
      "<title>Server Login</title></head><body><div class=\"center\"><FORM action=\"/signin\" method=\"post\"><h1>Server Login</h1><P><label for=\"uname\">Username: </label>"
      "<input id=\"uname\" maxlength=\"30\" type=\"text\" name=\"username\" placeholder=\"Enter your username\" /><br /><label for=\"pass\">Password: </label>"
      "<input id=\"pass\" maxlength=\"30\" type=\"password\" name=\"password\" placeholder=\"Enter your password\"/><br /><p class=\"noti\">";
    HTML_CODE += msg + "</p><br /><input type=\"reset\" /><input type=\"submit\" value=\"Sign In\" />";
  } else if(i == 2) {
     HTML_CODE += "<style>body, html {  height: 100%;}.container {  top: 0;  left: 0;  width: 100%;  height: 100%;}.col {  float: left;  width: 50%;  height: 50%; background-color: #c0c0c0;}"   
      ".buttons {background-color: #c0c0c0;position: absolute;left: 45%;z-index: 2;}a.but:link, a.but:visited, a.but:link:active, a.but:visited:active, a.but:link:hover, a.but:visited:hover { color: #00000000;text-decoration: none;cursor: pointer;}"
      ".cr:hover {background-color: #45a049;box-shadow: 0 12px 16px 0 rgba(0,0,0,0.24), 0 17px 50px 0 rgba(0,0,0,0.19);}.cr {  width: 120px;  display: inline-block;  border-radius: 4px;  background-color: #ff4747;  border: none;  color: #FFFFFF;  text-align: center;  padding: 5px;  transition: all 0.5s;  cursor: pointer;  margin: 5px;}"
      ".cr span {  cursor: pointer;  display: inline-block;  position: relative;  transition: 0.5s;}.cr span:after {  content: '\\00bb';  position: absolute;  opacity: 0;  top: 0;  right: -20px;  transition: 0.5s;}"
      ".cr:hover span {  padding-right: 25px;}.cr:hover span:after {  opacity: 1;  right: 0;}.center { height: 100%;position: relative;width: 100%;background-color: #c0c0c0;}"
      ".innerContainer {margin: 0;position: absolute;top: 50%;left: 50%;-ms-transform: translate(-50%, -50%);transform: translate(-50%, -50%);}.circle {height: 150px;width: 150px;border-radius: 50%;background-color: rgb(255, 72, 72);border-style: solid;border-width: 40px;border-color: #FFFF}"
      ".circle:hover {box-shadow: 0 12px 16px 0 rgba(0,0,0,0.24), 0 17px 50px 0 rgba(0,0,0,0.19);}.circle:active {border-color: #484848;box-shadow: 0 0 0 0 rgba(0,0,0,0.24), 0 0 0 0 rgba(0,0,0,0.19);}"
      ".text {position: absolute;left: 50%;transform: translate(-50%, -50%);font-weight: bold;font-size: x-large;top: 15%;}@media screen and (max-width: 990px) {.text {font-size: large;}.buttons {left: 43%;}}"
      "@media screen and (max-width: 752px) {.text {font-size: medium;}.buttons {left: 40%;}}@media screen and (max-width: 672px) {.text {font-size: small;}.buttons {left: 37.5%;}}"
      "@media screen and (max-width: 560px) {.col {  float: left;  width: 100%;  height: 50%;}.cr {width: 98%;}.buttons {width: 99%;position: unset;left: 0%;z-index: 0;}}"
      "@media screen and (max-height: 648px) {.col {  height: 320px;}}</style><script>function sendState(n,sw) {var xhttp = new XMLHttpRequest();xhttp.onreadystatechange = function() {if (this.readyState == 4 && this.status == 200) {}};"
      "xhttp.open(\"POST\", \"/control_room\", true);xhttp.setRequestHeader(\"Content-type\", \"application/x-www-form-urlencoded\");xhttp.send(\"sw=\" + n + \"&state=\" + sw);}"
      "function swOnOff(n) {if (document.getElementById(\"sw\"+n).style.backgroundColor == \"rgb(255, 72, 72)\") {document.getElementById(\"sw\"+n).style.backgroundColor = \"rgb(61, 153, 112)\";"
      "document.getElementById(\"txt\"+n).innerHTML = \"Device Switched \" + n + \" ON\";sendState(n,\"1\");} else {document.getElementById(\"sw\"+n).style.backgroundColor = \"rgb(255, 72, 72)\";"
      "document.getElementById(\"txt\"+n).innerHTML = \"Device Switched \" + n + \" OFF\";sendState(n,\"0\");}}</script><title>Switches</title></head><body>"
      "<div style=\"width: 100%; background-color: #c0c0c0; position: relative;\"><div class=\"buttons\">"
      "<a class=\"but\" href=\"/login?logout=1\"><button class=\"cr\" type=\"button\"><span>Logout</span></button></a></div></div>"
      "<div class=\"container\"><div class=\"col\"><div class=\"center\"><p id=\"txt1\" class=\"text\">Device Switched 1 OFF</p><div class=\"innerContainer \">"
      "<button id=\"sw1\" class=\"circle\" type=\"button\" name=\"sw\" value=\"1\"onclick=\"swOnOff(1)\" />";
      if (sw1 == 1) {
        HTML_CODE += "<script>document.getElementById(\"sw1\").style.backgroundColor = \"rgb(61, 153, 112)\";document.getElementById(\"txt1\").innerHTML = \"Device Switched 1 ON\";</script>";
      } else {
        HTML_CODE += "<script>document.getElementById(\"sw1\").style.backgroundColor = \"rgb(255, 72, 72)\";document.getElementById(\"txt1\").innerHTML = \"Device Switched 1 OFF\";</script>";
      }
      HTML_CODE += "</div></div></div><div class=\"col\"><div class=\"center\"><p id=\"txt2\" class=\"text\">Device Switched 2 OFF</p><div class=\"innerContainer\"><button id=\"sw2\" class=\"circle\" type=\"button\" name=\"sw\" value=\"2\"onclick=\"swOnOff(2)\" />";
      if (sw2 == 1) {
        HTML_CODE += "<script>document.getElementById(\"sw2\").style.backgroundColor = \"rgb(61, 153, 112)\";document.getElementById(\"txt2\").innerHTML = \"Device Switched 2 ON\";</script>";
      } else {
        HTML_CODE += "<script>document.getElementById(\"sw1\").style.backgroundColor = \"rgb(255, 72, 72)\";document.getElementById(\"txt1\").innerHTML = \"Device Switched 2 OFF\";</script>";
      }
      HTML_CODE += "</div></div></div><div class=\"col\"><div class=\"center\"><p id=\"txt3\" class=\"text\">Device Switched 3 OFF</p><div class=\"innerContainer\"><button id=\"sw3\" class=\"circle\" type=\"button\" name=\"sw\" value=\"3\"onclick=\"swOnOff(3)\" />";
      if (sw3 == 1) {
        HTML_CODE += "<script>document.getElementById(\"sw3\").style.backgroundColor = \"rgb(61, 153, 112)\";document.getElementById(\"txt3\").innerHTML = \"Device Switched 3 ON\";</script>";
      } else {
        HTML_CODE += "<script>document.getElementById(\"sw1\").style.backgroundColor = \"rgb(255, 72, 72)\";document.getElementById(\"txt1\").innerHTML = \"Device Switched 3 OFF\";</script>";
      }
      HTML_CODE += "</div></div></div><div class=\"col\"><div class=\"center\"><p id=\"txt4\" class=\"text\">Device Switched 4 OFF</p><div class=\"innerContainer\"><button id=\"sw4\" class=\"circle\" type=\"button\" name=\"sw\" value=\"4\"onclick=\"swOnOff(4)\" />";
      if (sw4 == 1) {
        HTML_CODE += "<script>document.getElementById(\"sw4\").style.backgroundColor = \"rgb(61, 153, 112)\";document.getElementById(\"txt4\").innerHTML = \"Device Switched 4 ON\";</script></div></div></div>";
      } else {
        HTML_CODE += "<script>document.getElementById(\"sw1\").style.backgroundColor = \"rgb(255, 72, 72)\";document.getElementById(\"txt1\").innerHTML = \"Device Switched 4 OFF\";</script></div></div></div>";
      }
  }
  HTML_CODE += "</div></body></html>";    
  return HTML_CODE;
}
