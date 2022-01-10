// ---------------------------------------------------------------- /
// Arduino I2C Scanner
// Re-writed by Arbi Abdul Jabbaar
// Using Arduino IDE 1.8.7
// Using GY-87 module for the target
// Tested on 10 September 2019
// This sketch tests the standard 7-bit addresses
// Devices with higher bit address might not be seen properly.

//The AS5600 sensor is addressd at 0x36

// Bob planner control
// measure distance with sensor and control motor with PWM signal.
//
// ---------------------------------------------------------------- /

#include <Wire.h>     //include Wire.h library
#include <SPI.h>
#include <EEPROM.h>
#include <Adafruit_SSD1305.h>
#include <Adafruit_GFX.h>

// Used for software SPI
#define OLED_CLK 13
#define OLED_MOSI 11

// Used for software or hardware SPI
#define OLED_CS 4
#define OLED_DC 8

// Used for I2C or SPI
#define OLED_RESET 9

#define UP_SW 15
#define DOWN_SW 14
#define CAL_SW 3
#define GO_SW 2

#define MOTOR_DIR 7
#define MOTOR_PWM 6
#define MOTOR_EN 5

// define the resoluation of a sensor count 8.5" / 4096
#define CNT 0.001936
#define AS5600 0x36

char str_rev[] = "rev 0.0";
byte agc,st,rhb,rlb,hb,lb;
byte conf_l,conf_h;

byte address = AS5600;
byte i2c_return = 0;
byte ep1,ep2;
int eeprom_correct;
float height_in,height_mm;
//int nDevices = 0;
int sensor_cnt;
//char d_str[20];
float planner_set;

bool up_sw,down_sw,cal_sw,go_sw;
bool dir = false;   //set the motor direction false is lower / true is raise
bool mot_en = false;

// software SPI
//Adafruit_SSD1305 display(128, 64, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);
// hardware SPI - use 7Mhz (7000000UL) or lower because the screen is rated for 4MHz, or it will remain blank!
Adafruit_SSD1305 display(128, 64, &SPI, OLED_DC, OLED_RESET, OLED_CS, 7000000UL);

//void updaate_display();

void setup()
{
  Wire.begin();
  Serial.begin(9600); // The baudrate of Serial monitor is set in 9600
  while (!Serial); // Waiting for Serial Monitor
  Serial.println("\nbegin");

  ep1 = EEPROM.read(0);   // this location holds the sensor value for 0.75"
  ep2 = EEPROM.read(1);
  
  Serial.print(ep1);
  Serial.print("   ");
  Serial.println(ep2);
  
  eeprom_correct = ep1*256 + ep2;

  pinMode(GO_SW,INPUT_PULLUP);
  pinMode(CAL_SW,INPUT_PULLUP);

  pinMode(UP_SW,INPUT_PULLUP);
  pinMode(DOWN_SW,INPUT_PULLUP);

  pinMode(MOTOR_EN,OUTPUT);
  pinMode(MOTOR_DIR,OUTPUT);
  pinMode(MOTOR_PWM,OUTPUT);
  digitalWrite(MOTOR_EN,LOW);
 

  if ( ! display.begin(0x3C) ) { //for SPI no device addess is required
    Serial.println("Unable to initialize OLED");
    while (1) yield();
  }

// set hystersis value on sensor
  Wire.beginTransmission(address);
  Wire.write(0x08);
  Wire.write(0x0C);
  Wire.endTransmission();
  
// read sensor conf registers
  Wire.beginTransmission(address);
  Wire.write(0x07);
  Wire.endTransmission();
  Wire.requestFrom(0x36,2);

  conf_h = Wire.read();
  conf_l = Wire.read();

  // init done
  display.setRotation(2);
  display.display(); // show splashscreen
  delay(1000);
  display.setTextColor(WHITE);
/*
  display.clearDisplay();
    // text display tests
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Hello, world!");
  display.display();
  delay(3000);
*/
   Wire.beginTransmission(address);
   byte i2c_return = Wire.endTransmission();

   display.clearDisplay();
   display.setTextSize(2);
   display.setCursor(0,0);
   switch (i2c_return){
   case 0:  
      display.println("sensor ok");
      break;
    case 4:
      display.println("no sensor  found");
      break;
    default:
      display.println("sensor     error");
      break;
   }

   display.println(str_rev);
   
    display.display();
    delay(2000);

    read_5600();

    planner_set = (eeprom_correct -(rhb*256 + rlb)) * CNT + 0.75;

    //test_motor();
}

void loop()
{
   //Serial.println("loop");
   
   int sensor_cnt = 0;
       
   read_switches();

   read_5600();
      
   sensor_cnt = sensor_cnt + (rhb*256 + rlb);   

   height_in = (eeprom_correct - sensor_cnt) * CNT + 0.75;

   height_mm = height_in * 25.4;

   if(up_sw  && !mot_en)  //if up sw sensed and motor ot enabled
   { 
     move_up();
     //planner_set = planner_set + 0.01;
     //Serial.println("move up");
   }

   if(down_sw && !mot_en) //if down sw sensed amd motor not enabled
   { 
     move_down();
     //planner_set = planner_set - 0.01;
     //Serial.println("move down");
   }

   if(mot_en && !(up_sw || down_sw)) // disbale motor if no up or down sw detected
   {
     digitalWrite(MOTOR_EN,LOW);
     mot_en = LOW;
    // Serial.println("motor disbale");
   }

   // if(go_sw)  move_to_position();
   if(go_sw) update_position();

   update_display();

  // when the switch is pushed the planner should be set for a height of 0.75"
  // this will store the sensor values to EEPROM and display 0.75"
  if(cal_sw)
  { //cal_sw = false;
    EEPROM.write(0,rhb);
    EEPROM.write(1,rlb);
    eeprom_correct = rhb*256 + rlb;
  }

  delay(100); // wait 5 seconds for the next I2C scan
}

// end of loop code
