//=============================================================================
// Description:  This sketch combines the capability to receive a UDP string
//               over wifi on the NodeMCU ESP8266, and print the string on
//               an LCD connected via an I2C backpack PCF8574 connected to 
//               the NodeMCU on pins D1 (SCL) and D2 (SDA).
//=============================================================================
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "wifipass.h"

//=============================================================================
// Global definitions section
//=============================================================================
WiFiUDP Udp;
unsigned int port = 5514;
const unsigned int LCD_WIDTH = 20;
const unsigned int LCD_HEIGHT = 4;
char packet[255];
char report[255];
LiquidCrystal_I2C  lcd(0x27,2,1,0,4,5,6,7);

//=============================================================================
// Initialization activities
//=============================================================================
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
  sprintf(report, "Listener started at IP %s, at port %d", 
      WiFi.localIP().toString().c_str(), port);
  Serial.printf(report);
  Serial.printf("\n");

  // activate LCD module
  lcd.begin (LCD_WIDTH, LCD_HEIGHT); // for 16 x 2 LCD module
  lcd.setBacklightPin(3,POSITIVE);
  lcd.setBacklight(HIGH);

  lcdPrint(report); 
}

//=============================================================================
// Looping activities
//=============================================================================
void loop()
{
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    Serial.printf("Received %d bytes from %s, port %d\n", packetSize, 
        Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int len = Udp.read(packet, 255);
    
    if (len > 0)
    {
      packet[len] = 0;
    }
    
    Serial.printf("UDP packet contents: %s\n", packet);
    lcdPrint(packet); 
  }
}

//=============================================================================
// This function accounts for the fact that the LCD skips lines.  This 
// manually sets the location of each line of a passed in string
//=============================================================================
void lcdPrint(String message)
{ 
  lcd.clear();  

  // truncate to fit into screen
  if(message.length() > LCD_WIDTH * LCD_HEIGHT)
  {
    message = message.substring(0, LCD_WIDTH * LCD_HEIGHT);
  }
  
  int numRows = (message.length() / LCD_WIDTH) + 1;
  int charInLast = message.length() % LCD_WIDTH;

  for (int i = 0; i < numRows; ++i)
  {
    lcd.setCursor(0,i);

    int start = i * LCD_WIDTH;
    String printString = (i != numRows - 1) ? 
      message.substring(start, start + LCD_WIDTH) : 
      message.substring(start, start + charInLast);

    lcd.print(printString);
  }
}
