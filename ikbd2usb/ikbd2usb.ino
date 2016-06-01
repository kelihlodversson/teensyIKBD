// set this to the hardware serial port you wish to use
#define HWSERIAL Serial1
extern uint16_t keymap[];
extern uint8_t keyboard_report_data[];

void setup() {
  Serial.begin(9600);
  HWSERIAL.begin(7812.5);
  Joystick.hat(-1); // Seems the lib initializes the hat value to 0 which is recognized as dpad up.
}

void loop() {
  int incomingByte;
  uint8_t media = 0;
  if (HWSERIAL.available() > 0) {
    incomingByte = HWSERIAL.read();
    Serial.print(incomingByte, HEX);
    Serial.print(" ");
    if ((incomingByte & 0xfc) == 0xf8) // Relative mouse reporting is f8-fb
    {
      uint8_t mouseLeft = incomingByte & 0x2;
      uint8_t mouseRight = incomingByte & 0x1;
      if(mouseLeft && !Mouse.isPressed(MOUSE_LEFT))
        Mouse.press(MOUSE_LEFT);
      else if (!mouseLeft && Mouse.isPressed(MOUSE_LEFT))
        Mouse.release(MOUSE_LEFT);
      if(mouseRight && !Mouse.isPressed(MOUSE_RIGHT))
        Mouse.press(MOUSE_RIGHT);
      else if (!mouseRight && Mouse.isPressed(MOUSE_RIGHT))
        Mouse.release(MOUSE_RIGHT);
        
      while(HWSERIAL.available() == 0);
      int8_t deltaX = HWSERIAL.read();
      while(HWSERIAL.available() == 0);
      int8_t deltaY = HWSERIAL.read();
      Serial.print("Mouse (dx: ");
      Serial.print(deltaX);
      Serial.print(" dy: ");
      Serial.print(deltaY);
      Serial.print(")\n");
      Mouse.move(deltaX*6, deltaY*6);
    }
    else if ((incomingByte & 0xfe) == 0xfe) // fe or ff joystick 1 or 2
    {
      int stick = incomingByte & 1;
      while(HWSERIAL.available() == 0);
      uint8_t joystickData = HWSERIAL.read();
      int direction = 0;
      switch(joystickData & 0xF)
      {
         case 0x1:
            direction = 0;
            break;
         case 0x9:
            direction = 45;
            break;
         case 0x8:
            direction = 90;
            break;
         case 0xa:
            direction = 135;
            break;
         case 0x2:
            direction = 180;
            break;
         case 0x6:
            direction = 225;
            break;
         case 0x4:
            direction = 270;
            break;
         case 0x5:
            direction = 315;
            break;
         case 0x0:
         default:
            direction = -1;
            break;
      }
      bool fireButton = joystickData & 0x80;
      Serial.print("Joystick #");
      Serial.print(stick);
      Serial.print(" direction: ");
      Serial.print(direction);
      Serial.print(" button: ");
      Serial.print(fireButton?"ON\n":"OFF\n");
    }
    else if (incomingByte < 0xf6)
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

