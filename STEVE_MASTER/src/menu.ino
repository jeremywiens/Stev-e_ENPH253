//Convoluted function to control stev-e's menu system
void led_display(){

    if(!digitalRead(select_button)){
        new_screen = true;
        
        
        if(menu_level == 0 && var_selected == 0){
            load();
            
        }
        else if(var_selected == 0){
            menu_level = 0;
        }
        else if(menu_level == 0 && var_selected == 1){
            save();
        }
        else if( menu_level == 0 && var_selected == 2){
            menu_level = 1;
        }

        //When user presses resume to allow Stev-e to continue on course
        else if(menu_level == 0 && var_selected == 3){
            display.clearDisplay();
            display.setTextSize(2);
            display.setTextColor(WHITE);
            display.setCursor(0,0);
            if(state == TestInitial){
                display.println("STEV-initial");
            }
            else if(state == TestSecond){
                display.println("STEV-final");
            }else{
                display.println("STEV-EEEEE");
            }
            display.display();
            delay(1000);
            var_selected = 0;
            show_display = false;
            showing_display = false;
            new_screen = false;
        }

        //Navigate menu
        else if(menu_level == 0 && var_selected == 4){
        menu_level = 2;
        }
        else if(menu_level == 0 && var_selected == 5){
            menu_level = 3;
        }

        //Changes selected value for tape following via menu system
        else if (menu_level == 1){
            if (var_selected == 1){
                    changeVal(&Kp, "Kp", 0.5);
            }
            if(var_selected == 2){
                changeVal(&Ki, "Ki", 0.1);
            }
            if(var_selected == 3){
                changeVal(&Kd, "Kd", 0.5);
            }
            if(var_selected == 4){
                changeVal(&gain, "Gain", 0.2);
            }
            if(var_selected == 5){
                changeVal(&initial_motor_speed, "Speed", 100);
            }
            if(var_selected == 6){
                changeVal(&slow_speed, "slow", 100);
            }
            new_screen = true;
        }

        //Shows what the IR sensors for tape following are reading, updates every 100 milli seconds
        else if(menu_level == 2){
            if(var_selected == 1){
                {
           changing = true;
        while(changing){
            delay(200);
            changing = digitalRead(select_button);
            int  analog = analogRead(INITIAL_RIGHT_QRD);
            int  analog1 = analogRead(INITIAL_LEFT_QRD);
            int  analog3 = analogRead(MIDDLE_RIGHT_QRD);
            int  analog4 = analogRead(MIDDLE_LEFT_QRD);
            int  analog2 = analogRead(EDGE_DETECT);


            display.clearDisplay();
            display.setTextSize(1);
            display.setTextColor(WHITE);
            display.setCursor(0,0);
            display.print(analog);
            display.println(" I_R");//initial right
            display.print(analog1);
            display.println(" I_L");
            display.print(analog2);
            display.println(" Edge");//edge detector
            display.print(analog3);
            display.println(" M_R");
            display.print(analog4);
            display.println(" M_L");
            display.display();
            delay(100);
            if(!changing){
                 display.clearDisplay();
                display.setTextSize(2);
                display.setTextColor(WHITE);
                display.setCursor(0,0);
                display.println("I love you");
                display.display();
                delay(100);
            }
        }
            }
        } 
        //When user selects to test initial tape following
        else if(var_selected == 2){
            state = TestInitial;
            Kp = i_Kp, Kd = i_Kd, Ki = i_Ki, gain = i_gain;
            initial_motor_speed = i_initial_motor_speed;
            display.clearDisplay();
            display.setTextSize(2);
            display.setTextColor(WHITE);
            display.setCursor(0,0);
            display.println("STEV-initial");
            display.display();
            delay(1000);
            var_selected = 0;
            show_display = false;
            showing_display = false;
            new_screen = false;
        }

        //When user selects to test secondary tape following
        else if(var_selected == 3){
            state = TestSecond;
            Kp = s_Kp, Kd = s_Kd, Ki = s_Ki, gain = s_gain;
            initial_motor_speed = s_initial_motor_speed;
            display.clearDisplay();
            display.setTextSize(2);
            display.setTextColor(WHITE);
            display.setCursor(0,0);
            display.println("STEV-second");
            display.display();
            delay(1000);
            var_selected = 0;
            show_display = false;
            showing_display = false;
            new_screen = false;
        }

        //Allows user to test steve in different states for tape following
        else if(var_selected == 4){
            if(state == TestInitial){
                i_Kp = Kp, i_Kd = Kd, i_Ki = Ki, i_gain = gain;
                i_initial_motor_speed = initial_motor_speed;
            }
            else if(state == TestSecond){
                s_Kp = Kp, s_Kd = Kd, s_Ki = Ki, s_gain = gain;
                s_initial_motor_speed = initial_motor_speed;
            }
            menu_level = 0;
            state = Check;
        
        }
        else if(var_selected == 5){
            
            delay(75);
            encoder_readings();
        }
        }

        //Controls menu for changing states to set Stev-e in any starting point
        else if (menu_level == 3){
            delay(200);
            menu_level = 0;
            if(var_selected == 1){
                changeState(IR);
            }
            else if(var_selected == 2){
                changeState(Dump);
            }
            else if(var_selected == 3){
                changeState(FirstEwok);
            }
            else if(var_selected == 4){
                changeState(ThirdEwok);
            }
        }
        var_selected = 0;
        delay(50);
    }

    //Moves cursor up
    else if(!digitalRead(up_button)){
        var_selected--;
        new_screen = true;
        delay(20);
    }

    //Moves cursos down
    else if(!digitalRead(down_button)){
        var_selected++;
        new_screen = true;
        delay(20);
    }

    //Ensures cursor does not exceed limits of menu
    if(var_selected < 0){
        var_selected = 0;
    }
    else if(var_selected >= num_menu_items)
        var_selected = num_menu_items - 1;

    //If button has been pressed, new screen will be displayed
  if(new_screen){
    int cursor = 0;
    display.clearDisplay();


    //Displays the menu and gives user different options
    switch(menu_level) {
        case 0:
            num_menu_items = 6;
            display.setTextSize(1);
            display.setTextColor(WHITE);
            display.setCursor(6,0);
            display.println("Load");
            display.setCursor(6,10);
            display.println("Save");
            display.setCursor(6,20);
            display.println("Tape");
            display.setCursor(6,30);
            display.println("Resume");
            display.setCursor(6,40);
            display.println("Testing");
            display.setCursor(6,50);
            display.println("Change State");
            cursor = var_selected*10;
            display.setCursor(0,cursor);
            display.println("*");
            display.display();
            break;

        case 1:
            num_menu_items = 7;
            display.setTextSize(1);
            display.setTextColor(WHITE);
            display.setCursor(7,0);
            display.println("<- back");
            display.setCursor(6,10);
            display.print("Kp = ");
            display.println(Kp);
            display.setCursor(6,20);
            display.print("Ki = ");
            display.println(Ki);
            display.setCursor(6,30);
            display.print("Kd = ");
            display.println(Kd);
            display.setCursor(6,40);
            display.print("Gain = ");
            display.println(gain);
            display.setCursor(6,50);
            display.print("speed = ");
            display.println(initial_motor_speed);
            display.setCursor(6,58);
            display.print("slow = ");
            display.println(slow_speed);
            cursor = var_selected*10;
            display.setCursor(0,cursor);
            display.println("*");
            display.display();
            break;
        case 2:
            num_menu_items = 6;
            display.setTextSize(1);
            display.setTextColor(WHITE);
            display.setCursor(7,0);
            display.println("<- back");
            display.setCursor(6,10);
            display.println("QRD Testing");
            display.setCursor(6,20);
            display.println("Initial Tape");
            display.setCursor(6,30);
            display.println("IR Testing");
            display.setCursor(6, 40);
            display.println("Resume");
            display.setCursor(6, 50);
            display.println("Encoders");
            cursor = var_selected*10;
            display.setCursor(0,cursor);
            display.println("*");
            display.display();
            break;
        case 3:
            num_menu_items = 5;
            display.setTextSize(1);
            display.setTextColor(WHITE);
            display.setCursor(7,0);
            display.println("<- back");
            display.setCursor(6,10);
            display.println("IR");
            display.setCursor(6,20);
            display.println("Dump");
            display.setCursor(6,30);
            display.println("FirstEwok");
            display.setCursor(6, 40);
            display.println("ThirdEwok");
            cursor = var_selected*10;
            display.setCursor(0,cursor);
            display.println("*");
            display.display();
            break;

        default:
            display.clearDisplay();
            break;
    }
   
    new_screen = false;
}
}

//Change values for tape following within the menu system
void changeVal(float *val, char name[],float change){
    float thisVal = *val;
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.print(name);
    display.print(" = ");
    display.println(thisVal);
    display.display();
    delay(100);
    changing = true;
    while(changing){
        changing = digitalRead(select_button);
        if(!changing){
            delay(50);
        }
        if(!digitalRead(up_button)){
            thisVal = thisVal + change;
            new_screen = true;
            delay(10);
         }
        else if(!digitalRead(down_button)){
            thisVal =  thisVal - change;
            new_screen = true;
            delay(10);
        }
        if(new_screen){
            display.clearDisplay();
            display.setTextSize(1);
            display.setTextColor(WHITE);
            display.setCursor(0,0);
            display.print(name);
            display.print(" = ");
            display.println(thisVal);
            display.display();
            new_screen = false;
        }
    }
    delay(50);
    *val = thisVal;

}

//Save values for tape following to EEPROM on bluepill
void save(){
    if(EEPROM.read(0) != 65535 || true){
          display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(0,0);
        display.println("Replace old values?");
        num_menu_items = 2;
         display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(6,10);
        display.println("no");
        display.setCursor(6,20);
        display.println("yes");
        var_selected = 1;
        int cursor = var_selected*10;
        display.setCursor(0,cursor);
        display.println("*");
        display.display();
        delay(200);
        changing = true;
        
        while(changing){

            new_screen = false;
            changing = digitalRead(select_button);
             if(!changing){
                new_screen = false;
                if(var_selected == 1){
                     display.clearDisplay();
                display.setTextSize(1);
                display.setTextColor(WHITE);
                display.setCursor(0,0);
                display.println("Not saving..");
                display.display();

                }

                //Saves current values if user selects to erase old values saved
                else if (var_selected == 2){
                    display.clearDisplay();
                    display.setTextSize(1);
                    display.setTextColor(WHITE);
                    display.setCursor(0,0);
                    display.println("Saving..");
                    float preKp = i_Kp*10;
                    int KpInt = preKp; 
                    EEPROM.write(0, KpInt);
                    float preKd = i_Kd*10;
                    int KdInt = preKd;
                    EEPROM.write(1, KdInt);
                    float preKi = i_Ki*10;
                    int KiInt = preKi;
                    EEPROM.write(2, KiInt);
                    float pregain = i_gain*10;
                    int gainInt = pregain;
                    EEPROM.write(3, gainInt);
                    EEPROM.write(4,i_initial_motor_speed);
                   

                    preKp = s_Kp*10;
                     KpInt = preKp; 
                    EEPROM.write(5, KpInt);
                    preKd = s_Kd*10;
                     KdInt = preKd;
                    EEPROM.write(6, KdInt);
                     preKi = s_Ki*10;
                     KiInt = preKi;
                    EEPROM.write(7, KiInt);
                     pregain = s_gain*10;
                     gainInt = pregain;
                    EEPROM.write(8, gainInt);
                    EEPROM.write(9,s_initial_motor_speed);
                    EEPROM.write(10, slow_speed);
                   // display.println(KiInt);
                    display.display();

                }
                delay(500);
            }
            else if(!digitalRead(up_button)){
                var_selected--;
                new_screen = true;
                delay(20);
            }
            else if(!digitalRead(down_button)){
                    var_selected++;
                    new_screen = true;
                    delay(20);
            }
            if(var_selected < 1)
                var_selected = 1;
            else if(var_selected > 2)
                var_selected = 2;
            if(new_screen){
                 display.clearDisplay();
                display.setTextSize(1);
                display.setTextColor(WHITE);
                display.setCursor(0,0);
                display.println("Replace old values?");
                num_menu_items = 2;
                display.setTextSize(1);
                display.setTextColor(WHITE);
                display.setCursor(6,10);
                display.println("no");
                display.setCursor(6,20);
                display.println("yes");
                cursor = var_selected*10;
                display.setCursor(0,cursor);
                display.println("*");
                display.display();
              }
        }

    }

}


//Load values from EEPROM to set as tape following values
void load(){
    //Check to see that saved values exist on device
    if(EEPROM.read(0) == 65535){
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(0,0);
        display.print("Error: No values are saved.");
        display.display();
        changing = true;
        while(changing){
            changing = digitalRead(select_button);
            if(!changing){
                delay(50);
            }
        }

    }

    //If so confirm that old values should be loaded
    else{
       display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(0,0);
        display.println("Load values?");
        num_menu_items = 2;
         display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(6,10);
        display.println("no");
        display.setCursor(6,20);
        display.println("yes");
        var_selected = 1;
        int cursor = var_selected*10;
        display.setCursor(0,cursor);
        display.println("*");
        display.display();
        delay(200);
        changing = true;
        
        while(changing){

            new_screen = false;
            changing = digitalRead(select_button);
             if(!changing){
                new_screen = false;
                if(var_selected == 1){
                display.clearDisplay();
                display.setTextSize(1);
                display.setTextColor(WHITE);
                display.setCursor(0,0);
                display.println("Not loading..");
                display.display();

                }

                //Loads old values
                else if (var_selected == 2){
                    display.clearDisplay();
                    display.setTextSize(1);
                    display.setTextColor(WHITE);
                    display.setCursor(0,0);
                    display.println("loading..");
                    display.display();
                    float preKp = EEPROM.read(0);
                    i_Kp = preKp/10;
                    float preKd = EEPROM.read(1);
                    i_Kd = preKd/10;
                    float preKi = EEPROM.read(2);
                    i_Ki = preKi/10;
                    float pregain = EEPROM.read(3);
                    i_gain = pregain/10;
                    i_initial_motor_speed = EEPROM.read(4);
                     preKp = EEPROM.read(5);
                    s_Kp = preKp/10;
                     preKd = EEPROM.read(6);
                    s_Kd = preKd/10;
                     preKi = EEPROM.read(7);
                    s_Ki = preKi/10;
                     pregain = EEPROM.read(8);
                    s_gain = pregain/10;
                    s_initial_motor_speed = EEPROM.read(9);
                    slow_speed = EEPROM.read(10);
                    delay(50);
                   // display.println(KiInt);
                    display.display();

                }
                delay(200);
            }
            else if(!digitalRead(up_button)){
                var_selected--;
                new_screen = true;
                delay(20);
            }
            else if(!digitalRead(down_button)){
                    var_selected++;
                    new_screen = true;
                    delay(20);
            }
            if(var_selected < 1)
                var_selected = 1;
            else if(var_selected > 2)
                var_selected = 2;
            if(new_screen){
                 display.clearDisplay();
                display.setTextSize(1);
                display.setTextColor(WHITE);
                display.setCursor(0,0);
                display.println("Load values?");
                num_menu_items = 2;
                display.setTextSize(1);
                display.setTextColor(WHITE);
                display.setCursor(6,10);
                display.println("no");
                display.setCursor(6,20);
                display.println("yes");
                cursor = var_selected*10;
                display.setCursor(0,cursor);
                display.println("*");
                display.display();
              }
        }
    }
}

//Test encoder values and see that the motors have similar speeds at equal power
void encoder_readings(){
    resetBothDist();
    changing = true;
    end_moving();
    float Speeed;
    int RSpeeeed;
    float preSped;
    int LSpeeeed;
    pwmWrite(right_mf, 35000);
    pwmWrite(left_mf, 35000);
        while(changing){
            resetBothDist();
            delay(1000);
            getPos();
            preSped = right_pos;
            Speeed = 1/preSped*1000;
            RSpeeeed = Speeed; 
            preSped = left_pos;
            Speeed = 1/preSped*1000; 
            LSpeeeed = Speeed;

            

            changing = digitalRead(select_button);
            display.clearDisplay();
            display.setTextSize(1);
            display.setTextColor(WHITE);
            display.setCursor(0,0);
            display.print("period = ");
            display.println(RSpeeeed);
            display.print("Right = ");
            display.println(right_pos);
            display.print("period = ");
            display.println(LSpeeeed);
            display.print("Right = ");
            display.println(left_pos);
            display.display();


}

end_moving();
}

//Initialize LED screen once on start up
void led_init(){
            // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  // init done
  
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("STEV-EEEEE");
  display.display();
  delay(50);
  new_screen = true;

}

//Allows you to select state from the menu system. Necessary for initialization
void changeState(States stat){
  resetBothDist();
  reset_error();
  switch(stat){
    case FirstEwok:
        state = FirstEwok;
        Kp = i_Kp, Kd = i_Kd, Ki = i_Ki, gain = i_gain;
        initial_motor_speed = i_initial_motor_speed; 
        slow_speed = 27500;
        encode_speed = 20000;
        turn_speed = 25000;  
        max_PWM = 65500;
        timeDelay = 1000;
        break;
    case FirstGap:
        state = FirstGap;
        break;

    case Stop:
        reset_error();
        max_PWM = 50000;
        delay(2000);
       bucketRaise();
       delay(5000);
       bucketLower();
        state = Stop;

        end_moving();
        break;

    case ThirdEwok:
      firstRaise = true;
      state = ThirdEwok;
      timeDelay = 100;
      max_PWM = 39000;
      break;

    case Dump:
      state = Dump;
      timeDelay = 120;
      max_PWM = 39000;
      reset_error();
      resetBothDist();
      dropPlate();
      readyForThirdEwok();
      break;

    case IR:
      state = IR;
      rightClawGrab2();
      delay(10);
      resetBothDist();
      break;

  }
}


