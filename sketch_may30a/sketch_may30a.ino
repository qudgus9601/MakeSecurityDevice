#define BUZ_PIN 19

// 부저 설정
int channel_flag=0;
int freq =5000;
int buzChannel = 0;
int resolution = 8; 

int human = 16; // 인체감지센서
int blue = 22; // 파란색 등
int red = 23; // 적색 등

int pirState = LOW; // 평시 off
int turn = 0; // 감지 된 물체 없음 (평시)
void setup(){
  Serial.begin(115200);
  // 부저 설정
  ledcSetup(buzChannel, freq, resolution);
  ledcAttachPin(BUZ_PIN,buzChannel);
  // 인체감지센서
  pinMode(human,INPUT);
  // LED
  pinMode(red, OUTPUT);
  pinMode(blue, OUTPUT);
}

void loop(){
  // 인체감지 센서값 저장
  turn = digitalRead(human);
  // 시리얼모니터 확인용
  Serial.println(turn);
  
  if (human == 1){
    turn = 1; // on
  }
  
  if (turn == 1){
  digitalWrite(red, HIGH);
  digitalWrite(blue, LOW);
  ledcWriteTone(buzChannel,4096);
  delay(200);
  digitalWrite(red, LOW);
  digitalWrite(blue, HIGH);
  ledcWriteTone(buzChannel,256);
  delay(200);
  }
  else if (turn == 0) {
    digitalWrite(red, LOW);
    digitalWrite(blue, LOW);
    ledcWriteTone(buzChannel,0);
  }
  
}
