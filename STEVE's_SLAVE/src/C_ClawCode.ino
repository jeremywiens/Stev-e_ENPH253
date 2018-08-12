int incrementDelayArm = 15;
int angleIncrementArm = 1;

void rightClawArmSetup(){
  right_claw_servo.attach(RIGHT_CLAW_SERVO);
  right_arm_servo.attach(RIGHT_ARM_SERVO);
  //pinMode(rightContact,INPUT_PULLDOWN);
  right_claw_servo.write(rightClawOpen);
  right_arm_servo.write(rightArmDown);
}

// Left side setup
void leftClawArmSetup() {
  left_claw_servo.attach(LEFT_CLAW_SERVO);
  left_arm_servo.attach(LEFT_ARM_SERVO);
  //pinMode(leftContact,INPUT_PULLDOWN);  
  left_claw_servo.write(leftClawOpen);
  left_arm_servo.write(leftArmDown);
}

// Purpose: Commands left side to follow pickup sequence
void pickupLeft(){
  Serial.println("initiate");
  left_claw_servo.write(leftClawClose);
  delay(clawCloseDelay);
  Serial.println("closed");
  raiseLeftArm();
  delay(raiseArmDelay);
  Serial.println("lifted");
  openLeftClaw();
  Serial.println("dropping");
  lowerLeftArm();
  ThirdEwokState = true;
}

// Purpose: Controls left claw speed
void openLeftClaw(){
  left_claw_servo.write(leftClawOpen);
}

// Purpose: Controls left arm speed
void lowerLeftArm(){
    for (int angle = leftArmUp; angle >= leftArmDown; angle -= angleIncrementArm) { 
      left_arm_servo.write(angle);
      delay (incrementDelayArm); // incrementDelayArm * (ArmUpAngle - ArmDownAngle) = 15 * (150-0) = 2.25 seconds
    }
}

void lowerLeftArmMid(){
    for (int angle = leftArmArch; angle >= leftArmMid; angle -= angleIncrementArm) { 
      left_arm_servo.write(angle);
      delay (incrementDelayArm); // incrementDelayArm * (ArmUpAngle - ArmDownAngle) = 15 * (150-0) = 2.25 seconds
    }
}

void lowerLeftArmMidGround(){
    for (int angle = leftArmMid; angle >= leftArmDown; angle -= angleIncrementArm) { 
      left_arm_servo.write(angle);
      //delay (incrementDelayArm); // incrementDelayArm * (ArmUpAngle - ArmDownAngle) = 15 * (150-0) = 2.25 seconds
    }
}

// Purpose: Controls left arm speed
void raiseLeftArm(){
    for (int angle = leftArmDown; angle <= leftArmUp; angle += angleIncrementArm) { 
      left_arm_servo.write(angle);
      delay (incrementDelayArm); // incrementDelayArm * (ArmUpAngle - ArmDownAngle) = 15 * (150-0) = 2.25 seconds
    }
  left_claw_servo.write(leftClawClose);
  
}
void raiseLeftArmPlate(){
  left_arm_servo.write(leftArmPlate);
  delay(100);
  left_claw_servo.write(leftClawClose);
  delay(5);
  leftArmArchState = true;
}

void raiseLeftArmArch(){
  left_arm_servo.write(leftArmArch);
}

// Purpose: Commands left side to follow pickup sequence
void pickupRight(){
  right_claw_servo.write(rightClawClose);
  delay(clawCloseDelay);
  raiseRightArm();
  delay(raiseArmDelay);
  openRightClaw();
  lowerRightArm();
  FirstEwokState = true;
}

void pickupEwok2(){
  right_claw_servo.write(rightClawClose);
  delay(clawCloseDelay);
  raiseRightArm();
  delay(raiseArmDelay);
  openRightClaw();
  delay(500);
  lowerRightArmArch();
  raiseLeftArmArch();
  SecondEwokState = true;
}

void readyForTroopers(){
  left_arm_servo.write(leftArmMid);
  right_arm_servo.write(rightArmMid);
  readyForTroopersState = true;
}


// Purpose: Controls left claw speed
void openRightClaw(){
  right_claw_servo.write(rightClawOpen);
}

// Purpose: Controls left arm speed
void lowerRightArm(){
    for (int angle = rightArmUp; angle <= rightArmDown; angle += angleIncrementArm) { 
      right_arm_servo.write(angle);
      delay (incrementDelayArm); // incrementDelayArm * (ArmUpAngle - ArmDownAngle) = 15 * (150-0) = 2.25 seconds
    }
}

// Purpose: Controls left arm speed
void raiseRightArm(){
    for (int angle = rightArmDown; angle >= rightArmUp; angle -= angleIncrementArm) { 
      right_arm_servo.write(angle);
      delay (incrementDelayArm); // incrementDelayArm * (ArmUpAngle - ArmDownAngle) = 15 * (150-0) = 2.25 seconds
    }
  
}

void lowerRightArmArch(){
    for (int angle = rightArmUp; angle <= rightArmArch; angle += angleIncrementArm) { 
      right_arm_servo.write(angle);
      delay (incrementDelayArm); // incrementDelayArm * (ArmUpAngle - ArmDownAngle) = 15 * (150-0) = 2.25 seconds
    }
  
}

void lowerRightArmMid(){
    for (int angle = rightArmArch; angle <= rightArmMid; angle += angleIncrementArm) { 
      right_arm_servo.write(angle);
      //delay (incrementDelayArm); // incrementDelayArm * (ArmUpAngle - ArmDownAngle) = 15 * (150-0) = 2.25 seconds
    }
}

void lowerRightArmMidGround(){
    for (int angle = rightArmMid; angle <= rightArmDown; angle += angleIncrementArm) { 
      right_arm_servo.write(angle);
      delay (incrementDelayArm); // incrementDelayArm * (ArmUpAngle - ArmDownAngle) = 15 * (150-0) = 2.25 seconds
    }
}

void dropEwok2(){
    for (int angle = rightArmMid; angle >= rightArmUp; angle -= angleIncrementArm) { 
      right_arm_servo.write(angle);
      delay (incrementDelayArm); // incrementDelayArm * (ArmUpAngle - ArmDownAngle) = 15 * (150-0) = 2.25 seconds
    }
  delay(500);
  openRightClaw();
}

void readyForThirdEwok(){
    right_arm_servo.write(rightArmDown);
    left_arm_servo.write(leftArmDown);
    openRightClaw();
    openLeftClaw();
    readyForThirdEwokState = true;
}