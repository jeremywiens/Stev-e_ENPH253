void encoderSetup(){
  pinMode(ENCODE_LEFT_WHEEL_A, INPUT_PULLUP);
  pinMode(ENCODE_RIGHT_WHEEL_A, INPUT_PULLUP);
  attachInterrupt(ENCODE_LEFT_WHEEL_A, left_interrupt, CHANGE);
  attachInterrupt(ENCODE_RIGHT_WHEEL_A, right_interrupt, CHANGE);

}

void right_interrupt()
{ 
    right_count++;
    if (right_count == 4){
        right_pos++;
        right_count = 0;
    }
}
void left_interrupt()
{
    left_count++;
    if (left_count == 4){
        left_pos++;
        left_count = 0;
    }
}

void sendEncoderVals(){
    byte rightArray[4];
    byte leftArray[4];
    rightArray[0] = (right_pos >> 24) & 0xFF;
    rightArray[1] = (right_pos >> 16) & 0xFF;
    rightArray[2] = (right_pos>> 8) & 0xFF;
    rightArray[3] = right_pos & 0xFF;
    leftArray[0] = (left_pos >> 24) & 0xFF;
    leftArray[1] = (left_pos >> 16) & 0xFF;
    leftArray[2] = (left_pos>> 8) & 0xFF;
    leftArray[3] = left_pos & 0xFF;
    Wire1.write(leftArray, 4);
    Wire1.write(rightArray, 4);
}

void resetEncoderVals(){
    right_pos = 0;
    right_count = 0;
    left_pos = 0;
    left_count = 0;
}