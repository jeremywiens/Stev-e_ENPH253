//A function to keep track of which state requires which sensors
void sensors(States stat){
    switch(stat){
        case FirstEwok:
            tape_sensors(stat);
            stop_sensors(stat);
            break;
        case FirstGap:
            encodeForward();
            stop_sensors(stat);
            break;
        case SecondEwok:
          stop_sensors(stat);
          tape_sensors(stat);
          break;
        case ThirdEwok:
          stop_sensors(stat);
          tape_sensors(stat);
          break;
        case JustTape:
          tape_sensors(JustTape);
          break;
        case IR:
          IR_sensors(IR);
          break;
        default:
          break;

    }

}

//If certain sensors are triggered in different states, Stev-e responds differently.
//These are the crucial cases where stev-e picks up and ewok or drops a plate, and then changes state.
void stop_sensors(States stat){
    switch(stat){
        case FirstEwok:

          if(!digitalRead(RIGHT_CLAW)){
              end_moving();
              rightClawGrab();
              delay(10);
              resetBothDist();
              state = FirstGap;
              initial_motor_speed = s_initial_motor_speed;
              gain = 110;
              reset_error();
          }
           else if((analogRead(EDGE_DETECT) > EDGE_THRESHOLD) && (analogRead(MIDDLE_LEFT_QRD) > EDGE_THRESHOLD) ){
              end_moving();
              firstGap(10);
              dropPlate();
              state = Stop; //First gap if no initial ewok
          }
                break;
        case FirstGap:
            if(analogRead(EDGE_DETECT) > EDGE_THRESHOLD ){
              int tempSpeed = encode_speed;
              encode_speed = 25000;
              end_moving();
              firstGap(9);
              delay(10);
              reset_error();
              dropPlate();
              delay(750);
              QRD_TRHESHOLD_TAPE = 1000;
              crossFirstBridge(stat);
              end_moving();
              initial_motor_speed = slow_speed - 3900;
              reset_error();
              state = SecondEwok;
              max_PWM = 28000;
              encode_speed = tempSpeed;
              tempKp = Kp;
              Kp = 80;
            }
              break;

        case SecondEwok:
            //If ewok sensed
            if(!digitalRead(RIGHT_CLAW)){
              end_moving();
              pwmWrite(left_mb, 32000);
              pwmWrite(right_mb, 32000);
              delay(190);
              end_moving();
              rightClawGrab2();
              delay(10);
              resetBothDist();
              state = IR;
              initial_motor_speed = s_initial_motor_speed;
              max_PWM = 55000;
              gain = s_gain;
              Kp = tempKp;
            }
            break;

        case ThirdEwok:
        //Look for edge is a boolean turned true after Stev-e is ready for third ewok
          if(lookForEdge){
            if(analogRead(EDGE_DETECT) > EDGE_THRESHOLD && analogRead(MIDDLE_RIGHT_QRD) > EDGE_THRESHOLD && analogRead(MIDDLE_LEFT_QRD) > EDGE_THRESHOLD ){
              end_moving();
              delay(motorDelay);
              pwmWrite(right_mb, 19000);
              pwmWrite(left_mb, 19000);
              delay(100);
              end_moving();
              leftClawGrab();
              state = Dump;
            }
          }
          break;

        default:
          end_moving();
          break;
      }
    }

//Function that returns true when Stev-e is over tape and can start tape following
bool isTape(States stat){
  int digitalLeft = 1;
  int digitalRight = 1;
  if(stat == FirstGap || stat == FirstEwok){
    left_qrd = analogRead(INITIAL_LEFT_QRD);
    right_qrd = analogRead(INITIAL_RIGHT_QRD);
  }
  else{
    left_qrd = analogRead(MIDDLE_LEFT_QRD);
    right_qrd = analogRead(MIDDLE_RIGHT_QRD);
  }
  if(left_qrd > QRD_TRHESHOLD_TAPE){
    return true;
  }
  if(right_qrd > QRD_TRHESHOLD_TAPE){
    return true;  
  }
  return false;
}

//Uses the IR sensors to calculate the error associated with the location of Stev-e
//with respect to the tape. This error is used in PID calculation.
void tape_sensors(States stat) {


  //negative PID_value <-- left motor speed increase, right motor speed decrease
  //positive PID_value <-- right motor speed increase, left motor speed decrease

  int digitalLeft = 1;
  int digitalRight = 1;
  if(stat == FirstGap || stat == FirstEwok){
    left_qrd = analogRead(INITIAL_LEFT_QRD);
    right_qrd = analogRead(INITIAL_RIGHT_QRD);
  }
  else{
    left_qrd = analogRead(MIDDLE_LEFT_QRD);
    right_qrd = analogRead(MIDDLE_RIGHT_QRD);
  }

  if(left_qrd < QRD_TRHESHOLD_TAPE){
    digitalLeft = 0;
  }
  if(stat == ThirdEwok){
    if(right_qrd < 2200){
      digitalRight = 0;
    }
  }
  else{
  if(right_qrd < QRD_TRHESHOLD_TAPE){
    digitalRight = 0;
  }
  }


  //concating sensor readings together
  int readings = digitalLeft * 10 + digitalRight;
  if (readings != 0)
    previous = readings;
  
  switch (readings) {

    case 1:
      error = 1;
      break;
    case 11:
      error = 0;
      break;

    case 10:
      error = -1;
      break;    
      case 0:
      if (previous == 1) 
        error = 2;
      else if (previous == 10)
        error = -2;
      else
        error = 0;
       break;
  }
 }


//Uses the front IR sensors to help align Stev-e with the edge in front of it.
bool gapAlign(int scenario){
  if(scenario == 1){
    int digitalLeft = 1;
    int digitalRight = 1;
    bool atEdge = false;

    left_qrd = analogRead(MIDDLE_LEFT_QRD);
    right_qrd = analogRead(MIDDLE_RIGHT_QRD);

    if(left_qrd < EDGE_THRESHOLD){
      digitalLeft = 0;
    }
    if(right_qrd < EDGE_THRESHOLD){
      digitalRight = 0;
    }

    if(digitalLeft && digitalRight){
        atEdge = true;
    }
    return atEdge;
  }

  else if(scenario == 2){
    if(analogRead(EDGE_DETECT) < EDGE_THRESHOLD){
        return true;
    }
    return false;
  }
}



//IR sensor function
//Changes state to ThirdEwok once received 10K
//signal from R2D2
void IR_sensors(States stat) {
  while(stat == IR) {
    int prevFreq = getSignal();
    while(prevFreq == 10) {
        if(getSignal() == 1) {
            prevFreq = ONEK;
        }
    }
    while(prevFreq == 1) {
        if(getSignal() == 10) {
            stat = ThirdEwok;
            prevFreq = TENK;
        }
    }
  }
}

//returns the type of signal detected by the IR circuit. 1 for 1K 10 for 10K 0 for no signal and 5 for an error (1 and 10 k detected simultaneously)
int getSignal() {
    if(digitalRead(oneKpin) == HIGH && digitalRead(tenKpin) == HIGH) {
        return NOSIG;
    }
    else if(digitalRead(oneKpin) == HIGH && digitalRead(tenKpin) == LOW) {
        return TENK;
    }
    else if(digitalRead(oneKpin) == LOW && digitalRead(tenKpin) == HIGH) {
        return ONEK;
    }
    return ERR;
}