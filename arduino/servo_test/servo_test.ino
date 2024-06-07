int pwmpin = 9; // pwm servo pin
int btnpin = 7; // «input button»
int readbtn = LOW;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(pwmpin, OUTPUT);
  pinMode(btnpin, INPUT);
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  servo(500);//0度（初期化）
}

void servo(int pos){
  digitalWrite(pwmpin, HIGH);
  delayMicroseconds(pos);
  digitalWrite(pwmpin, LOW);
  delayMicroseconds(16000); 
  delayMicroseconds(8000); 
}

// the loop function runs over and over again forever
void loop() {
  readbtn = digitalRead(btnpin);
  if (readbtn == HIGH) {
   servo(2500);//180度
  } else {
   servo(1500);//90度
  }
}
