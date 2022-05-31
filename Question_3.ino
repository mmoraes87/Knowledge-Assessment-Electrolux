//**********************************************//
// Global Variables definitions                 //
//**********************************************//
typedef struct mathResults {
  float average;
  uint16_t maxValue;
  uint16_t minValue;
  uint16_t *newArray;     // pointer to the new vector, as its size is only known during runtime
  uint8_t sizeNewArray;
} mathResults;

//**********************************************//
// Functions prototypes                         //
//**********************************************//
mathResults doCalculation(uint16_t numArray[], const uint8_t arrayLen);
void printResults(mathResults results);


void setup() {
  Serial.begin(115200);   // init Arduino Serial
  while(!Serial);         // wait for Serial to be initialized
}

void loop() {
  uint16_t testArray[] = {2, 24, 163, 102, 54, 2545, 2354, 127, 845, 65300, 987};
  
  mathResults results = doCalculation(testArray, sizeof(testArray) / sizeof(uint16_t));
  printResults(results);
}

/* **************************************************** */
/* Function name:        doCalculation                  */
/* Function description: calculate average, minimum and */
/*                       maximum value of a given array */
/*                       also create a new array with   */
/*                       the even numbers and its size  */
/* Input params:    numArray: array with given values   */
/*                  arrayLen: size of given array       */
/* Outpu params:    mathResults: struct with results    */
/* **************************************************** */
mathResults doCalculation(uint16_t numArray[], const uint8_t arrayLen) {
  // declare and initialize local variables
  uint16_t evenNumbers[arrayLen];
  uint16_t maxV = 0;              // initialize the maximum value as the minumum possible
  uint16_t minV = 0xFFFF;         // initialize the minimum value as the maximum possible
  uint32_t soma = 0;
  uint8_t newIndex = 0;           // index counter for the new vector
  mathResults res;                // variable to store all the results

  // iterate the given array
  for (int i = 0; i < arrayLen; i++) {
    uint16_t val = numArray[i];         // get current array value
    
    soma += val;      // sum all the values to calculate the average
    
    if (val > maxV)   // check if current value is the new MAX value
      maxV = val;

    if (val < minV)   // check if current value is the new MIN value
      minV = val;

    // check if number is even to add to a new array
    if (!(val % 2))
      evenNumbers[newIndex++] = val;
  }

  // store all the data to the return variable
  res.average = soma / arrayLen;
  res.maxValue = maxV;
  res.minValue = minV;
  res.newArray = evenNumbers;
  res.sizeNewArray = newIndex;    // the index of the next position is the size of the array

  return res;
}

/* **************************************************** */
/* Function name:        printResults                   */
/* Function description: print results through serial   */
/* Input params:    mathResults: computed values        */
/* Outpu params:     n/a                                */
/* **************************************************** */
void printResults(mathResults results) {
  Serial.print("Average: "); Serial.println(results.average);
  Serial.print("Max value: "); Serial.println(results.maxValue);
  Serial.print("Min value: "); Serial.println(results.minValue);
  Serial.print("Even numbers: [");

  for (int i = 0; i < results.sizeNewArray; i++) {
    if (i == 0) {
      Serial.print(results.newArray[i]);
    }
    else {
      Serial.print(", ");
      Serial.print(results.newArray[i]);
    }
  }
  Serial.println("]\nSize new array: "); Serial.println(results.sizeNewArray);
  
  Serial.print("Average: "); Serial.println(results.average);

  delay(10000);
}
