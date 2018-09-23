
/*
  Getting Started example sketch for nRF24L01+ radios
  This is a very basic example of how to send data from one node to another
  Updated: Dec 2014 by TMRh20
*/

#include "printf.h" // To allow printf to Serial

// Interfaces
#include <SPI.h>
#include "RF24.h"
#include <Servo.h>
//#include <ServoTimer2.h>

// Adafruit RGB display
#include <Wire.h>


#define MOTOR_PIN_A  5
#define MOTOR_PIN_B  6

#define SERVO_PIN    2
//#define RELEVAGE_PIN   3

#define RADIO_CE     9
#define RADIO_CS    10


long MOT_REVERSE = 0;
long MOT_SPEED = 0;

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
// radio (ce_pin, cs_pin)
RF24 radio( RADIO_CE, RADIO_CS);
/**********************************************************/

byte addresses[][6] = {"1Node", "2Node"};

int cur_x;
int cur_y;
int i_servo2_pos;

Servo myservo;
//ServoTimer2 myservo2;

void setup() {

  // Init the buffers
  cur_x = -1;
  cur_y = -1;


  pinMode( MOTOR_PIN_A, OUTPUT);
  pinMode( MOTOR_PIN_B, OUTPUT);
  //  pinMode(RELEVAGE_PIN, OUTPUT);

  // attach servoS
  myservo.attach(SERVO_PIN);

//  myservo2.attach(RELEVAGE_PIN);
 // myservo2.write(255);

  MOT_SPEED = 1024;
  // init motor : stopped
  setMotor (MOT_REVERSE, 0);
  delay(1000);

//  analogWrite(RELEVAGE_PIN, 0);

  // Init serial communication
  Serial.begin(115200);

  // Init printf to serial
  printf_begin();

  radio.begin();

  // Set the PA Level low to prevent power supply related issues since this is a
  // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  radio.setPALevel(RF24_PA_LOW);

  // Open a writing and reading pipe on each radio, with opposite addresses
  radio.openWritingPipe(addresses[0]);
  radio.openReadingPipe(1, addresses[1]);

  // Start the radio listening for data
  radio.startListening();

  radio.printDetails();


}

void HexPrint(char * string)
{
  if (strlen(string) > 0)
  {
    for (int i = 0; i < strlen(string); i++)
    {
      char caract[3];
      sprintf(caract, "%X", string[i]);
      Serial.println(caract);
    }
  }
}


void setMotor(int mot_speed, boolean reverse)
{
  int pwm_speed = 0;

  printf("Set motor");
  if (mot_speed > 255)
    pwm_speed = 255;
  else
    pwm_speed = mot_speed;

  if (! reverse)
  {
    analogWrite(MOTOR_PIN_A, pwm_speed);
    analogWrite(MOTOR_PIN_B, 0);
  }
  else
  {
    analogWrite(MOTOR_PIN_A, 0);
    analogWrite(MOTOR_PIN_B, pwm_speed);
  }
}


void loop()
{
  char message[32] = "";
  char sz_line1[17] = "";
  char sz_line2[17] = "";

  int i_x_joy = 0;
  int i_y_joy = 0;
  int i_btn_joy = 0;
  int i_btn_up = 0;
  int i_btn_down = 0;
  int i_btn_left = 0;
  int i_btn_right = 0;

  int i_new_x = 0;
  int i_new_y = 0;


  int i_status = 0;

  if ( radio.available())
  {
    while (radio.available()) {                                   // While there is data ready
      radio.read( message, 32);                                   // Get the payload
    }

  }
  i_status = sscanf(message, "%d %d", &i_x_joy, &i_y_joy ); //, &i_btn_joy, &i_btn_up, &i_btn_down, &i_btn_left, &i_btn_right);

  if (i_status > 0)
  {

    i_new_x = map(i_x_joy, 0, 1024, 0, 180);
    if ( cur_x != i_new_x)
    {
      printf("Servo cur_x %i\n", cur_x);
      cur_x = i_new_x;
      myservo.write(cur_x );
    }

    i_new_y = i_y_joy; // map(i_y_joy, 0, 1024, -255, 255);
    if ( cur_y != i_new_y)
    {
      cur_y = i_new_y;

      //joystick centré : arrêt
      if (abs (cur_y - 512) <= 10)
        setMotor (0, 0);
      else
      {
        if (cur_y <= 512)
        {
          MOT_SPEED = (512 - cur_y) / 2;
          MOT_REVERSE = true;
        }
        else
        {
          MOT_SPEED = (cur_y - 512 ) / 2;
          MOT_REVERSE = false;
        }

        setMotor (MOT_SPEED, MOT_REVERSE);
      }
    }

//    //     confirm servo2 position
//    if (i_btn_up)
//    {
//      if (i_servo2_pos++ >= 255)
//        i_servo2_pos = 255;
//    }
//    else if (i_btn_down)
//    {
//      if (i_servo2_pos-- <= 0)
//        i_servo2_pos = 0;
//    }
//    analogWrite(RELEVAGE_PIN, i_servo2_pos );

  }

} // Loop
