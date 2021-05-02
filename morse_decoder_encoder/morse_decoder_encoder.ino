/*
  This program encodes text input to the serial monitor to morse code via a flashing LED. 
  When the side button is pressed, the program switches to decoder mode, and by pressing
  a morse code pattern to a button (connected to a LED which will flash when the button 
  is pressed), the code will be decoded to text printed to the serial monitor.
  English language alphabet letters and number digits are acceptable.
*/

static String letters[] = { ".-", "-...", "-.-.", "-..", ".", "..-.", "--.",
                   "....","..", ".---", "-.-", ".-..", "--", "-.",
                   "---", ".--.", "--.-", ".-.", "...", "-", "..-",
                   "...-", ".--", "-..-", "-.--", "--.." };

static String numbers[] = {"-----", ".----", "..---", "...--", "....-", ".....",
                   "-....", "--...", "---..", "----."};

//for encoder
int dotDelay = 200;
int ledPin = 13;
char ch;
//for decoder
int inputPin = 4;
int ledPin2 = 12;
//for switch
int switchPin = 2;

unsigned long signal_len,t1,t2; 
String code = "";  


bool encode = true;
int switchState;

const int BUTTON_BOUNCE = 10;
const int PRESS_DOT = 300;

void setup() {
  pinMode(switchPin, INPUT_PULLUP); //switch button
  pinMode(ledPin, OUTPUT); // encoder led
  pinMode(ledPin2, OUTPUT); // decoder led
  pinMode(inputPin, INPUT_PULLUP); //decoder button
  Serial.begin(9600);
}

void loop() {
  switchState = digitalRead(switchPin);
  if (switchState == LOW) { //if it is, turn on/off encoder
    encode = !encode;
  } 

  if (encode) { // encoder 
    if (Serial.available()) {
      ch = Serial.read();
      if (ch >= 'a' && ch <= 'z') {
          flashSequence(letters[ch - 'a']);
      } else if (ch >= 'A' && ch <= 'Z') {
          flashSequence(letters[ch - 'A']);
      } else if (ch >= '0' && ch <= '9') {
          flashSequence(numbers[ch - '0']);
      } else if (ch == ' ') {
        delay(dotDelay*4);
      }
    }
  } else { // decoder
    NextDotDash();
    while ((millis() - t2) < 700) {       //if time between button press greater than 0.7sec, skip loop and go to next alphabet    
      if (digitalRead(inputPin) == LOW){
          NextDotDash();
      }
    }
    treeConvertor(); 
  }
  
}

void NextDotDash() {
    while (digitalRead(inputPin) == HIGH) {}
    t1 = millis();                            //time at button press
    digitalWrite(ledPin2, HIGH);               //LED on while button pressed
    while (digitalRead(inputPin) == LOW) {}
    t2 = millis();                            //time at button release
    digitalWrite(ledPin2, LOW);                //LED off on button release
    signal_len = t2 - t1;                     //time for which button is pressed
    if (signal_len > BUTTON_BOUNCE) {         //to account for switch debouncing
      code += readCode();                      //function to read dot or dash
    } 
}

// for encoder
void flashSequence(String sequence) {
  for (int j = 0; j < sequence.length(); j++) {
    flash(sequence[j]);
  }
  //int i = 0;
  //while (sequence[i] != NULL) {
    //flash(sequence[i]);
    //i++;
  //}
  delay(dotDelay * 3);
}

// for encoder
void flash(char d) {
  digitalWrite(ledPin, HIGH);
  if (d == '.') {
    delay(dotDelay);
  } else { // -
    delay(dotDelay * 3);
  }
  
  digitalWrite(ledPin, LOW);
  delay(dotDelay);
}

// for decoder
char readCode() {
  if (signal_len < PRESS_DOT && signal_len > BUTTON_BOUNCE) {
    return '.';                        //if button press less than 0.3sec, it is a dot
  }
  else if (signal_len > PRESS_DOT) {
    return '-';                        //if button press more than 0.3sec, it is a dash
  }
}

// for decoder (no numbers)
void convertor() {
  static String letters[] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-",
                             ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..", "\0"
                            };
  int i = 0;
  if (code == ".-.-.-") {
    Serial.print(" ");        //for break
  } else {
    while (letters[i] != "\0") { //loop for comparing input code with letters array
      if (letters[i] == code) {
        Serial.print(char('A' + i));
        break;
      }
      i++;
    }
    if (letters[i] == "\0") {
      Serial.println("");  //if input code doesn't match any letter, error
    }
  }
  code = ""; //reset code to blank string
}

// for decoder
void treeConvertor() {
  static int TREE_SIZE = 63;
  static char tree[] = { '\0', 'E', 'T', 'I', 'A', 'N', 'M', 'S', 'U', 'R', 'W', 'D',
                        'K', 'G', 'O', 'H', 'V', 'F', '\0', 'L', ' ', 'P', 'J', 'B',
                         'X', 'C', 'Y', 'Z', 'Q', '\0', '\0', '5', '4', '\0', '3', '\0', 
                         '\0', '\0', '2', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '1', 
                         '6', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '7', '\0', '\0',
                         '\0', '8', '\0', '9', '0'};
  int i = 0;
  char save;
  //Serial.print(code);
  for (int j = 0; j < code.length(); j++) {
    if (code.charAt(j) == '.') { //dit
      i = 2*i + 1;
    } if (code.charAt(j) == '-') { //dah
      i = 2*i + 2;
    }
  }
  
  if (tree[i] != '\0' && i < 63) {
    Serial.print(tree[i]);
  } else {
    Serial.println("");
  }
  
  code = "";
  
}
