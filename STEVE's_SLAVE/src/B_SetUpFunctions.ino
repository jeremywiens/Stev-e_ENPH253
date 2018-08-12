void rightClawArmSetup(){
  right_claw_servo.attach(RIGHT_CLAW_SERVO);
  right_arm_servo.attach(RIGHT_ARM_SERVO);
  right_claw_servo.write(rightClawOpen);
  right_arm_servo.write(rightArmDown);
}

void leftClawArmSetup() {
  left_claw_servo.attach(LEFT_CLAW_SERVO);
  left_arm_servo.attach(LEFT_ARM_SERVO);
  left_claw_servo.write(leftClawOpen);
  left_arm_servo.write(leftArmDown);
}

void bucketRaiseSetup(){
  pinMode(BUCKET_RAISE, PWM);
  pinMode(BUCKET_LOWER, PWM);
  pwmWrite(BUCKET_LOWER, 0);
  pwmWrite(BUCKET_RAISE, 0);
  pinMode(BUCKET_LIMIT, INPUT_PULLUP);
}

void firstPlateDropSetup() {
  first_plate_drop_servo.attach(PLATE_DROP_ONE_SERVO);
  first_plate_drop_servo.write(min_plate_drop_one_pos);
}

void secondPlateDropSetup() {
  second_plate_drop_servo.attach(PLATE_DROP_TWO_SERVO);
  second_plate_drop_servo.write(min_plate_drop_two_pos);
}

void bucketSetup() {
  bucket_servo.attach(BUCKET_SERVO);
  bucket_servo.write(bucket_up);
  bucket_servo_2.attach(BUCKET_SERVO_2);
  bucket_servo_2.write(bucket_dumpable);
}

void IICSetup(){
  Wire1.begin(4);
  Wire1.onReceive(receiveEvent);
  Wire1.onRequest(requestEvent);
}