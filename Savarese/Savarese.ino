
#include "arduino_secrets.h" 
#define BROKER_IP    "mqtt.eclipse.org"
#define DEV_NAME     ""
#define MQTT_USER    ""
#define MQTT_PW      ""
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;
#include <MQTT.h>
#ifdef ARDUINO_SAMD_MKRWIFI1010
#include <WiFiNINA.h>
#elif ARDUINO_SAMD_MKR1000
#include <WiFi101.h>
#elif ESP8266
#include <ESP8266WiFi.h>
#else
#error unknown board
#endif
#include <U8g2lib.h>
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif
#include <Arduino.h>
#include <ArduinoJson.h>

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, SCL, SDA, U8X8_PIN_NONE);


u8g2_uint_t offset; 
u8g2_uint_t width;
int cont;
const char *text = "U8g2 ";
const int buttonPin = 6; 
const int ledPin =  5;
int flag = 0;
int sflag = 0;
String citta = "";
WiFiClient net;
MQTTClient client;
void connect() {
 Serial.print("checking wifi...");
 while (WiFi.status() != WL_CONNECTED) {
   Serial.print(".");
   delay(1000);
 }
 Serial.print("\nconnecting...");
 while (!client.connect(DEV_NAME, MQTT_USER, MQTT_PW)) {
   Serial.print(".");
   delay(1000);
 }
 Serial.println("\nconnected!");
 client.subscribe("/MeteoResponse");
}

void messageReceived(String &topic, String &payload){
   u8g2.begin();
   StaticJsonDocument<400> meteo;
   deserializeJson(meteo, payload);
    String name = meteo[0]["name"];
    String temp = meteo[1]["temp"];
    String hum = meteo[2]["hum"];
    int weat = meteo[3]["weat"]; 
    String weatcond = meteo[4]["weatcond"];   
   Serial.println(weatcond);
    u8g2.setFont(u8g2_font_t0_11b_tf); 
    u8g2.setFontDirection(0);
    u8g2.clearBuffer();  
    u8g2.setCursor(0, 10);            
    u8g2.print(name);
    u8g2.setCursor(0, 25);            
    u8g2.print("Temperature: " + temp);
    u8g2.setFont(u8g2_font_unifont_t_symbols);
    u8g2.drawGlyph(90, 25, 0x2103);
    u8g2.setFont(u8g2_font_t0_11b_tf); 
    u8g2.setCursor(0, 40);
    u8g2.print("Humidity: " + hum + "%");
    u8g2.setFont(u8g2_font_t0_11b_tf);
    u8g2.setCursor(0, 55);
    u8g2.print(weatcond);
    u8g2.setFont(u8g2_font_unifont_t_symbols);
    
    if(weat>=200 && weat<=531){
      u8g2.drawGlyph(100, 50, 0x2602);
    }
    if(weat>=600 && weat<=622){
      u8g2.drawGlyph(100, 50, 0x2603);
    }
    if(weat>=701 && weat<=781){
      u8g2.drawGlyph(100, 50, 0x2590);
    }
    if(weat == 800){
      u8g2.drawGlyph(100, 50, 0x2600);
    }
    if(weat>=801 && weat<=804){
      u8g2.drawGlyph(100, 50, 0x2601);
    }
    u8g2.sendBuffer();         
 
}

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
 Serial.begin(115200);
 u8g2.begin();
 WiFi.begin(ssid, pass);
 client.begin(BROKER_IP, 1883, net);
 client.onMessage(messageReceived);
 connect();
}
void loop() {
  client.loop();
  if (!client.connected()) {
   connect();
 }
   switch(cont){
    case 1:
    citta = "Turin";
    break;
    case 2:
    citta = "Milan";
    break;
    case 3:
    citta = "Rome";
    break;
    case 4:
    citta = "Naples";
    break;
    default:
    cont = 1;
    break;
      
   }
       if( (digitalRead(buttonPin) == LOW) && (flag == 0) && (sflag == 0) )
      {
        flag = 1;
        digitalWrite(ledPin, HIGH);
        client.publish("/MeteoRequest", citta);
        cont++;
        sflag = 1;
      }
      if( (digitalRead(buttonPin) == LOW) && (flag == 1) && (sflag == 0) )
      {
        flag = 0;
        digitalWrite(ledPin, HIGH);
        client.publish("/MeteoRequest", citta);
        cont++;
        sflag = 1;
      }
      if( (digitalRead(buttonPin) == HIGH)) // if the button is not pressed

      {
        digitalWrite(ledPin, LOW);
        sflag = 0;
        delay(100);
      }

}
