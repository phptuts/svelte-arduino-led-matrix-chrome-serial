#include "LedControlMS.h"
LedControl lc=LedControl(12,11,10,1);


void setup() {
   Serial.begin(115200);
  // put your setup code here, to run once:
 /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,8);
  /* and clear the display */
  lc.clearDisplay(0);
  /** 10 milliseconds is all we need anything else to slow */
  Serial.setTimeout(10);

}

void loop() {
    if (Serial.available() <= 0) {
      return;
    }
    
    String usbString = Serial.readStringUntil('|');
    Serial.println(usbString);
    if (usbString.indexOf(':') == -1) {
      return;
    }

    if (usbString == "clear") {
      lc.clearDisplay(0);
      return;
    }

    for (int row = 0; row < 8; row += 1) {
      for (int col = 0; col < 8; col += 1) {
         int ledNumber = row * 8 + col;
         bool isOn = getValue(usbString, ':', ledNumber) == "1";
         lc.setLed(0, row, col, isOn);
         // Serial.println(String(row) + " " + String(col) + " " + String(isOn) + " " + getValue(usbString,':', ledNumber));
      }
    }
   


}

String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}