int Servo_operation_decision() {
  Serial.println("----------Servo_operation_decision(start)----------");
  Serial.print("Landing_Operation_LegNum : ");
  Serial.print(Landing_Operation_LegNum[0]); Serial.print(Landing_Operation_LegNum[1]);
  Serial.print(Landing_Operation_LegNum[2]); Serial.println(Landing_Operation_LegNum[3]);
  Serial.print("작동 전 angle : ");
  Serial.print(Servo_angle[0]); Serial.print(Servo_angle[1]);
  Serial.print(Servo_angle[2]); Serial.println(Servo_angle[3]);
  Serial.print("Servo_mode : "); Serial.println(Servo_mode);
  Serial.print("Echo_mode : "); Serial.println(Echo_mode);


  if (Echo_mode == "land" && Servo_mode == "land") {
    Servo_operation(Servo_mode);
    
    for (int count = 0; count <= 3; count += 1) {
      (digitalRead(Button_leg[count]) == LOW) ? Button_leg_error[count] = 1 : Button_leg_error[count] = 0;
      Landing_Operation_LegNum[count] = 1 - (Servo_error[count] || Button_leg_error[count]);
    }

    if (Compare(Landing_Operation_LegNum, Array_set_down) && Compare(Button_leg_error, Array_set_up)) { //착륙 성공. 성공내용 전송
      Landing_result = Radio_LandingSuccess;
    }
    else if (Compare(Landing_Operation_LegNum, Array_set_down) && ~(Compare(Button_leg_error, Array_set_up))) {//착륙 실패. 실패내용 전송
      Landing_result = Radio_LandingFail;
    }


    //    if (Compare(Landing_Operation_LegNum, Array_set_down)) {
    //      memcpy(Landing_Operation_LegNum, Array_set_up, 4 * sizeof(int));
    //      Servo_mode = "little";
    //      Serial.println("Little Change");
    //    }
  }

  else {
    Serial.println("서보모터 작동안함");
  }

  Servo_Time_past = millis();
  Serial.println("----------Servo_operation_decision(finish)----------");
  return 0;
}


int Servo_operation(String mode) {
  //14=1cm=7.193도=0.125pi,  250~500
  Serial.println("----------Servo_operation(start)----------");
  //원하는 다리만 모두 내림.
  if (mode == "set") {
    for (int Servo_count = 0; Servo_count <= 7; Servo_count += 1) {
      pwm.setPWM(Servo_count, 0, 250);
    }
    Serial.println("랜딩기어 초기 세팅완료");
  }

  else {
    Serial.println("------Servo_operation(little/land mode in)------");
    for (int count = 0; count <= 3; count += 1) {
      if ( Landing_Operation_LegNum[count] == 1 && count == 0) {
        Servo_angle[count] += Servo_AdditionalAngle;
        if (Servo_angle[count] >= 500) {
          Servo_angle[count] = 500; Servo_error[count] = 1;
          Serial.print(count + 1); Serial.println("번 다리 다펴짐(랜드모드)--->착륙불가능");
        }
        pwm.setPWM(0, 0, Servo_angle[count]); pwm.setPWM(1, 0, Servo_angle[count]);
      }
      else if ( Landing_Operation_LegNum[count] == 1 && count == 1) {
        Servo_angle[count] += Servo_AdditionalAngle;
        if (Servo_angle[count] >= 500) {
          Servo_angle[count] = 500; Servo_error[count] = 1;
          Serial.print(count + 1); Serial.println("번 다리 다펴짐(랜드모드)--->착륙불가능");
        }
        pwm.setPWM(2, 0, Servo_angle[count]); pwm.setPWM(3, 0, Servo_angle[count]);
      }
      else if ( Landing_Operation_LegNum[count] == 1 && count == 2) {
        Servo_angle[count] += Servo_AdditionalAngle;
        if (Servo_angle[count] >= 500) {
          Servo_angle[count] = 500; Servo_error[count] = 1;
          Serial.print(count + 1); Serial.println("번 다리 다펴짐(랜드모드)--->착륙불가능");
        }
        pwm.setPWM(4, 0, Servo_angle[count]); pwm.setPWM(5, 0, Servo_angle[count]);
      }
      else if ( Landing_Operation_LegNum[count] == 1 && count == 3) {
        Servo_angle[count] += Servo_AdditionalAngle;
        if (Servo_angle[count] >= 500) {
          Servo_angle[count] = 500; Servo_error[count] = 1;
          Serial.print(count + 1); Serial.println("번 다리 다펴짐(랜드모드)--->착륙불가능");
        }
        pwm.setPWM(6, 0, Servo_angle[count]); pwm.setPWM(7, 0, Servo_angle[count]);
      }
    }
    Serial.println("------Servo_operation(little/land mode out)------");
  }
  Serial.println("----------Servo_operation(out)----------");
  return 0;
}
