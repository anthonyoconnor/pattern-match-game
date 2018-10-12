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

int leds[] = {led1, led2, led3, led4};
byte numberOfLeds = 4;

int button1 = 2;
int button2 = 3;
int button3 = 4;
int button4 = 5;
int buttons[] = {button1, button2, button3, button4};
byte numberOfButtons = 4;

bool gameRunning;

void setup()
{
  Serial.begin(9600);
  setRandomSeed();

  for (byte i = 0; i < numberOfLeds; i++)
  {
    pinMode(leds[i], OUTPUT);
  }

  for (byte i = 0; i < numberOfButtons; i++)
  {
    pinMode(buttons[i], INPUT);
  }

  initLevel1Game();
  startLevel1Game();
}

void loop()
{
  level1GameLoop();
}

void setRandomSeed()
{
  int r = 0;
  for (int i = A0; i <= A7; i++)
  {
    r += analogRead(i);
  }
  randomSeed(r);
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

  for (byte i = 0; i < numberOfLeds; i++)
  {
    turnOnIfPressed(buttons[i], leds[i]);
  }

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
    int rand = random(0, 4);

    ledToMatch = leds[rand];

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

  flashAllLeds();
}

void showFailedEndGame()
{
  Serial.println("Game over");
  turnOffAllLeds();

  flashAllLeds();

  gameRunning = false;
}

void flashAllLeds()
{
  for (byte i = 0; i < 3; i++)
  {
    turnOnAllLeds();
    delay(delayBetweenBlinks);
    turnOffAllLeds();
    delay(delayBetweenBlinks);
  }
}

void turnOnAllLeds()
{
  for (byte i = 0; i < numberOfLeds; i++)
  {
    turnOnLed(leds[i]);
  }
}

void turnOffAllLeds()
{
  for (byte i = 0; i < numberOfLeds; i++)
  {
    turnOffLed(leds[i]);
  }
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
