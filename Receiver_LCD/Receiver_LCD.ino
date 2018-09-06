
/*
* Getting Started example sketch for nRF24L01+ radios
* This is a very basic example of how to send data from one node to another
* Updated: Dec 2014 by TMRh20
*/

#include "printf.h" // To allow printf to Serial

// Interfaces
#include <SPI.h>
#include "RF24.h"
#include <Servo.h>

// Adafruit RGB display
#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>

// The shield uses the I2C SCL and SDA pins. On classic Arduinos
// this is Analog 4 and 5 so you can't use those for analogRead() anymore
// However, you can connect other I2C sensors to the I2C bus and share
// the I2C bus.
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();


#define MOTOR_PWM_A  3
#define SERVO_PIN    5
#define RADIO_CE     9
#define MOTOR_BRK_B  8
#define MOTOR_BRK_A  4 // 9
#define RADIO_CS    10
#define MOTOR_PWM_B 11
#define MOTOR_DIR_A  5 //12
#define MOTOR_DIR_B 13

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
// radio (ce_pin, cs_pin)
RF24 radio( RADIO_CE, RADIO_CS);
/**********************************************************/

byte addresses[][6] = {"1Node","2Node"};

// These #defines make it easy to set the backlight color
#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7

int cur_x;
int cur_y;
Servo myservo;

void setup() {

  // Init the buffers
  cur_x = -1;
  cur_y = -1;

  pinMode( MOTOR_PWM_A, OUTPUT);
  pinMode( MOTOR_BRK_B, OUTPUT);
  pinMode( MOTOR_BRK_A, OUTPUT);
  pinMode( MOTOR_PWM_B, OUTPUT);
  pinMode( MOTOR_DIR_A, OUTPUT);
  pinMode( MOTOR_DIR_B, OUTPUT);

  analogWrite( MOTOR_PWM_A, 0);
  digitalWrite( MOTOR_BRK_B, LOW);
  digitalWrite( MOTOR_BRK_A, LOW);
  analogWrite( MOTOR_PWM_B, 0);
  digitalWrite( MOTOR_DIR_A, HIGH);
  digitalWrite( MOTOR_DIR_B, HIGH);
  // attach servo
  myservo.attach(SERVO_PIN);
  
  // Init the LCD
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  lcd.setBacklight(WHITE ); 
//  lcd.print ("Starting...");

  // Init serial communication
  Serial.begin(115200);
  Serial.println(F("RF24/examples/GettingStarted"));
  Serial.println(F("*** PRESS 'T' to begin transmitting to the other node"));

  // Init printf to serial
  printf_begin();
  
  radio.begin();

  // Set the PA Level low to prevent power supply related issues since this is a
 // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  radio.setPALevel(RF24_PA_LOW);
  
  // Open a writing and reading pipe on each radio, with opposite addresses
    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1,addresses[1]);
  
  // Start the radio listening for data
  radio.startListening();

  radio.printDetails();
}

void HexPrint(char * string)
{
  if (strlen(string) > 0)
  {
      for (int i=0; i < strlen(string); i++)
      {
        char caract[3];
        sprintf(caract, "%X", string[i]);
        Serial.println(caract);
      }
  }
}

void loop() 
{
  
    int got_time=0;
    char message[17]="";
    char sz_line1[17]="";
    char sz_line2[17]="";
    
    int i_x_joy = 0;
    int i_y_joy= 0;
    int i_btn_joy=0;
    int i_btn_up=0;
    int i_btn_down=0;
    int i_btn_left = 0;
    int i_btn_right = 0;

    int i_new_x=0;
    int i_new_y=0;
    

    int i_status=0;
    
    if( radio.available())
    {
       Serial.println("radio available");
       while (radio.available()) {                                   // While there is data ready
          radio.read( message, 17); //sizeof(int) );             // Get the payload
       }
    }
    else
    {
       lcd.setCursor(1,0); // Line 0, column 0
 //      lcd.print("no radio");
    }

    lcd.setCursor(0,0); // Line 0, column 0
    i_status=sscanf(message, "%d %d", &i_x_joy, &i_y_joy ); //, &i_btn_joy, &i_btn_up, &i_btn_down, &i_btn_left, &i_btn_right);
    lcd.print(message);
    if (i_status > 0)
    {

      
       lcd.setCursor(0,0); // Line 0, column 0
     // lcd.print("message");
       lcd.setCursor(1,1); // Line 0, column 0
       
       i_new_x=map(i_x_joy, 0, 1024, 0, 180);
       if( cur_x != i_new_x)
       {
           cur_x = i_new_x;
           myservo.write(cur_x);
           
       }

       i_new_y =map(i_y_joy, 0, 1024, -255, 255);
       if( cur_y != i_new_y)
       {
           cur_y=i_new_y;
           if (cur_y == 0)
              digitalWrite (MOTOR_BRK_A, HIGH);
           else 
           { 
              digitalWrite (MOTOR_BRK_A, LOW);

              if (cur_y < 0)
              {
                 digitalWrite (MOTOR_DIR_A, LOW); // reverse direction
              }
              else
              {
                 digitalWrite (MOTOR_DIR_A, HIGH); // forward direction
              } 
              
              analogWrite (MOTOR_PWM_A, abs( cur_y));
           }
       }
           
       

    }


    Serial.println (i_status);

} // Loop

