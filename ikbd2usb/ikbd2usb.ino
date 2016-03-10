// set this to the hardware serial port you wish to use
#define HWSERIAL Serial1
extern uint16_t keymap[];
extern uint8_t keyboard_report_data[];

void setup() {
  Serial.begin(9600);
  HWSERIAL.begin(7812.5);
}

void loop() {
  int incomingByte;
  uint8_t media = 0;
  if (HWSERIAL.available() > 0) {
    incomingByte = HWSERIAL.read();
    Serial.print(incomingByte, HEX);
    Serial.print(" ");
    if (incomingByte < 0xf6)
    {
      uint16_t scancode = keymap[incomingByte&0x7f];
      bool keyUp = incomingByte & 0x80;
      Serial.print(keyUp ? "Key up  ":"Key down ");
      if(scancode & 0xff00)
      {
        if( keyUp )
        {
          Keyboard.release(scancode);
        }
        else
        {
          Keyboard.press(scancode);
        }
      }
      else if (scancode == 0xff) // UNDO
      {
        if( keyUp )
        {
          Keyboard.release(KEY_Z);
          Keyboard.set_modifier(keyboard_report_data[0] & ~(MODIFIERKEY_RIGHT_GUI));
          Keyboard.send_now();
       }
        else
        {
          Keyboard.set_modifier(keyboard_report_data[0] | MODIFIERKEY_RIGHT_GUI);
          Keyboard.send_now();
          Keyboard.press(KEY_Z);
        }
      }
      else // media key
      {
        if (keyUp)
          media &= ~(scancode);
        else
          media |= scancode;
        Keyboard.set_media(media);
        Keyboard.send_now();
      }
      Serial.print(keyboard_report_data[0], HEX);
      Serial.print(" "); 
      Serial.print(" "); 
      Serial.println(scancode, HEX);
    }
  }
}

