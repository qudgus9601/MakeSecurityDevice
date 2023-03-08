#if defined(ESP32)
#include <WiFi.h>
#include <FirebaseESP32.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#endif

// 토큰
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

// 와이파이
#define WIFI_SSID "Father"
#define WIFI_PASSWORD "kejo500800"

// API 키 값
#define API_KEY "AIzaSyBOxfLwt0N5m8NTKWfha2XZRD6V_W_g47g"

// realtimedb 주소
#define DATABASE_URL "changgong-c2b03-default-rtdb.firebaseio.com" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

// 파베랑 연결된 구글
#define USER_EMAIL "qudgus1012@gmail.com"
#define USER_PASSWORD "qudgus98@@"

// 파베 정보
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;

int count = 0;

// 여기서부터 프로젝트
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
int order = 0; // 사용자 평시에는 꺼둠 앱에서 받을 값

void setup()
{

  Serial.begin(115200);

  // 와이파이
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  // 파이어베이스
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);
  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.database_url = DATABASE_URL;
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  Firebase.begin(&config, &auth);

  // 부저 설정
  ledcSetup(buzChannel, freq, resolution);
  ledcAttachPin(BUZ_PIN,buzChannel);
  // 인체감지센서
  pinMode(human,INPUT);
  // LED
  pinMode(red, OUTPUT);
  pinMode(blue, OUTPUT);
}

void loop()
{
  // 연결 확인용
  if (Firebase.ready() && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0))
  {
    sendDataPrevMillis = millis();
    Serial.printf("Set int... %s\n", Firebase.setInt(fbdo, "/test/int", count) ? "ok" : fbdo.errorReason().c_str());
    Serial.printf("Get int... %s\n", Firebase.getInt(fbdo, "/test/int") ? String(fbdo.intData()).c_str() : fbdo.errorReason().c_str());
    FirebaseJson json;
    json.add("value", count);
    Serial.printf("Push json... %s\n", Firebase.pushJSON(fbdo, "/test/push", json) ? "ok" : fbdo.errorReason().c_str());
    json.set("value", count + 100);
    Serial.printf("Update json... %s\n\n", Firebase.updateNode(fbdo, String("/test/push/" + fbdo.pushName()), json) ? "ok" : fbdo.errorReason().c_str());
    count++;
  }
  if (order == 1){
     // 인체감지 센서값 저장
    digitalRead(human);
    // 시리얼모니터 확인용
    Serial.printf("current state : ",turn);
    
    // 누군가 감지 되면 발작버튼 on 사용자가 off 할 때 까지
    if (human == 1){
      turn = 1; // on
    }
    if (turn == 1){
      digitalWrite(red, HIGH);
      digitalWrite(blue, LOW);
      ledcWriteTone(buzChannel,512);
      delay(50);
      ledcWriteTone(buzChannel,256);
      delay(200);
      digitalWrite(red, LOW);
      digitalWrite(blue, HIGH);
      ledcWriteTone(buzChannel,0);
      delay(200);
    }
    else if (turn == 0) {
      digitalWrite(red, LOW);
      digitalWrite(blue, LOW);
      ledcWriteTone(buzChannel,0);
    }
  }
  else if (order == 0) {
    ledcWriteTone(buzChannel,0);
    digitalWrite(red, LOW);
    digitalWrite(blue, LOW);
  }
  
}
