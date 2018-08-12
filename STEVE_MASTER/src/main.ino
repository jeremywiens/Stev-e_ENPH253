#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <EEPROM.h>

//IRPINs
#define tenKpin PA11
#define oneKpin PA12

//Pins for I2C communication
#define Wire WIRE2

// Tape following pins
#define INITIAL_RIGHT_QRD PB0
#define INITIAL_LEFT_QRD PB1
#define MIDDLE_RIGHT_QRD PA6
#define MIDDLE_LEFT_QRD PA7
#define EDGE_DETECT PA5 //A4 sucks

//Motor control pins
#define left_mb PA3 //define motor inputs
#define left_mf PA2
#define right_mb PA1
#define right_mf PA0

// Claw pins
#define RIGHT_CLAW PB13
#define LEFT_CLAW PB12


// Back up bumper pins
#define RIGHT_BUMPER PB14
#define LEFT_BUMPER PB15

// Menu control pins
#define select_button PB5
#define up_button PB3
#define down_button PB4

// STATE MACHINE
enum States{
  Check, FirstEwok, FirstGap, SecondEwok, IR, ThirdEwok, Dump, SecondGap, Hell, Stop, TestInitial, TestSecond, JustTape
  };

// State initialized for Stev-e check up
int state = Check;


//  variable distances which can be changed
int rightDist = 0;
int leftDist = 0;

// Encoder values
double left_vel = 0;
double right_vel = 0;
int left_pos = 0;
int right_pos = 0;
//Values to control timing for encoders
unsigned long timeIs;
unsigned long lastTime = 0;
int timeDelay = 1000;

// IR constants
const int TENK = 10;
const int ONEK = 1;
const int ERR = 5;
const int NOSIG = 0;

//Wire Stuff for now placed here
TwoWire WIRE2 (2, I2C_FAST_MODE);

//Constants for Tape following and eddge dection
int QRD_TRHESHOLD_TAPE  = 1000;  //analog readings below 1000 if line detected 
#define EDGE_THRESHOLD 1400 //normally 2200, if greater, there is an edge

//Global variables for tape following
int max_PWM = 65500;
int max_error = 65000;

int left_qrd = 0;
int right_qrd = 0;
int previous = 0;
int selected = 0;
int counter = 0;

//adjustable constants for PID-tape
float Kp = 0 , Ki = 0, Kd = 0, gain = 0;
float initial_motor_speed =  0;
float slow_speed = 27500;
float encode_speed = 20000;
int turn_speed = 25000;
int tempKp = 0;

//Adjustable constants for the initial tape following
float i_Kp = 160, i_Ki = 0, i_Kd = 70, i_gain = 115;
float i_initial_motor_speed = 39000;


//Adjustable constants for the secondary tape following
float s_Kp = 130, s_Ki = 0, s_Kd = 80, s_gain = 90;
float s_initial_motor_speed = 26000;
//Global variables for PID-Tape
float error = 0, P = 0, I = 0, D = 0, PID_value = 0;
float previous_error = 0, previous_I = 0;

//PID functions
void read_sensor_values(void);
void calculate_pid(void);
void motor_control(void);

int motorDelay = 1; //Motor delay to stop stalling
int testVal = 0;

//Boolean to know which state to prepare the claws for
bool firstRaise = false;
bool lookForEdge = false;
bool secondRaise = false;

// Menu set-up

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 
static const unsigned char PROGMEM logo16_glcd_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

// Booleans and values to control menu
bool changing = false;
bool show_display = false;
bool showing_display  = false;
bool new_screen = false;
int var_selected = 0;
int menu_level = 0;
int num_menu_items = 1;


void setup() {
  //I2C setup
  Wire.begin();

  //IR signal setup
  pinMode(oneKpin, INPUT);
  pinMode(tenKpin, INPUT);
  
  //EEPROM setup
  EEPROM.init();
  EEPROM.PageBase0 = 0x801F000;
  EEPROM.PageBase1 = 0x801F800;
  EEPROM.PageSize  = 0x400;

  //Serial setup via bluetooth connection
  Serial.begin(230400, SERIAL_8E1);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  //Motor pins set up
  pinMode(left_mb, PWM);
  pinMode(left_mf, PWM);
  pinMode(right_mb, PWM);
  pinMode(right_mf, PWM);

  //Tape and edge detection pins
  pinMode(INITIAL_LEFT_QRD, INPUT);
  pinMode(INITIAL_RIGHT_QRD, INPUT);
  pinMode(MIDDLE_RIGHT_QRD, INPUT);
  pinMode(MIDDLE_LEFT_QRD, INPUT);
  pinMode(EDGE_DETECT, INPUT);

  //Claw triggers
  pinMode(RIGHT_CLAW, INPUT_PULLUP);
  pinMode(LEFT_CLAW, INPUT_PULLUP);

  //Back up bumper triggers
  pinMode(RIGHT_BUMPER, INPUT_PULLUP);
  pinMode(LEFT_BUMPER, INPUT_PULLUP);
  
  //Menu button setup
  pinMode(up_button, INPUT_PULLUP);
  pinMode(down_button, INPUT_PULLUP);
  pinMode(select_button, INPUT_PULLUP);
  
  //Initialize led screen for menu
  led_init();
}

void loop() {

    //Menu control
     if(!showing_display){
         show_display = !digitalRead(select_button);
         if(show_display)
            delay(100);
     }
     if(show_display || showing_display){
       showing_display = true;
         end_moving();
         new_screen = true;
        led_display();
    }
    
    
   
    else{
    //Get encoder values after set amount of time if not showing display
    timeIs = millis();
    if(timeIs > (lastTime + timeDelay)){
        getEncoder(FirstEwok);

    }
     //State machine
    switch (state){
      //Initialize stev-e and check functionality
      case Check:
        check();
        end_moving();
        delay(500);
        digitalWrite(LED_BUILTIN, HIGH);
        break;
      case FirstEwok:
        sensors(FirstEwok);
        pid();
        break;
      case FirstGap:
        initial_motor_speed = encode_speed;
        sensors(FirstGap);
        break;
      case SecondEwok:
        sensors(SecondEwok);
        pid();
        break;
      case ThirdEwok:
        sensors(ThirdEwok);
        pid();
        claws();
        break;
      case IR:
        end_moving();
        delay(50);
        sensors(IR);
        afterIR();
        break;
      case Stop:
        end_moving();
        break;
      case Dump:
        prepareDump();
        bucketDump();
        delay(50);
        state = SecondGap;
      break;
      case SecondGap:
        findSecondEdge();
        dropSecondPlate();
        end_moving();
        crossSecondPlate();
        state = Hell;
        break;
      case Hell:
        getLastEwok();
        crossFloppyBridge();
        grabChewy();
        state = Stop;
        break;
      case TestInitial:
        test_initial();
        break;
      case TestSecond:
        test_second();
        break;
      default:
        end_moving();
        break;

    }
    }
}

void end_moving(){
        pwmWrite(left_mf, 0);
        pwmWrite(left_mb, 0);
    
        pwmWrite(right_mb, 0);
        pwmWrite(right_mf, 0);
}

void check(){
  state = Stop;
  Kp = i_Kp, Kd = i_Kd, Ki = i_Ki, gain = i_gain;
  slow_speed = 27500;
  encode_speed = 20000;
  turn_speed = 25000;  
  max_PWM = 65500;
  timeDelay = 1000;
  initial_motor_speed = i_initial_motor_speed;
}

//Resets error for PID use
void reset_error(){
  previous = 0;
  error = 0, P = 0, I = 0, D = 0, PID_value = 0;
  previous_error = 0, previous_I = 0;
}
