// -------------------- INIZIO DEL CODICE PER ARDUINO UNO WIFI REV2 --------------------

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Impostazioni del display LCD I2C
LiquidCrystal_I2C lcd(0x27, 16, 2); // Usa 0x3F se 0x27 non funziona

// Stringhe per il display
char array1[]="Pronto Premi BUT";
char array2[]="ZAPPER VER. 0.4 ";
char array5[]="Ciclo ZAPPER ";
char array6[]="Ciclo STOP ";
char array7[]="FINE Ciclo ";
char array8[]="FINE SESSIONE ";

// Definizione dei pin
int ledred = 8;
int buzz = 9;
const int uscita = 10;
int start_button_pin = 11; // Ho rinominato il pin per chiarezza
int ledyellow = 12;
int ledgreen = 13;

// Durate in secondi
int runFor7 = 420;  // 7 minuti
int runFor20 = 1200; // 20 minuti

/****************************************************************
 * Funzione setPwmFrequency_4809
 * QUESTA FUNZIONE È SPECIFICA PER L'ATmega4809 (Arduino Uno WiFi Rev2)
 * Manipola il registro del Timer A (TCA0) per cambiare la frequenza PWM
 * sui pin D3, D5, D6, D9, D10.
 * Frequenza = 16MHz / (prescaler * 256)
 ****************************************************************/
void setPwmFrequency_4809(int prescaler) {
  // Disabilita temporaneamente il timer per cambiare il prescaler
  TCA0.SINGLE.CTRLA &= ~(TCA_SINGLE_ENABLE_bm);
  
  // Imposta il nuovo prescaler
  switch (prescaler) {
    case 1:
      TCA0.SINGLE.CTRLA = (TCA0.SINGLE.CTRLA & ~TCA_SINGLE_CLKSEL_gm) | TCA_SINGLE_CLKSEL_DIV1_gc;
      break;
    case 2:
      TCA0.SINGLE.CTRLA = (TCA0.SINGLE.CTRLA & ~TCA_SINGLE_CLKSEL_gm) | TCA_SINGLE_CLKSEL_DIV2_gc;
      break;
    case 4:
      TCA0.SINGLE.CTRLA = (TCA0.SINGLE.CTRLA & ~TCA_SINGLE_CLKSEL_gm) | TCA_SINGLE_CLKSEL_DIV4_gc;
      break;
    case 8:
      TCA0.SINGLE.CTRLA = (TCA0.SINGLE.CTRLA & ~TCA_SINGLE_CLKSEL_gm) | TCA_SINGLE_CLKSEL_DIV8_gc;
      break;
    // Aggiungi altri case se necessario per altri divisori
    default: // Default al prescaler standard di sistema
      TCA0.SINGLE.CTRLA = (TCA0.SINGLE.CTRLA & ~TCA_SINGLE_CLKSEL_gm) | TCA_SINGLE_CLKSEL_DIV16_gc;
      break;
  }
  
  // Riabilita il timer
  TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm;
}

void tempo(int runFor) {
  for (int count = runFor; count > 0; --count) {
    lcd.setCursor(0, 1);
    lcd.print(count);
    lcd.print("S ");
    lcd.print(count / 60);
    lcd.print("m ");
    lcd.print(count % 60);
    lcd.print("s ");
    // Pulisci il resto della riga
    lcd.print("    ");
    delay(1000);
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Pausa Finita");
  delay(1000);
}

void zapper(int numerodiciclo) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(array5);
  lcd.print(numerodiciclo);
  analogWrite(uscita, 128);
  digitalWrite(ledyellow, HIGH);
  digitalWrite(ledgreen, LOW);
  digitalWrite(ledred, LOW);
  tempo(runFor7);
}

void riposo(int numerodiciclo) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(array6);
  lcd.print(numerodiciclo);
  analogWrite(uscita, 0);
  digitalWrite(ledred, HIGH);
  digitalWrite(ledyellow, LOW);
  digitalWrite(ledgreen, LOW);
  tempo(runFor20);
}

void suona(int numerodiciclo) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(array7);
  lcd.print(numerodiciclo);
  for (int i = 0; i < 80; i++) {
    digitalWrite(buzz, HIGH); delay(2);
    digitalWrite(buzz, LOW); delay(2);
  }
  for (int i = 0; i < 100; i++) {
    digitalWrite(buzz, HIGH); delay(4);
    digitalWrite(buzz, LOW); delay(4);
  }
  delay(1000);
}

void setup() {
  pinMode(uscita, OUTPUT);
  pinMode(ledred, OUTPUT);
  pinMode(ledyellow, OUTPUT);
  pinMode(ledgreen, OUTPUT);
  pinMode(buzz, OUTPUT);
  // Imposta il pin 11 come input con pull-up per il pulsante
  pinMode(start_button_pin, INPUT_PULLUP); 

  Serial.begin(9600);
  lcd.init();
  lcd.backlight();

  // --- MODIFICA FONDAMENTALE ---
  // Imposta la frequenza PWM desiderata usando la NUOVA funzione.
  // Prescaler = 2 -> Frequenza = 16,000,000 / (2 * 256) = 31,250 Hz
  setPwmFrequency_4809(2);
  
  analogWrite(uscita, 0); // Uscita spenta all'inizio
  
  digitalWrite(ledgreen, HIGH);
  digitalWrite(ledred, LOW);
  digitalWrite(ledyellow, LOW);

  lcd.setCursor(0, 0);
  lcd.print(array1);
  lcd.setCursor(0, 1);
  lcd.print(array2);
}

void loop() {
  // Logica di avvio con pulsante fisico sul pin 11.
  // Collega un pulsante tra il pin 11 e GND.
  if (digitalRead(start_button_pin) == LOW) {
    
    // Attendi il rilascio del pulsante per evitare avvii multipli
    delay(50);
    while(digitalRead(start_button_pin) == LOW);
    
    zapper(1); suona(1);
    
    lcd.noDisplay(); riposo(1); lcd.display(); suona(1);
    
    zapper(2); suona(2);
    
    lcd.noDisplay(); riposo(2); lcd.display(); suona(2);
    
    zapper(3); suona(3);
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(array8);
    delay(10000);
    suona(3); suona(3);
    
    // Ritorna allo stato di attesa
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(array1);
    lcd.setCursor(0, 1);
    lcd.print(array2);
    digitalWrite(ledgreen, HIGH);
    digitalWrite(ledred, LOW);
    digitalWrite(ledyellow, LOW);
  }
  
  delay(100);
}
// -------------------- FINE DEL CODICE --------------------
