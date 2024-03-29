# 비평탄 지형에서 드론의 안전한 이・착륙을 위한 가변형 랜딩기어의 설계

## 프로젝트 개요
해당 프로젝트는 평지에서만 안전한 착륙을 할 수 있다는 드론의 문제점을 해결하고자 고안되었으며, 동아리 개별 팀 프로젝트에서부터 이후 결점을 보완해 캡스톤 디자인으로 이어진 프로젝트입니다. 경사면, 울퉁불퉁한 지형 등과 같은 비평탄 지형에서의 착륙을 도와주게 설계되었으며 추가적으로 부표와 같은 착지면의 기울기가 계속 변화하는 상황에서의 안정성을 유지하기 위한 기능도 포함하고 있습니다. 이 프로젝트는 이전에 진행한 프로젝트에서 아쉬운 점을 보완함으로써 진행하였습니다. last project 폴더를 통해 이전 프로젝트의 코드를 확인할 수 있으며, 두 프로젝트 전부 Arduino로 개발했습니다.

### 이전 프로젝트 (2020.08 ~ 2020.12)

#### 이전 프로젝트 소개
![prototype_landing](https://user-images.githubusercontent.com/89852937/235650397-8b466f87-506f-48be-8b01-dbc047d163e8.gif)

랜딩기어의 각 다리에 부착된 초음파센서를 통해 각 다리가 닿는 착지면과의 거리를 측정해 특정 높이에서 스위치를 누르면 다리를 하강시키고, 다리의 끝에 부착된 압력센서가 인식되면 하강을 멈추고 동작을 정지합니다.

#### 이전 프로젝트의 부족한 점
1. 착륙 진행 중에 불필요한 프로세스가 많다.
    * 특정 높이에서 별도의 스위치를 눌러야 동작하는 점에서 조종자가 특정 높이를 기억해야하고 별도의 동작을 취해야 한다는 점에서 불편함이 발생합니다.
2. 착륙 진행이 매끄럽지 않고 오래 걸린다.
    * 착륙 진행 중 호버링이 유지되어야 하는데, 바람 혹은 조종미숙으로 기존의 측정한 착지면에서 벗어날 경우를 위해 초음파 센서로 거리 측정을 다리가 어느정도 하강했을 때 다시 측정하는 일련의 동작이 오히려 착륙에 대한 난이도를 높히게 되었습니다.

## 캡스톤 프로젝트 (2021.01 ~ 2021.06)
![tilt_landing_resize](https://user-images.githubusercontent.com/89852937/235647764-d0a3ae13-567f-43f8-ad7c-32a8da2b6881.gif)

랜딩기어의 중심부에 부착된 초음파 센서로 착지면과의 거리가 가까워짐을 감지해 랜딩기어를 동작하여 착륙을 시작합니다.
각 다리의 서보모터에 전류감지센서를 통해 서보모터의 부하가 증가함을 통해 지면에 닿음을 감지합니다. 이후 기울기센서를 통해 자세를 교정합니다.

## 개선점 및 추가된 점
1. 착륙 프로세스 간소화
![개선점1](https://user-images.githubusercontent.com/89852937/234542587-564f69e1-8a43-4bb6-ad17-0c1b1a1c5fca.png)

  * 기존의 착륙 프로세스에서 스위치 동작과 착지면과의 거리 측정과 하강 동작의 루프를 수정해 동작을 간소화시켰습니다. 각 동작 별로 별도의 루프를 갖도록 해 한 동작이 끝나야 다음 동작을 수행하도록 했습니다. 이로써 착륙 소요시간을 대폭 줄이고 조종자가 받는 착륙에 대한 부담을 완화시켰습니다.

2. 기울기 센서로 자세교정

<img src="https://user-images.githubusercontent.com/89852937/235606596-abc2d19f-7148-48c3-97bc-4806bfdd6cb3.gif" width="300" height="400"> <img src="https://user-images.githubusercontent.com/89852937/235607995-050fcc24-d49d-4b1b-851a-0de93d1086bb.png" width="300" height="400">

  * 기울기센서를 사용해 9개의 상태로 구분하고, 기울어짐에 따라 수평상태가 될 때 까지 서보모터의 각도를 조절해 자세를 교정합니다.

----------

## 사용 부품
* __MCU__ : Arduino Nano
* __Servo Driver__ : PCA9685
* __Servo Motor__ : MG996R (8 pcs)
* __IMU__ : MPU6050
* __Current Sensor__ : INA219 (4 pcs)

---------

