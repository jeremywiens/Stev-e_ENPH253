void pickupRight(){
    closeRightClaw();
    delay(clawCloseDelay);
    raiseRightArm(rightArmUp, rightArmDown, incrementDelayArm);
    delay(raiseArmDelay);
    openRightClaw();
    lowerRightArm(rightArmUp, rightArmDown, incrementDelayArm);
    FirstEwokState = true;
}

void dropFirstPlate(){
    first_plate_drop_servo.write(max_plate_drop_one_pos);
    delay(100);
    firstPlateDropState = true;
}

void raiseLeftArmPlate(){
    raiseLeftArm(leftArmPlate, leftArmDown, 0);
    closeLeftClaw();
    delay(5);
    leftArmArchState = true;
}

void raiseBucket(){
    bucket_servo_2.write(bucket_hold);
    delay(300);
    pwmWrite(BUCKET_RAISE, bucketRaise);
    //delay(2000);
    bool atLimit = false;
    while(!atLimit){
        digitalRead(BUCKET_LIMIT);
        delay(10);
        atLimit = !digitalRead(BUCKET_LIMIT);
    }
    pwmWrite(BUCKET_RAISE, 0);
    bucketKeepUp = true;
    bucketRaiseState = true;
}

void maintainBucket(){
    bool atLimit = !digitalRead(BUCKET_LIMIT);
    if(!atLimit){
        pwmWrite(BUCKET_RAISE, bucketMaintain);
    }
    else{
        pwmWrite(BUCKET_RAISE, 0);
    }
}

void lowerBucket(){
    bucket_servo_2.write(bucket_dumpable);
    bucketKeepUp = false;
    pwmWrite(BUCKET_RAISE, 0);
    delay(100);
    pwmWrite(BUCKET_LOWER, bucketLower);
    delay(bucketLowerDelay);
    pwmWrite(BUCKET_LOWER, 0);
    bucketLowerState = true;
}


void pickupEwok2(){
    closeRightClaw();
    delay(clawCloseDelay);
    raiseRightArm(rightArmUp, rightArmDown, incrementDelayArm);
    delay(raiseArmDelay);
    openRightClaw();
    delay(500);
    lowerRightArm(rightArmUp, rightArmArch, incrementDelayArm);
    raiseLeftArm(leftArmArch, leftArmPlate, 0);
    SecondEwokState = true;
}

void readyForTroopers(){
    Serial.println("getting ready for troopers");
    lowerRightArm(rightArmArch, rightArmMid, 0);
    lowerLeftArm(leftArmArch, leftArmMid, 0);
    readyForTroopersState = true;
}

void readyForThirdEwok(){
    lowerLeftArm(leftArmMid, leftArmDown, 0);
    openLeftClaw();
    lowerRightArm(rightArmMid, rightArmDown, 0);
    openRightClaw();
    readyForThirdEwokState = true;
}

void grabChewy(){
    pickupLeft();
    grabChewyState = true;
}

void pickupLeft(){
    closeLeftClaw();
    delay(clawCloseDelay);
    raiseLeftArm(leftArmUp, leftArmDown, incrementDelayArm);
    delay(raiseArmDelay);
    openLeftClaw();
    delay(50);
    openLeftClaw();
    lowerLeftArm(leftArmUp, leftArmDown, incrementDelayArm - 2);
    ThirdEwokState = true;
}

void bucketDrop(){
    for (int angle = bucket_up; angle >= bucket_down; angle--) { 
      bucket_servo.write(angle);
      delay (5);
    }
    delay(3000);
    for (int angle = bucket_down; angle <= bucket_up; angle++) { 
      bucket_servo.write(angle);
      delay (2);
    }
    bucketState = true;
}

void bucketShake(){
    int count = 0;
    while (count < 2){
        for (int angle = bucket_up; angle >= bucket_mid; angle--) { 
            bucket_servo.write(angle);
            delay (5);
        }
        for (int angle = bucket_mid; angle <= bucket_up; angle++) { 
            bucket_servo.write(angle);
            delay (2);
        }
        count++;
    }
}

void dropSecondPlate(){
    second_plate_drop_servo.write(max_plate_drop_two_pos);
    delay(100);
    secondPlateDropState = true;
}

void doneFunc(int trig){
    send = trig;
    trigger = 0;
}