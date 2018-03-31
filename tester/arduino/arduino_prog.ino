uint16_t speed_delay = 71;
String msg = "";

void setup() {
  Serial.begin(9600);
  
  pinMode(9, OUTPUT);
  pinMode(8, OUTPUT);
  
  TCCR1A = 0;
  TCCR1B = 0;

  OCR1A = 10000;
  TCCR1A = (1<<COM1A0);
  TCCR1B = (1<<CS11) | (1<<WGM12);
}

void loop() {
  digitalWrite(8, 1);
  delay(speed_delay);
  digitalWrite(8, 0);
  delay(5);

  if (Serial.available() > 0) {
    char c = Serial.read();
    if (c == ';') {
      on_message();
      msg = "";
    } else {
      msg += c;
    }
  }
}

void on_message() {
  if (msg[0] == 's') {
    speed_delay = msg.substring(1).toInt();
  } else if (msg[0] == 'r') {
    OCR1A = msg.substring(1).toInt();
  }
}

