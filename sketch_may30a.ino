
int human=19;
int speaker=21;
int blue=22;
int red=23;

int turn=0;

void setup() {
pinMode(speaker,OUTPUT);
pinMode(red,OUTPUT);
pinMode(blue,OUTPUT);
pinMode(human,INPUT);


}

void loop() {

  turn = digitalRead(human);

  if (turn == 1) {
  digitalWrite(red,HIGH);
  digitalWrite(blue,LOW);
  tone(speaker, 523);
  delay(300);
  digitalWrite(blue,HIGH);
  digitalWrite(red,LOW);
  noTone(speaker);
  delay(300);
  }
  else {
    digitalWrite(red,LOW);
    digitalWrite(blue,LOW);
    noTone(speaker);
  }
}
