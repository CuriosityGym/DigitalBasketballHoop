//We always have to include the library
#include "LedControl.h"
#include "font.h"

#include <Wire.h>
#define BRIGHTNESS 2
#define DATAIN 12
#define CLK 11
#define CS 10
#define DEVICECOUNT 2

int topSensor = 3;
int bottomSensor = 2;
volatile unsigned long topMilliCount=0;
volatile unsigned long bottomMilliCount=0;
uint8_t goalCount=0;
byte firstMatrixData[8];
byte secondMatrixData[8];
volatile boolean topSensorRegistered=false;
volatile boolean bottomSensorRegistered=false;

LedControl lc=LedControl(DATAIN,CLK,CS,DEVICECOUNT);
void setup() 
{
    for(int i=0;i<DEVICECOUNT;i++)
      {
        lc.shutdown(i,false);// Wake up displays
        lc.setIntensity(i,BRIGHTNESS);// Set intensity levels
        lc.clearDisplay(i);// Clear Displays    
      }
      setCountOfGoals();
    attachInterrupt(digitalPinToInterrupt(topSensor), topSensorRead, CHANGE);
    attachInterrupt(digitalPinToInterrupt(bottomSensor), bottomSensorRead, FALLING);
    Serial.begin(115200);
}

void loop() 
{
  cli();
  
  if(topSensorRegistered && bottomSensorRegistered)
  {
     
    // Serial.println(bottomMilliCount,DEC);
    // Serial.println(topMilliCount,DEC);
    if(bottomMilliCount>topMilliCount)
    {
        // correct ball triggered
     
      Serial.println(bottomMilliCount-topMilliCount,DEC);
      delay(1000);
      topSensorRegistered=false;
      bottomSensorRegistered=false;
      bottomMilliCount=0;
      topMilliCount=0;
      goalCount=goalCount+1;
      setCountOfGoals();
    }
  }

  sei();
}

void topSensorRead() 
{
    topSensorRegistered=true;
    topMilliCount=millis();
  //Serial.println("Here 1");
}

void bottomSensorRead() 
{
   bottomSensorRegistered=true;
   bottomMilliCount=millis();
  //Serial.println("Here 2");
}

void setCountOfGoals()
{
  uint8_t firstDigit,secondDigit;
 
  if(goalCount<10)
  {
    firstDigit=0;
    secondDigit=goalCount;
  }

   if(goalCount>9)
  {
    firstDigit=floor(goalCount/10);
    secondDigit=goalCount%10;
  }
  setMatrixDigit(firstDigit,0);
  setMatrixDigit(secondDigit,1);
  for(int i=0;i<8; i++)
  {
     
     //lc.setRow(2,i,firstMatrixData[i]);
      lc.setRow(1,i,firstMatrixData[i]);
      lc.setRow(0,i,secondMatrixData[i]);
        
  }
  

}



void setMatrixDigit(int digit, uint8_t whichArray)
{
  byte shiftedData;
   for(int i=0;i<8;i++)
   {
      
       if(digit==0)
       {
        
           shiftedData=zero[i];
       } 

        if(digit==1)
       {
           shiftedData=one[i];
       } 

        if(digit==2)
       {
           shiftedData=two[i];
       } 

        if(digit==3)
       {
         shiftedData=three[i];
       } 

        if(digit==4)
       {
         shiftedData=four[i];
       } 

        if(digit==5)
       {
          shiftedData=five[i];
       } 
       
        if(digit==6)
       {
         shiftedData=six[i];
       } 

        if(digit==7)
       {
          shiftedData=seven[i];
       } 

        if(digit==8)
       {
         shiftedData=eight[i];
       } 

        if(digit==9)
       {
          shiftedData=nine[i];
       } 

        

      if(whichArray==0)
      { 
       firstMatrixData[i]=shiftedData;
      }

       if(whichArray==1)
      { 
       secondMatrixData[i]=shiftedData;
      }
   }
}

