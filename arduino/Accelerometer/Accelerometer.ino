//Arduino Sketch
const int  buttonPin0 = 0;  // the pin that the pushbutton is attached to
const int  buttonPin1 = 1;  // the pin that the pushbutton is attached to
const int  buttonPin2 = 2;  // the pin that the pushbutton is attached to


int buttonState0 = 0;         // current state of the button
int buttonState1 = 0;         // current state of the button
int buttonState2 = 0;         // current state of the button
int buttonState3 = 0;         // current state of the button

void setup() {
  // initialize the button pin as a input:
  pinMode(buttonPin0, INPUT);
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);

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
  // read the pushbutton input pin:
  buttonState0 = analogRead(buttonPin0);
  buttonState1 = analogRead(buttonPin1);
  buttonState2 = analogRead(buttonPin2);

  if(Serial.available() > 0){
    Serial.read();
    aprintf(":%d%d%d%d%d%d%d%d%d%d%d%d",
    buttonState0, buttonState1, buttonState2,
    0, 0, 0,
    0, 0, 0 , 
    0, 0, 0);
  }
}


