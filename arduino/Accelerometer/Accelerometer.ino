#include <iostream>

//Arduino Sketch
const int  acclPin0 = 0;  // the pin that the pushbutton is attached to
const int  acclPin1 = 1;  // the pin that the pushbutton is attached to
const int  acclPin2 = 2;  // the pin that the pushbutton is attached to

const int  acclPin3 = 3;  // the pin that the pushbutton is attached to
const int  acclPin4 = 4;  // the pin that the pushbutton is attached to
const int  acclPin5 = 5;  // the pin that the pushbutton is attached to

const int  acclPin6 = 6;  // the pin that the pushbutton is attached to
const int  acclPin7 = 7;  // the pin that the pushbutton is attached to
const int  acclPin8 = 8;  // the pin that the pushbutton is attached to

const int  acclPin9 = 9;  // the pin that the pushbutton is attached to
const int  acclPin10 = 10;  // the pin that the pushbutton is attached to
const int  acclPin11 = 11;  // the pin that the pushbutton is attached to

void setup() {
  // initialize the button pin as a input:
  pinMode(acclPin0, INPUT);
  pinMode(acclPin1, INPUT);
  pinMode(acclPin2, INPUT);
   
  pinMode(acclPin3, INPUT);
  pinMode(acclPin4, INPUT);
  pinMode(acclPin5, INPUT);
  
  pinMode(acclPin6, INPUT);
  pinMode(acclPin7, INPUT);
  pinMode(acclPin8, INPUT);
  
  pinMode(acclPin9, INPUT);
  pinMode(acclPin10, INPUT);
  pinMode(acclPin11, INPUT);

  // initialize serial communication:
  Serial.begin(115200);
}

// A printf function, taken from...
// https://gist.github.com/EleotleCram/eb586037e2976a8d9884
int aprintf(const char *str, ...) {
  int i, j, count = 0;
  va_list argv;
  va_start(argv, str);
  for(i = 0, j = 0; str[i] != '\0'; ++i) {
    if (str[i] == '%') {
      count++;

      Serial.write(reinterpret_cast<const uint8_t*>(str+j), i-j);
      int v;
      switch (str[++i]) {
      case 'd': 
        v = va_arg(argv, int);
        if(v < 100){
          Serial.print(0); 
          if(v < 10){
            Serial.print(0); 
          }
        }
        Serial.print(v);
        break;
      case 'l': 
        Serial.print(va_arg(argv, long));
        break;
      case 'f': 
        Serial.print(va_arg(argv, double));
        break;
      case 'c': 
        Serial.print((char) va_arg(argv, int));
        break;
      case 's': 
        Serial.print(va_arg(argv, char *));
        break;
      case '%': 
        Serial.print("%");
        break;
      default:;
      };

      j = i+1;
    }
  };
  va_end(argv);

  if(i > j) {
    Serial.write(reinterpret_cast<const uint8_t*>(str+j), i-j);
  }

  return count;
}

void loop() {
  //if(Serial.available() > 0){
    //Serial.read();
    //delay(100);
    aprintf(":%d%d%d%d%d%d%d%d%d%d%d%d",
    //aprintf(":%d %d %d - %d %d %d - %d %d %d - %d %d %d\n",
    analogRead(acclPin0), analogRead(acclPin1),  analogRead(acclPin2),
    analogRead(acclPin3), analogRead(acclPin4),  analogRead(acclPin5),
    analogRead(acclPin6), analogRead(acclPin7),  analogRead(acclPin8),
    analogRead(acclPin9), analogRead(acclPin10), analogRead(acclPin11));
    //Serial.flush();
  //}
}


