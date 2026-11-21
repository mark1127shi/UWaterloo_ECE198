const int xPin = A0;
const int yPin = A1;
const int buttonPin = A2;


void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT_PULLUP);

}

void loop() {
  int x = analogRead(xPin);
  int y = analogRead(yPin);
  bool pressed = digitalRead(buttonPin) == LOW;

  Serial.print("X: ");
  Serial.print(x);
  Serial.print("  Y: ");
  Serial.print(y);
  Serial.print("  Button: ");

  if(pressed){
    Serial.println("pressed");
  }else{
    Serial.println("released");
  }
  delay(100);
}
