#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

const char* ssid = "ssid";
const char* password = "password";

WiFiUDP Udp;
unsigned int port = 5514;
char packet[255];
LiquidCrystal_I2C  lcd(0x27,2,1,0,4,5,6,7);

void setup()
{
  Serial.begin(9600);
  Serial.println();
  
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("Connection Successful");
  Udp.begin(port);
  Serial.printf("Listener started at IP %s, at port %d\n", WiFi.localIP().toString().c_str(), port);

  // activate LCD module
  lcd.begin (20,4); // for 16 x 2 LCD module
  lcd.setBacklightPin(3,POSITIVE);
  lcd.setBacklight(HIGH);

  lcd.home ();                   // go home
  lcd.print("Hello, ARDUINO ");  
}

void loop()
{
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int len = Udp.read(packet, 255);
    
    if (len > 0)
    {
      packet[len] = 0;
    }
    
    Serial.printf("UDP packet contents: %s\n", packet);
    lcd.home();
    lcd.clear();
    lcd.print(packet); 
  }
}
