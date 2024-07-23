#include <Keyboard.h>

//include <Mouse.h>

void setup() {
  Keyboard.begin();
}

void loop() {
  Keyboard.press('a');
  delay(100);
  Keyboard.releaseAll();
  delay(1000);
}


