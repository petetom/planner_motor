



void test_motor() {
      //motor test

    digitalWrite(MOTOR_EN,HIGH);
    analogWrite(MOTOR_PWM, 127);
    delay (5000);
    analogWrite(MOTOR_PWM,0);
    delay(2000);

    digitalWrite(MOTOR_DIR,HIGH);
    analogWrite(MOTOR_PWM,127);
    delay(5000);
    analogWrite(MOTOR_PWM,0);

    digitalWrite(MOTOR_EN,LOW);

}

void move_up()
{
    digitalWrite(MOTOR_EN,HIGH);
    digitalWrite(MOTOR_DIR,HIGH); //
    analogWrite(MOTOR_PWM,95);
    mot_en = true;
    sw_time = millis();
    //Serial.println("move up enable");
  
}

void move_down()
{
    digitalWrite(MOTOR_EN,HIGH);
    digitalWrite(MOTOR_DIR,LOW); //
    analogWrite(MOTOR_PWM,80);
    mot_en = true;
    sw_time = millis();
   // Serial.println("move down enable");
}

void update_position()
{
  float current_height = height_in;
  
  //move down 0.1" then back to potition
    digitalWrite(MOTOR_EN,HIGH);
    digitalWrite(MOTOR_DIR,LOW); //
    analogWrite(MOTOR_PWM,70);
    while(height_in < (current_height + 0.05))
    { 
      sensor_cnt = read_5600();
      height_in =(eeprom_correct - sensor_cnt) * CNT + 0.75;
      update_display();
    }
    // planner height should have dropped 0.05"
    
    analogWrite(MOTOR_PWM,0);
    digitalWrite(MOTOR_DIR,HIGH);
    delay(1000);
    analogWrite(MOTOR_PWM,85);
    while(height_in > current_height + 0.004)
    {
      sensor_cnt = read_5600();
      height_in =(eeprom_correct - sensor_cnt) * CNT + 0.75;
      update_display();
    }
    analogWrite(MOTOR_PWM,0);
    digitalWrite(MOTOR_EN,LOW); //disbale motor
}


void move_to_position() {
// function to control planner height, if the new height is smaller than
// the current postion then just move up to position.  If the new height
// is larger than then the current position then lower past the set 
// height by 0.1" and then move back to set position.

  //Serial.println("cal go");

  if (planner_set < height_in)  //then we move up
  {
    digitalWrite(MOTOR_EN,HIGH);
    digitalWrite(MOTOR_DIR,HIGH); //
    analogWrite(MOTOR_PWM,60);
    while(height_in > planner_set + 0.01)
    { sensor_cnt = read_5600();
      height_in =(eeprom_correct - sensor_cnt) * CNT + 0.75;
      update_display();
    }
  }
  else  // we need to move down and then come back up
  {
    digitalWrite(MOTOR_EN,HIGH);
    digitalWrite(MOTOR_DIR,LOW); //
    analogWrite(MOTOR_PWM,50);
    while(height_in < planner_set + 0.05)
    {
      sensor_cnt = read_5600();
      height_in =(eeprom_correct - sensor_cnt) * CNT + 0.75;
      update_display();
    }
    analogWrite(MOTOR_PWM,0);
    digitalWrite(MOTOR_DIR,HIGH);
    delay(1000);
    analogWrite(MOTOR_PWM,30);
    while(height_in > planner_set + 0.004)
    {
      sensor_cnt = read_5600();
      height_in =(eeprom_correct - sensor_cnt) * CNT + 0.75;
      update_display();
    }
  }
  analogWrite(MOTOR_PWM,0);
  digitalWrite(MOTOR_EN,LOW);
  
//  Serial.println("cal done");
}
