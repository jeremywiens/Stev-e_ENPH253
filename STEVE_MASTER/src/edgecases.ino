//Important functions specific to where Stev-e is relative to the course
//Organized sequantially and by importance

//What to do at first gap. 
//Requires backUp which is number of encoder values to back up after aligned to gap
void firstGap(int backUp){
    bool atEdge = false;
    end_moving();
    delay(motorDelay);

    //brake
    pwmWrite(left_mb, slow_speed);
    pwmWrite(right_mb, slow_speed);
    delay(50);
    end_moving();
    delay(motorDelay);

    //Initial alignment
    while(!atEdge){
        atEdge = gapAlign(1);
        pwmWrite(right_mf, slow_speed);
    }
    end_moving();
    delay(motorDelay);

    //brake
    pwmWrite(right_mb, slow_speed);
    delay(50);
    end_moving();
    delay(motorDelay);

    atEdge = false;

    //Second alignment
    while(!atEdge){
        atEdge = gapAlign(2);
        pwmWrite(left_mb, slow_speed+500);
    }

    //brake
    end_moving();
    delay(motorDelay);
    pwmWrite(left_mf, slow_speed);
    atEdge = false;
    delay(50);
    end_moving();
    delay(motorDelay);
    
    //Back up specified amount
    resetBothDist();
    getPos();
    while(right_pos < (backUp) && left_pos < (backUp)){
        getPos();

        if(left_pos < (backUp - 3)){
            pwmWrite(left_mb, slow_speed);
        }
        else{
            pwmWrite(left_mb, 0);
            pwmWrite(left_mf, 1000);
        }
        if(right_pos < backUp){
            pwmWrite(right_mb, slow_speed + 2500);
        }
        else{
            pwmWrite(right_mb, 0);
            pwmWrite(right_mf, 1000);
        }
    }
    
    end_moving();
    delay(50);


}

void crossFirstBridge(States stat){

    //Get on bridge
    int tempEncodespeed = encode_speed;
    encode_speed = encode_speed + 4000;
    goForward(32);
    encode_speed = tempEncodespeed;
    end_moving();
    delay(20);
   
   //Find tape on bridge
    scan(8, SecondEwok);
    end_moving();
    delay(20);

    //Follow tape on bridge for specified amount
    resetBothDist();
    lastTime = millis();
    getPos();
    int initTime = millis();
    timeIs = millis();
    initial_motor_speed = s_initial_motor_speed + 1000;
    while(right_pos < 15 && left_pos < 15 && timeIs < (initTime+ + 1500)){
        sensors(JustTape);
        pid();
        timeIs = millis();
        if(timeIs > (lastTime + 100)){
            lastTime = millis();
            getPos();
        }

        
    }
    
    //Go forward for specified amount to get off bridge
    end_moving();
    lastTime = millis();
    leftDist = 42;
    rightDist = 42;
    resetBothDist();
    getPos();
    while(right_pos < rightDist && left_pos < leftDist){
        if(left_pos < leftDist){
            pwmWrite(left_mf, slow_speed+1000);
            pwmWrite(left_mb, 0);
        }
        else{
            pwmWrite(left_mb, 1000);
            pwmWrite(left_mf, 0);
        }
        if(right_pos < rightDist){
            pwmWrite(right_mf, slow_speed+1000);
            pwmWrite(right_mb, 0);
        }
        else{
            pwmWrite(right_mb, 1000);
            pwmWrite(right_mf, 0);
        }

        timeIs = millis();
        if(timeIs > (lastTime + 50)){
            lastTime = millis();
            getPos();
        }
    }
    
    end_moving();
    delay(50);

    //Change state and find tape off bridge
    state = SecondEwok;
    scan(30, SecondEwok);
    end_moving();
}

//A function that helps Stev-e find tape underneath him in order to tape follow
//Requires state in order to know which sensors are active, and clicks to know how long to scan for
void scan(int clicks, States stat){
 bool skipSearch = false;
    lastTime = millis();
    resetBothDist();
    getPos();
    int initialTime = millis();
    timeIs = millis();

    //Scans left first
    while(!skipSearch && ((right_pos < clicks) && (timeIs < (initialTime + clicks*70))) ){
        skipSearch = isTape(stat);
        pwmWrite(left_mf, 0);
        pwmWrite(left_mb, turn_speed);
        pwmWrite(right_mb, 0);
        pwmWrite(right_mf,turn_speed);
        timeIs = millis();
        if(timeIs > (lastTime + 50)){
            lastTime = millis();
            getPos();
        }
        if(skipSearch == true){
            reset_error();
        }
    }

    end_moving();
    delay(motorDelay);

    lastTime = millis();
    resetBothDist();
    getPos();

    //If no tape found, scan right
    while(!skipSearch && left_pos < clicks*3){
        skipSearch = isTape(stat);
        pwmWrite(left_mf, turn_speed);
        pwmWrite(left_mb, 0);
        pwmWrite(right_mf,0);
        pwmWrite(right_mb, turn_speed);
        timeIs = millis();
        if(timeIs > (lastTime + 50)){
            lastTime = millis();
            getPos();
        }
        if(skipSearch == true){
            reset_error();
        }
    }
    end_moving();
}

//Using the PID algorithm and encoder values, go forwad
//Must reset encoders before using
void encodeForward(){
    getPos();
    int offset = right_pos - left_pos;
    if(offset > max_error)
        offset = max_error;
    else if (offset < -max_error)
        offset = -max_error;
    error = offset;
    pid();

}

//Using the PID algorithm and encoder values, go backwards
//Must reset encoders before using
void encodeBackward(){
    getPos();
    int offset = -right_pos + left_pos;
    if(offset > max_error)
        offset = max_error;
    else if (offset < -max_error)
        offset = -max_error;
    error = offset;
    pid();
}

//Using encoders to make sharp turns, requires a number of degrees to turn, and true if it is a right turn
void turn(int degrees, bool rightTurn){
    int rightTurnDist = degrees/3;
    int leftTurnDist = degrees/3;
    resetBothDist();
    getPos();

    if(rightTurn){    
        while(right_pos < rightTurnDist || left_pos < leftTurnDist){
            getPos();
            if(left_pos < leftTurnDist){
                pwmWrite(left_mf, turn_speed);
                pwmWrite(left_mb, 0);
            }
            else{
                pwmWrite(left_mb, 5000);
                pwmWrite(left_mf, 0);
            }
            if(right_pos < rightTurnDist){
                pwmWrite(right_mb, turn_speed);
                pwmWrite(right_mf, 0);
            }
            else{
                pwmWrite(right_mb, 0);
                pwmWrite(right_mf, 5000);
            }
    }}
    else{
        while(right_pos < rightTurnDist || left_pos < leftTurnDist){
            getPos();
            if(left_pos < leftTurnDist){
                pwmWrite(left_mb, turn_speed);
                pwmWrite(left_mf, 0);
            }
            else{
                pwmWrite(left_mf, 5000);
                pwmWrite(left_mb, 0);
            }
            if(right_pos < rightTurnDist){
                pwmWrite(right_mf, turn_speed);
                pwmWrite(right_mb, 0);
            }
            else{
                pwmWrite(right_mf, 0);
                pwmWrite(right_mb, 5000);
            }
        }   
    }

    end_moving();
}

//Go backwards a certain amount
//Requires clicks to know how much to reverse
void goBackwards(int clicks){
    resetBothDist();
    getPos();
    while(right_pos < clicks && left_pos < clicks){   //check reverse distances here!
            getPos();

            if(left_pos < clicks){
                pwmWrite(left_mb, encode_speed);
                pwmWrite(left_mf, 0);
            }
            else{
                pwmWrite(left_mb, 0);
                pwmWrite(left_mf, 1000);
            }
            if(right_pos < clicks){
                pwmWrite(right_mb, encode_speed);
                pwmWrite(right_mf, 0);
            }
            else{
                pwmWrite(right_mb, 0);
                pwmWrite(right_mf, 1000);
            }
        }
        end_moving();

}

//Go forwards a certain amount
//Requires clicks to know how far to go
void goForward(int clicks){
    resetBothDist();
    getPos();
    while(right_pos < clicks && left_pos < clicks){   //check reverse distances here!
            getPos();

            if(left_pos < clicks){
                pwmWrite(left_mf, encode_speed);
                pwmWrite(left_mb, 0);
            }
            else{
                pwmWrite(left_mf, 0);
                pwmWrite(left_mb, 1000);
            }
            if(right_pos < clicks){
                pwmWrite(right_mf, encode_speed);
                pwmWrite(right_mb, 0);
            }
            else{
                pwmWrite(right_mf, 0);
                pwmWrite(right_mb, 1000);
            }
        }
        

    }

//A function to follow an edge, this is specifically used to cross the floppy bridge with a 
//progressing motor power
//Requires clicks to know how far to travel
void edgeBump(int leftClicks){
    resetBothDist();
    reset_error();
    getPos();
    int bridgeSpeed = 23000;
    max_error = 65000;
    gain = 50;
    Kd = 75, Kp = 40, Ki = 0;

    initial_motor_speed = bridgeSpeed;
    while(left_pos < leftClicks){

        getPos();
        initial_motor_speed = bridgeSpeed + left_pos*40;
        if(analogRead(EDGE_DETECT)> EDGE_THRESHOLD){
            error = 10;
        }
        else{
            error = -2;
        }
        pid();

    }
}

//A function to follow an edge using PID
void edgeBumpNoClicks(){

    if(analogRead(EDGE_DETECT)> EDGE_THRESHOLD){
         error = 10;
        }
    else{
        error = -2;
        }
    pid();

    }

//A function to change constants after IRSignal from R2D2
void afterIR(){
    timeDelay = 120;
    max_PWM = 39000;
    reset_error();
    scan(15, IR);
    resetBothDist();
    state = ThirdEwok;
    firstRaise = true;
    initial_motor_speed = s_initial_motor_speed;
}

//A function to move claws on Stev-e after a certain distance in order to dodge some obstacles.
void claws(){
    if((right_pos > 100 || left_pos > 100 ) && firstRaise){
        firstRaise = false;
        end_moving();
        readyForTroopers();
        end_moving();
        scan(10, ThirdEwok);

        secondRaise = true;
    }

    else if((right_pos > 260 || left_pos > 260) && secondRaise){
        secondRaise = false;
        lookForEdge = true;
        end_moving();
        readyForThirdEwok();
        scan(10, ThirdEwok);
    }
    
}

//After third ewok grabbed, prepare for dump
void prepareDump() {

    //Align with edge
    end_moving();
    turn(6, true);
    end_moving();
    goBackwards(15);
    end_moving();
    delay(1);
    pwmWrite(left_mf, slow_speed);
    pwmWrite(right_mf, slow_speed);
    bool thisEdge = false;
    while(!thisEdge){
        if((analogRead(EDGE_DETECT) > EDGE_THRESHOLD)){
            thisEdge = true;
        }
    }

    //Turn about 90 degrees
    end_moving();
    firstGap(0);
    end_moving();
    thisEdge = false;
    resetBothDist();
    getPos();
    pwmWrite(right_mb, turn_speed);
    pwmWrite(left_mf, turn_speed - 1500);
    Serial.println("Turning");
    while(!thisEdge){
        getPos();
        if(right_pos > 64){
            thisEdge = true;
        }

    }

    //Go backwards
    end_moving();
    delay(10);
    pwmWrite(right_mf, slow_speed);
    pwmWrite(left_mb, slow_speed);
    delay(50);
    end_moving();
    delay(1000);
    end_moving();


    gain = 50;
    Kd = 75, Kp = 100, Ki = 0;
    initial_motor_speed = -encode_speed;
    end_moving();
    resetBothDist();
    reset_error();
    delay(motorDelay);

    //Use PId on encoders to go backwards and find wall to dump first three ewoks
    bool hitRightBumper = false;
    bool hitLeftBumper = false;
    bool hitBumper = false;
    int bonus = 0;
    while ((!hitLeftBumper) || !hitRightBumper){
        if(!hitLeftBumper && hitBumper){
            pwmWrite(left_mb, slow_speed + bonus);
            pwmWrite(left_mf, 0);
        }
        if(!hitRightBumper && hitBumper){
            pwmWrite(right_mb, slow_speed + bonus);
            pwmWrite(right_mf, 0);
        }

        if (!digitalRead(RIGHT_BUMPER)){
            pwmWrite(right_mf, 0);
            pwmWrite(right_mb, slow_speed);
            hitRightBumper = true;
            hitBumper = true;
            bonus = 8000;
        }
        if (!digitalRead(LEFT_BUMPER)){
            pwmWrite(left_mf, 0);
            pwmWrite(left_mb, slow_speed);
            
            hitLeftBumper = true;
            hitBumper = true;
            bonus = 8000;
        }
        if(!hitBumper){
            encodeBackward();
        }
    }
    end_moving();
}

//After dumping, find second second edge to cross gap
void findSecondEdge(){
    //Find first edge to follow
    goForward(4);
    end_moving();
    pwmWrite(right_mf, slow_speed);
    bool atEdge = false;
    while(!atEdge){
        if((analogRead(EDGE_DETECT) > EDGE_THRESHOLD)){
                atEdge = true;
            }

    }
    end_moving();

    //Follow this edge until tape is found
    resetBothDist();
    reset_error();
    bool notThere = true;
    initial_motor_speed = 23000;
    while(notThere){
        edgeBumpNoClicks();
        if((analogRead(MIDDLE_RIGHT_QRD) > EDGE_THRESHOLD)){
            notThere = false;
        }
    }
    end_moving();
    delay(1000);

    //Turn roughly 25 degrees
    turn(26, true);
    Serial.println("turning!");
    end_moving();

    //Use pid on encoders to go forward till edge is detected
    initial_motor_speed = encode_speed;
    notThere = false;
    getPos();
    while(!notThere){
       if((analogRead(EDGE_DETECT) > EDGE_THRESHOLD) && left_pos > 15)
            notThere = true;
        encodeForward();
    }
    end_moving();

    //Align with edge to prepare for plate drop
    firstGap(10);
    end_moving();
    goBackwards(1);
    end_moving();
    delay(10);

}

//Cross second gap to get to last ewok, and Chewy
void crossSecondPlate(){
    delay(1000);
    end_moving();
    resetBothDist();
    reset_error();
    initial_motor_speed = 62000;

    //Go forward to get on plate
    bool seeEdge = false;
    getPos();
    while(!seeEdge){
       if(left_pos > 10)
            seeEdge = true;
        encodeForward();
    }
    end_moving();
    resetBothDist();

    //Find tape on plate
    end_moving();
    pwmWrite(right_mf, 60000);
    bool atEdge = false;
    while(!atEdge){
        if((analogRead(EDGE_DETECT) > EDGE_THRESHOLD)){
                atEdge = true;
            }

    }
    end_moving();

    //Follow tape up the plate
    resetBothDist();
    reset_error();
    bool notThere = true;
    initial_motor_speed = 62000;
    while(notThere){
        edgeBumpNoClicks();
        if((analogRead(MIDDLE_RIGHT_QRD) > 3300)){
            notThere = false;
        }
    }
    end_moving();

    //Go foraward until off of plate
    initial_motor_speed = 63000;
    max_error = initial_motor_speed;
    notThere = false;
    reset_error();
    resetBothDist();
    getPos();
    while(!notThere){
       if(left_pos > 50)
            notThere = true;
        encodeForward();
    }

    //brake
    end_moving();
    delay(20);
    pwmWrite(left_mb, 25000);
    pwmWrite(right_mb, 25000);
    delay(50);
    end_moving();
    delay(100);

}

//A function to navigate the top section, then find and grab the last ewok
void getLastEwok(){
    
    //Turn specified amount
    end_moving();
    resetBothDist();
    getPos();
    pwmWrite(left_mf, slow_speed);
    //pwmWrite(right_mb, slow_speed-2000);
    bool notThere = true;
    while(notThere){
        getPos();
        if(left_pos > 18){
            notThere = false;
        }
    }
    end_moving();
    delay(500);
    resetBothDist();
    reset_error();
    // goForward(60);
    // pwmWrite(left_mf, slow_speed);
    // pwmWrite(left_mb, 0);
    // pwmWrite(right_mf, slow_speed);
    // pwmWrite(right_mb, 0);

    //Encode forward until edge found and ewok is grabbed
    initial_motor_speed = encode_speed;
    max_error = encode_speed;
    notThere = false;
    bool clawClosed = false;
    getPos();
    while(!notThere){
       if(analogRead(EDGE_DETECT) > EDGE_THRESHOLD  || !digitalRead(LEFT_CLAW))
            notThere = true;
        encodeForward();
        if(left_pos > 24 && !clawClosed){
            closeLeftClaw();
            clawClosed = true;
            end_moving();
            turn(10, true);
            end_moving();
            reset_error();
            resetBothDist();
        }
    }

    //Brake and pick up last ewok
    end_moving();
    pwmWrite(left_mb, 20000);
    pwmWrite(right_mb, 20000);
    delay(50);
    end_moving();
    delay(500);
    leftClawGrab();

}

//A function to find and cross the floppy bridge to get to Chewy
void crossFloppyBridge(){
    end_moving();
    bool offEdge = true;
    pwmWrite(left_mf, slow_speed);
    pwmWrite(right_mb, slow_speed - 3500);
    while(offEdge){
        if(analogRead(EDGE_DETECT) < EDGE_THRESHOLD)
            offEdge = false;
        }
    end_moving();
    delay(200);
    resetBothDist();
    reset_error();

    //Follow edge up bridge
    edgeBump(260);
    end_moving();
    
}

//A function to find and grab chewy
void grabChewy(){
    initial_motor_speed = 35000;
    max_error = initial_motor_speed;
    reset_error();
    resetBothDist();
     gain = 90;
    Kd = 75, Kp = 100, Ki = 0;

    //Exit floppy bridge
    bool overBridge = false;
    getPos();
    while(!overBridge){
       if(left_pos > 25)
            overBridge = true;
        encodeForward();
    }
    end_moving();
    delay(2000);


    //Turn right to align claw with chewy
    turn(14, true);
    end_moving();
    initial_motor_speed = encode_speed;
    max_error = initial_motor_speed;
    reset_error();
    resetBothDist();
     gain = 90;
    Kd = 75, Kp = 100, Ki = 0;
    bool atChewy = false;
    getPos();

    //Go forward till at chewy
    while(!atChewy){
       if(left_pos > 35)
            atChewy = true;
        encodeForward();
    }
    end_moving();
    delay(1000);
    leftClawGrabChewy();
    delay(1000);
}

//A function to raise the bucket and send chewy and last ewok back to safety
void sendThemHome(){
    bucketRaise();
    turn(7, true);
    goForward(15);
    bucketLower();
   // dance();
}
