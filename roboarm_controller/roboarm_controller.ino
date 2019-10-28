/* Sweep
 by Filipe Barretto
*/

#include <Servo.h>


// SERVOS DEFAULT POSITIONS
const int CLAW_SERVO_PIN = 8;
const int CLAW_SERVO_SPEED = 15;
const int CLAW_SERVO_MIN_ANGLE = 150;
const int CLAW_SERVO_MAX_ANGLE = 180;

const int BASE_SERVO_PIN = 9;
const int BASE_SERVO_SPEED = 25;
const int BASE_SERVO_MIN_ANGLE = 0;
const int BASE_SERVO_CENTER_ANGLE = 90;
const int BASE_SERVO_MAX_ANGLE = 150;

const int LEFT_SERVO_PIN = 10;
const int LEFT_SERVO_SPEED = 15;
const int LEFT_SERVO_MIN_ANGLE = 30;
const int LEFT_SERVO_MAX_ANGLE = 170;

const int RIGHT_SERVO_PIN = 11;
const int RIGHT_SERVO_SPEED = 25;
const int RIGHT_SERVO_MIN_ANGLE = 90;
const int RIGHT_SERVO_MAX_ANGLE = 140;

// ROBOARM COMMANDS
const String OPEN_CLAW_COMMAND = "open_claw";
const String CLOSE_CLAW_COMMAND = "close_claw";
const String BASE_RIGHT_COMMAND = "base_right";
const String BASE_LEFT_COMMAND = "base_left";
const String BASE_CENTER_COMMAND = "base_center";
const String RAISE_ARM_COMMAND = "raise_arm";
const String LOWER_ARM_COMMAND = "lower_arm";
const String ARM_FORWARD_COMMAND = "arm_forward";
const String ARM_BACK_COMMAND = "arm_back";
const String RESET_COMMAND = "reset";
const String SERVE_CAPSULE_COMMAND = "serve_capsule";



// DEFAULT VARIABLES
const int DEFAULT_DELAY = 800;

Servo claw_servo;
Servo base_servo;
Servo left_servo;
Servo right_servo;


int pos = 90;    // variable to store the servo position

int incomingByte = 0;   // for incoming serial data
String command = "";


// CONTROL CLAW SERVO (OPEN AND CLOSE CLAW)
void open_claw() {
  for (pos = claw_servo.read(); pos >= CLAW_SERVO_MIN_ANGLE; pos -= 1) {
    claw_servo.write(pos);
    delay(CLAW_SERVO_SPEED);
  }
}

void close_claw() {
  for (pos = claw_servo.read(); pos <= CLAW_SERVO_MAX_ANGLE; pos += 1) {
    claw_servo.write(pos);
    delay(CLAW_SERVO_SPEED);
  }
}

// CONTROL BASE SERVO (TURN LEFT AND RIGHT)
void turn_left() {
  for (pos = base_servo.read(); pos >= BASE_SERVO_MIN_ANGLE; pos -= 1) {
    base_servo.write(pos);
    delay(BASE_SERVO_SPEED);
  }
}

void turn_right() {
  for (pos = base_servo.read(); pos <= BASE_SERVO_MAX_ANGLE; pos += 1) {
    base_servo.write(pos);
    delay(BASE_SERVO_SPEED);
  }
}

void center_base() {
  if (base_servo.read() < BASE_SERVO_CENTER_ANGLE) {
    for (pos = base_servo.read(); pos <= BASE_SERVO_CENTER_ANGLE; pos += 1) {
      base_servo.write(pos);
      delay(BASE_SERVO_SPEED);
    }
  } else {
    for (pos = base_servo.read(); pos > BASE_SERVO_CENTER_ANGLE; pos -= 1) {
      base_servo.write(pos);
      delay(BASE_SERVO_SPEED);
    }
  }
}

// CONTROL LEFT SERVO (RAISE AND LOWER ARM)
void lower_arm() {
  for (pos = left_servo.read(); pos >= LEFT_SERVO_MIN_ANGLE; pos -= 1) {
    left_servo.write(pos);
    delay(LEFT_SERVO_SPEED);
  }
}

void raise_arm() {
  for (pos = left_servo.read(); pos <= LEFT_SERVO_MAX_ANGLE; pos += 1) {
    left_servo.write(pos);
    delay(LEFT_SERVO_SPEED);
  }
}


// CONTROL RIGHT SERVO (MOVE ARM FORWARD AND BACK)
void arm_back() {
  for (pos = right_servo.read(); pos >= RIGHT_SERVO_MIN_ANGLE; pos -= 1) {
    right_servo.write(pos);
    delay(LEFT_SERVO_SPEED);
  }
}

void arm_forward() {
  for (pos = right_servo.read(); pos <= RIGHT_SERVO_MAX_ANGLE; pos += 1) {
    right_servo.write(pos);
    delay(RIGHT_SERVO_SPEED);
  }
}


// OTHER FUNCTIONS

void reset() {
  close_claw();
  center_base();
  raise_arm();
  arm_back();
}

void grab_capsule() {
  turn_right();
  delay(DEFAULT_DELAY);
  open_claw();
  delay(DEFAULT_DELAY);
  arm_forward();
  delay(DEFAULT_DELAY);
  close_claw();
  delay(DEFAULT_DELAY);
  arm_back();
  delay(DEFAULT_DELAY);
}

void drop_capsule() {
  turn_left();
  delay(DEFAULT_DELAY);
  arm_forward();
  delay(DEFAULT_DELAY);
  open_claw();
  delay(DEFAULT_DELAY);
  arm_back();
  delay(DEFAULT_DELAY);
  close_claw();
}

void serve_capsule() {
  reset();
  grab_capsule();
  drop_capsule();
  reset();
}

void setup() {
  Serial.begin(9600);
  claw_servo.attach(CLAW_SERVO_PIN);
  close_claw();

  base_servo.attach(BASE_SERVO_PIN);
  center_base();

  left_servo.attach(LEFT_SERVO_PIN);
  raise_arm();

  right_servo.attach(RIGHT_SERVO_PIN);
  arm_back();
  
}

void loop() {

  if (Serial.available() > 0) {
      // READ INCOMING SERIAL BYTE
      command = Serial.readString();
      command.trim();
      Serial.println(command);

      if (command.equals(OPEN_CLAW_COMMAND)) {
          Serial.println("OPENING CLAW...");
          open_claw();
          Serial.println("CLAW OPENED.");
      } else if (command.equals(CLOSE_CLAW_COMMAND)) {
          Serial.println("CLOSING CLAW...");
          close_claw();
          Serial.println("CLAW CLOSED");
      } else if (command.equals(BASE_RIGHT_COMMAND)) {
          Serial.println("TURNING RIGHT...");
          turn_right();
          Serial.println("TURNED RIGHT.");
      } else if (command.equals(BASE_LEFT_COMMAND)) {
          Serial.println("TURNING LEFT...");
          turn_left();
          Serial.println("TURNED LEFT.");
      } else if (command.equals(BASE_CENTER_COMMAND)) {
          Serial.println("CENTERING BASE...");
          center_base();
          Serial.println("BASE CENTERED.");
      } else if (command.equals(RAISE_ARM_COMMAND)) {
          Serial.println("RAISING ARM...");
          raise_arm();
          Serial.println("ARM RAISED.");
      } else if (command.equals(LOWER_ARM_COMMAND)) {
          Serial.println("LOWERING ARM...");
          lower_arm();
          Serial.println("ARM LOWERED.");
      } else if (command.equals(ARM_BACK_COMMAND)) {
          Serial.println("MOVING ARM BACK...");
          arm_back();
          Serial.println("ARM MOVED BACK.");
      } else if (command.equals(ARM_FORWARD_COMMAND)) {
          Serial.println("MOVING ARM FORWARD...");
          arm_forward();
          Serial.println("ARM MOVED FORWARD.");
      } else if (command.equals(RESET_COMMAND)) {
          Serial.println("RESETING POSITION...");
          reset();
          Serial.println("POSITION RESET.");
      } else if (command.equals(SERVE_CAPSULE_COMMAND)) {
          Serial.println("SERVING CAPSULE...");
          serve_capsule();
          Serial.println("CAPSULE SERVED.");
      } else if (incomingByte == 103) {
          grab_capsule();
      } else if (incomingByte == 100) {
          drop_capsule();
      } else if (incomingByte == 114) {
          reset();
      } else if (incomingByte == 116) {
        for (int i = 0; i < 3; i += 1) {
          grab_capsule();
          drop_capsule(); 
        }
      }
      
      
   }
  
}
