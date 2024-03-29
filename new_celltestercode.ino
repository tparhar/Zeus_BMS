// OHHHH THIS IS FOR THE REST PACK, SO WE'RE TESTING INDIVIDUAL BATTERIES MAKES SENSE
// I'M GUESSING WE DON'T JUST WANT TO DO INDIVIDUAL BATTERIES SO NEED TO FIND A WAY TO DO MULTIPLE AT ONCE (5)
// LOW on MOSFET_Pin = bad, HIGH on MOSFET_Pin = good





#include "U8glib.h"  //Library for display 
#define MOSFET_Pin 2
#define MOSFET_Pin 3
#define MOSFET_Pin 4
#define MOSFET_Pin 5
#define MOSFET_Pin 6
#define Bat_Pin A0
#define Bat_Pin A1
#define Bat_Pin A2  

#define Bat_Pin A3
#define Bat_Pin A4
#define Res_Pin A5

// Hello
U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NONE);  // I2C interface for OLED 
float Capacity = 0.0; // total capacity of batteries?
float Res_Value = 8.0;  // pretty sure the resistor we're using is 8 ohms
float Vcc = 4.410; //Measure it and change - think this is input Vcc to Arduino
float Current = {0.0, 0.0, 0.0, 0.0, 0.0};  // tbh no idea what the difference is b/w Current and mA
float Bat_mAs = {0.0, 0.0, 0.0, 0.0, 0.0};
float Bat_Volts = {0.0, 0.0, 0.0, 0.0, 0.0};
float Res_Volt = 0.0; // either resistor voltage rating or voltage drop across the resistor
unsigned long previousMillis = 0; 
unsigned long millisPassed = 0;  
float sample1 =0.000; // no idea what this is
float sample2= 0.000;
int x = 0;
int row = 0;

void draw(void) {
   u8g.setFont(u8g_font_fub14r); 
   if ( Bat_Volt < 1){
    u8g.setPrintPos(10,40);      // battery voltage checks, not sure if this is for individual batteries or total battery voltage
    u8g.println("No Battrey"); 
   }
   else if ( Bat_Volt > 4.3){
    u8g.setPrintPos(3,40);       
    u8g.println("High Voltage"); 
   }
   else if(Bat_Volt < 2.9){
    u8g.setPrintPos(3,40);      
    u8g.println("Low Voltage"); 
   }
   else if(Bat_Volt >= 2.9 && Bat_Volt < 4.3  ){
      
   u8g.drawStr(0, 20, "V: ");  // these are all how everything displays to the screen
   u8g.drawStr(0, 40, "I: ");
   u8g.drawStr(0, 60, "mAh: ");
   u8g.setPrintPos(58,20);       
   u8g.print( Bat_Volt,2);  
   u8g.println("V"); 
   u8g.setPrintPos(58,40);        
   u8g.print( mA,0);  
   u8g.println("mA"); 
   u8g.setPrintPos(58, 60);       
   u8g.print( Capacity ,1);    
  
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
  
  void loop() {
  
  for(int i=0;i< 100;i++)
  {
   sample1=sample1+analogRead(Bat_Pin); // reading analog voltage from battery pin, adding that to sample1, looping for 100 cycles
   delay (2);
  }
  sample1=sample1/100; 
  Bat_Volt = 2* sample1 *Vcc/ 1024.0; // voltage read from test cell

   for(int i=0;i< 100;i++)
  {
   sample2=sample2+analogRead(Res_Pin); // reading analog voltage from resistor pin (or could be result pin not too sure), adding that to sample1, looping ofr 100 cycles
   delay (2);
  }
  sample2=sample2/100;
  Res_Volt = 2* sample2 * Vcc/ 1024.0; // voltage read from resistor? 
  
  if ( Bat_Volt > 4.3){ // battery voltage too high
    digitalWrite(MOSFET_Pin, LOW); // make MOSFET_Pin (idk what that does) go low
    buz(); // buzzer buzzes
    Serial.println( "Warning High-V! "); 
    delay(1000);
   }
   
   else if(Bat_Volt < 2.9){ // battery voltage too low
      digitalWrite(MOSFET_Pin, LOW);
      buz(); // buzzer buzzes
      Serial.println( "Warning Low-V! ");
      delay(1000);
  }
  else if(Bat_Volt > 2.9 && Bat_Volt < 4.3  ) { // battery voltage good OHHH THIS IS IF IT'S GOOD SO YOU DO ALL THE CALCULATIONS
      digitalWrite(MOSFET_Pin, HIGH); // make MOSFET_Pin go high
      millisPassed = millis() - previousMillis;
      Current = (Bat_Volt - Res_Volt) / Res_Value; // this is just Ohm's law to find current, don't know why Res_Volt and the other thing are on different pins though
      mA = Current * 1000.0 ;
      Capacity = Capacity + mA * (millisPassed / 3600000.0);
      previousMillis = millis();
      row++;
      x++;
      delay(100); 
 
     }

  u8g.firstPage();  
  do { // finally doing the displaying to the screen about the battery info
    draw();
  } while( u8g.nextPage() );
  
 }    
