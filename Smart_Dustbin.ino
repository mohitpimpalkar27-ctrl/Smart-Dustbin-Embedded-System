#include <Servo.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// =====================================================
//                    PIN DEFINITIONS
// =====================================================

// Hand detection HC-SR04
const int HAND_TRIG = 2;
const int HAND_ECHO = 3;

// Garbage level HC-SR04
const int LEVEL_TRIG = 4;
const int LEVEL_ECHO = 5;

// Servo
const int SERVO_PIN = 6;

// Push button
const int BUTTON_PIN = 7;

// LEDs
const int GREEN_LED = 8;
const int YELLOW_LED = 9;
const int RED_LED = 10;

// Buzzer
const int BUZZER = 11;


// =====================================================
//                    OLED SETTINGS
// =====================================================

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET -1

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  OLED_RESET
);


// =====================================================
//                    SERVO OBJECT
// =====================================================

Servo lidServo;


// =====================================================
//                    SYSTEM SETTINGS
// =====================================================

// Height of the virtual dustbin
const float BIN_HEIGHT_CM = 30.0;

// Hand distance for opening
const float HAND_OPEN_DISTANCE = 20.0;

// Hand distance for closing
const float HAND_CLOSE_DISTANCE = 30.0;

// Servo angles
const int OPEN_ANGLE = 90;
const int CLOSED_ANGLE = 0;

// Garbage level thresholds
const int WARNING_LEVEL = 60;
const int FULL_LEVEL = 85;


// =====================================================
//                    VARIABLES
// =====================================================

bool lidOpen = false;

bool previousHandDetected = false;

bool oledAvailable = false;

unsigned long disposalCount = 0;

unsigned long lastOLEDUpdate = 0;

unsigned long lastSerialUpdate = 0;

unsigned long lastBuzzerChange = 0;

unsigned long lastButtonChange = 0;

bool buzzerState = false;

bool lastButtonState = HIGH;


// =====================================================
//                    SETUP
// =====================================================

void setup()
{
  Serial.begin(9600);


  // ---------------------------------------------------
  // Ultrasonic sensors
  // ---------------------------------------------------

  pinMode(HAND_TRIG, OUTPUT);
  pinMode(HAND_ECHO, INPUT);

  pinMode(LEVEL_TRIG, OUTPUT);
  pinMode(LEVEL_ECHO, INPUT);


  // ---------------------------------------------------
  // LEDs
  // ---------------------------------------------------

  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);


  // ---------------------------------------------------
  // Buzzer
  // ---------------------------------------------------

  pinMode(BUZZER, OUTPUT);


  // ---------------------------------------------------
  // Push button
  // ---------------------------------------------------

  pinMode(BUTTON_PIN, INPUT_PULLUP);


  // ---------------------------------------------------
  // Servo
  // ---------------------------------------------------

  lidServo.attach(SERVO_PIN);

  lidServo.write(CLOSED_ANGLE);


  // ---------------------------------------------------
  // Turn outputs OFF
  // ---------------------------------------------------

  digitalWrite(GREEN_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(RED_LED, LOW);
  digitalWrite(BUZZER, LOW);


  // ---------------------------------------------------
  // OLED INITIALIZATION
  // ---------------------------------------------------

  Wire.begin();

  if (display.begin(
        SSD1306_SWITCHCAPVCC,
        0x3C))
  {
    oledAvailable = true;

    display.clearDisplay();

    display.setTextColor(SSD1306_WHITE);

    display.setTextSize(2);

    display.setCursor(5, 5);
    display.println("SMART");

    display.setCursor(5, 28);
    display.println("DUSTBIN");

    display.setTextSize(1);

    display.setCursor(20, 52);
    display.println("SYSTEM START");

    display.display();

    delay(2000);
  }
  else
  {
    oledAvailable = false;

    Serial.println("WARNING: OLED initialization failed!");
    Serial.println("System will continue without OLED.");
  }


  // ---------------------------------------------------
  // Startup message
  // ---------------------------------------------------

  Serial.println();
  Serial.println("====================================");
  Serial.println("        SMART DUSTBIN SYSTEM");
  Serial.println("====================================");
  Serial.println("System: ONLINE");
  Serial.println("Automatic Lid: ENABLED");
  Serial.println("Garbage Monitoring: ENABLED");
  Serial.println("OLED Display: ENABLED");
  Serial.println("Disposal Counter: ENABLED");
  Serial.println("====================================");
  Serial.println();
}


// =====================================================
//                 ULTRASONIC FUNCTION
// =====================================================

float readDistance(int trigPin, int echoPin)
{
  // Ensure trigger is LOW
  digitalWrite(trigPin, LOW);

  delayMicroseconds(2);


  // Send trigger pulse
  digitalWrite(trigPin, HIGH);

  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);


  // Read echo
  unsigned long duration =
    pulseIn(
      echoPin,
      HIGH,
      30000
    );


  // Timeout / sensor error
  if (duration == 0)
  {
    return -1;
  }


  // Convert microseconds to centimetres
  float distance =
    duration / 58.0;


  return distance;
}


// =====================================================
//                 FILL CALCULATION
// =====================================================

int calculateFillPercentage(float distance)
{
  if (distance < 0)
  {
    return -1;
  }


  float fillPercentage =
    ((BIN_HEIGHT_CM - distance)
     / BIN_HEIGHT_CM) * 100.0;


  // Minimum 0%
  if (fillPercentage < 0)
  {
    fillPercentage = 0;
  }


  // Maximum 100%
  if (fillPercentage > 100)
  {
    fillPercentage = 100;
  }


  return (int)fillPercentage;
}


// =====================================================
//                 BIN STATUS
// =====================================================

String getBinStatus(int fillPercentage)
{
  if (fillPercentage < 0)
  {
    return "ERROR";
  }


  if (fillPercentage < WARNING_LEVEL)
  {
    return "NORMAL";
  }


  if (fillPercentage < FULL_LEVEL)
  {
    return "WARNING";
  }


  return "FULL";
}


// =====================================================
//                 AUTOMATIC LID CONTROL
// =====================================================

void controlLid(float handDistance)
{
  bool handDetected = false;


  // -----------------------------------------------
  // Detect hand
  // -----------------------------------------------

  if (
    handDistance > 0 &&
    handDistance <= HAND_OPEN_DISTANCE
  )
  {
    handDetected = true;
  }


  // -----------------------------------------------
  // OPEN LID
  // -----------------------------------------------

  if (
    handDetected &&
    !lidOpen
  )
  {
    Serial.println(">>> HAND DETECTED");
    Serial.println(">>> LID OPENING");


    lidServo.write(OPEN_ANGLE);

    lidOpen = true;


    // Count a new disposal event only once
    if (!previousHandDetected)
    {
      disposalCount++;

      Serial.print(">>> Disposal Event: ");
      Serial.println(disposalCount);
    }
  }


  // -----------------------------------------------
  // CLOSE LID
  // -----------------------------------------------

  if (
    !handDetected &&
    lidOpen &&
    handDistance >= HAND_CLOSE_DISTANCE
  )
  {
    Serial.println(">>> USER MOVED AWAY");
    Serial.println(">>> LID CLOSING");


    lidServo.write(CLOSED_ANGLE);

    lidOpen = false;
  }


  // -----------------------------------------------
  // If sensor gives error while lid is open,
  // close the lid for safety.
  // -----------------------------------------------

  if (
    handDistance < 0 &&
    lidOpen
  )
  {
    Serial.println(">>> HAND SENSOR ERROR");
    Serial.println(">>> SAFETY: CLOSING LID");


    lidServo.write(CLOSED_ANGLE);

    lidOpen = false;
  }


  previousHandDetected = handDetected;
}


// =====================================================
//              LED + BUZZER CONTROL
// =====================================================

void updateBinStatus(int fillPercentage)
{
  // Turn all LEDs OFF first

  digitalWrite(GREEN_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(RED_LED, LOW);


  // ---------------------------------------------------
  // SENSOR ERROR
  // ---------------------------------------------------

  if (fillPercentage < 0)
  {
    digitalWrite(RED_LED, HIGH);


    // Error buzzer
    if (
      millis() - lastBuzzerChange >= 1000
    )
    {
      lastBuzzerChange = millis();

      buzzerState = !buzzerState;

      digitalWrite(
        BUZZER,
        buzzerState
      );
    }

    return;
  }


  // ---------------------------------------------------
  // NORMAL
  // ---------------------------------------------------

  if (fillPercentage < WARNING_LEVEL)
  {
    digitalWrite(GREEN_LED, HIGH);

    digitalWrite(BUZZER, LOW);

    buzzerState = false;
  }


  // ---------------------------------------------------
  // WARNING
  // ---------------------------------------------------

  else if (fillPercentage < FULL_LEVEL)
  {
    digitalWrite(YELLOW_LED, HIGH);

    digitalWrite(BUZZER, LOW);

    buzzerState = false;
  }


  // ---------------------------------------------------
  // FULL
  // ---------------------------------------------------

  else
  {
    digitalWrite(RED_LED, HIGH);


    // Buzzer ON/OFF every 500 ms

    if (
      millis() - lastBuzzerChange >= 500
    )
    {
      lastBuzzerChange = millis();

      buzzerState = !buzzerState;

      digitalWrite(
        BUZZER,
        buzzerState
      );
    }
  }
}


// =====================================================
//                 OLED DISPLAY
// =====================================================

void updateOLED(
  float handDistance,
  float levelDistance,
  int fillPercentage
)
{
  if (!oledAvailable)
  {
    return;
  }


  display.clearDisplay();


  // ---------------------------------------------------
  // Title
  // ---------------------------------------------------

  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(1);

  display.setCursor(20, 0);

  display.println("SMART DUSTBIN");


  // ---------------------------------------------------
  // Fill percentage
  // ---------------------------------------------------

  display.setCursor(0, 12);

  display.print("Fill: ");


  if (fillPercentage < 0)
  {
    display.println("ERROR");
  }
  else
  {
    display.print(fillPercentage);
    display.println("%");
  }


  // ---------------------------------------------------
  // Fill bar
  // ---------------------------------------------------

  display.drawRect(
    0,
    22,
    128,
    11,
    SSD1306_WHITE
  );


  if (fillPercentage >= 0)
  {
    int barWidth =
      map(
        fillPercentage,
        0,
        100,
        0,
        124
      );


    display.fillRect(
      2,
      24,
      barWidth,
      7,
      SSD1306_WHITE
    );
  }


  // ---------------------------------------------------
  // Status
  // ---------------------------------------------------

  display.setCursor(0, 36);

  display.print("Status: ");

  display.println(
    getBinStatus(fillPercentage)
  );


  // ---------------------------------------------------
  // Lid
  // ---------------------------------------------------

  display.setCursor(0, 46);

  display.print("Lid: ");

  if (lidOpen)
  {
    display.print("OPEN");
  }
  else
  {
    display.print("CLOSED");
  }


  // ---------------------------------------------------
  // Disposal counter
  // ---------------------------------------------------

  display.setCursor(75, 46);

  display.print("#");

  display.print(disposalCount);


  // ---------------------------------------------------
  // System status
  // ---------------------------------------------------

  display.setCursor(0, 57);

  display.print("System: ONLINE");


  display.display();
}


// =====================================================
//                 SERIAL MONITOR
// =====================================================

void printSystemStatus(
  float handDistance,
  float levelDistance,
  int fillPercentage
)
{
  Serial.println("------------------------------------");


  Serial.print("Hand Distance: ");

  if (handDistance < 0)
  {
    Serial.println("SENSOR ERROR");
  }
  else
  {
    Serial.print(handDistance);
    Serial.println(" cm");
  }


  Serial.print("Bin Distance: ");

  if (levelDistance < 0)
  {
    Serial.println("SENSOR ERROR");
  }
  else
  {
    Serial.print(levelDistance);
    Serial.println(" cm");
  }


  Serial.print("Fill Level: ");

  if (fillPercentage < 0)
  {
    Serial.println("SENSOR ERROR");
  }
  else
  {
    Serial.print(fillPercentage);
    Serial.println("%");
  }


  Serial.print("Status: ");

  Serial.println(
    getBinStatus(fillPercentage)
  );


  Serial.print("Lid: ");

  if (lidOpen)
  {
    Serial.println("OPEN");
  }
  else
  {
    Serial.println("CLOSED");
  }


  Serial.print("Disposal Count: ");

  Serial.println(disposalCount);


  Serial.println("------------------------------------");
}


// =====================================================
//                 BUTTON CONTROL
// =====================================================

void checkButton()
{
  bool currentButtonState =
    digitalRead(BUTTON_PIN);


  // Detect button press
  if (
    lastButtonState == HIGH &&
    currentButtonState == LOW
  )
  {
    // Simple debounce
    if (
      millis() - lastButtonChange > 200
    )
    {
      lastButtonChange = millis();


      Serial.println();
      Serial.println(">>> BUTTON PRESSED");
      Serial.println(">>> RESETTING DISPOSAL COUNT");


      disposalCount = 0;
    }
  }


  lastButtonState =
    currentButtonState;
}


// =====================================================
//                     MAIN LOOP
// =====================================================

void loop()
{
  // ===================================================
  // Read hand sensor
  // ===================================================

  float handDistance =
    readDistance(
      HAND_TRIG,
      HAND_ECHO
    );


  // Small delay between ultrasonic readings
  delay(30);


  // ===================================================
  // Read garbage level sensor
  // ===================================================

  float levelDistance =
    readDistance(
      LEVEL_TRIG,
      LEVEL_ECHO
    );


  // ===================================================
  // Calculate fill percentage
  // ===================================================

  int fillPercentage =
    calculateFillPercentage(
      levelDistance
    );


  // ===================================================
  // Automatic lid
  // ===================================================

  controlLid(handDistance);


  // ===================================================
  // Garbage status
  // ===================================================

  updateBinStatus(
    fillPercentage
  );


  // ===================================================
  // Push button
  // ===================================================

  checkButton();


  // ===================================================
  // OLED update
  // ===================================================

  if (
    millis() - lastOLEDUpdate >= 200
  )
  {
    lastOLEDUpdate = millis();


    updateOLED(
      handDistance,
      levelDistance,
      fillPercentage
    );
  }


  // ===================================================
  // Serial Monitor update
  // ===================================================

  if (
    millis() - lastSerialUpdate >= 1000
  )
  {
    lastSerialUpdate = millis();


    printSystemStatus(
      handDistance,
      levelDistance,
      fillPercentage
    );
  }


  delay(50);
}
