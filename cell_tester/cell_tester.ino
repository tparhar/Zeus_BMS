#include <LCDWIKI_GUI.h> // Core graphics library
#include <LCDWIKI_SPI.h> // Hardware-specific library

// parameters define
#define MOSFET_Pin 2
#define Bat_Pin A0
#define Res_Pin A1
#define BUZ_Pin 9

#define MODEL ILI9488_18
#define CS A5
#define CD A3
#define RST A4
#define LED -1


LCDWIKI_SPI mylcd(MODEL,CS,CD,RST,LED);

// define some colour values
#define BLACK 0x0000
#define RED 0xF800
#define WHITE 0xFFFF

float SoC = 0.0; 
float Res_Value = 10.0;  
float Vcc = 4.56; //Measure it and change  
float Current = 0.0; 
float mA=0;         
float Bat_Volt = 0.0;  
float Res_Volt = 0.0;  
// unsigned long previousMillis = 0; 
// unsigned long millisPassed = 0;  
float sample1 =0.000;
float sample2= 0.000;
// int x = 0;
// int row = 0;

// Drawing function for voltage, amperage, and capacity
void draw(void) {
   mylcd.Set_Text_Mode(0);
   mylcd.Fill_Screen(BLACK);
   mylcd.Set_Text_Back_colour(BLACK);
   mylcd.Set_Text_Size(5);
   if ( Bat_Volt < 1){
    mylcd.Print_String("No Battery", 10, 40);
    // u8g.setPrintPos(10,40);      
    // u8g.println("No Battery"); 
   }
   else if ( Bat_Volt > 4.3){
    mylcd.Print_String("High Voltage", 10, 40);
    // u8g.setPrintPos(3,40);       
    // u8g.println("High Voltage"); 
   }
   else if(Bat_Volt < 2.9){
    mylcd.Print_String("Low Voltage", 10, 40);
    // u8g.setPrintPos(3,40);      
    // u8g.println("Low Voltage"); 
   }
   else if(Bat_Volt >= 2.9 && Bat_Volt < 4.3  ){
    mylcd.Set_Text_Size(4);
    mylcd.Print_String("V:     V", 0, 0);
    mylcd.Print_String("I:       mA", 0, 80);
    mylcd.Print_String("SoC:      %", 0, 160);
    mylcd.Print_Number_Float(Bat_Volt, 2, 50, 0, '.', 0, ' ');
    mylcd.Print_Number_Float(mA, 2, 50, 80, '.', 0, ' ');
    mylcd.Print_Number_Float(SoC, 2, 100, 160, '.', 0, ' ');
      
  //  u8g.drawStr(0, 20, "V: ");  
  //  u8g.drawStr(0, 40, "I: ");
  //  u8g.drawStr(0, 60, "mAh: ");
  //  u8g.setPrintPos(58,20);       
  //  u8g.print( Bat_Volt,2);  
  //  u8g.println("V"); 
  //  u8g.setPrintPos(58,40);        
  //  u8g.print( mA,0);  
  //  u8g.println("mA"); 
  //  u8g.setPrintPos(58, 60);       
  //  u8g.print( Capacity ,1);    
  
  }
}

float estimateSoC(float voltage) {
  if (voltage >= 4.2) return 100.0;
  else if (voltage <= 2.8) return 0.0;
  else return (voltage - 2.8) * (100.0 / (4.2 - 2.8));
}

// Buzzer function
void buz()
{
digitalWrite(BUZ_Pin, HIGH);
delay(100);          
digitalWrite(BUZ_Pin, LOW);  
delay(10);            
}

void setup() {
  mylcd.Init_LCD();
  mylcd.Fill_Screen(BLACK);
  Serial.begin(9600);
  // Setting A4 and A5 as INPUT_PULLUP allows
  // for serial communication with the OLED
  //  pinMode(A4,INPUT_PULLUP);
  //  pinMode(A5, INPUT_PULLUP);
  pinMode(MOSFET_Pin, OUTPUT);
  // Not using Pin 9 for the buzzer
  pinMode(BUZ_Pin, OUTPUT);
  digitalWrite(MOSFET_Pin, LOW); 
}

  void loop() {
    sample1 = 0.00;
    sample2 = 0.00;
    SoC = 0.00;

    // Taking 100 samples of voltage, and
    // calculating the average
    for(int i=0;i< 100;i++)
    {
    sample1=sample1+analogRead(Bat_Pin);
    delay (2);
    }
    // Taking the average of 100 samples
    sample1=sample1/100;
    // Multiplying measured voltage by 2
    // because there is a voltage divider
    // with two 10k resistors.
    Bat_Volt = 2* sample1 *Vcc/ 1024.0; 

    // Doing the same thing as above but
    // with the resistor
    for(int i=0;i< 100;i++)
    {
    sample2=sample2+analogRead(Res_Pin); 
    delay (2);
    }
    sample2=sample2/100;
    Res_Volt = 2* sample2 * Vcc/ 1024.0;
    
    if ( Bat_Volt > 4.3){
      digitalWrite(MOSFET_Pin, LOW); 
      buz();
      Serial.println( "Warning High-V! ");
      delay(1000);
    }
    
    else if(Bat_Volt < 2.9){
      digitalWrite(MOSFET_Pin, LOW);
      buz();
      Serial.println( "Warning Low-V! ");
      delay(1000);
    }
    else if(Bat_Volt > 2.9 && Bat_Volt < 4.3  ) { 
      digitalWrite(MOSFET_Pin, HIGH);
      // millisPassed = millis() - previousMillis;
      Current = (Bat_Volt - Res_Volt) / Res_Value;
      mA = Current * 1000.0 ;
      SoC = estimateSoC(Bat_Volt);
      // Capacity = mA * (millisPassed / 3600000.0);
      // previousMillis = millis();
      // row++;
      // x++;
      delay(100); 
  
      }

    draw();
    delay(3000);
  
 }