
/*
* Getting Started example sketch for nRF24L01+ radios
* This is a very basic example of how to send data from one node to another
* Updated: Dec 2014 by TMRh20
*/

#include <SPI.h>
#include "RF24.h"

#include "printf.h" // printf to Serial

/****************** User Config ***************************/
/***      Set this radio as radio number 0 or 1         ***/
bool radioNumber = 1;
// Used to control whether this node is sending or receiving
bool role = 1;

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
// radio(ce_pin, cs_pin);
RF24 radio(9, 10);
/**********************************************************/

byte addresses[][6] = {"1Node","2Node"};

int up_button = 2;
int down_button = 4;
int left_button = 5;
int right_button = 3;
int start_button = 6;
int select_button = 7;
int joystick_button = 8;
int joystick_axis_x = A0;
int joystick_axis_y = A1;
int buttons[] = {up_button, down_button, left_button, right_button, start_button, select_button, joystick_button};



void setup() {
  Serial.begin(115200);

  printf_begin(); // Defines stdout as Serial

  for (int i; i < 7; i++)
  {
   pinMode(buttons[i], INPUT);
   digitalWrite(buttons[i], HIGH);
  }
  
  radio.begin();

  // Set the PA Level low to prevent power supply related issues since this is a
 // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  radio.setPALevel(RF24_PA_LOW);
  
  // Open a writing and reading pipe on each radio, with opposite addresses
    radio.openWritingPipe(addresses[1]);
    radio.openReadingPipe(1,addresses[0]);
  
  // Start the radio listening for data
  radio.startListening();
  radio.printDetails();


}

void loop() {
  

    char message[32] = "Hello, world!";
    radio.stopListening();                                    // First, stop listening so we can talk.
    
    Serial.println(F("Now sending"));

//    sprintf(message, "%d %d %d %d %d %d %d", map(analogRead(joystick_axis_x), 0, 1024, 0, 255) , map(analogRead(joystick_axis_y) , 0, 1024, 0, 255), digitalRead(joystick_button), digitalRead(up_button), digitalRead(down_button), digitalRead(left_button), digitalRead(right_button));
    sprintf(message, "%04d %04d %d %d %d %d %d", analogRead(joystick_axis_x), analogRead(joystick_axis_y), digitalRead(joystick_button), digitalRead(up_button), digitalRead(down_button), digitalRead(left_button), digitalRead(right_button));
    Serial.println(message);
    
    unsigned long start_time = micros();                             // Take the time, and send it.  This will block until complete
//     if (!radio.write(  &start_time , sizeof(unsigned long) ))
     if (!radio.write(  message , 32 ))
     {
       Serial.println(F("failed"));
     }
        
    radio.startListening();                                    // Now, continue listening
    
    unsigned long started_waiting_at = micros();               // Set up a timeout period, get the current microseconds
    boolean timeout = false;                                   // Set up a variable to indicate if a response was received or not
       
    // Try again 10s later
    delay(100);

} // Loop
