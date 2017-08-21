#include <openGLCD.h>
#include "bitmaps.h"
#include <Keypad.h>
#include <TimerFour.h>
#include <TimerThree.h>
#include <EEPROM.h>
#include "EEPROMAnything.h"

/*********************************** EEPROM Stuff *******************************************/
struct normalVolume
{
    float volume;
    int dose, hour_on, minute_on, second_on, hour_off, minute_off, second_off;
}vol1;

struct footSwitch
{
  float volume;
}foot;

struct normalTime{
  float rate;
  int dose, hour_on, minute_on, second_on, hour_off, minute_off, second_off;
}time1;



void writeNormalVolume(int num) 
{
  num--;
  EEPROM_writeAnything(100 + 20*num, vol1);
}

void readNormalVolume(int num)
{
  num--;
  EEPROM_readAnything(100 + 20*num, vol1);
}

void writeFS(int num) 
{
  num--;
  EEPROM_writeAnything(300 + 20*num, foot);
}

void readFS(int num)
{
  num--;
  EEPROM_readAnything(300 + 20*num, foot);
}

void writeNormalTime(int num) 
{
  num--;
  EEPROM_writeAnything(500 + 20*num, time1);
}

void readNormalTime(int num)
{
  num--;
  EEPROM_readAnything(500 + 20*num, time1);
}


/*********************************** Keypad Initialization***********************************/
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns

//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  { '1', '2', '3', 'A'   },
  
  { '4', '5', '6', 'B'   },
  
  { '7', '8', '9', 'C'   },
  
  { '*', '0', '#', 'D'   }
};

byte rowPins[ROWS] = { 40, 41, 42, 43 };  //connect to the row pinouts of the keypad
byte colPins[COLS] = { 44, 45, 46, 47 }; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

/*********************************** GUI Variable ******************************************/
char hstate = '1';
char sstate = '0';
char tstate = '0';
char fstate = '0';
char input = '1';
char pump[10] = "Select" , tube[10] = "Select" , data[10] = "done";
char sel[7] = "Select";
char menuMode = '0';

/*********************************** Motor Variables***************************************/
float vol = 0;
float rate = 0;
float d_vol = 0;
char buffer1[8];
int spdi = 3;
int spd[8] = { 10, 50, 100, 200, 300, 400, 500, 600};
int ton, toff;
int hour_on, minute_on, second_on;
int hour_off, minute_off, second_off;
int dose = 1;

const float inRad = 1.5;
const float pumpRad = 20.0;
const float pi = 3.141592654;
const float tPre = 100000;
const int mode = 32;
float delta1, delta2, delta3;

volatile unsigned long clkCount = 0;
unsigned long clkCountCopy = 0;
volatile unsigned long timerTick = 0;
unsigned long timerTickCopy = 0;

//GUI and Navigation Function Declarations
void disp1(void);
void disp1Aa(void);
void disp1Ab(void);
void disp1Ba(void);
void disp1Bb(void);
void disp4A(void);
void disp4B(void);
void disp4C(void);
void disp4D(void);
void disp4F(void);
void disp5A(void);
void dispvolpara1(void);
void dispvolpara2(void);
void disp1navigate(char);
void disp1Aanavigate(char);
void homedisplay(void);
void homeinvert(char);
void homeinavigate(void);
void heffect(void);
void invert1A(char);
void invert1A(char);
void pumpmenua(void);
void pumpmenub(void);
void tubemenua(void);
void tubemenub(void);
void openingscreen(void);
void one(void);
void back(void);
void invert2(char);
void voldisplay(void);
void volnavigate(void);
void veffect(void);
void speedmenu1(void);
float getvol(void);
void effect(char);
void splback(void);
void invert4A(char);
void fback(void);

//Motor Related Funcion Declarations
void clkDelay(void);
void bigDelay(void);
void setRpm(float);
void copyMode(void);
void discreteDisp(int, int);
void setDelta1(void);
void displayFloat(float, int, int);
void priming(void);
float calibFormula(float);
void calibMotor(int,int);
boolean calibQuery(void);
void calibDisp(float, float);
void disp5C(void);


//profileSave
void disp6(void);
void disp6navigate(char);
void six(void);
void normalFs(void);
void disp6A(void);
void disp6Aa(void);
void disp6Ab(void);
void profileMenu(void);
int parseChar(char);
void readProfileAa(int);
void readProfileAb(int);
void disp6Aanavigate(char);
void disp6B(void);
void readProfileB(int);

void disp4Evolume(void);
void saveVolume(void);
void saveProfileVolume(int);

void disp4Erate(void);
void saveRate(void);
void saveProfileRate(void);

void disp4Efoot(void);
void saveFoot(void);
void saveProfileFoot(int);

void disp4Aa(void);

//Motor Control Pins
int clk = 6;
int dir = 11;
int ena = 10;

int footSwitch = 5;
int readPriming = 4;
int primingIndicator = 3;

void setup() {
Serial.begin(9600);
/*********************************** Motor Initialization ***********************************/
  setDelta1();

  Timer4.initialize(50);
  Timer4.attachInterrupt(clkDelay);

  Timer3.attachInterrupt(bigDelay);

  pinMode(clk, OUTPUT);
  pinMode(dir, OUTPUT);
  pinMode(ena, OUTPUT);

  pinMode(footSwitch, INPUT);
  pinMode(readPriming, INPUT);
  pinMode(primingIndicator, OUTPUT);

  digitalWrite(clk, LOW);
  digitalWrite(dir, LOW);
  digitalWrite(ena, LOW);

/*********************************** GUI Initialization **************************************/
  GLCD.Init();

  // GLCD.SelectFont(System5x7);
  GLCD.DrawBitmap(murophy, 0, 0, BLACK);
  delay(1000);
  GLCD.InvertRect(0, 0, 128, 64);
  delay(1000);

  GLCD.ClearScreen();
  // GLCD.DrawBitmap(sjcelogo, 0, 0, BLACK);
  //delay(2000);
  //openingscreen();
  //delay(1000);

  GLCD.SelectFont(System5x7);
  homedisplay();
  //homenavigate();
  //delay(1000);

  vol1.volume = 10.1;
  vol1.dose = 5;
  vol1.hour_on = 0;
  vol1.minute_on = 0;
  vol1.second_on = 10;
  
  vol1.hour_off = 0;
  vol1.minute_off = 0;
  vol1.second_off = 1;

  writeNormalVolume(1);
  
}

void  loop() {

/*********************************** Navigation Functionality ***********************************/
  switch (hstate) {
  case '1':
            switch (sstate) {
            case'0':
                    disp1();
                    break;
            case'1':
                   {
                    switch (tstate) {
                        case'0':
                        case'1':
                                disp1Aa();
                                break;
                        case'2':
                                input = '1';
                                disp1Ab();
                                break;
                        }
                    }
                    break;
            case'2':
                    {
                      switch (tstate) {
                        case'0':
                        case'1':
                                disp1Ba();
                                break;
                        case'2':
                                input = '1';
                                disp1Bb();
                                break;
                        }
                    }
                    break;
            }
            break;
  case '2':
            {
              switch (sstate) {
                  case'0':
                  case'1':
                          menuMode = 'F';
                          calibDisp(120.0,120.0);
                          break;
                  case'2':
                          break;
              }
            }
            break;
  case '3':
            {
                switch (sstate) {
                  case'A':
                  case'B':
                          break;
                }
            }
            break;
  case '4':
            {
              switch (sstate) {
                  case'0': disp4();
                  case'1': menuMode = 'V';
                           switch(fstate){
                                  case'1':{
                                            switch (tstate)
                                            {
                                            case '0':
                                              //input = '1';
                                              disp4A();
                                              break;
                                            case '1':
                                              disp4Aa();
                                              break;
                                            case'2':
                                              disp4B();
                                              break;
                                            case'3':
                                              disp4C();
                                              break;
                                             }
                                          }
                                          break;
                                  case '2':
                                          {
                                            switch (tstate){
                                              case'0':
                                                      //input = '4';
                                                      dispvolpara2();
                                                      break;
                                              case'4':
                                                      disp4D();
                                                      break;
                                              case'5':
                                                      disp4Evolume();
                                                      break;
                                              case '6':
                                                      disp4F();
                                                      break;
                                              }
                                          }
                                 }
                   case'2': menuMode = 'R';
                           switch(fstate){
                                  case'1':{
                                            switch (tstate)
                                            {
                                            case '0':
                                              //input = '1';
                                              disp4A();
                                              break;
                                            case '1':
                                              disp4Aa();
                                              break;
                                            case'2':
                                              disp4B();
                                              break;
                                            case'3':
                                              disp4C();
                                              break;
                                             }
                                          }
                                          break;
                                  case '2':
                                          {
                                            switch (tstate){
                                              case'0':
                                                      //input = '4';
                                                      dispvolpara2();
                                                      break;
                                              case'4':
                                                      disp4D();
                                                      break;
                                              case'5':
                                              
                                                      disp4Erate();
                                                      break;
                                              case '6':
                                                      disp4F();
                                                      break;
                                              }
                                          }
                          }
                }
            }
            break;
  case '5':
          {
            switch (sstate) {
                case'1':
                        switch(tstate){
                            case'0': 
                                    disp5A();
                                    break;          
                            case'1': 
                                    vol = getvol();
                                    break;
                            case'2':
                                    disp4Efoot();
                                    break;
                            case'3':
                                    disp5C();
                                    break;
                        } 
                        break;
                case'2':
                        copyMode();
                        break;
                case'0':
                        voldisplay();
                        break;
      
              }
           }
           break;
  case '6':
           switch (sstate) {
            case'0':
                    disp6();
                    break;
            case'1':
                   {
                    switch (tstate) {
                        case'0':
                                disp6A();
                                break;
                        case'1':
                                disp6Aa();
                                break;
                       case'2':
                                disp6Ab();
                                break;
                        }
                    }
                    break;
            case'2':
                    {
                      switch (tstate) {
                        case'0':
                                disp6B();
                                break;
                        }
                    }
                    break;
            }
            break;
      
 case '0':
          homedisplay();    
   }

 
}

/*********************************** Home Screen Block **********************************/
void homedisplay() {
  GLCD.ClearScreen();
  GLCD.DrawBitmap(homescreen, 0, 0, BLACK);
  delay(100);
  GLCD.CursorTo(0, 3);
  GLCD.print("Config");
  GLCD.CursorTo(8, 3);
  GLCD.print("Calib");
  GLCD.CursorTo(15, 3);
  GLCD.print("Serial");

  GLCD.CursorTo(0, 7);
  GLCD.print("Dosing");
  GLCD.CursorTo(9, 7);
  GLCD.print("Foot");
  GLCD.CursorTo(15, 7);
  GLCD.print("Profil");

  homeinvert(hstate);
  //GLCD.InvertRect(0,0,40,32);

  homenavigate();
}

void homenavigate() {
  char customKey = '1';
  while (customKey != '#')
  {
    customKey = customKeypad.getKey();
    heffect(customKey);
    priming();
  }
  
}

void homeinvert(char a) {
  switch (a) {
      case '1' :
        GLCD.InvertRect(0, 0, 40, 32);
        break;
      case '2' :
        GLCD.InvertRect(40, 0, 50, 32);
        break;
      case '3' :
        GLCD.InvertRect(90, 0, 38, 32);
        break;
      case '4' :
        GLCD.InvertRect(0, 32, 40, 32);
        break;
      case '5' :
        GLCD.InvertRect(40, 32, 50, 32);
        break;
      case '6' :
        GLCD.InvertRect(90, 32, 38, 32);
        break;
      default  :
        break;
      }
  }
  
  void heffect(char a) {
    switch (a) {
        case 'A' :
                  homeinvert(hstate);
                  hstate--;
                  if (hstate == '0')
                    hstate = '3';
                  else if (hstate == '3')
                    hstate = '6';
                  homeinvert(hstate);
                  break;
      
        case 'B' :
                  homeinvert(hstate);
                  hstate++;
                  if (hstate == '4')
                    hstate = '1';
                  else if (hstate == '7')
                    hstate = '4';
                  homeinvert(hstate);
                  break;
      
        case 'C':
                  homeinvert(hstate);
                  if (hstate < '4')
                    hstate += 3;
                  else
                    hstate -= 3;
                  homeinvert(hstate);
                  break;
      
        case 'D' :
                  homeinvert(hstate);
                  if (hstate < '4')
                    hstate += 3;
                  else
                    hstate -= 3;
                  homeinvert(hstate);
                  break;
      }
}

/******************************************** Display 1 *******************************************/
void disp1()
{
  one();
  char customKey = '1';
  //GLCD.InvertRect(12,23,114,10);
  invert1A(customKey);
  while (customKey != '#' && customKey != '*')
  {
    disp1navigate(customKey);
    customKey = customKeypad.getKey();
  }
  if (customKey == '*')
  {
    back();
  }

}

void disp1Aa() {
  GLCD.DrawBitmap(rightarrow, 0, 0, BLACK);
  pumpmenua();
  tstate = '1';
  input = '1';
  invert1Aa(input);
  char customKey = customKeypad.getKey();

  while (customKey != '#' && customKey != '*' && customKey != 'B'  )
  {
    customKey = customKeypad.getKey();
    disp1Aanavigate(customKey);
  }
  if (customKey == 'B')
  {
    tstate = '2';
  }
  else if (customKey == '*' )
  {
    back();
  }
  else if (customKey == '#')
  {
    strcpy(pump, "done");
    back();
  }
}

void disp1Ab()
{
  GLCD.ClearScreen();
  GLCD.DrawBitmap(leftarrow, 0, 0, BLACK);
  pumpmenub();
  invert1Aa(input);
  char customKey = customKeypad.getKey();

  while (customKey != '#' && customKey != '*' && customKey != 'A')
  {
    customKey = customKeypad.getKey();
    disp1Aanavigate(customKey);
  }
  if (customKey == 'A')
  {
    tstate = '1';
  }
  else if (customKey == '*')
  {
    back();
  }
  else
  {
    strcpy(pump, "done");
  }
}

void disp1Ba()
{
  GLCD.ClearScreen();
  GLCD.DrawBitmap(rightarrow, 0, 0, BLACK);
  tstate = '1';
  tubemenua();
  input = '1';
  invert1Aa(input);
  char customKey = customKeypad.getKey();

  while (customKey != '#' && customKey != '*' && customKey != 'B')
  {
    customKey = customKeypad.getKey();
    disp1Aanavigate(customKey);
  }
  if (customKey == 'B')
  {
    tstate = '2';
  }
  else if (customKey == '*')
  {
    back();
  }
  else if (customKey == '#')
  {
    back();
    strcpy(tube, "done");
  }
}

void disp1Bb()
{
  GLCD.ClearScreen();
  GLCD.DrawBitmap(leftarrow, 0, 0, BLACK);
  tubemenub();
  input = '1';
  invert1Aa(input);
  char customKey = customKeypad.getKey();

  while (customKey != '#' && customKey != '*' && customKey != 'A')
  {
    customKey = customKeypad.getKey();
    disp1Aanavigate(customKey);
  }
  if (customKey == 'A')
  {
    tstate = '1';
  }
  else if (customKey == '*')
  {
    back();
  }
  else if (customKey == '#')
  {
    back();
    strcpy(tube, "done");
    //setDelta1();
  }
}

/******************************************** Pump Menu *******************************************/
void pumpmenua() {
  GLCD.CursorTo(7, 0);
  GLCD.print("1)DG6-1");
  GLCD.CursorTo(7, 1);
  GLCD.print("2)DG10-1");
  GLCD.CursorTo(7, 2);
  GLCD.print("3)DG10-2");
  GLCD.CursorTo(7, 3);
  GLCD.print("4)DG6-4");
  GLCD.CursorTo(7, 4);
  GLCD.print("5)DG10-4");
  GLCD.CursorTo(7, 5);
  GLCD.print("6)DG6-8");
  GLCD.CursorTo(7, 6);
  GLCD.print("7)DG10-8");
  GLCD.CursorTo(7, 7);
  GLCD.print("8)YZ15");
}

void pumpmenub() {
  GLCD.CursorTo(7, 0);
  GLCD.print("1)YZ25");
  GLCD.CursorTo(7, 1);
  GLCD.print("2)2xYZ15");
  GLCD.CursorTo(7, 2);
  GLCD.print("3)2xYZ25");
  GLCD.CursorTo(7, 3);
  GLCD.print("4)4xYZ15");
  GLCD.CursorTo(7, 4);
}

/******************************************** Tube Menu *******************************************/
void tubemenua() {
  GLCD.CursorTo(7, 0);
  GLCD.print("1)type 1");
  GLCD.CursorTo(7, 1);
  GLCD.print("2)type 2");
  GLCD.CursorTo(7, 2);
  GLCD.print("3)type 3");
  GLCD.CursorTo(7, 3);
  GLCD.print("4)type 4");
  GLCD.CursorTo(7, 4);
  GLCD.print("5)type 5");
  GLCD.CursorTo(7, 5);
  GLCD.print("6)type 6");
  GLCD.CursorTo(7, 6);
  GLCD.print("7)type 7");
  GLCD.CursorTo(7, 7);
  GLCD.print("8)type 8");
}

void tubemenub() {
  GLCD.CursorTo(7, 0);
  GLCD.print("1)type 9");
  GLCD.CursorTo(7, 1);
  GLCD.print("2)type 10");
  GLCD.CursorTo(7, 2);
  GLCD.print("3)type 11");
  GLCD.CursorTo(7, 3);
  GLCD.print("4)type 12");
  GLCD.CursorTo(7, 4);
  GLCD.print("5)type 13");
  GLCD.CursorTo(7, 5);
  GLCD.print("6)type 14");
  GLCD.CursorTo(7, 6);
  GLCD.print("7)type 15");
  GLCD.CursorTo(7, 7);
  GLCD.print("8)type 16");
}

/******************************************** Project Title*******************************************/
void openingscreen() {
  GLCD.SelectFont(Verdana12);
  GLCD.CursorTo(0, 1);
  GLCD.print("Design & Development");
  GLCD.CursorTo(0, 2);
  GLCD.print("   of Peristaltic Pump : ");
  GLCD.CursorTo(0, 3);
  GLCD.print("Feature Enhancement");
}

/******************************************** Pump Head Selection *******************************************/
void one()
{
  GLCD.ClearScreen();
  GLCD.CursorTo(1, 1);
  GLCD.print("Select :");
  GLCD.CursorTo(1, 3);
  GLCD.Printf(" 1)Pump Head: %s" , pump);
  GLCD.CursorTo(1, 5);
  GLCD.Printf(" 2)Tube Size: %s" , tube);
}

void back() {
  if (tstate != '0')
  {
    tstate = '0';
    sstate = '0';
  }
  else if (sstate != '0')
  {
    sstate = '0';
    homedisplay();
  }
}

void disp1navigate(char customKey)
{
  switch (customKey) {
  case 'C':
    customKey = '1';
    break;
  case 'D':
    customKey = '2';
    break;
  }
  if (customKey == '1' || customKey == '2' )
  {
    invert1A(sstate);
    sstate = customKey;
    //tstate='1';
    invert1A(sstate);
  }
}

void invert1A(char c)
{
  switch (c)
  {
  default:
  case 'C':
  case '1':
    GLCD.InvertRect(12, 23, 114, 10);
    break;
  case 'D':
  case '2':
    GLCD.InvertRect(12, 39, 114, 10);
    break;
  }
}

void disp1Aanavigate(char customKey)
{
  if (customKey == '1' || customKey == '2' || customKey == '3' || customKey == '4' || customKey == '5' || customKey == '6' || customKey == '7' || customKey == '8')
  {
    invert1Aa(input);
    input = customKey;
    invert1Aa(input);
  }
}

void invert1Aa(char c)
{
  switch (c)
  {
  default:
    break;
  case '1':
    GLCD.InvertRect(35, 0, 58, 7);
    break;
  case '2':
    GLCD.InvertRect(35, 7, 58, 8);
    break;
  case '3':
    GLCD.InvertRect(35, 15, 58, 8);
    break;
  case '4':
    GLCD.InvertRect(35, 23, 58, 8);
    break;
  case '5':
    GLCD.InvertRect(35, 31, 58, 8);
    break;
  case '6':
    GLCD.InvertRect(35, 39, 58, 8);
    break;
  case '7':
    GLCD.InvertRect(35, 47, 58, 8);
    break;
  case '8':
    GLCD.InvertRect(35, 55, 58, 8);
    break;
  }
}

void disp4(){
  sstate='1';
  fstate='0';
  tstate='0';
  GLCD.ClearScreen();
  GLCD.DrawBitmap(dosedisp, 0, 0, BLACK);
  GLCD.CursorTo(2, 7);
  GLCD.print("Volume");
  GLCD.CursorTo(12, 7);
  GLCD.print("Flow Rate");
  GLCD.InvertRect(0, 0, 65, 64);
  dosenavigate();
}

void dosenavigate() {
  char customKey = '1';
  fstate = '1';
  while (customKey != '#' && customKey != '*')
  {
    customKey = customKeypad.getKey();
    deffect(customKey);
  }
  if (customKey == '*'){
    fstate = '0';
    back();
  }
}

void deffect(char customKey) {
  switch (customKey) {
  case 'A':
    customKey = '1';
    break;
  case 'B':
    customKey = '2';
    break;
  }
  if (customKey == '1' || customKey == '2' )
  {
    invert2(sstate);
    sstate = customKey;
    invert2(sstate);
  }
}

void voldisplay() {
  GLCD.ClearScreen();
  GLCD.DrawBitmap(footselect, 0, 0, BLACK);
  // delay(100);

  GLCD.CursorTo(1, 6);
  GLCD.print("Foot");
  GLCD.CursorTo(4, 7);
  GLCD.print("Switch");

  GLCD.CursorTo(12, 6);
  GLCD.print("Copy");
  GLCD.CursorTo(15, 7);
  GLCD.print("Mode");

  GLCD.InvertRect(0, 0, 65, 64);
  volnavigate();
}

void volnavigate() {
  char customKey = '1';
  sstate = '1';
  while (customKey != '#' && customKey != '*')
  {
    customKey = customKeypad.getKey();
    veffect(customKey);
  }
  if (customKey == '*')
    back();
}

void veffect(char customKey) {
  switch (customKey) {
  case 'A':
    customKey = '1';
    break;
  case 'B':
    customKey = '2';
    break;
  }
  if (customKey == '1' || customKey == '2' )
  {
    invert2(sstate);
    sstate = customKey;
    invert2(sstate);
  }
}

void invert2(char c)
{
  switch (c)
  {
  default:
    break;
  case '1':
    GLCD.InvertRect(0, 0, 65, 64);
    break;
  case '2':
    GLCD.InvertRect(66, 0, 128, 64);
    break;
  }
}

void disp4A()
{ 
  input = '1';
  tstate='1';
  GLCD.ClearScreen();
  dispvolpara1();
  invert4A(input);
  char customKey = customKeypad.getKey();

  while (customKey != '#' && customKey != '*' && customKey != 'B')
  {
    disp4A1navigate(customKey);
    customKey = customKeypad.getKey();
  }
  if (customKey == '*')
  {
    fback();
  }
  else if (customKey == 'B')
  {
    tstate = '0';
    fstate = '2'; //dispvolpara2()
  }
}

void dispvolpara1()
{
  GLCD.DrawBitmap(volright, 0, 0, BLACK);
  GLCD.CursorTo(5, 0);
  GLCD.Printf("Parameters");
  GLCD.CursorTo(0, 2);
  char arr[10];
  if(sstate == '1'){
    sprintf(arr, "%5.2f", vol);
    GLCD.Printf("1) Volume :%sml", buffer1);
  }
  if(sstate == '2'){
    sprintf(arr, "%5.2f", rate);
    GLCD.Printf("1) Flowrate:%s", buffer1);
    GLCD.CursorTo(4,3);
    GLCD.Printf("(ml/min)");
  }
  GLCD.CursorTo(0, 4);
  GLCD.Printf("2) No of doses: %d", dose);
  GLCD.CursorTo(0, 6);
  GLCD.Printf("3) ON Time-%d:%d:%d", hour_on, minute_on, second_on);
}

void dispvolpara2()
{
  input = '4';
  tstate = '4';
  GLCD.ClearScreen();
  GLCD.DrawBitmap(volleft, 0, 0, BLACK);
  GLCD.CursorTo(5, 0);
  GLCD.Printf(" Parameters");
  GLCD.CursorTo(4, 2);
  GLCD.Printf("4)OFF Time-%d:%d:%d", hour_off, minute_off, second_off);
  GLCD.CursorTo(4, 4);
  GLCD.Printf("5)Save ");
  GLCD.CursorTo(4, 6);
  GLCD.Printf("6)Start ");
  invert4A(input);
  char customKey = customKeypad.getKey();
  while (customKey != '#' && customKey != '*' && customKey != 'A')
  {
    disp4A2navigate(customKey);
    customKey = customKeypad.getKey();
  }
  if (customKey == '*')
  {
    fback();
  }
  else if (customKey == 'A')
  {
    fstate = '1'; //dispvolpara1();
    tstate = '0';
  }
}

/******************************************** Read Volume Data From Keypad *******************************************/
float getvol()
{
  GLCD.ClearScreen();
  GLCD.DrawBitmap(inpvol, 0, 0, BLACK);
  float v = 0;
  GLCD.SelectFont(Arial14);
  char digit;
  digit = customKeypad.getKey();
  while (digit != '#' && digit != '*' )
  {
    if (digit != NO_KEY)
      v = v * 10.0 + (digit - '0');
    displayFloat(v, 3, 2);
    digit = customKeypad.getKey();
  }
  splback();
  GLCD.SelectFont(System5x7);
  return v;
}

void disp4Aa()
{
  if(sstate == '1')
    vol = getvol();
  else if(sstate == '2')
    rate = getvol();
}
/********************************************************************************************************************/

void speedmenu1()
{
  GLCD.CursorTo(7, 0);
  GLCD.print("1)10");
  GLCD.CursorTo(7, 1);
  GLCD.print("2)50");
  GLCD.CursorTo(7, 2);
  GLCD.print("3)100");
  GLCD.CursorTo(7, 3);
  GLCD.print("4)200");
  GLCD.CursorTo(7, 4);
  GLCD.print("5)300");
  GLCD.CursorTo(7, 5);
  GLCD.print("6)400");
  GLCD.CursorTo(7, 6);
  GLCD.print("7)500");
  GLCD.CursorTo(7, 7);
  GLCD.print("8)600");
}

void disp4B()
{
  GLCD.ClearScreen();
  GLCD.DrawBitmap(border, 0, 0, BLACK);
  GLCD.CursorTo(3, 2);
  dose = 0;
  GLCD.print("Enter No of Doses");
  GLCD.SelectFont(Arial14);
  char digit;
  digit = customKeypad.getKey();
  while (digit != '#' && digit != '*' )
  {
    if (digit != NO_KEY)
      dose = dose * 10 + (digit - '0');

    GLCD.CursorTo(4, 2);
    GLCD.print(dose);
    digit = customKeypad.getKey();
  }
  splback();
  GLCD.SelectFont(System5x7);
}

void disp4C()
{
  int i = 0;
  hour_on = 0;
  minute_on = 0;
  second_on = 0;
  GLCD.ClearScreen();
  GLCD.DrawBitmap(border, 0, 0, BLACK);
  GLCD.CursorTo(3, 1);
  GLCD.print("Enter ON Time :");
  GLCD.CursorTo(5, 3);
  GLCD.print("( HH:MM:SS )");
  GLCD.CursorTo( 4, 5);
  char digit;
  digit = customKeypad.getKey();
  while (digit != '#' && digit != '*' && i < 2 )
  {
    if (digit != NO_KEY)
    {
      hour_on = hour_on * 10 + (digit - '0');
      i++;
    }
    GLCD.SelectFont(System5x7);
    GLCD.CursorTo(7, 4);
    GLCD.SelectFont(Arial14);
    GLCD.Printf("%d:%d:%d", hour_on , minute_on, second_on);
    digit = customKeypad.getKey();
  }
  if (digit == '*')
    splback();
  digit = customKeypad.getKey();

  while (digit != '#' && digit != '*' && i < 4 )
  {
    if (digit != NO_KEY)
    {
      minute_on = minute_on * 10 + (digit - '0');
      i++;
    }
    GLCD.SelectFont(System5x7);
    GLCD.CursorTo(7, 4);
    GLCD.SelectFont(Arial14);
    GLCD.Printf("%d:%d:%d", hour_on, minute_on, second_on);
    digit = customKeypad.getKey();
  }
  if (digit == '*')
    splback();
  digit = customKeypad.getKey();

  while (digit != '#' && digit != '*' && i < 6 )
  {
    if (digit != NO_KEY)
    {
      second_on = second_on * 10 + (digit - '0');
      i++;
    }
    GLCD.SelectFont(System5x7);
    GLCD.CursorTo(7, 4);
    GLCD.SelectFont(Arial14);
    GLCD.Printf("%d:%d:%d", hour_on, minute_on, second_on);
    digit = customKeypad.getKey();
  }
  GLCD.SelectFont(System5x7);
  splback();
}

void disp4D()
{
  int i = 0;
  hour_off = 0;
  minute_off = 0;
  second_off = 0;
  GLCD.ClearScreen();
  GLCD.DrawBitmap(border, 0, 0, BLACK);
  GLCD.CursorTo(3, 1);
  GLCD.print("Enter OFF Time :");
  GLCD.CursorTo(5, 3);
  GLCD.print("( HH:MM:SS )");
  GLCD.CursorTo( 4, 5);
  char digit;
  digit = customKeypad.getKey();
  while (digit != '#' && digit != '*' && i < 2 )
  {
    if (digit != NO_KEY)
    {
      hour_off = hour_off * 10 + (digit - '0');
      i++;
    }
    GLCD.SelectFont(System5x7);
    GLCD.CursorTo(7, 4);
    GLCD.SelectFont(Arial14);
    GLCD.Printf("%d:%d:%d", hour_off, minute_off, second_off);
    digit = customKeypad.getKey();
  }
  if (digit == '*')
    splback();
  digit = customKeypad.getKey();

  while (digit != '#' && digit != '*' && i < 4 )
  {
    if (digit != NO_KEY)
    {
      minute_off = minute_off * 10 + (digit - '0');
      i++;
    }
    GLCD.SelectFont(System5x7);
    GLCD.CursorTo(7, 4);
    GLCD.SelectFont(Arial14);
    GLCD.Printf("%d:%d:%d", hour_off, minute_off, second_off);
    digit = customKeypad.getKey();
  }
  if (digit == '*')
    splback();
  digit = customKeypad.getKey();

  while (digit != '#' && digit != '*' && i < 6 )
  {
    if (digit != NO_KEY)
    {
      second_off = second_off * 10 + (digit - '0');
      i++;
    }
    GLCD.SelectFont(System5x7);
    GLCD.CursorTo(7, 4);
    GLCD.SelectFont(Arial14);
    GLCD.Printf("%d:%d:%d", hour_off, minute_off, second_off);
    digit = customKeypad.getKey();
  }
  GLCD.SelectFont(System5x7);
  splback();
}

void disp4A1navigate(char customKey)
{
  if (customKey == '1' || customKey == '2' || customKey == '3')
    effect(customKey);
}

void disp4A2navigate(char customKey)
{
  if ( customKey == '4' || customKey == '5' || customKey == '6' )
    effect(customKey);
}

void effect(char customKey)
{
  invert4A(tstate );
  tstate = customKey;
  invert4A(tstate );
}

void splback()
{
  if (tstate != '0')
    tstate = '0';
}

void fback()
{
  sstate = '0';
  fstate = '0';
  tstate = '0';
}

void disp4F()
{
  tstate = '6';

  ton = hour_on * 3600 + minute_on * 60 + second_on;
  toff = hour_off * 3600 + minute_off * 60 + second_off;

  if(sstate == '2'){
    float tmin = (float)ton/60.0;
    vol = rate * tmin;
  }
  discreteDisp(ton, toff);

  splback();
  GLCD.SelectFont(System5x7);
}

void invert4A(char ch)
{
  switch (ch)
  {
  case '1':
    GLCD.InvertRect(00, 15, 110, 9);
    break;
  case '2':
    GLCD.InvertRect(00, 31, 110, 10);
    break;
  case '3':
    GLCD.InvertRect(00, 46, 110, 10);
    break;
  case '4':
    GLCD.InvertRect(20, 15, 108, 10);
    break;
  case '5':
    GLCD.InvertRect(20, 31, 108, 10);
    break;
  case '6':
    GLCD.InvertRect(20, 46, 108, 10);
    break;
  }
}

void disp4Evolume()
{
  GLCD.ClearScreen();
  GLCD.print("Select Profile Number...");
  delay(500);

  saveVolume();
  splback();
}

void saveVolume()
{
  GLCD.ClearScreen();
  profileMenu();
  input = '1';
  char customKey = '1', prevKey = '1';
  invert1Aa(customKey);
  while (customKey != '#' && customKey != '*')
  {
    
    if(customKey == 'C')
    {
      if(prevKey == '1')
        prevKey = '8';
      else
      prevKey -= 1;
    }
    else if(customKey == 'D')
    {
      if(prevKey == '8')
        prevKey = '1';
      else
      prevKey += 1;
    }
    else
    prevKey = customKey;
    disp6Aanavigate(customKey);
    
    customKey = customKeypad.waitForKey(); 
  }
  if (customKey == '#')
  {
    saveProfileVolume(parseChar(prevKey)); 
  }
  else if (customKey == '*')
  {
    splback();
  }
}

void saveProfileVolume(int num)
{
  char customKey = '1';
  readNormalVolume(num);
  GLCD.ClearScreen();
  GLCD.CursorTo(2, 2);
  GLCD.print("Volume  :");
  GLCD.print(vol1.volume);
  GLCD.CursorTo(2, 3);
  GLCD.print("Dose    :");
  GLCD.print(vol1.dose);
  GLCD.CursorTo(2, 4);
  GLCD.print("Ontime  :");
  GLCD.Printf("%d:%d:%d", vol1.hour_on , vol1.minute_on, vol1.second_on);
  GLCD.CursorTo(2, 5);
  GLCD.print("Offtime :");
  GLCD.Printf("%d:%d:%d", vol1.hour_off , vol1.minute_off, vol1.second_off);
  GLCD.CursorTo(2, 6);
  GLCD.print("Rewrite Profile");
  GLCD.InvertRect(12, 47, 108, 8);

  
  while (customKey != '#' && customKey != '*')
  {    
    customKey = customKeypad.waitForKey();
  }
  if (customKey == '#')
  {
    // Need to check whether all parameters are entered correctly, but have left for future work.
    Serial.println(vol);
    vol1.volume = vol;
    vol1.dose = dose;
    vol1.hour_on = hour_on; vol1.minute_on = minute_on; vol1.second_on = second_on;
    vol1.hour_off = hour_off; vol1.minute_off = minute_off; vol1.second_off = second_off;
    writeNormalVolume(num);

    
    GLCD.ClearScreen();
    GLCD.print("Saving Profile!!");
    delay(1500);
  }
  else if (customKey == '*')
  {
    splback();
  }
}

void disp4Erate()
{
  GLCD.ClearScreen();
  GLCD.print("Select Profile Number...");
  delay(500);

  saveRate();
  splback();
}

void saveRate()
{
  GLCD.ClearScreen();
  profileMenu();
  input = '1';
  char customKey = '1', prevKey = '1';
  invert1Aa(customKey);
  while (customKey != '#' && customKey != '*')
  {
    
    if(customKey == 'C')
    {
      if(prevKey == '1')
        prevKey = '8';
      else
      prevKey -= 1;
    }
    else if(customKey == 'D')
    {
      if(prevKey == '8')
        prevKey = '1';
      else
      prevKey += 1;
    }
    else
    prevKey = customKey;
    disp6Aanavigate(customKey);
    
    customKey = customKeypad.waitForKey(); 
  }
  if (customKey == '#')
  {
    saveProfileRate(parseChar(prevKey)); 
  }
  else if (customKey == '*')
  {
    splback();
  }
}

void saveProfileRate(int num)
{
  char customKey = '1';
  readNormalTime(num);
  GLCD.ClearScreen();
  GLCD.CursorTo(2, 2);
  GLCD.print("Rate    :");
  GLCD.print(time1.rate);
  GLCD.CursorTo(2, 3);
  GLCD.print("Dose    :");
  GLCD.print(time1.dose);
  GLCD.CursorTo(2, 4);
  GLCD.print("Ontime  :");
  GLCD.Printf("%d:%d:%d", time1.hour_on , time1.minute_on, time1.second_on);
  GLCD.CursorTo(2, 5);
  GLCD.print("Offtime :");
  GLCD.Printf("%d:%d:%d", time1.hour_off , time1.minute_off, time1.second_off);
  GLCD.CursorTo(2, 6);
  GLCD.print("Rewrite Profile");
  GLCD.InvertRect(12, 47, 108, 8);

  
  while (customKey != '#' && customKey != '*')
  {    
    customKey = customKeypad.waitForKey();
  }
  if (customKey == '#')
  {
    // Need to check whether all parameters are entered correctly, but have left for future work.
    Serial.println(vol);
    time1.rate = rate;
    time1.dose = dose;
    time1.hour_on = hour_on; time1.minute_on = minute_on; time1.second_on = second_on;
    time1.hour_off = hour_off; time1.minute_off = minute_off; time1.second_off = second_off;
    writeNormalTime(num);

    
    GLCD.ClearScreen();
    GLCD.print("Saving Profile!!");
    delay(1500);
  }
  else if (customKey == '*')
  {
    splback();
  }
}




void disp4Efoot()
{
  GLCD.ClearScreen();
  GLCD.print("Select Profile Number...");
  delay(500);

  saveFoot();
  splback();
}

void saveFoot()
{
  GLCD.ClearScreen();
  profileMenu();
  input = '1';
  char customKey = '1', prevKey = '1';
  invert1Aa(customKey);
  while (customKey != '#' && customKey != '*')
  {
    
    if(customKey == 'C')
    {
      if(prevKey == '1')
        prevKey = '8';
      else
      prevKey -= 1;
    }
    else if(customKey == 'D')
    {
      if(prevKey == '8')
        prevKey = '1';
      else
      prevKey += 1;
    }
    else
    prevKey = customKey;
    disp6Aanavigate(customKey);
    
    customKey = customKeypad.waitForKey(); 
  }
  if (customKey == '#')
  {
    saveProfileFoot(parseChar(prevKey)); 
  }
  else if (customKey == '*')
  {
    splback();
  }
}

void saveProfileFoot(int num)
{
  char customKey = '1';
  readFS(num);
  GLCD.ClearScreen();
  
  GLCD.CursorTo(3, 3);
  GLCD.print("Volume :");
  GLCD.print(foot.volume);
  GLCD.CursorTo(3, 5);
  GLCD.print(" Rewrite Profile");
  GLCD.InvertRect(18, 39, 102, 8);

  
  while (customKey != '#' && customKey != '*')
  {    
    customKey = customKeypad.waitForKey();
  }
    
  if (customKey == '#')
  {
    foot.volume = vol;
    writeFS(num);

    
    GLCD.ClearScreen();
    GLCD.print("Saving Profile!!");
    delay(1500);
  }
  else if (customKey == '*')
  {
    splback();
  }
}


void disp5A()
{ 
  GLCD.ClearScreen();
  GLCD.CursorTo(5, 0);
  GLCD.Printf("Parameters");
  GLCD.CursorTo(0, 2);
  char arr[10];
  GLCD.Printf("1) Volume :%sml", buffer1);
  sprintf(arr, "%5.2f", vol);
  GLCD.CursorTo(0, 4);
  GLCD.Printf("2) Save Profile");
  GLCD.CursorTo(0, 6);
  GLCD.Printf("3) Start");
  input = '1';
  tstate='1';
  invert4A(input);
  char customKey = customKeypad.getKey();

  while (customKey != '#' && customKey != '*')
  {
    disp4A1navigate(customKey);
    customKey = customKeypad.getKey();
  }
  if (customKey == '*')
  {
    //splback();
    back();
  }
}

/******************************************** Motor Related Functions *******************************************/
void setDelta1() {
  delta1 = 2.0 * pi * pi * inRad * inRad * pumpRad / 1000.0;
  delta2 = delta1;
  delta3 = delta1;
}

void setRpm(float r) {
  float rps = r / 60.00;
  int oneRotation = 200 * mode;
  float nPul = rps * (float)oneRotation;
  float tdelay = 1.0 / (2.00 * nPul);
  tdelay = tdelay * 1000000.0;
  Timer4.initialize(tdelay);
}

void copyMode() {
  GLCD.ClearScreen();
  GLCD.SelectFont(System5x7);
  GLCD.CursorTo(0,1);
  GLCD.Printf("Press the Foot Switch");
  GLCD.CursorTo(0,3);
  GLCD.Printf("to START dispensing...");
 
  char key = NO_KEY;
  while(digitalRead(footSwitch) != HIGH);
  key = NO_KEY;
  
  delay(500);

  GLCD.ClearScreen();
  GLCD.CursorTo(1,1);
  GLCD.Printf("Dispensing Liquid...");
  GLCD.CursorTo(0,3);
  GLCD.Printf("Press the Foot Switch");
  GLCD.CursorTo(1,5);
  GLCD.Printf("to STOP dispensing...");
  
  setRpm(120.0);
  Timer3.initialize(100);
  timerTickCopy = 0;
  noInterrupts();
  timerTick = 0;
  interrupts();
  digitalWrite(ena, HIGH);
  
  
  while(digitalRead(footSwitch) != HIGH);

  
  noInterrupts();
  timerTickCopy = timerTick;
  interrupts();
  digitalWrite(ena, LOW);
  key = NO_KEY;
  unsigned long comp = timerTickCopy;
  
  delay(500);
  
  GLCD.ClearScreen();
  GLCD.CursorTo(1,3);
  GLCD.Printf("Press Foot Switch to");
  GLCD.CursorTo(2,5);
  GLCD.Printf("START Copy Dose...");
  int doseNum = 0;     
  while(1){
  if( digitalRead(footSwitch) ){
        doseNum++;
        GLCD.ClearScreen();
        GLCD.CursorTo(1, 2);
        GLCD.Printf("Dispensing Copy Dose");
        GLCD.CursorTo(3, 4);
        GLCD.Printf("Dose Number - %d", doseNum);
        timerTickCopy = 0;
        noInterrupts();
        timerTick = 0;
        interrupts();
        digitalWrite(ena, HIGH);
        while(timerTickCopy < comp)
        {
          noInterrupts();
          timerTickCopy = timerTick;
          interrupts();
        }
       digitalWrite(ena, LOW);
       
       delay(500);
       
       GLCD.ClearScreen();
       GLCD.CursorTo(3, 1);
       GLCD.Printf("Press * to Exit");
       GLCD.CursorTo(1, 3);
       GLCD.Printf("Press Foot Switch to");
       GLCD.CursorTo(3, 5);
       GLCD.Printf("Repeat Copy Dose");
      }
  if(key == '*'){
       sstate = '0';
       break;
      }
  key = customKeypad.getKey();
   }
}

void calibMotor(float r, float v){
  setRpm(r);
  vol = v;

  Timer3.initialize(tPre);

  float time = v/(delta1*r)*60.0*1000000.0;
  unsigned long tCompOn = (unsigned long) time / tPre;

  timerTickCopy = 0;

  noInterrupts();
  timerTick = 0;
  interrupts();
  digitalWrite(ena, HIGH);

  while (timerTickCopy <= tCompOn) {
    noInterrupts();
    timerTickCopy = timerTick;
    interrupts();
  }
  digitalWrite(ena, LOW);
}

void discreteDisp(int onTime, int offTime) {
  setDelta1();
  float v1 = vol;
  Timer3.initialize(tPre);

  float t = (float)onTime/60.0;
  float n = vol/delta1;
  float rpm = n/t;

  if(calibQuery())
    calibDisp(rpm,100);

  GLCD.ClearScreen();
  GLCD.CursorTo(1, 1);
  GLCD.Printf("Dose dispense begins");
  delay(1000);

  vol = v1;

  t = (float)onTime/60.0;
  n = vol/delta3;
  rpm = n/t;
  setRpm(rpm);

  unsigned long tCompOn = onTime / tPre * 1000000;
  unsigned long tCompOff = offTime / tPre * 1000000;

  GLCD.ClearScreen();
  GLCD.DrawBitmap(volpump, 0, 0, BLACK);
  GLCD.SelectFont(System5x7);
  GLCD.CursorTo(15, 1);
  GLCD.Printf("Doses");
  GLCD.CursorTo(15, 3);
  GLCD.Printf("left:");
  GLCD.CursorTo(1, 5);
  GLCD.Printf("Target :");
  displayFloat(vol, 4, 6);
  GLCD.CursorTo(1, 1);
  GLCD.Printf("Dispensed :");
  GLCD.CursorTo(15, 5);
  GLCD.Printf("%d", dose);

  for (int i = 0; i < dose; i++) {
    int d = dose - (i+1);

    timerTickCopy = 0;

    noInterrupts();
    timerTick = 0;
    interrupts();
    digitalWrite(ena, HIGH);

    while (timerTickCopy <= tCompOn) {
      noInterrupts();
      timerTickCopy = timerTick;
      interrupts();
      d_vol = timerTickCopy * vol / tCompOn;
      displayFloat(d_vol, 6, 2);
    }
    digitalWrite(ena, LOW);

    timerTickCopy = 0;

    d_vol = 0;
    displayFloat(d_vol, 6, 2);
    GLCD.CursorTo(15, 5);
    GLCD.Printf("%d", d);

    noInterrupts();
    timerTick = 0;
    interrupts();

    while ((timerTickCopy <= tCompOff) && dose != 1) {
      noInterrupts();
      timerTickCopy = timerTick;
      interrupts();
    }
  }
}

void clkDelay() {
  digitalWrite(clk, !digitalRead(clk));
  clkCount++;
}

void bigDelay() {
  timerTick++;
}

void displayFloat(float num, int pos1, int pos2) {
  float f;
  int i1, i2;

  i1 = (int)num;
  f = (num - (float)i1) * 100;
  i2 = (int)f;

  sprintf(buffer1, "%-d.%-2d", i1, i2);
  GLCD.CursorTo(pos1, pos2);
  GLCD.Printf("%s", buffer1);
}

void priming() {
  //setDelta1();
  setRpm(120.0);

  while(digitalRead(readPriming) ) {
    digitalWrite(ena, HIGH);
    digitalWrite(primingIndicator, HIGH);
   // Serial.println("priming");
  }
 // Serial.println("Notpriming");
  digitalWrite(ena, LOW);
  digitalWrite(primingIndicator, LOW);
}

void disp5C()
{
  GLCD.ClearScreen();
  GLCD.CursorTo(1, 2);
  GLCD.Printf("Press Foot Switch");
  if( digitalRead(footSwitch) )
  {
  tstate = '3';
  int i;
  char customKey;
  d_vol = 0;

  GLCD.ClearScreen();
  GLCD.DrawBitmap(footdisp, 0, 0, BLACK);
  GLCD.SelectFont(System5x7);
  GLCD.CursorTo(1, 2);
  GLCD.Printf("Target :");
  displayFloat(vol, 9, 2);
  GLCD.CursorTo(1, 5);
  GLCD.Printf("Dispensed :");
  displayFloat(d_vol, 13, 5);

  setRpm(120.0);
  double nRotation = vol / delta2;
  double nPulse = nRotation * (float)mode * 200.0;
  long nPulInt = (long)nPulse;

  while(1) {
    d_vol = 0;

  GLCD.ClearScreen();
  GLCD.DrawBitmap(footdisp, 0, 0, BLACK);
  GLCD.SelectFont(System5x7);
  GLCD.CursorTo(1, 2);
  GLCD.Printf("Target :");
  displayFloat(vol, 9, 2);
  GLCD.CursorTo(1, 5);
  GLCD.Printf("Dispensed :");

    noInterrupts();
    clkCount = 0;
    interrupts();
    digitalWrite(ena, HIGH);

    clkCountCopy = 0;

    while (clkCountCopy / 2 <= nPulInt){
      noInterrupts();
      clkCountCopy = clkCount;
      interrupts();
      d_vol = vol * (float)clkCountCopy/(2.0 * (float)nPulInt);
      displayFloat(d_vol, 13, 5);
    }
    digitalWrite(ena, LOW);

    customKey=customKeypad.getKey();
    while(customKey != '*' && digitalRead(footSwitch)!= HIGH )
    {
      customKey=customKeypad.getKey();
      GLCD.ClearScreen();
      GLCD.CursorTo(1, 2);
      GLCD.Printf("Press Foot Switch to Continue or back to  exit");
    }
    if(customKey == '*')
    {
      splback();

      GLCD.SelectFont(System5x7);
      break;
    }
  }
}
}

void calibDisp(float r, float v)
{
  char key;
  float vol1 = 0;
  sstate='1';
  GLCD.ClearScreen();

  //GLCD.DrawBitmap(border, 0, 0, BLACK);
  GLCD.CursorTo(1,1);
  GLCD.Printf("PLease use Priming Switch to ensure the Tube is completely filled with liquid before calibrating !!!");

  key=customKeypad.getKey();
  while(key!='#')
  {
    key=customKeypad.getKey();
  }
  GLCD.ClearScreen();
  GLCD.DrawBitmap(border, 0, 0, BLACK);
  GLCD.CursorTo(3,3);
  GLCD.Printf("Calibration Dose");
  GLCD.CursorTo(3,5);
  GLCD.Printf(" In Progress... ");

  //setDelta1();
  calibMotor(r,v);

  GLCD.ClearScreen();
  GLCD.DrawBitmap(border, 0, 0, BLACK);
  GLCD.CursorTo(1,1);
  GLCD.Printf("Enter Dispensed volume:");

  vol1 = getvol();

  if(menuMode == 'F'){
    delta2 = calibFormula(vol1);
    GLCD.ClearScreen();
    GLCD.DrawBitmap(border, 0, 0, BLACK);
    GLCD.CursorTo(3,3);
    GLCD.Printf(" Calibrated for ");
    GLCD.CursorTo(3,4);
    GLCD.Printf("FootSwitch Mode!!");
    delay(2000);
    splback();    
    back();
  }
  if(sstate == '1'){
    delta3 = calibFormula(vol1);
    GLCD.ClearScreen();
    GLCD.DrawBitmap(border, 0, 0, BLACK);
    GLCD.CursorTo(3,3);
    GLCD.Printf("  Calibrated for  ");
    GLCD.CursorTo(1,4);
    GLCD.Printf("Volume Dosing Mode !!");
    delay(2000);
  }
  if(sstate == '2'){
    delta3 = calibFormula(vol1);
    GLCD.ClearScreen();
    GLCD.DrawBitmap(border, 0, 0, BLACK);
    GLCD.CursorTo(3,3);
    GLCD.Printf("   Calibrated for   ");
    GLCD.CursorTo(1,4);
    GLCD.Printf("Flow Rate Dosing Mode!!");
    delay(2000);
  }
}

float calibFormula(float v1){
  float err = v1 - vol;
  float d = delta1 + delta1*err/vol;
  return d;
}

boolean calibQuery(){
  GLCD.ClearScreen();
  GLCD.SelectFont(System5x7);
  GLCD.CursorTo(1, 1);
  GLCD.Printf("Do you want to");
  GLCD.CursorTo(1, 3);
  GLCD.Printf("caliberate?!");
  char key = customKeypad.getKey();
  while(key != '#' && key != '*'){
    key = customKeypad.getKey();
  }
  if(key == '#')
    return true;
  if(key == '*'){
    return false;
  }
}

/*********************************** Profile Save ******************************/

void disp6()
{
  six();
  char customKey = '1';
  //GLCD.InvertRect(12,23,114,10);
  invert1A(customKey);
  while (customKey != '#' && customKey != '*')
  {
    disp6navigate(customKey);
    customKey = customKeypad.getKey();
  }
  if (customKey == '*')
  {
    back();
  }
}

void disp6navigate(char customKey)
{
  switch (customKey)
  {
  case 'C':
    customKey = '1';
    break;
  case 'D':
    customKey = '2';
    break;
  }
  if (customKey == '1' || customKey == '2' )
  {
    invert1A(sstate);
    sstate = customKey;
    //tstate='1';
    invert1A(sstate);
  }
}

void six()
{
  GLCD.ClearScreen();
  GLCD.CursorTo(1, 1);
  GLCD.print("Select :");
  GLCD.CursorTo(1, 3);
  GLCD.Printf(" 1)Dosing Mode");
  GLCD.CursorTo(1, 5);
  GLCD.Printf(" 2)Foot Switch");
}

//////////////////////////////////////////////////////////////

void disp6A()
{
  volumeFR();
  char customKey = '1';
  //GLCD.InvertRect(12,23,114,10);
  invert1A(customKey);
  while (customKey != '#' && customKey != '*')
  {
    disp6Anavigate(customKey);
    customKey = customKeypad.getKey(); 
  }
  if (customKey == '*')
  {
    back();
  }
}

void volumeFR()
{
  GLCD.ClearScreen();
  GLCD.CursorTo(1, 1);
  GLCD.print("Select :");
  GLCD.CursorTo(1, 3);
  GLCD.Printf(" 1)Volume");
  GLCD.CursorTo(1, 5);
  GLCD.Printf(" 2)Flow Rate");
}

void disp6Anavigate(char customKey)
{
  switch (customKey) {
  case 'C':
    customKey = '1';
    break;
  case 'D':
    customKey = '2';
    break;
  }
  if (customKey == '1' || customKey == '2' )
  {
    invert1A(tstate);
    tstate = customKey;
    //tstate='1';
    invert1A(tstate);
  }
}

void disp6Aa()
{
  GLCD.ClearScreen();
  profileMenu();
  input = '1';
  char customKey = '1', prevKey = '1';
  invert1Aa(customKey);
  while (customKey != '#' && customKey != '*')
  {
    
    if(customKey == 'C')
    {
      if(prevKey == '1')
        prevKey = '8';
      else
      prevKey -= 1;
    }
    else if(customKey == 'D')
    {
      if(prevKey == '8')
        prevKey = '1';
      else
      prevKey += 1;
    }
    else
    prevKey = customKey;
    disp6Aanavigate(customKey);
    
    customKey = customKeypad.waitForKey(); 
  }
  if (customKey == '#')
  {
    readProfileAa(parseChar(prevKey)); 
  }
  else if (customKey == '*')
  {
    back();
  }
}

void disp6Aanavigate(char customKey)
{
  if (customKey == '1' || customKey == '2' || customKey == '3' || customKey == '4' || customKey == '5' || customKey == '6' || customKey == '7' || customKey == '8')
  {
    invert1Aa(input);
    input = customKey;
    invert1Aa(input);
  }
  else if (customKey == 'C')
  {
    invert1Aa(input);
    input -= 1;
    if(input == '0')
      input = '8';
    invert1Aa(input);
  }
  else if (customKey == 'D')
  {
    invert1Aa(input);
    input += 1;
    if(input == '9')
      input = '1';
    invert1Aa(input);
  }
  
}


void readProfileAa(int num)
{
  char customKey = '1';
  readNormalVolume(num);
  GLCD.ClearScreen();
  GLCD.CursorTo(2, 2);
  GLCD.print("Volume  :");
  GLCD.print(vol1.volume);
  GLCD.CursorTo(2, 3);
  GLCD.print("Dose    :");
  GLCD.print(vol1.dose);
  GLCD.CursorTo(2, 4);
  GLCD.print("Ontime  :");
  GLCD.Printf("%d:%d:%d", vol1.hour_on , vol1.minute_on, vol1.second_on);
  GLCD.CursorTo(2, 5);
  GLCD.print("Offtime :");
  GLCD.Printf("%d:%d:%d", vol1.hour_off , vol1.minute_off, vol1.second_off);
  GLCD.CursorTo(2, 6);
  GLCD.print(" Load Profile");
  GLCD.InvertRect(12, 47, 90, 8);

  
  while (customKey != '#' && customKey != '*')
  {    
    customKey = customKeypad.waitForKey();
  }
  if (customKey == '#')
  {
     vol = vol1.volume;
     dose = vol1.dose;
     hour_on = vol1.hour_on; minute_on = vol1.minute_on; second_on = vol1.second_on;
     hour_off = vol1.hour_off; minute_off = vol1.minute_on; second_off = vol1.second_off;
     disp4F();
  }
  else if (customKey == '*')
  {
    back();
  }
  
}

void disp6Ab()
{
  GLCD.ClearScreen();
  profileMenu();
  input = '1';
  char customKey = '1', prevKey = '1';
  invert1Aa(customKey);
  while (customKey != '#' && customKey != '*')
  {
    
    if(customKey == 'C')
    {
      if(prevKey == '1')
        prevKey = '8';
      else
      prevKey -= 1;
    }
    else if(customKey == 'D')
    {
      if(prevKey == '8')
        prevKey = '1';
      else
      prevKey += 1;
    }
    else
    prevKey = customKey;
    disp6Aanavigate(customKey);
    
    customKey = customKeypad.waitForKey(); 
  }
  if (customKey == '#')
  {
    readProfileAb(parseChar(prevKey)); 
  }
  else if (customKey == '*')
  {
    back();
  }
}

void readProfileAb(int num)
{
  char customKey = '1';
  readNormalTime(num);
  
  GLCD.ClearScreen();
  GLCD.CursorTo(2, 2);
  GLCD.print("Flow Rate :");
  GLCD.print(time1.rate);
  GLCD.CursorTo(2, 3);
  GLCD.print("Dose      :");
  GLCD.print(time1.dose);
  GLCD.CursorTo(2, 4);
  GLCD.print("Ontime    :");
  GLCD.Printf("%d:%d:%d", time1.hour_on , time1.minute_on, time1.second_on);
  GLCD.CursorTo(2, 5);
  GLCD.print("Offtime   :");
  GLCD.Printf("%d:%d:%d", time1.hour_off , time1.minute_off, time1.second_off);
  GLCD.CursorTo(2, 6);
  GLCD.print(" Load Profile");
  GLCD.InvertRect(12, 47, 96, 8);

  
  while (customKey != '#' && customKey != '*')
  {    
    customKey = customKeypad.waitForKey();
  }
  if (customKey == '#')
  {
    rate = time1.rate;
    dose = time1.dose;
    hour_on = time1.hour_on ; minute_on = time1.minute_on, second_on = time1.second_on ;
    hour_on = time1.hour_off ; minute_off = time1.minute_off, second_off = time1.second_off ;
    disp4F();
  }
  else if (customKey == '*')
  {
    back();
  }
  
}

//////////////////////////////////////////////////////////////////////////////

void disp6B()
{
  GLCD.ClearScreen();
  profileMenu();
  input = '1';
  char customKey = '1', prevKey = '1';
  invert1Aa(customKey);
  while (customKey != '#' && customKey != '*')
  {
    
    if(customKey == 'C')
    {
      if(prevKey == '1')
        prevKey = '8';
      else
      prevKey -= 1;
    }
    else if(customKey == 'D')
    {
      if(prevKey == '8')
        prevKey = '1';
      else
      prevKey += 1;
    }
    else
    prevKey = customKey;
    disp6Aanavigate(customKey);
    
    customKey = customKeypad.waitForKey(); 
  }
  if (customKey == '#')
  {
    readProfileB(parseChar(prevKey)); 
  }
  else if (customKey == '*')
  {
    hstate = '6';
    sstate = '0';
  }
}

void readProfileB(int num)
{
  char customKey = '1';
  readFS(num);
  GLCD.ClearScreen();
  
  GLCD.CursorTo(3, 3);
  GLCD.print("Volume :");
  GLCD.print(foot.volume);
  GLCD.CursorTo(3, 5);
  GLCD.print(" Load Profile");
  GLCD.InvertRect(18, 39, 84, 8);

  
  while (customKey != '#' && customKey != '*')
  {    
    customKey = customKeypad.waitForKey();
  }
  if (customKey == '#')
  {
    vol = foot.volume;
    disp5C();
  }
  else if (customKey == '*')
  {
    hstate = '6';
    sstate = '0';
  }
  
}

//////////////////////////////////////////////////////////////////

void profileMenu() {
  GLCD.CursorTo(6, 0);
  GLCD.print("Profile 1");
  GLCD.CursorTo(6, 1);
  GLCD.print("Profile 2");
  GLCD.CursorTo(6, 2);
  GLCD.print("Profile 3");
  GLCD.CursorTo(6, 3);
  GLCD.print("Profile 4");
  GLCD.CursorTo(6, 4);
  GLCD.print("Profile 5");
  GLCD.CursorTo(6, 5);
  GLCD.print("Profile 6");
  GLCD.CursorTo(6, 6);
  GLCD.print("Profile 7");
  GLCD.CursorTo(6, 7);
  GLCD.print("Profile 8");
}

int parseChar(char key)
{
  int num;
  switch(key){
    case '1':
            num = 1;
            break;    
    case '2':
            num = 2;
            break;
    case '3':
            num = 3;
            break;   
    case '4':
            num = 4;
            break;    
    case '5':
            num = 5;
            break;
    case '6':
            num = 6;
            break; 
    case '7':
            num = 7;
            break;    
    case '8':
            num = 8;
            break;
    case '9':
            num = 9;
            break; 
    default :
            num =0;
            break;                  
  }
  return num;
}

