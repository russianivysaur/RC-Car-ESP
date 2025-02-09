#define PIN 25
#define LED_PIN 2
void setup() {
  // put your setup code here, to run once:
  pinMode(PIN,OUTPUT);
  pinMode(LED_PIN,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  analogWrite(PIN,10000);
  digitalWrite(LED_PIN,HIGH);
  delay(5000);
  digitalWrite(LED_PIN,LOW);
  analogWrite(PIN,0);
  delay(5000);
}
