// ___________________________Calculator on Arduino with LCD Touchscreen display HX8352A 400x240______________________________________//
// First working version 2.2
// Nurnberg 3.02.19


#include <math.h>
#include <stdint.h>
#include "TouchScreen.h"
#include <Adafruit_GFX.h>    
#include <MCUFRIEND_kbv.h>



uint8_t YP = A1;  // must be an analog pin, use "An" notation!
uint8_t XM = A2;  // must be an analog pin, use "An" notation!
uint8_t YM = 7;   // can be a digital pin
uint8_t XP = 6;   // can be a digital pin
uint8_t SwapXY = 0;


#define MINPRESSURE 10
#define MAXPRESSURE 1000


uint16_t TS_LEFT = 121;
uint16_t TS_RT  = 930;
uint16_t TS_TOP = 115;
uint16_t TS_BOT = 964;

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300); //300 is the sensitivity
MCUFRIEND_kbv tft;
TSPoint tp;


// Assign human-readable names to some common 16-bit color values:
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

String symbol[5][4] = {
  { "C", "", "%", "/" },
  { "7", "8", "9", "*" },
  { "4", "5", "6", "-" },
  { "1", "2", "3", "" },
  { "0", ",", "=", "+" }
};

 int X, Y, comma = 0, equal = 0, actions = 0, count;
 double Num1, Num2, Number;
 char action;
 boolean result = false;
 



void setup(void) {
  Serial.begin(9600);
  tft.reset();
  tft.begin(0x5252);                                                    //to enable HX8352A driver code
  tft.fillScreen(BLACK);

  IntroScreen();
  
  draw_BoxNButtons();  

}

void loop() {
  TSPoint p = waitTouch();
   X = p.x; Y = p.y;
  Serial.print(X); Serial.print(','); Serial.println(Y);// + " " + Y);

  DetectButtons();

  if (result==true) CalculateResult();

  DisplayResult();   

  delay(300);
}

TSPoint waitTouch() {
  TSPoint p;
   do {
     p = ts.getPoint(); 
     pinMode(XM, OUTPUT);
     pinMode(YP, OUTPUT);
     pinMode(XP, OUTPUT);
     pinMode(YM, OUTPUT);
   } 
   while((p.z < MINPRESSURE )|| (p.z > MAXPRESSURE));
   p.x = map(p.x, TS_LEFT, TS_RT, 0, 240);
   p.y = map(p.y, TS_TOP, TS_BOT, 0, 400);;
   return p;
 }

void DetectButtons(){  
  
    if (X<60) //Detecting Buttons on Column 1
   {
    if (Y > 340)
    {
        Serial.println ("Button 0");                                                          //Button 0 is Pressed
        if (comma == 0) Number = Number*10 + 0;                                               //Pressed twice
        else if (comma > 0) comma ++;
    }
    
     if (Y < 340 && Y > 280) //If Button 1 is pressed
    {
        Serial.println ("Button 1");
         
        if (Number == 0 && comma == 0) Number=1;
    
        else if (Number != 0 && comma == 0) Number = Number*10 + 1;                              //Pressed twice
    
        else if (comma > 0) Number = Number + (1 / pow(10, comma)), comma++;

        }
        
    
     if (Y < 280 && Y > 220)                                                                    //If Button 4 is pressed
    {
        Serial.println ("Button 4"); 
       
        if (Number == 0 && comma == 0) Number = 4;
    
        else if (Number != 0 && comma == 0) Number = Number * 10 + 4;                             //Pressed twice
    
        else if (comma > 0) Number = Number + (4 / pow(10, comma)), comma++;

    }
    
     if (Y < 220 && Y > 160)                                                                     //If Button 7 is pressed
    {
        Serial.println ("Button 7");
        
        if (Number == 0 && comma == 0) Number = 7;
    
        else if (Number != 0 && comma == 0) Number = Number * 10 + 7;                              //Pressed twice
    
        else if (comma > 0) Number = Number + (7 / pow(10, comma)), comma++;

    } 
    
    if (Y < 160 && Y > 100) //If cancel Button is pressed
    {
       Serial.println ("Button Cancel");
       
       Number = Num1 = Num2 = 0;
    
       result = false;
    
       comma = 0;

       equal = 0;

       actions = 0;
    }
       
  }

    if (X > 60 && X < 120)                                                                  //Detecting Buttons on Column 2
  {
    
    if (Y > 340 ) //If Button " , " is pressed
    { 
      Serial.println ("Comma");
      
      if (comma == 0) comma++;
    }
    
    
     if (Y < 340 && Y > 280)
    {
       Serial.println ("Button 2"); 
       
       if (Number == 0 && comma == 0) Number = 2;
    
       else if (Number != 0 && comma == 0) Number = Number * 10 + 2;                     //Pressed twice
    
       else if (comma > 0) Number = Number + (2 / pow(10, comma)), comma++;

    }
    
     if (Y < 280 && Y > 220)
    {
       Serial.println ("Button 5"); 
     
       if (Number == 0 && comma == 0) Number = 5;
    
       else if (Number != 0 && comma == 0) Number = Number * 10 + 5;                      //Pressed twice
    
       else if (comma > 0) Number = Number + (5 / pow(10, comma)), comma++;

    }
    
     if (Y < 220 && Y > 160)
    {
       Serial.println ("Button 8"); 
     
       if (Number == 0 && comma == 0) Number = 8;
    
       else if (Number != 0 && comma == 0) Number = Number * 10 + 8;                          //Pressed twice
    
       else if (comma > 0) Number = Number + (8 / pow(10, comma)), comma++;

    } 
    
    if (Y < 160 && Y > 100)                                                                     // Sqrt
    {
      Serial.println ("Sqrt"); 
     
      Number = sqrt(Number); 
    }  

  }

    if (X > 120 && X < 180)                                                                    //Detecting Buttons on Column 3
  {
    if (Y > 340)
    {
       Serial.println ("Button Equal"); 
       if(equal == 0){
       Num2 = Number;
       result = true;
       comma = 0;
       equal++;
       }
       else{
        Num1 = Number;
        result = true;
        comma = 0;
       }
    }
    
     if (Y < 340 && Y > 280)
    {
        Serial.println ("Button 3"); 

        if (Number == 0 && comma == 0) Number = 3;
    
        else if (Number != 0 && comma == 0) Number = Number * 10 + 3;                     //Pressed twice
    
        else if (comma > 0) Number = Number + (3 / pow(10, comma)), comma++;

    }
    
     if (Y < 280 && Y > 220)
    {
       Serial.println ("Button 6"); 
    
       if (Number == 0 && comma == 0) Number = 6;
    
       else if (Number != 0 && comma == 0) Number = Number * 10 + 6;                          //Pressed twice
    
       else if (comma > 0) Number = Number + (6 / pow(10, comma)), comma++;

    }
    
     if (Y < 220 && Y > 160)
    {
       Serial.println ("Button 9");
    
       if (Number == 0 && comma == 0) Number = 9;
    
       else if (Number != 0 && comma == 0) Number = Number * 10 + 9;                           //Pressed twice
    
       else if (comma > 0) Number = Number + (9 / pow(10, comma)), comma++;

    } 
    
     if (Y < 160 && Y > 100)                                                                     // procent %
    {
      Serial.println ("Button %"); 
      
      Number = (Num1 / 100) * Number; 
      comma = 0;
    }   
  }

     
     if (X > 180)                                                                                //Detecting Buttons on Column 4
  {
    Num1 = Number;    
    Number =0;
    tft.setCursor(200, 20);
    tft.setTextColor(RED);
    
     if (Y > 280)
     {
        Serial.println ("Addition"); 
        action = 1;
        tft.println('+'); 
        comma = 0;
     }
    
     if (Y < 280 && Y > 220)
     {
        Serial.println ("Subtraction");
       
          action = 2;
          tft.println('-');
          comma = 0;
         
          
     }
    
     if (Y < 220 && Y > 160)
     {
        Serial.println ("Multiplication");
        action = 3;
        tft.println('*');
        comma = 0;
      }
    
     if (Y < 160 && Y > 100)
    {
        Serial.println ("Devesion");
        action = 4;
        tft.println('/');
        comma = 0;
    }  

    delay(300);
  }  
}

void CalculateResult()
{
  if (action == 1)
    Number = Num1 + Num2;
    

  if (action == 2)
    Number = Num1 - Num2;
    

  if (action == 3)
    Number = Num1 * Num2;
    

  if (action == 4)
    Number = Num1 / Num2;
    
   
}

void DisplayResult()
{
    tft.fillRect(0, 0, 240, 99, BLACK);                                                             //clear result box
    tft.setCursor(10, 20); 
    tft.setTextSize(3);
    tft.setTextColor(WHITE);
    tft.println(Number);                                                                            //update new value
  
}

void IntroScreen()
{
   /*
  //Draw Cirsles
  for (int v=1; v<=300; v+=4)
  tft.drawCircle(120, 200, v, WHITE);
   for (int v=1; v<=300; v+=4)
  tft.drawCircle(120, 200, v, BLACK);

  tft.setRotation(2);
  tft.setCursor (55, 120);
  tft.setTextSize (3);
  tft.setTextColor(WHITE);
  tft.println("ARDUINO");
  tft.setCursor (30, 160);
  tft.println("CALCULATOR");
  tft.setCursor (30, 390);
  tft.setTextSize (1);
  tft.setTextColor(WHITE);
  tft.println("vers.2.0");
  delay(2000);
  */

}

void draw_BoxNButtons()
{

  tft.setRotation(2);

    //Draw the Result Box
  tft.fillRect(0, 0, 240, 400, BLACK);


   //Draw circles buttons
  for (int j=0;j<5;j++) {
     for (int i=0;i<4;i++) {
      tft.drawCircle(30 + (60*i), 130 + (60*j), 29, WHITE);
     }
  }

  // Draw button "Plus"
  tft.drawFastVLine(181, 310, 60, WHITE);
  tft.drawFastVLine(239, 310, 60, WHITE);
  tft.fillRect(182, 315, 57, 58, BLACK);
     
 
/*
  //Draw Horizontal Lines
  for (int h=100; h<=400; h+=60)
  tft.drawFastHLine(0, h, 240, WHITE);
  tft.drawFastHLine(0, 399, 240, WHITE);
  tft.drawFastHLine(180, 340, 238, BLACK);
  
  //Draw Vertical Lines
  for (int v=0; v<=180; v+=60)
  tft.drawFastVLine(v, 100, 300, WHITE);
  tft.drawFastVLine(239, 100, 300, WHITE);
*/

    //Display keypad lables 
  for (int j=0;j<5;j++) {
     for (int i=0;i<4;i++) {
        if (j == 0 && i == 1){
          tft.setCursor(68, 123);
          tft.setTextSize(2);
          tft.setTextColor(WHITE);
          tft.println("Sqrt");
        }
        else {
        tft.setCursor(22 + (60*i), 120 + (60*j));
        tft.setTextSize(3);
        tft.setTextColor(WHITE);
        tft.println(symbol[j][i]);
        }
     } 
  }
}
