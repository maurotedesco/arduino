//# Inizio della programmazione zapper with arduino 4-4-2017
//Zapper with arduino and bluetooth Ver. init 0.8
/****************************************************************
*   Funzione con genera frequenza
*   Modulo bluetooth per comunicazione App
****************************************************************/ 
// include the library code

char array1[]="Pronto Premi BUT";  //the string to print on the LCD
char array2[]="ZAPPER VER. 0.8 ";  //the string to print on the LCD
char array3[]="Time        ";  //the string to print on the LCD
char array4[]="SEC        ";  //the string to print on the LCD
char array5[]="Ciclo ZAPPER ";  //the string to print on the LCD
char array6[]="Ciclo STOP ";  //the string to print on the LCD
char array7[]="FINE Ciclo  ";  //the string to print on the LCD
char array8[]="FINE SESSIONE ";  //the string to print on the LCD

#define rank1 64
#define rank2 128
#define rank3 192
// Variables will change:


int ledred = 8;
int buzz = 9 ; // pin per buzz
const int uscita = 10;    // the number of the pulse pin

int timer = 11;
int ledyellow = 12;
int ledgreen = 13;
// variabili teporali....secondi 
int runFor7 = 420 ; // 7 minuti -- 60 x 7 = 420
int runFor20 = 1200; // 20 minuti -- 60 x 20 = 1200


void setPwmFrequency(int pin, int divisor) {
// funzione
/**
 * Divides a given PWM pin frequency by a divisor.
 * 
 * The resulting frequency is equal to the base frequency divided by
 * the given divisor:
 *   - Base frequencies:
 *      o The base frequency for pins 3, 9, 10, and 11 is 31250 Hz.
 *      o The base frequency for pins 5 and 6 is 62500 Hz.
 *   - Divisors:
 *      o The divisors available on pins 5, 6, 9 and 10 are: 1, 8, 64,
 *        256, and 1024.
 *      o The divisors available on pins 3 and 11 are: 1, 8, 32, 64,
 *        128, 256, and 1024.
 * 
 * PWM frequencies are tied together in pairs of pins. If one in a
 * pair is changed, the other is also changed to match:
 *   - Pins 5 and 6 are paired on timer0
 *   - Pins 9 and 10 are paired on timer1
 *   - Pins 3 and 11 are paired on timer2
 * 
 * Note that this function will have side effects on anything else
 * that uses timers:
 *   - Changes on pins 3, 5, 6, or 11 may cause the delay() and
 *     millis() functions to stop working. Other timing-related
 *     functions may also be affected.
 *   - Changes on pins 9 or 10 will cause the Servo library to function
 *     incorrectly.
 * 
 * Thanks to macegr of the Arduino forums for his documentation of the
 * PWM frequency divisors. His post can be viewed at:
 *   http://forum.arduino.cc/index.php?topic=16612#msg121031
 */
  byte mode;
  if(pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    if(pin == 5 || pin == 6) {
      TCCR0B = TCCR0B & 0b11111000 | mode;
    } else {
      TCCR1B = TCCR1B & 0b11111000 | mode;
    }
  } else if(pin == 3 || pin == 11) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 32: mode = 0x03; break;
      case 64: mode = 0x04; break;
      case 128: mode = 0x05; break;
      case 256: mode = 0x06; break;
      case 1024: mode = 0x7; break;
      default: return;
    }
    TCCR2B = TCCR2B & 0b11111000 | mode;
  }
}

//  FINE Funzione

// funzione stampa dato 
void StampaMSG(char msg[]) {
//lcd.setCursor(0,0);
//lcd.clear();
//lcd.print(msg[5]);
}

void tempo(int runFor) {
   for(int timer = runFor;timer > 0; --timer){
    lcd.setCursor(0,1);
    lcd.print(timer);
    lcd.print("S=");
    lcd.print(timer/60);
    lcd.print(".");
    lcd.print(timer%60);
    lcd.print("STOP!");  
   delay(1000);
   }
   lcd.setCursor(0,1);
   lcd.clear();
   lcd.print(" Ready");  
}

void setup() {
  pinMode(uscita, OUTPUT);
  Serial.begin(9600);
  pinMode(ledred, OUTPUT);
  pinMode(ledyellow, OUTPUT);
  pinMode(ledgreen, OUTPUT);
  pinMode(buzz, OUTPUT);
  
  //setto il led in ON, cioè verde
  digitalWrite(ledgreen, HIGH);  // turn the LED on (HIGH is the voltage level)
  digitalWrite(ledred, LOW);
  digitalWrite(ledyellow, LOW);
  
// Set pin 10's PWM frequency to 31250 Hz (31250/1 = 31250)
// Note that the base frequency for pins 3, 9, 10, and 11 is 31250 Hz
setPwmFrequency(uscita, 1);
analogWrite(uscita,0);  //uscita volt 0
lcd.print(array1);
lcd.setCursor(0,1);
lcd.print(array2);
// Pronto per partire

}
void zapper(int numerodiciclo) {
 //ciclo di 7 minuti

     //lcd.print(array5);
     //lcd.print(numerodiciclo);
     analogWrite(uscita,128); // Frequenza fissa, volt variabile che dipende dal secondo numeroda, 128 significa metà volt, il range è: 0-255
      //setto il led in ON, cioè verde
      digitalWrite(ledgreen, LOW);  // turn the LED YELLOW on (HIGH is the voltage level)
      digitalWrite(ledred, LOW);
      digitalWrite(ledyellow, HIGH);
     tempo(runFor7);
}

void riposo(int numerodiciclo) {
  // ciclo di 20 minuti
     //lcd.print(array6);
     //lcd.print(numerodiciclo);
     analogWrite(uscita,0); // Frequenza fissa, volt variabile che dipende dal secondo numeroda, 128 significa metà volt, il range è: 0-255
      //setto il led in RED ON
      digitalWrite(ledgreen, LOW);  // turn the LED YELLOW on (HIGH is the voltage level)
      digitalWrite(ledred, HIGH);
      digitalWrite(ledyellow, LOW);
     tempo(runFor20);
}

void suona(int numerodiciclo) {
  // ciclo di 10 secondi O PIÙ DIPENDE DALLA variabile
  int i;
     //lcd.print(array7);
     //lcd.print(numerodiciclo);
      for(i=0;i<80;i++)  
      {
            digitalWrite(buzz,HIGH);
            delay(2);//wait for 2ms
            digitalWrite(buzz,LOW);
            delay(2);//wait for 2ms
       }
    //output another frequency
      for(i=0;i<100;i++)
      {
        digitalWrite(buzz,HIGH);
        delay(4);//wait for 2ms
        digitalWrite(buzz,LOW);
        delay(4);//wait for 2ms
      }
      
     delay(1000);
     
}

void loop() {
 //inizio il ciclo di 7 minuti quando viene premuto il pulsante di avvio
  if(timer >= 10) {  				// pulsante Premuto

   //// aggiungere buzzer per avviso
    //primo ciclo di 7 minuti
     
    zapper(1);
    suona(1);

     // primo ciclo di fermo 20 minuti
    riposo(1);
    suona(1);

    //secondo ciclo di 7 minuti 
    zapper(2);
    suona(2);
    
    // secondo ciclo di fermo 20 minuti
    riposo(2);
    suona(2);
    
    //Terzo ciclo di 7 minuti 
    zapper(3);
    suona(3);
    
    //lcd.print(array8);
    delay(10000);
    // avviso che e finito il ciclo
    suona(3);
    suona(3);
    timer = 1; // azzero il timer per fermare il ciclo e sstampare attesa
   } else {   
      lcd.print(array1);
      lcd.print(array2);
   }

 //Serial.println("uscita analogica in PWM 50%");
 lcd.print(array1);
 lcd.setCursor(0,1);
 lcd.print(array2);
 
  //setto il led in ON, cioè verde
  digitalWrite(ledgreen, HIGH);  // turn the LED on (HIGH is the voltage level)
  digitalWrite(ledred, LOW);
  digitalWrite(ledyellow, LOW);
}
