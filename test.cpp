//Arduino IDE
//tools > Board and select your Arduino board model.
//Tools > Port and select the port your Arduino is connected to.

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
}
void loop() {
  digitalWrite(LED_BUILTIN, HIGH);   
  delay(1000);                       
  digitalWrite(LED_BUILTIN, LOW);    
  delay(1000);                      
}
