//**********************************************//
// Constants Definitions                        //
//**********************************************//
#define BTN_DOWN  0
#define BTN_UP    1

#define STATE_OFF       0
#define STATE_ON        1
#define STATE_PROTECTED 2

//**********************************************//
// Functions prototypes                         //
//**********************************************//
void stateMachine();      // function prototype

//**********************************************//
// Global Variables definitions                 //
//**********************************************//
volatile byte btnState = BTN_UP;

void setup() {
  Serial.begin(115200);   // init Arduino Serial
  while(!Serial);         // wait for Serial to be initialized
  
  DDRE &= ~(1 << INT4);   // set pin to input
  EICRB |= (1 << ISC41) | // set pin to falling edge interrupt
           (0 << ISC40);
  EIFR |= (1 << INT4);    // clear pin interrupt flag
  EIMSK |= (1 << INT4);   // enable pin interrupt

  Serial.println("Start => State OFF");
}

void loop() {
  stateMachine();
}

/* **************************************************** */
/* Function name:        stateMachine                   */
/* Function description: state machine implementation   */
/* Input params:    N/A                                 */
/* Outpu params:    N/A                                 */
/* **************************************************** */
void stateMachine() {
  static uint8_t currentState = STATE_OFF;
  static uint32_t timer = 0;
  static uint32_t btnPressedTime = 0;

  switch (currentState) {
    case STATE_OFF: {
      if (btnState == BTN_DOWN) {
        btnPressedTime = millis();    // set start time for button pressed timer
        currentState = STATE_ON;      // set next state
        
        Serial.println("State OFF => State ON");
      }
      break;
    }

    case STATE_ON: {
      if (btnState == BTN_UP) {
        uint32_t endTime = millis();    // set end time for button pressed timer
        timer = endTime;                // set start time for next state timer (non blocking delay)
        currentState = STATE_PROTECTED; // set next state

        // print elapsed time with button pressed
        char str[50];
        sprintf(str, "Button was pressed for: %d ms", (uint16_t)(endTime - btnPressedTime));
        Serial.println(str);
        Serial.println("State ON => State PROTECTED");
      }
      break;
    }

    case STATE_PROTECTED: {
      if (millis() - timer >= 10000) {  // check if 10 seconds has passed
        timer = 0;                      // reset timer value
        currentState = STATE_OFF;       // set next state
        
        Serial.println("State PROTECTED => State ON");
      }
      break;
    }

    default: {
      break;
    }
  }
}

// button was implemented with an active-low switch. Inverted edges!
// also, button has debounce circuitry in hardware
ISR(INT4_vect) {
  if (btnState == BTN_DOWN){
    EICRB &= ~(1 << ISC40);  // set pin to falling edge interrupt
    btnState = BTN_UP;
  }
  else {
    EICRB |= (1 << ISC40);   // set pin to risingg edge interrupt
    btnState = BTN_DOWN;
  }
}
