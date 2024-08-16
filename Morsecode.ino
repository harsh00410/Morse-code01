#include "thingProperties.h"

// Define the LED pin
const int ledPin = LED_BUILTIN;

// Morse Code Timing (in milliseconds)
const int dotDuration = 200;
const int dashDuration = 600;
const int letterPause = 600;
const int wordPause = 1400;

// Morse code sequence for "HARSH"
String morseCode[] = { "....", ".-", ".-.", "...", "...." };

// Variable to track the current position in Morse code sequence
int currentCharIndex = 0;
bool isBlinking = false;

void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  // Set LED pin as output
  pinMode(ledPin, OUTPUT);
  delay(1500);

  // Initialize cloud properties
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  // Debugging
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
}

void loop() {
  // Update Arduino IoT Cloud
  ArduinoCloud.update();

  // If blinking is active, blink the name in Morse code
  if (isBlinking) {
    blinkName();
  } else {
    digitalWrite(ledPin, LOW);  // Ensure LED is off when not blinking
  }
}

// Function to blink Morse code for a given code
void blinkMorse(String code) {
  for (int i = 0; i < code.length(); i++) {
    if (code[i] == '.') {
      digitalWrite(ledPin, HIGH);
      delay(dotDuration);
      digitalWrite(ledPin, LOW);
    } else if (code[i] == '-') {
      digitalWrite(ledPin, HIGH);
      delay(dashDuration);
      digitalWrite(ledPin, LOW);
    }
    delay(dotDuration);  // Pause between dots and dashes
  }
}

// Function to blink the name "HARSH" in Morse code
void blinkName() {
  // Blink current character
  blinkMorse(morseCode[currentCharIndex]);
  delay(letterPause);  // Pause between letters

  // Move to the next character
  currentCharIndex++;
  
  // If all characters are done, reset to the beginning
  if (currentCharIndex >= 5) {
    currentCharIndex = 0;
    delay(wordPause);  // Pause between repetitions
  }
}

// This function is called whenever the lED variable changes
void onLEDChange() {
  // Start or stop blinking based on the new value of lED
  isBlinking = lED;
  
  // Reset the blinking sequence when starting
  if (isBlinking) {
    currentCharIndex = 0;  // Reset to start of Morse code
  }
}