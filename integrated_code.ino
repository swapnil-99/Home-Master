#include <ESP8266WiFi.h>   //ESP8266 header file
#include <FirebaseArduino.h>//Firebase header file

// Set these to run example.
/*
    firebase authentication
 */
#define FIREBASE_HOST "example.firebaseio.com"
#define FIREBASE_AUTH "token_or_secret"
/*
    wifi authentication
 */
#define WIFI_SSID "POCOPHONE"
#define WIFI_PASSWORD "12345678"


/*
    regarding LDR
 */
int a;        
int LED=D0;
/*
    regarding for three ir
 */
int LED1=D7; //To display regarding gas knob
int LED2=D6;//To display regarding door open and close
int LED3=D2;//To display..
int obstaclePin1=D3; //Signal from the IR sensor arrives on D3
int obstaclePin2=D4;//Signal from the IR sensor arrives on D4
int obstaclePin3=D5;//Signal from the PIR sensor arrives on D5
/*
    regarding ultrasonic sensor
 */
 int echoPin = D1; //Obtain the time of echo from the ultrasonic sensor
int triggerPin =D8; //Triggers the ultrasonic sensor

void setup() {
  // put your setup code here, to run once:
  /*
   * for  ldr
   */
  pinMode(D0,OUTPUT);  
  pinMode(A0,INPUT);
  /*
    for led regarding three ir
   */
  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT);
  pinMode(LED3,OUTPUT);
  pinMode(obstaclePin1,INPUT);
  pinMode(obstaclePin2,INPUT);
  pinMode(obstaclePin3,INPUT);
  /*
      regarding ultrasonic sensor
   */
  pinMode(triggerPin , OUTPUT);
  pinMode(echoPin , INPUT);
  Serial.begin(115200);

   

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}
int n=0;
void loop() {
  // put your main code here, to run repeatedly:
  /*
      for ldr
   */
   a=analogRead(A0);
 if(a<1000)
 {
  digitalWrite(LED,1);
 }
 else
 {
  digitalWrite(LED,0);
 }
 
 delay(1000);

 /*
    for  three ir
  */
  int hasObstacle1=digitalRead(obstaclePin1);//Read if the output of the IR sensor is HIGH or LOW for gas knob
  int hasObstacle2=digitalRead(obstaclePin2);//Read if the output of the IR sensor is HIGH or LOW for door
  int hasObstacle3=digitalRead(obstaclePin3);//Read if the output of the PIR sensor is HIGH or LOW for 
  
  if (hasObstacle1==0) //The IR sensor used is ACTIVE LOW hence when obstacle is detected, hasObstacle = 0
  {
  Serial.println("gas knob is closed");
  digitalWrite(LED1,1);
  }

  else
  {
    Serial.println("gas knob is opened");
    digitalWrite(LED1,0);
    
  }
  delay(200);
  if(hasObstacle2==0)
  {
    Serial.println("door is closed");
    digitalWrite(LED2,1);
  }
  else
  {
    Serial.println("door is open");
    digitalWrite(LED2,0);
  }
  delay(200);
  if(hasObstacle3==0)
  {
    Serial.println("Intruder is coming");
    digitalWrite(LED1,1);
  }
  else
  {
    Serial.println("intruder is not coming");
    digitalWrite(LED1,0);
    
  }
  delay(200);
  /*
        regarding ultrasonic sensor
   */
  digitalWrite(triggerPin, LOW); //Initially make sure no trigger is provided
  delay(2);
  digitalWrite(triggerPin,HIGH); //Pulse is provided for 10ms to trigger the ultrasonic sensor
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  float time = pulseIn(echoPin, HIGH); //Receive the time of echo from the ultrasonic sensor 
  float distance = 0.017*time; //Assuming speed of sound is 340m/s
  Serial.print("Distance = ");
  Serial.println(distance); 
  if(distance<=50)
  {
    Serial.println("Garage is open");
  }
  else
  {
    Serial.println("Garage is close");
  }
  delay(100);

  /*
      Firebase coding as well as wifi
   */
   Firebase.setFloat("number", 42.0);
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /number failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);
  
  // update value
  Firebase.setFloat("number", 43.0);
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /number failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);

  // get value 
  Serial.print("number: ");
  Serial.println(Firebase.getFloat("number"));
  delay(1000);

  // remove value
  Firebase.remove("number");
  delay(1000);

  // set string value
  Firebase.setString("message", "hello world");
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /message failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);
  
  // set bool value
  Firebase.setBool("truth", false);
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /truth failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);

  // append a new value to /logs
  String name = Firebase.pushInt("logs", n++);
  // handle error
  if (Firebase.failed()) {
      Serial.print("pushing /logs failed:");
      Serial.println(Firebase.error());  
      return;
  }
  Serial.print("pushed: /logs/");
  Serial.println(name);
  delay(1000);
}
