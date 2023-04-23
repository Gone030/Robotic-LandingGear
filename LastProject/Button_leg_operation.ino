int Button_leg_operation() {
  for (int count = 0; count <= 3; count += 1) {//압력센서 판단.
    Serial.println("버튼 감지");
    (digitalRead(Button_leg[count]) == LOW) ? Button_leg_error[count] = 1 : Button_leg_error[count] = 0;
  }
  return 0;
}
