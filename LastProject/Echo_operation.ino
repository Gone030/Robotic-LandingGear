int Echo_operation_decision() {
  Serial.println("----------Echo_operation_decision(in)----------");
  Echo_operation();

  if (Echo_mode == "explore") {//탐색모드
    for (int count = 0; count <= 3; count += 1) { //착륙 가능거리인지 판단.
      if (Echo_distance[count] == 0) { //초음파센서 작동불능 랜딩기어 오프상태로 전환. 조종자가 직접 내려야함. Set모드 유지.
        Serial.println("초음파센서 작동 불능");
        Echo_mode="inoperative";
        Servo_mode = "set";
        Servo_operation_decision();
        break;
      }
      Serial.print(count + 1); Serial.println("번 다리 초음파탐색모드 결과 검사");
      if (Echo_distance[count] <= Echo_ExploreDefaultValue && Echo_distance[count] >= Echo_landDefaultValue[count] + Echo_AdditonalDis) {
        Landing_Posibility[count] = 1;
      }
      else {
        Landing_Posibility[count] = 0;
      }

      if (Landing_Posibility[count] == 0) {
        Serial.print(count + 1); Serial.println("번 다리 착륙가능거리 벗어남");
      }
      else {
        Serial.print(count + 1); Serial.println("번 다리 착륙가능거리 들어옴");
      }
    }
    Echo_ExploreResult = 1000 * Landing_Posibility[3] + 100 * Landing_Posibility[2] + 10 * Landing_Posibility[1] + Landing_Posibility[0];
    //착륙가능여부 보냄 컨트롤러에서 버튼누르면 착륙 시작하도록 바꿔보자. 지금은 범위안에만 들어오면 바로 착륙시작함. 피에조도 사용하자.

    if (Compare(Landing_Posibility, Array_set_up)) {//착륙가능
      Serial.print("착륙가능 체크 완료, 착륙 가능성 : ");
      Serial.print(Landing_Posibility[0]); Serial.print(Landing_Posibility[1]);
      Serial.print(Landing_Posibility[2]); Serial.println(Landing_Posibility[3]);
      Serial.println("---------- Land Possible ----------");
      Echo_mode = "land";
      Servo_mode = "land";
      Serial.print(" 모드 바꿈(에코 : "); Serial.print(Echo_mode);
      Serial.print(", 서보 : "); Serial.print(Servo_mode); Serial.println(")");
      memcpy(Landing_Operation_LegNum, Array_set_up, 4 * sizeof(int));
    }

    if (Echo_mode == "land") {
      for (int count = 0; count <= 3; count += 1) {
        if (Echo_distance[count] > Echo_ExploreDefaultValue) {
          Servo_operation("set");
          Echo_mode = "explore";
          break;
        }
      }
    }
    Echo_Time_past = millis();
    Serial.println("----------Echo_operation_decision(out)----------");
  }
}
int Echo_operation() {
  Serial.println("----------Echo_operation(in)----------");
  for (int count = 0; count <= 3; count += 1) {
    digitalWrite(trig[count], HIGH);
    delayMicroseconds(10);//딜레이말고 다른거로 바꾸기.
    digitalWrite(trig[count], LOW);

    double duration = pulseIn(echo[count], HIGH);
    Echo_distance[count] = (((double)(340 * duration) / 1000) / 2);
    Serial.print(count + 1);  Serial.print("번 거리 : ");
    Serial.println(Echo_distance[count]);
  }
  Serial.println("----------Echo_operation(out)----------");
  return 0;
}
