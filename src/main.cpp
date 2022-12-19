#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define NUM_LEDS 60
#define PIN 8

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

int blueButton = 2;
int redButton = 3;
int scoreLimit = 3;

int blueScore;
int redScore;
int delayButtonTime = 20;
int ledPosition;
int delta = 50;

byte lu[8] = {
    0b01111,
    0b10000,
    0b01000,
    0b00111,
    0b00011,
    0b00001,
    0b00111,
    0b01001};
byte ru[8] = {
    0b11110,
    0b00001,
    0b00010,
    0b11100,
    0b11000,
    0b10000,
    0b11100,
    0b10010};
byte ld[8] = {
    0b01001,
    0b00101,
    0b00011,
    0b00001,
    0b00011,
    0b00111,
    0b01111,
    0b11111};
byte rd[8] = {
    0b10010,
    0b10100,
    0b11000,
    0b10000,
    0b11000,
    0b11100,
    0b11110,
    0b11111};

bool isButtonPressed(int button)
{
  if (digitalRead(button) == LOW)
  {
    delayMicroseconds(delayButtonTime);
    if (digitalRead(button) == LOW)
    {
      return true;
    }
  }
  return false;
}

void clean()
{
  for (int i = 0; i <= NUM_LEDS; i++)
  {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    pixels.show();
  }
}

void start()
{
  lcd.home();
  lcd.print("      PING");
  lcd.setCursor(0, 1);
  lcd.print("      PONG");
  pixels.setPixelColor(0, pixels.Color(0, 0, 255));
  pixels.setPixelColor(1, pixels.Color(0, 0, 255));
  pixels.setPixelColor(2, pixels.Color(0, 0, 255));
  pixels.setPixelColor(3, pixels.Color(0, 0, 255));
  pixels.setPixelColor(4, pixels.Color(0, 0, 255));
  pixels.setPixelColor(55, pixels.Color(255, 0, 0));
  pixels.setPixelColor(56, pixels.Color(255, 0, 0));
  pixels.setPixelColor(57, pixels.Color(255, 0, 0));
  pixels.setPixelColor(58, pixels.Color(255, 0, 0));
  pixels.setPixelColor(59, pixels.Color(255, 0, 0));
  pixels.show();
  ledPosition = 0;
}

void b2r()
{
  clean();
  for (int p = ledPosition; p <= NUM_LEDS; p++)
  {
    if (p <= 5)
    {
      pixels.setPixelColor(p, pixels.Color(0, 0, 255));
    }
    if (p <= 10 & p >= 5)
    {
      pixels.setPixelColor(p, pixels.Color(255, 255, 0));
    }
    if (p <= 50 & p >= 10)
    {
      pixels.setPixelColor(p, pixels.Color(0, 255, 0));
    }
    if (p <= 55 & p >= 50)
    {
      pixels.setPixelColor(p, pixels.Color(255, 255, 0));
    }
    if (p >= 55)
    {
      pixels.setPixelColor(p, pixels.Color(255, 0, 0));
    }
    pixels.setPixelColor(p - 1, pixels.Color(0, 0, 0));
    pixels.show();
    delay(delta);
    if (p >= 55 & p < NUM_LEDS & isButtonPressed(redButton) == 1)
    {
      screen();
      ledPosition = p;
      if (delta > 5)
      {
        delta -= 5;
      }
      r2b();
    }
    if (p < 55 & isButtonPressed(redButton))
    {
      blueScore++;
      screen();
      delta = 50;
      ledPosition = NUM_LEDS;
      r2b();
    }
    if (redScore >= scoreLimit)
    {
      redWin();
    }
  }
  if (int p = NUM_LEDS)
  {
    blueScore++;
    screen();
    delta = 50;
    ledPosition = NUM_LEDS;
    r2b();
  }
}

void r2b()
{
  clean();
  for (int o = ledPosition; o >= 0; o--)
  {
    if (o > 54)
    {
      pixels.setPixelColor(o, pixels.Color(255, 0, 0));
    }
    if (o<55 & o> 49)
    {
      pixels.setPixelColor(o, pixels.Color(255, 255, 0));
    }
    if (o<50 & o> 9)
    {
      pixels.setPixelColor(o, pixels.Color(0, 255, 0));
    }
    if (o<10 & o> 4)
    {
      pixels.setPixelColor(o, pixels.Color(255, 255, 0));
    }
    if (o < 5)
    {
      pixels.setPixelColor(o, pixels.Color(0, 0, 255));
    }
    pixels.setPixelColor(o + 1, pixels.Color(0, 0, 0));
    pixels.show();
    delay(delta);
    if (o < 5 & o != 0 & isButtonPressed(blueButton) == 1)
    {
      screen();
      ledPosition = o;
      if (delta > 5)
      {
        delta -= 5;
      }
      b2r();
    }
    if (o > 5 & isButtonPressed(blueButton))
    {
      redScore++;
      screen();
      delta = 50;
      ledPosition = 0;
      b2r();
    }
    if (blueScore >= scoreLimit)
    {
      blueWin();
    }
  }
  if (int o = ledPosition)
  {
    redScore++;
    screen();
    delta = 50;
    ledPosition = 0;
    b2r();
  }
}

void screen()
{
  lcd.clear();
  lcd.home();
  lcd.print("BLUE ");
  lcd.print(blueScore);
  lcd.setCursor(0, 1);
  lcd.print("RED  ");
  lcd.print(redScore);
}

void gameOver()
{
  redScore = 0;
  blueScore = 0;
  lcd.clear();
  lcd.home();
  lcd.write(byte(0));
  lcd.write(byte(1));
  lcd.print("  ");
  lcd.print("GAMEOVER");
  lcd.print("  ");
  lcd.write(byte(0));
  lcd.write(byte(1));
  lcd.setCursor(0, 1);
  lcd.write(byte(2));
  lcd.write(byte(3));
  lcd.setCursor(14, 1);
  lcd.write(byte(2));
  lcd.write(byte(3));
}

void blinkLCD()
{
  for (int i = 0; i <= 2; i++)
  {
    lcd.noBacklight();
    delay(500);
    lcd.backlight();
    delay(500);
  }
}

void blueWin()
{
  gameOver();
  lcd.setCursor(2, 1);
  lcd.print("  BLUE WON");
  for (int x = 0; x < NUM_LEDS; x++)
  {
    pixels.setPixelColor(x, pixels.Color(0, 0, 255));
    pixels.show();
  }
  blinkLCD();
  fade();
  delay(500);
  lcd.clear();
  asm volatile("  jmp 0");
}

void redWin()
{
  gameOver();
  lcd.setCursor(2, 1);
  lcd.print("  RED  WON");
  for (int x = NUM_LEDS; x >= 0; x--)
  {
    pixels.setPixelColor(x, pixels.Color(255, 0, 0));
    pixels.show();
  }
  blinkLCD();
  fade();
  delay(500);
  lcd.clear();
  asm volatile("  jmp 0");
}

void fade()
{
  for (int b = 255; b >= 1; b--)
  {
    pixels.setBrightness(b);
    pixels.show();
    if (isButtonPressed(blueButton) || isButtonPressed(redButton))
    {
      break;
    }
    delay(5);
  }
  for (int b = 1; b <= 255; b++)
  {
    pixels.setBrightness(b);
    pixels.show();
    if (isButtonPressed(blueButton) || isButtonPressed(redButton))
    {
      break;
    }
    delay(5);
  }
}

void setup()
{
  pixels.begin();
  pinMode(redButton, INPUT_PULLUP);
  pinMode(blueButton, INPUT_PULLUP);
  //  lcd.begin();
  lcd.backlight();
  lcd.createChar(0, lu);
  lcd.createChar(1, ru);
  lcd.createChar(2, ld);
  lcd.createChar(3, rd);
}

void loop()
{
  start();
  fade();
  if (isButtonPressed(blueButton))
  {
    ledPosition = 0;
    pixels.setBrightness(255);
    b2r();
  }
  if (isButtonPressed(redButton))
  {
    pixels.setBrightness(255);
    ledPosition = NUM_LEDS;
    r2b();
  }
}
