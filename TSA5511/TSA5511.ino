////////////////////////////////////////////////
//                                            //
//           ArduFMPLL by MichTronics         //
//                      v0.1                  //
//                Working with the            //
//        DRFS6 v2.0 3meter FM 6W TX Kit      //
//                                            //
////////////////////////////////////////////////

#include <Wire.h>
#include <OneButton.h>
#include <SimpleRotary.h>
#include <LiquidCrystal_I2C.h>

int readLbs = 1;
int configScreen = 0;

bool up = false;
bool down = false;
bool middle = false;

byte rotaryT;
byte arrowChar[] = {
  B01100,
  B00110,
  B00011,
  B11111,
  B11111,
  B00011,
  B00110,
  B01100
};
LiquidCrystal_I2C lcd(0x3f, 20, 4);
OneButton button(A0,true);
SimpleRotary rotary(2,3,4);

void setup()
{
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
  Wire.begin(); // join i2c bus
  button.attachClick(singleClick);
  button.attachDoubleClick(doubleClick);
  button.attachPress(longClick);
  lcd.setCursor(4,0);
  lcd.print("MichTronics");
  lcd.setCursor(3,1);
  lcd.print("ArduFMPLL v0.1");
  lcd.setCursor(0,3);
  lcd.print("Radio West-Friesland");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.createChar(0, arrowChar);
  lcd.home();
  lcd.write(0);
  pll_set_frequency(94500);
}
int test = 1;

void loop()
{  
  rotaryT = rotary.rotate();
  debugRotary();
  button.tick();
  if (readLbs == 1){
    readLockbyte();
  }
  startScreen();
  //pll_set_frequency(94500);
}

void debugRotary() {
  if (rotaryT == 1) {
    Serial.println("LEFT");
  }
  if (rotaryT == 2) {
    Serial.println("RIGHT");
  }
}

void startScreen() {  
  if (rotaryT == 1 & configScreen == 0) {
    lcd.setCursor(0,0);
    lcd.createChar(0, arrowChar);
    lcd.home();
    lcd.write(0);
    lcd.setCursor(1,0);
    test = test - 1;
    lcd.print(" ");
    lcd.print(test);
    lcd.print(" ");
    Serial.println("LEFT");
  }
  if (rotaryT == 2 & configScreen == 0) {
    lcd.setCursor(1,0);
    test = test + 1;
    lcd.print(" ");
    lcd.print(test);
    lcd.print(" ");
    Serial.println("RIGHT");
  }
}

void pll_set_frequency(long pllfreq) {
  char data[2];
  pllfreq = pllfreq / 8;
  pllfreq = pllfreq / 1;
  pllfreq = pllfreq * 1000;
  pllfreq = pllfreq / 6250;
  data[0] = (pllfreq & 0xFF00) >> 8;
  data[1] = pllfreq & 0x00FF;

  // Init byte
  Wire.beginTransmission(96);
  Wire.write(0xCE);
  Wire.write(0x00);
  Wire.endTransmission();

  delay(12);

  // Frequentie byte
  Wire.beginTransmission(96);
  Wire.write(data, 2);
  Wire.endTransmission();

  delay(27);

  // Lock byte
  Wire.beginTransmission(96);
  Wire.write(0xCE);
  Wire.write(0x20);
  Wire.endTransmission();
}

void readLockbyte(){
  Wire.requestFrom(96, 2);
  if ( Wire.available() >= 1 | readLbs == 1)
  {
    if (Wire.read() == 0x48){
      //Serial.println("Lock");
      startTransmitting();
      readLbs = 0;
    } else
    {
      //Serial.println("NoLock");
    }
  }
}

void startTransmitting(){
  // byte begin transmitting DRFS6 v2.0
  Wire.beginTransmission(96);
  Wire.write(0xCE);
  Wire.write(0x24);
  Wire.endTransmission();
}

void singleClick() {
  Serial.println("SINGLE");
  configScreen = 1;
  lcd.clear();
  lcd.setCursor(4,0);
  lcd.print("Instellingen");
}

void doubleClick() {
  Serial.println("DOUBLE");
  
}

void longClick() {
  Serial.println("LONG");
}
