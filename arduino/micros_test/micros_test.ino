unsigned long time;

void setup() {
  Serial.begin(9600);
}

void loop() {
  time = micros();
  Serial.println(time);

  delay(1000); // 1秒おきに送信
}

//http://www.musashinodenpa.com/arduino/ref/index.php?f=0&pos=2549