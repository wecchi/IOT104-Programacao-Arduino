void setup() {
  // iniciando as configurações de operação:
  pinMode(13, OUTPUT);
}

void loop() {
  // piscar o LED:
  digitalWrite(13, HIGH);
  delay(1000);
  digitalWrite(13, LOW);
  delay(2000);
  digitalWrite(13, HIGH);
  delay(500);
  digitalWrite(13, LOW);
  delay(500);
  digitalWrite(13, HIGH);
  delay(1000);
}
