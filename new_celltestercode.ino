/* The idea for this code is that I pretty much just recycled the old code, turned it into a function (including the display part),
   and then depending on the input to the page, you get the info for that cell and then display it. The idea is that we'll only be
   1 cell at a time. Can change to code that instead gets all of the batteries' info, then can display at any time.*/





#include "U8glib.h"  //Library for display 
#define Bat_Pin1 A0
#define Bat_Pin2 A1
#define Bat_Pin3 A2  
#define Bat_Pin4 A3
#define Bat_Pin5 A4
#define Res_Pin A5
int MOSFET_PINS = {2, 3, 4, 5, 6};
char BAT_PINS = {"A0", "A1", "A2", "A3", "A4"};


U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NONE);  // I2C interface for OLED 
float Capacities = {0,0, 0.0, 0.0, 0.0, 0.0}; // total capacity of batteries?
float Res_Value = 8.0;  // pretty sure the resistor we're using is 8 ohms
float Vcc = 4.410; //Measure it and change - think this is input Vcc to Arduino
float Current = {0.0, 0.0, 0.0, 0.0, 0.0};  // tbh no idea what the difference is b/w Current and mA
float Bat_mAs = {0.0, 0.0, 0.0, 0.0, 0.0};
float Bat_Volts = {0.0, 0.0, 0.0, 0.0, 0.0};
float Res_Volt = 0.0; // either resistor voltage rating or voltage drop across the resistor
unsigned long previousMillis = 0; 
unsigned long millisPassed = 0;  
float batsamples = {0.0, 0.0, 0.0, 0.0, 0.0};
float res_sample = 0.0;
int x = 0;
int row = 0;

void draw(battery) {
  u8g.setFont(u8g_font_fub14r); 
  if ( Bat_Volts[battery] < 1){
  u8g.setPrintPos(10,40);      // battery voltage checks, not sure if this is for individual batteries or total battery voltage
  u8g.println("No Battery"); 
  }
  else if ( Bat_Volts[battery] > 4.3){
  u8g.setPrintPos(3,40);       
  u8g.println("High Voltage"); 
  }
  else if(Bat_Volts[battery] < 2.9){
  u8g.setPrintPos(3,40);      
  u8g.println("Low Voltage"); 
  }
  else if(Bat_Volts[battery] >= 2.9 && Bat_Volts[battery] < 4.3  ){
    
  u8g.drawStr(0, 20, "V: ");  // these are all how everything displays to the screen
  u8g.drawStr(0, 40, "I: ");
  u8g.drawStr(0, 60, "mAh: ");
  u8g.setPrintPos(58,20);       
  u8g.print( Bat_Volt[battery],2);  
  u8g.println("V"); 
  u8g.setPrintPos(58,40);        
  u8g.print( Bat_mAs[battery],0);  
  u8g.println("mA"); 
  u8g.setPrintPos(58, 60);       
  u8g.print( Capacities[battery] ,1);    

}
}

void buz() // makes the buzzer buzz
{
digitalWrite(9, HIGH);
delay(100);          
digitalWrite(9, LOW);  
delay(10);            

}   

void setup() {
  Serial.begin(9600);    // setting baudRate (data transfer rate) for the serial port
  pinMode(A4,INPUT_PULLUP); // initializing a bunch of pins in different modes
  pinMode(A5, INPUT_PULLUP);
  pinMode(MOSFET_Pin, OUTPUT);
  pinMode(9, OUTPUT);
  digitalWrite(MOSFET_Pin, LOW); 
}

void tester(battery) {

  for(int i=0;i< 100;i++)
  {
  batsamples[battery]=batsamples[battery]+analogRead(BAT_PINS[battery]); // reading analog voltage from battery pin, adding that to sample1, looping for 100 cycles
  delay (2);
  }
  batsamples[battery]=batsamples[battery]/100; 
  Bat_Volts[battery] = 2* batsamples[battery] *Vcc/ 1024.0; // voltage read from test cell

  for(int i=0;i< 100;i++)
  {
  res_sample=res_sample+analogRead(Res_Pin); // reading analog voltage from resistor pin (or could be result pin not too sure), adding that to sample1, looping ofr 100 cycles
  delay (2);
  }
  res_sample=res_sample/100;
  Res_Volt = 2* res_sample * Vcc/ 1024.0; // voltage read from resistor? 

  if ( Bat_Volts[battery] > 4.3){ // battery voltage too high
    digitalWrite(MOSFET_PINS[battery], LOW); // make MOSFET_PIN (idk what that does) go low
    buz(); // buzzer buzzes
    Serial.println( "Warning High-V! "); 
    delay(1000);
  }

  else if(Bat_Volts[battery] < 2.9){ // battery voltage too low
      digitalWrite(MOSFET_PINS[battery], LOW);
      buz(); // buzzer buzzes
      Serial.println( "Warning Low-V! ");
      delay(1000);
  }
  else if(Bat_Volts[battery] > 2.9 && Bat_Volts[battery] < 4.3  ) { // battery voltage good OHHH THIS IS IF IT'S GOOD SO YOU DO ALL THE CALCULATIONS
      digitalWrite(MOSFET_PINS[battery], HIGH); // make MOSFET_Pin go high
      millisPassed = millis() - previousMillis;
      Current = (Bat_Volts[battery-1] - Res_Volt) / Res_Value; // this is just Ohm's law to find current, don't know why Res_Volt and the other thing are on different pins though
      mA = Current * 1000.0 ;
      Capacity = Capacity + mA * (millisPassed / 3600000.0);
      previousMillis = millis();
      row++;
      x++;
      delay(100); 

    }

  u8g.firstPage();  
  do { // finally doing the displaying to the screen about the battery info
    draw(battery);
  } while( u8g.nextPage() );
}
  
  void loop() {
    int page = "some input"
    tester(page); // probably not the most efficient code it might be too slow
 }    
