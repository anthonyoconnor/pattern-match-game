int delayBetweenBlinks = 500;
int delayForPickBlink = 600;

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

const int maxSequence = 10;
int ledSequence[maxSequence];
int buttonSequence[maxSequence];
int currentLevel;
const int maxSteps = 3;
int currentStep;

int difficultyButton = 6;
int difficulty = 0;

bool gameRunning;

int lastLed = 0;

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
}

void loop()
{
  if (difficulty == 2 && checkPressed(difficultyButton) || difficulty == 1 && !checkPressed(difficultyButton))
  {
    gameRunning = false;
  }

  if (!gameRunning)
  {
    if (checkPressed(difficultyButton))
    {
      difficulty = 1;
    }
    else
    {
      difficulty = 2;
    }

    initGame();
    startGame();
  }

  gameLoop();
}

void resetSequence()
{
  for (byte i = 0; i < maxSequence; i++)
  {
    ledSequence[i] = 0;
    buttonSequence[i] = 0;
  }
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

void gameLoop()
{
  if (!gameRunning)
  {
    return;
  }

  boolean sequenceFull = false;

  int sequencePressed = 0;

  while (sequencePressed < currentLevel)
  {
    bool button1Pressed = checkPressed(button1);
    bool button2Pressed = checkPressed(button2);
    bool button3Pressed = checkPressed(button3);
    bool button4Pressed = checkPressed(button4);

    if (button1Pressed + button2Pressed + button3Pressed + button4Pressed == 0)
    {
      continue;
    }

    for (byte i = 0; i < numberOfLeds; i++)
    {
      boolean pressed = turnOnIfPressed(buttons[i], leds[i]);
      if (pressed)
      {
        buttonSequence[sequencePressed] = i;
      }
    }

    sequencePressed++;

    waitForAllNotPressed();

    if (button1Pressed + button2Pressed + button3Pressed + button4Pressed > 1)
    {
      // More than one button was pressed at a time so end game.
      showFailedEndGame();
      gameRunning = false;
      return;
    }
  }

  printSequences();

  if (!verifySequences())
  {
    showFailedEndGame();
    gameRunning = false;
    return;
  }

  if (checkForVictory())
  {
    showSuccessEndGame();

    gameRunning = false;
  }
  else
  {
    fillNextSequence();
    showNextSequence();
  }
}

boolean verifySequences()
{
  for (byte i = 0; i < maxSequence; i++)
  {
    if (buttonSequence[i] != ledSequence[i])
    {
      return false;
    }
  }

  return true;
}

boolean checkForVictory()
{
  return (currentLevel == maxSequence && currentStep == maxSteps);
}

void printSequences()
{
  Serial.println("Button Sequence:");

  for (byte i = 0; i < maxSequence; i++)
  {
    Serial.print(buttonSequence[i]);
    Serial.print(" ");
  }

  Serial.println("LED Sequence:");

  for (byte i = 0; i < maxSequence; i++)
  {
    Serial.print(ledSequence[i]);
    Serial.print(" ");
  }

  Serial.println(" ");
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

  turnOffAllLeds();
}

void startGame()
{
  Serial.println("Starting game");
  resetSequence();
  currentLevel = 1;
  currentStep = 0;

  fillNextSequence();
  showNextSequence();
  gameRunning = true;
}

int getNextLed()
{
  int ledToMatch = 0;
  do
  {
    int rand = random(0, 4);

    ledToMatch = rand;

  } while (ledToMatch == lastLed);

  lastLed = ledToMatch;

  return ledToMatch;
}

void fillNextSequence()
{
  currentStep++;
  Serial.println(currentStep);

  if (currentStep > maxSteps)
  {
    if (difficulty != 1)
    {
      currentLevel++;
    }

    currentStep = 1;
  }

  for (byte i = 0; i < currentLevel; i++)
  {
    ledSequence[i] = getNextLed();
  }
}

void showNextSequence()
{
  turnOffAllLeds();
  delay(delayBetweenBlinks);

  for (byte i = 0; i < currentLevel; i++)
  {
    turnOnLed(leds[ledSequence[i]]);
    delay(delayForPickBlink);
    turnOffLed(leds[ledSequence[i]]);
    delay(delayBetweenBlinks);
  }
}

void initGame()
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
  Serial.println("Game over - Failed");
  turnOffAllLeds();

  flashAllLeds();
}

void showSuccessEndGame()
{
  Serial.println("Game over - Success");
  turnOffAllLeds();

  turnOnAllLeds();
  delay(5000);
  turnOffAllLeds();
  flashAllLeds();
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

bool turnOnIfPressed(int button, int led)
{
  int buttonStatus = digitalRead(button);

  if (buttonStatus == HIGH)
  {
    digitalWrite(led, HIGH);
    return true;
  }
  else
  {
    digitalWrite(led, LOW);
    return false;
  }
}
