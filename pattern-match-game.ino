// Check that the circuit works correctly.
// Each button will turn on one LED.

int ledToMatch = 0;
int delayBetweenBlinks = 500;
int delayForPickBlink = 600;
int lastLed = 0;

int led1 = 12;
int led2 = 11;
int led3 = 10;
int led4 = 9;

int button1 = 2;
int button2 = 3;
int button3 = 4;
int button4 = 5;

bool gameRunning;

void setup()
{
  Serial.begin(9600);
  randomSeed(analogRead(0));
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);

  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  pinMode(button3, INPUT);
  pinMode(button4, INPUT);

  initLevel1Game();
  startLevel1Game();
}

void loop()
{
  level1GameLoop();
}

void level1GameLoop()
{
  if (!gameRunning)
  {
    return;
  }

  bool button1Pressed = checkPressed(button1);
  bool button2Pressed = checkPressed(button2);
  bool button3Pressed = checkPressed(button3);
  bool button4Pressed = checkPressed(button4);

  if (button1Pressed + button2Pressed + button3Pressed + button4Pressed == 0)
  {
    return;
  }

  turnOnIfPressed(button1, led1);
  turnOnIfPressed(button2, led2);
  turnOnIfPressed(button3, led3);
  turnOnIfPressed(button4, led4);

  waitForAllNotPressed();

  if (button1Pressed + button2Pressed + button3Pressed + button4Pressed > 1)
  {
    //too many buttons pressed.
    showFailedEndGame();
    initLevel1Game();
    startLevel1Game();
  }
  else
  {

    if ((ledToMatch == led1 && button1Pressed) || (ledToMatch == led2 && button2Pressed) || (ledToMatch == led3 && button3Pressed) || (ledToMatch == led4 && button4Pressed))
    {
      pickNextLed();
    }
    else
    {
      showFailedEndGame();
      initLevel1Game();
      startLevel1Game();
    }
  }
}

void waitForAllNotPressed()
{
  bool anyPressed = true;
  while (anyPressed)
  {
    bool button1Pressed = checkPressed(button1);
    bool button2Pressed = checkPressed(button2);
    bool button3Pressed = checkPressed(button3);
    bool button4Pressed = checkPressed(button4);

    if (button1Pressed + button2Pressed + button3Pressed + button4Pressed == 0)
    {
      anyPressed = false;
    }
    else
    {
      delay(250);
    }
  }
}

void showNextLed()
{
  turnOnLed(ledToMatch);
  delay(delayForPickBlink);
  turnOffLed(ledToMatch);
}

void pickNextLed()
{
  turnOffAllLeds();
  delay(500);
  do
  {
    int rand = random(1, 5);

    // The lED pins may not be sequential in the final version
    // so did not use random(led4, led1+1) here since that only
    // works if they are always sequential.
    if (rand == 1)
    {
      ledToMatch = led1;
    }

    if (rand == 2)
    {
      ledToMatch = led2;
    }

    if (rand == 3)
    {
      ledToMatch = led3;
    }

    if (rand == 4)
    {
      ledToMatch = led4;
    }
  } while (ledToMatch == lastLed);

  lastLed = ledToMatch;
  showNextLed();
}

void startLevel1Game()
{
  Serial.println("Starting game");
  pickNextLed();
  gameRunning = true;
}

void initLevel1Game()
{
  turnOnLed(led1);
  delay(delayBetweenBlinks);
  turnOnLed(led2);
  delay(delayBetweenBlinks);
  turnOnLed(led3);
  delay(delayBetweenBlinks);
  turnOnLed(led4);
  delay(delayBetweenBlinks);

  turnOffAllLeds();
  delay(delayBetweenBlinks);
  turnOnAllLeds();
  delay(delayBetweenBlinks);
  turnOffAllLeds();
  delay(delayBetweenBlinks);
  turnOnAllLeds();
  delay(delayBetweenBlinks);
  turnOffAllLeds();
  delay(delayBetweenBlinks);
}

void showFailedEndGame()
{
  Serial.println("Game over");
  turnOffAllLeds();

  for (byte i = 0; i < 3; i++)
  {
    turnOnAllLeds();
    delay(delayBetweenBlinks);
    turnOffAllLeds();
    delay(delayBetweenBlinks);
  }

  gameRunning = false;
}

void turnOnAllLeds()
{
  turnOnLed(led1);
  turnOnLed(led2);
  turnOnLed(led3);
  turnOnLed(led4);
}

void turnOffAllLeds()
{
  turnOffLed(led1);
  turnOffLed(led2);
  turnOffLed(led3);
  turnOffLed(led4);
}

void turnOnLed(int led)
{
  digitalWrite(led, HIGH);
}

void turnOffLed(int led)
{
  digitalWrite(led, LOW);
}

bool checkPressed(int button)
{
  int buttonStatus = digitalRead(button);

  return buttonStatus == HIGH;
}

void turnOnIfPressed(int button, int led)
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
