//Test the initial set of IR sensors for tape following.
void test_initial(){
    timeIs = millis();
    if(timeIs > (lastTime + timeDelay)){
        getEncoder(FirstEwok);

    }
    sensors(FirstEwok);
    pid();
}

//Test the second set of IR sensors for tape following
void test_second(){
        timeIs = millis();
    if(timeIs > (lastTime + timeDelay)){
        getEncoder(ThirdEwok);

    }
    sensors(ThirdEwok);
    
    pid();
}

void getEncoder(States stat){
            getPos();
//             display.clearDisplay();
//             display.setTextSize(1);
//             display.setTextColor(WHITE);
//             display.setCursor(0,0);
//             if (stat == ThirdEwok){        
//                 display.println("STEV-second");
// }
//             else{
//                 display.println("STEV-initial");

//             }
//             display.print("Right = ");
//             display.println(right_pos);
//             display.print("Left = ");
//             display.println(left_pos);
//             display.display();
            lastTime = millis();
}