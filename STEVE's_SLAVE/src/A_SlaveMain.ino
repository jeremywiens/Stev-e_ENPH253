#include "Arduino.h"
#include "Servo.h"
#include "Wire_slave.h"
#include <ext_interrupts.h>

#define PLATE_DROP_ONE_SERVO PA7
#define LEFT_ARM_SERVO PB1
#define LEFT_CLAW_SERVO PB0
#define RIGHT_ARM_SERVO PA3
#define RIGHT_CLAW_SERVO PA2
#define BUCKET_SERVO PA0
#define BUCKET_SERVO_2 PA8
#define BUCKET_LOWER PB6
#define BUCKET_RAISE PB7
#define PLATE_DROP_TWO_SERVO PA1
#define BUCKET_LIMIT PB14

#define ENCODE_RIGHT_WHEEL_A PB9
#define ENCODE_LEFT_WHEEL_A PB8

Servo first_plate_drop_servo;
Servo second_plate_drop_servo;
Servo left_arm_servo;
Servo left_claw_servo;
Servo right_claw_servo;
Servo right_arm_servo;
Servo bucket_servo;
Servo bucket_servo_2;

// Bucket PWM Speeds
const int bucketRaise = 55000;
const int bucketMaintain = 20000;
const int bucketLower = 20000;
const int bucketLowerDelay = 500;

// Right Claw Servo Angles
const int rightClawClose = 30;
const int rightClawOpen = 175; 

// Right Arm Pin and Servo Angles;
const int rightArmDown = 180;
const int rightArmUp = 10;
const int rightArmMid = 110;
const int rightArmArch = 20;

//Left Claw Servo Angles
const int leftClawClose = 180;
const int leftClawOpen =30;

// Left Arm Servo Angles
const int leftArmDown = 0;
const int leftArmUp = 170;
const int leftArmMid = 60;
const int leftArmPlate = 95;
const int leftArmArch = 140;

// Lift delays
const int raiseArmDelay = 500;
const int clawCloseDelay = 750;
const int incrementDelayArm = 15;
const int angleIncrementArm = 1;

//Bucket and First Plate Drop Servo Positions
const int min_plate_drop_one_pos = 0;
const int max_plate_drop_one_pos = 180;
const int bucket_up = 160;
const int bucket_down = 40;
const int bucket_mid = 130;
const int min_plate_drop_two_pos = 180;
const int max_plate_drop_two_pos = 0;
const int bucket_dumpable = 10;
const int bucket_hold = 170;

//Trigger States
const int first_plate_drop_trig = 14;
const int right_claw_pick_up_trig = 2;
const int ewok_2_pick_up_trig = 3;
const int left_claw_pick_up_trig = 4;
const int left_claw_lift_trig = 5;
const int bucket_trig = 6;
const int lift_left_claw_arch = 7;
const int ready_for_troopers = 8;
const int ready_for_third_ewok = 9;
const int second_plate_drop_trig = 10;
const int reset_both = 13;
const int close_left_claw = 15;
const int bucket_raise = 16;
const int bucket_lower = 17;
const int left_claw_chewy = 18;


//Encoder positions
volatile u_int32_t left_pos = 0;
volatile u_int32_t right_pos = 0;
volatile u_int8_t right_count = 0;
volatile u_int8_t left_count = 0;

//Master Communication variables
volatile u_int8_t trigger = 0;
volatile int send = 0;
volatile bool position = true;
volatile bool received = false;

//State flags
bool FirstEwokState = false;
bool firstPlateDropState = false;
bool leftArmArchState = false;
bool SecondEwokState = false;
bool readyForTroopersState = false;
bool readyForThirdEwokState = false;
bool ThirdEwokState = false;
bool bucketState = false;
bool secondPlateDropState = false;
bool closeLeftClawState = false;
bool bucketRaiseState = false;
bool bucketKeepUp = false;
bool bucketLowerState = false;
bool grabChewyState = false;

void setup()
{
  Serial.begin(230400, SERIAL_8E1);
  bucketRaiseSetup();
  leftClawArmSetup();
  rightClawArmSetup();
  encoderSetup();
  firstPlateDropSetup();
  secondPlateDropSetup();
  bucketSetup();
  IICSetup();
  raiseLeftArmPlate();
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
}


//trying something, if doesn't work get rid of functions
void requestEvent() {
  if (position == true){
    sendEncoderVals();
  }
  else {   
      Wire1.write(send);
      if(send != 0){
          position = true;
      }
  }

}
void receiveEvent(int howmany){
    trigger = Wire1.read();
    if (trigger == reset_both){
        resetEncoderVals();
    }
    else {
        send = 0;
        position = false;
    }
}

void loop(){
    if(bucketKeepUp){
        maintainBucket();
    }
    switch (trigger){
       
        case 0:
        break;

        case right_claw_pick_up_trig:
            if (FirstEwokState == false){
                pickupRight();
            }
            doneFunc(right_claw_pick_up_trig);
            break;

        case first_plate_drop_trig:
            if (firstPlateDropState == false){
                dropFirstPlate();
            }
            doneFunc(first_plate_drop_trig);
            break;

        // case lift_left_claw_arch:
        //     if (leftArmArchState == false){
        //         raiseLeftArmPlate();
        //     }
        //     doneFunc(lift_left_claw_arch);
        //     break;

        case ewok_2_pick_up_trig:
            if (SecondEwokState == false){
                pickupEwok2();
            }
            doneFunc(ewok_2_pick_up_trig);
            break;

        case ready_for_troopers:
            if (readyForTroopersState == false){
                readyForTroopers();
            }
            doneFunc(ready_for_troopers);
            bucketShake();
            break;

        case ready_for_third_ewok:
            if (readyForThirdEwokState == false){
                readyForThirdEwok();
            }
            doneFunc(ready_for_third_ewok);
            break;

        case left_claw_pick_up_trig:
            if (ThirdEwokState == false){
                pickupLeft();
            }
            doneFunc(left_claw_pick_up_trig);
            break;

        case bucket_trig:
            if (bucketState == false){
                bucketDrop();
                left_arm_servo.write(leftArmPlate);
            }
            doneFunc(bucket_trig);
            break;

        case second_plate_drop_trig:
            if (secondPlateDropState == false){
                dropSecondPlate();
                left_arm_servo.write(leftArmDown);
            }
            doneFunc(second_plate_drop_trig);
            ThirdEwokState = false;
            break;
        case close_left_claw:
            if (closeLeftClawState == false){
                left_claw_servo.write(leftClawClose);
            }
            doneFunc(close_left_claw);
            break;
        case bucket_raise:
            if (bucketRaiseState == false){
                trigger = 0;
                //Serial.println("bucket raise state");
                raiseBucket();
            }
            //Serial.println("raised bucket");
            //doneFunc(bucket_raise);
            send = bucket_raise;
            break;
        case bucket_lower:
            if(bucketLowerState == false){
                lowerBucket();
            }
            doneFunc(bucket_lower);
            break;

        case left_claw_chewy:
            if(grabChewyState == false){
                grabChewy();
            }
            doneFunc(left_claw_chewy);
            break;
        default:
        break;

    }
}