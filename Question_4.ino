//**********************************************//
// Global Variables definitions                 //
//**********************************************//
volatile byte msgReceived = 0;


void setup() {
  // Hardware serial initialization
  // Hardware interrupt configuration
}


// due the the lack of time and the Arduino IDE (already implements the ISR)
// this just simulates the flag of received message from UART
void loop() {
  if (msgReceived)   {
    msgReceived = 0;    // reset flag

    byte command, index = 0, message[64];
    int32_t value;

    // read all the message
    while (Serial.available() > 0) {
      message[index++] = Serial.read();
    }

    // check message length and CRC. Expected 5 bytes: command = 1, values = 4
    if (messageIntegrity(message)) {
      // parse the command, since the message is OK
      command = message[0];   // first byte is a command: 0-255 possible commands

      // parse the value: 4 bytes MSB first
      for (int i = 1; i < 5; i++) {
        value = (value << 8) + message[i];
      }

      // do something with the information received
      executeInstruction(command, value);
    }
  }
}

// for a short ISR, just set a flag and poll the information each cycle
// parsing and exectuing instruction could be slow. this prevents losing
// other interruptions. a queue could also be implemented to avoid
// message overwriting
ISR(USART0_RX_vect) {
  msgReceived = 1;      // set flag
}
