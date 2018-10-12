// Check that the circuit works correctly.
// Each button will turn on one LED.

void setup()
{
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);

  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
}

void loop()
{
  checkPair(2, 12);
  checkPair(3, 11);
  checkPair(4, 10);
  checkPair(5, 9);
}

void checkPair(int button, int led)
{
  int buttonStatus = digitalRead(button);

  if (buttonStatus == HIGH)
  {
    digitalWrite(led, HIGH);
  }
  else
  {
    digitalWrite(led, LOW);
  }
}
