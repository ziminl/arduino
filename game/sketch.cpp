#include <Mouse.h>

void setup() {
  Serial.begin(9600);
  Mouse.begin();
}

void loop() {
  if (Serial.available() > 0) {
    char inChar = (char)Serial.read();
    if (inChar == 'C') {
      Mouse.click(MOUSE_LEFT);
    }
  }
}
