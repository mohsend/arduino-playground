void setup() {
  // put your setup code here, to run once:
  pinMode(A0, INPUT);
  pinMode(3, OUTPUT);
  pinMode(4, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int in = analogRead(A0);
  int out = map(in, 0, 1024, 0, 255);
  analogWrite(3, out);
  float period = pulseIn(4, LOW);
  float permin = 1000000.0 / period * 6.0;
  Serial.println(permin);
  delay(1000);
}
