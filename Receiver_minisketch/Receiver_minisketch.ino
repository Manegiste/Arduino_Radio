
/*
* Getting Started example sketch for nRF24L01+ radios
* This is a very basic example of how to send data from one node to another
* Updated: Dec 2014 by TMRh20
*/

#include "printf.h" // To allow printf to Serial

// Interfaces
#include <SPI.h>
#include "RF24.h"


// Adafruit RGB display
#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>




#define RADIO_CE     9

#define RADIO_CS    10


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



void setup() {

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
    
    int i_status=0;
    
    if( radio.available())
    {
  //     Serial.println("radio available");
       while (radio.available()) {                                   // While there is data ready
          radio.read( message, 17); //sizeof(int) );             // Get the payload
          
       }
       Serial.println(message);
    }
    
} // Loop

