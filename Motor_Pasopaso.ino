#define IN1 25
#define IN2 26
#define IN3 27
#define IN4 14

int estado = 0;

float Tm = 100;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);

pinMode(IN1,OUTPUT);
pinMode(IN2,OUTPUT);
pinMode(IN3,OUTPUT);
pinMode(IN4,OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  switch (estado){
  case 0:
    digitalWrite (IN1, HIGH);
    digitalWrite (IN2, LOW);
    digitalWrite (IN3, HIGH);
    digitalWrite (IN4, LOW);
    delay(5);
    estado++;
  break;
  case 1:
    digitalWrite (IN1, HIGH);
    digitalWrite (IN2, LOW);
    digitalWrite (IN3, LOW);
    digitalWrite (IN4, HIGH);
    delay(5);
    estado++;
  break;
  case 2:
    digitalWrite (IN1, LOW);
    digitalWrite (IN2, HIGH);
    digitalWrite (IN3, LOW);
    digitalWrite (IN4, HIGH);
    delay(5);
    estado++;
    break;
  case 3:
    digitalWrite (IN1, LOW);
    digitalWrite (IN2, HIGH);
    digitalWrite (IN3, HIGH);
    digitalWrite (IN4, LOW);
    delay(5);
    estado=0;
  }
}
