#include <Wire.h>
#include "DS3231.h"
// Include the libraries we need
#include <OneWire.h>
#include <DallasTemperature.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h"
// 0X3C+SA0 - 0x3C or 0x3D
#define I2C_ADDRESS 0x3C
// Define proper RST_PIN if required.
#define RST_PIN -1
SSD1306AsciiAvrI2c oled;
// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 2

    // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
    OneWire oneWire(ONE_WIRE_BUS);
    DallasTemperature sensors(&oneWire);
    int r=0;
    int a=0;
    int v;
    int i;
    int m;
    int n=0;
    int c=0;
    RTClib RTC;
    int sensor_pin = A0;
    int output_value ;
    int sensorPin = 2;
    float tempValue;
    
void setup () {
    #if RST_PIN >= 0
    oled.begin(&Adafruit128x64, I2C_ADDRESS, RST_PIN);
    #else // RST_PIN >= 0
    oled.begin(&Adafruit128x64, I2C_ADDRESS);
    #endif // RST_PIN >= 0
    Serial.begin(57600);
    Serial.println("Dallas Temperature IC Control Library Demo");
    Wire.begin();
    Serial.println("Reading From the Sensor ...");
    pinMode(LED_BUILTIN,OUTPUT);
    delay(2000);
}

void loop () {
    oled.set1X();
    oled.setFont(System5x7);
    output_value = analogRead(sensor_pin);
    //tempValue = analogRead(sensorPin);
    output_value = map(output_value,550,10,100,0);
    DateTime now = RTC.now();
    m = now.minute();
    if (n != m)
    {
        //ispisianje vremena na ekran
        n = m;
        oled.clear();
        oled.print("    ");
        oled.print(now.year(), DEC);
        oled.print('/');
        oled.print(now.month(), DEC);
        oled.print('/');
        oled.print(now.day(), DEC);
        oled.print("  ");
        oled.print(now.hour(), DEC);
        oled.print(':');
        oled.print(now.minute(), DEC);
        oled.println();
        
        //ispisivanje vlaznosti na ekran
        oled.print("    ");
        oled.print("Mositure: ");   
        oled.print(output_value);
        oled.println("%");

        //ispisivanje temperature na ekran
        oled.print("     ");
        oled.print("temp: ");
        tempValue = sensors.getTempCByIndex(0);
        output_value = analogRead(sensor_pin);
        sensors.requestTemperatures(); // Send the command to get temperatures
        // After we got the temperatures, we can print them here.
        // We use the function ByIndex, and as an example get the temperature from the first sensor only.
        oled.println(sensors.getTempCByIndex(0));  
        tempValue = sensors.getTempCByIndex(0);
        oled.println(); 

        //upozorenje za premalu vlaznost
        if(output_value > 840)
        {
            a++;   
            oled.set2X();
            oled.println("SIPATI VODE");
            
        }
        //upozorenje za preveliku temperaturu
        if(tempValue > 27)
        {   
            c++;
            oled.set2X();
            oled.println("   SUNCE!");
        }
        for(i=0;i<30;i++)
        {
            //ponovno unosenje podataka radi konstantne provere
            sensors.requestTemperatures(); // Send the command to get temperatures
            tempValue = sensors.getTempCByIndex(0);
            output_value = analogRead(sensor_pin);
            Serial.println(tempValue);
            Serial.println(output_value);
            //blinkanje led diode
            digitalWrite(LED_BUILTIN,HIGH);
            delay(100);
            digitalWrite(LED_BUILTIN,LOW);               
            delay(100);
            //ispisivanje upozorenjaza temperaturu             
            if(c > 0)
            {
                oled.clear();
                oled.println(); 
                oled.println("   SUNCE!"); 
                
                r++;        
            }
            delay(900);
            //ispisivanje upozorenja za vlaznost
            if(a > 0)
            {
                oled.clear();
                oled.println(); 
                oled.println("SIPATI VODE");
                r++;
            }
            delay(900);
            //izlazenje iz for funkcije ako su svi uslovi ispunjeni
            if(output_value < 840 && tempValue < 27)
            {
                break; 
            }
            if(output_value < 840)
            {
                a=0;                  
            }
            if(tempValue < 27)
            {
                c=0;
            }
        }
    }  
    //potvrda da je sve uredu posle ispunjavanja zahteva      
    if(r > 0)
    {
       oled.clear();
       oled.set2X();
       oled.println();
       oled.println("UREDU JE");
    }
    c=0;
    a=0;
    r=0;
}
