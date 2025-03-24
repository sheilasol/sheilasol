#include <DS1307.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

const byte ROWS = 4; // Four rows
const byte COLS = 3; // Three columns

// Define the Keymap

char keys[ROWS][COLS] = {

  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}

};

// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
byte rowPins[ROWS] = {8,7,6,5};
// Connect keypad COL0, COL1 and COL2 to these Arduino pins.
byte colPins[COLS] = {4,3,2};

//Create the Keypad
Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
 

DS1307  rtc(A4, A5);
Servo servo_test;      //initialize a servo object for the connected servo  
LiquidCrystal_I2C lcd (0x27, 16, 2); 

int t1, t2, t3, t4, t5, t6;

boolean feed = true; // condition for alarm

char key;
int r[6];
 
void setup() 
{ 
  servo_test.attach(10);   // attach the signal pin of servo to pin9 of arduino
  rtc.begin();
  lcd.begin(16,2);
  lcd.backlight();
  servo_test.writeMicroseconds(1500);
  Serial.begin(9600);
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  
} 
 
void loop() 
{ 
  int buttonPress;
  buttonPress = digitalRead(A3);

  if (buttonPress==1)
    setFeedingTime();

  lcd.setCursor(0,1);
  lcd.print("Time:  ");
  String t = "";
  t = rtc.getTimeStr(); 
  t1 = t.charAt(0)-48;
  t2 = t.charAt(1)-48;
  t3 = t.charAt(3)-48;
  t4 = t.charAt(4)-48;
  t5 = t.charAt(6)-48;
  t6 = t.charAt(7)-48;
  lcd.print(rtc.getTimeStr());
 
  lcd.setCursor(0,0);
  lcd.print("Date: ");
  lcd.print(rtc.getDateStr());
 
  if (t1==r[0] && t2==r[1] && t3==r[2] && t4==r[3]&& t5<1 && t6<3 && feed==true)
 { 
  servo_test.writeMicroseconds(700);                  //command to rotate the servo to the specified angle 
  delay(500);   
  servo_test.writeMicroseconds(1500);
  feed=false;
 } 
}       

void setFeedingTime()
{
  char j=0;
  feed = true;
  int i = 0;
   
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Set feeding Time");
  delay(1000);
  lcd.clear();
  lcd.print("HH:MM");
  lcd.setCursor(0,1);
  
  while(1){  
   key = kpd.getKey();

  if(key)
  {
    Serial.println(key);
    lcd.setCursor(j,1);
    
    lcd.print(key);
    
    r[i] = key-48;
    i+=1;
    j+=1;

    if (j == 2)
    {
      lcd.print(":"); j++;
    }
    delay(500);
  }

  if (key == '#')
  {key=0; break; }
  }
}