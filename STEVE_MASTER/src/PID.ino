void pid(){
    calculate_pid();
    motor_control();
}

//Requires error to have been caclulated, and Kp, Kd, and Ki to have been tuned
//for appropriate action.
  void calculate_pid() {

  P = error;
  I = I + error;
  D = error - previous_error;

  PID_value = gain*((Kp * P) + (Ki * I) + (Kd * D));

  previous_error = error;
}

void motor_control() {

  // Calculating the effective motor speed:
  int left_motor_speed = initial_motor_speed + PID_value;
  int right_motor_speed = initial_motor_speed - PID_value;
  bool l_forward = true;
  bool r_forward = true; 

  if(left_motor_speed < 0){
      left_motor_speed = -left_motor_speed;
      l_forward = false;
  }
  if (left_motor_speed > max_PWM){
    left_motor_speed = max_PWM;
  }
  if(right_motor_speed < 0){
    right_motor_speed = -right_motor_speed;
    r_forward = false;
  }
  if (right_motor_speed > max_PWM){
    right_motor_speed = max_PWM;
  }

  //give motors their speed
    if(l_forward == true){
        pwmWrite(left_mf, left_motor_speed);
        pwmWrite(left_mb, 0);
    }
    else{
        pwmWrite(left_mb, left_motor_speed);
        pwmWrite(left_mf, 0);
        
    }
    if(r_forward == true){
        pwmWrite(right_mf, right_motor_speed);
        pwmWrite(right_mb, 0);
    }
    else{
        pwmWrite(right_mb, right_motor_speed);
        pwmWrite(right_mf, 0);
    }
}