//https://github.com/miguelbalboa/rfid

#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal.h>
#include <Servo.h>
#define RST_PIN 9
#define SS_PIN 10

byte readCard[4];
char* myTags[100] = {};
int tagsCount = 0;
String tagID = "";
boolean successRead = false;
boolean correctTag = false;
int proximitySensor;
boolean doorOpened = false;
MFRC522 mfrc522(SS_PIN, RST_PIN);
LiquidCrystal lcd(2, 3, 4, 5, 6, 7); //Parameters: (rs, enable, d4, d5, d6, d7)
Servo myServo;

void setup() {
SPI.begin(); 
mfrc522.PCD_Init();
lcd.begin(16, 2);
myServo.attach(8);
myServo.write(10); 

lcd.print("-No Master Tag!-");
lcd.setCursor(0, 1);
lcd.print(" SCAN NOW");

while (!successRead) {
successRead = getID();
  
if ( successRead == true) {

myTags[tagsCount] = strdup(tagID.c_str());
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("Master Tag Set!");
tagsCount++;
}
}
successRead = false;
printNormalModeMessage();
}

void loop() {
int proximitySensor = analogRead(A0);
if (proximitySensor > 200) {
if ( ! mfrc522.PICC_IsNewCardPresent()) { //If a new PICC placed to RFID reader continue
return;
}
  
if ( ! mfrc522.PICC_ReadCardSerial()) { //Since a PICC placed get Serial and continue
return;
}
  
tagID = "";
  
for ( uint8_t I = 0; I < 4; i++) { 
readCard[i] = mfrc522.uid.uidByte[i];
tagID.concat(String(mfrc522.uid.uidByte[i], HEX)); 
}

tagID.toUpperCase();
mfrc522.PICC_HaltA(); 
correctTag = false;


if (tagID == myTags[0]) {
lcd.clear();
lcd.print("Program mode:");
lcd.setCursor(0, 1);
lcd.print("Add/Remove Tag");

while (!successRead) {
successRead = getID();
if ( successRead == true) {
for (int I = 0; I < 100; i++) {
if (tagID == myTags[i]) {
myTags[i] = "";
lcd.clear();
lcd.setCursor(0, 0);
lcd.print(" Tag Removed!");
printNormalModeMessage();
return;
}
}
myTags[tagsCount] = strdup(tagID.c_str());
lcd.clear();
lcd.setCursor(0, 0);
lcd.print(" Tag Added!");
printNormalModeMessage();
tagsCount++;
return;
}

}

}

successRead = false;

for (int I = 0; I < 100; i++) {

if (tagID == myTags[i]) {

lcd.clear();

lcd.setCursor(0, 0);

lcd.print(" Access Granted!");


  
myServo.write(170); //unlock



  
printNormalModeMessage();

correctTag = true;

}

}

if (correctTag == false) {

lcd.clear();

lcd.setCursor(0, 0);

lcd.print(" Access Denied!");

printNormalModeMessage();

}

}

else {

lcd.clear();

lcd.setCursor(0, 0);

lcd.print(" Door Opened!");

while (!doorOpened) {

proximitySensor = analogRead(A0);

if (proximitySensor > 200) {

doorOpened = true;

}

}

doorOpened = false;

delay(500);

myServo.write(10);

printNormalModeMessage();

}

}

uint8_t getID() {



if ( ! mfrc522.PICC_IsNewCardPresent()) {

return 0;

}

if ( ! mfrc522.PICC_ReadCardSerial()) { //Since a PICC placed get Serial and continue

return 0;

}

tagID = "";

for ( uint8_t I = 0; I < 4; i++) {

readCard[i] = mfrc522.uid.uidByte[i];

tagID.concat(String(mfrc522.uid.uidByte[i], HEX));

}

tagID.toUpperCase();

mfrc522.PICC_HaltA();
return 1;

}

void printNormalModeMessage() {

delay(1500);

lcd.clear();

lcd.print("-Access Control-");

lcd.setCursor(0, 1);

lcd.print(" Scan Your Tag!");

}
