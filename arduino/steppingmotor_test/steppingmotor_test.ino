#include <AccelStepper.h>
AccelStepper stepperA(4, 4, 3, 2, 1); //下記参照

void setup() {
  stepperA.setMaxSpeed(1000); //最大回転速度を指定(1000step/s)
  stepperA.setAcceleration(1000); //加速度を指定(1000step/ss)
  pinMode(10, INPUT_PULLUP); //10番ピンを内部プルアップ
}

void loop() {
  //もし10番ピンが押されていてかつ、ステッピングモータが目標位置にいれば
  if ((digitalRead(10) == LOW) && (stepperA.distanceToGo() == 0)) {
    stepperA.move(200); //目標値を現在値から200ステップ時計回りしたところに指定
  }
  stepperA.run(); //ステッピングモータを回す
}

//https://github.com/waspinator/AccelStepper