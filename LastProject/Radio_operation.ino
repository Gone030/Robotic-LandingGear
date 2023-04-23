int Radio_operation_decision() {
  if (radio.available()) {
    Serial.println("시작");
    Radio_operation("recieve", 0);
  }
  if (Landing_Button == "off") {
    if (Radio_DataRecieve == Radio_On) {
      Serial.println("온신호받음");
      //온 신호 받음
      Landing_Button = "on";
      Piezo_operation();
      Radio_operation("send", Radio_On);
      Radio_DataRecieve=0;
    }
  }
  else if (Landing_Button == "on") {
    if (Echo_mode == "explore") {
      //탐색모드 신호 보내기.
      Serial.println("탐색모드 신호 보냄");
      Radio_operation("send", Echo_ExploreResult);
    }
    else if (Echo_mode == "inoperative") {
      //초음파센서 작동불능
      Radio_operation("send", Radio_EchoInoperative);
      Serial.println("초음파센서 작동 불능 신호 보냄");
      Landing_Button == "off";
    }
    else if (Landing_result == Radio_LandingSuccess) {
      //착륙 성공
      Serial.println("착륙성공 신호 보냄");
      Radio_operation("send", Radio_LandingSuccess);
      Landing_Button = "off";
      Radio_operation("send", Radio_Off);
      Radio_DataRecieve=0;
    }
    else if (Landing_result == Radio_LandingFail) {
      //착륙 실패
      Serial.println("착륙실패 신호 보냄");
      Radio_operation("send", Radio_LandingFail);
      Radio_DataRecieve=0;
    }

  }

  Radio_Time_past = millis();

  return 0;
}

int Radio_operation(String mode, int data) {
  if (mode == "recieve") {
    Radio_DataRecieve = 0;
    radio.read(&Radio_DataRecieve, sizeof(Radio_DataRecieve));
    Serial.print("recieved data : ");
    Serial.println(Radio_DataRecieve);
  }

  else if (mode == "send") {
    radio.openWritingPipe(address[1]); //송신 주소로 변환
    radio.stopListening();//송신모드

    Serial.print("send data : ");
    Serial.println(data);

    for (int count = 1; count <= 10; count++) { //10번 보내기
      radio.write(&data, sizeof(data));
    }

    radio.openReadingPipe(1, address[0]); //수신 주소로 변환
    radio.startListening();//다시 수신모드로.
    Radio_Mode = "recieve";

  }

  else {
    Serial.print("Check Radio_mode error : ");
    Serial.println(Radio_Mode);
    Radio_Mode = "recieve";
  }
  return 0;
}
