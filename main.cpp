#include <SoftwareSerial.h>
#define RX 10
#define TX 11

SoftwareSerial espp(RX,TX); // RX | TX

String ssid = "MonaConnect";
String password = "";
String host = "10.22.60.126";
String PORT = "5000";
String Command  = "";
String post = "";
String body = "";
int id = 1;
int countTrueCommand;
int countTimeCommand; 
boolean found = false; 
void setup() {
  randomSeed(analogRead(0));
  Serial.begin(115200);
  espp.begin(115200);
  sendCommand("AT",5,"OK"); // check if connection is okay
  sendCommand("AT+CWMODE=1",5,"OK"); // set client mode
  sendCommand("AT+CWJAP=\""+ ssid +"\",\""+ password +"\"",20,"OK");
  
}

void loop() {
   sendCommand("AT+CIPSTART=\"TCP\",\""+ host +"\"," + PORT,15,"OK");
   body = "";
    body = "{\"tank_id\":"+ String(id);
    body+= ",\"water_level\":" +String(getWaterLevel());
    body+= "}";
    post="";
    post = "POST /tank HTTP/1.1\r\nHost: ";
    post += host;
    post += "\r\nContent-Type: application/json\r\nContent-Length:";
    post += body.length();
    post += "\r\n\r\n";
    post += body;
    post += "\r\n";
    Command = "AT+CIPSEND=";
    Command+= String(post.length());
    sendCommand(Command, 10, "OK");
    sendCommand(post, 10,"OK");
    sendCommand("AT+CIPCLOSE=0", 15, "OK");
    delay(200);
}


void sendCommand(String command, int maxTime, char readReplay[]) {
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while(countTimeCommand < (maxTime*1))
  {
    espp.println(command);//at+cipsend
    if(espp.find(readReplay))//ok
    {
      found = true;
      break;
    }
  
    countTimeCommand++;
  }
  
  if(found == true)
  {
    Serial.println("Success");
    countTrueCommand++;
    countTimeCommand = 0;
    id ++;
  }
  
  if(found == false)
  {
    Serial.println("Failed");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }
  
  found = false;
 }

 int getWaterLevel() {
  return random(10, 200);
 }


