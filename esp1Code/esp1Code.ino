int onboardLed = 2;
void setup() {
  // put your setup code here, to run once:
  pinMode(onboardLed, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(onboardLed, HIGH);
  delay(500);
  digitalWrite(onboardLed, LOW);
  delay(500);
}
