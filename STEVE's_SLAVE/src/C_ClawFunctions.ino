void openLeftClaw(){
  left_claw_servo.write(leftClawOpen);
}

void closeLeftClaw(){
  left_claw_servo.write(leftClawClose);
}

void lowerLeftArm(int topAngle, int lowerAngle, int delayTime){
    if (delayTime == 0){
      left_arm_servo.write(lowerAngle);
    }
    else{
      for (int angle = topAngle; angle >= lowerAngle; angle -= angleIncrementArm) { 
      left_arm_servo.write(angle);
      delay (delayTime);
      }
    }
}

void raiseLeftArm(int topAngle, int lowerAngle, int delayTime){
  if (delayTime == 0){
    left_arm_servo.write(topAngle);
  }
  else{
    for (int angle = lowerAngle; angle <= topAngle; angle += angleIncrementArm) { 
    left_arm_servo.write(angle);
    delay (delayTime);
    }
  }
}

void openRightClaw(){
  right_claw_servo.write(rightClawOpen);
}

void closeRightClaw(){
  right_claw_servo.write(rightClawClose);
}

void lowerRightArm(int topAngle, int lowerAngle, int delayTime){
  if(delayTime == 0){
    right_arm_servo.write(lowerAngle);
  }
  else{
    for (int angle = topAngle; angle <= lowerAngle; angle += angleIncrementArm) { 
      right_arm_servo.write(angle);
      delay (delayTime);
    }
  }
}

void raiseRightArm(int topAngle, int lowerAngle, int delayTime){

  if (delayTime == 0){
    right_arm_servo.write(topAngle);
  }
  else{
    for (int angle = lowerAngle; angle >= topAngle; angle -= angleIncrementArm) { 
      right_arm_servo.write(angle);
      delay (incrementDelayArm);
    }
  }
}
