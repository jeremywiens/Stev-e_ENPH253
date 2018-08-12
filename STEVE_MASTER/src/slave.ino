// initiate slave trigs

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
const int position_trig = 19;

// volatile int done = 0;
int wireTime;

//volatile bool done = false;

void bucketRaise(){
  checkReceive(bucket_raise);
}

void bucketLower(){
  checkReceive(bucket_lower);
}

void leftClawGrabChewy(){
  checkReceive(left_claw_chewy);
}

void readyForThirdEwok(){
  checkReceive(ready_for_third_ewok);
}

void readyForTroopers(){
  checkReceive(ready_for_troopers);
}

void closeLeftClaw(){
  checkReceive(close_left_claw);
}

void dropPlate(){ 
  checkReceive(first_plate_drop_trig);
}

void dropSecondPlate(){
  checkReceive(second_plate_drop_trig);
}
void rightClawGrab2(){
  checkReceive(ewok_2_pick_up_trig);
}

void rightClawGrab(){
  checkReceive(right_claw_pick_up_trig);
}
void raiseLeftArm(){
  checkReceive(lift_left_claw_arch);
}
void leftClawGrab(){
  checkReceive(left_claw_pick_up_trig);
}

void bucketDump(){
  checkReceive(bucket_trig);
}

//Grab the value of the encoders from the slave board
void getPos(){
  byte a, b, c, d, e, f, g, h;
  Wire.requestFrom(4, 8);
  while (Wire.available()){

      a = Wire.read();
      b = Wire.read();
      c = Wire.read();
      d = Wire.read();
      e = Wire.read();
      f = Wire.read();
      g = Wire.read();
      h = Wire.read();
  }
  left_pos = a;
  left_pos = (left_pos << 8) | b;
  left_pos = (left_pos << 8) | c;
  left_pos = (left_pos << 8) | d;

  right_pos = e;
  right_pos = (right_pos << 8) | f;
  right_pos = (right_pos << 8) | g;
  right_pos = (right_pos << 8) | h;
}

//Reset the value of the encoders to zero on the slave board
void resetBothDist(){
  Wire.beginTransmission(4);
  Wire.write(reset_both);
  Wire.endTransmission();
}



void checkReceive(int trigger) {
  int receive = 0;
  while (receive != trigger){
    Wire.beginTransmission(4);
    Wire.write(trigger);
    Wire.endTransmission();
    delayMicroseconds(500);
    Wire.requestFrom(4, 1);
    while (Wire.available()){
      receive = Wire.read();
    }
  }
}