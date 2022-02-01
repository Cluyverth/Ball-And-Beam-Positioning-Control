
//! INCLUDES
#include <Ultrasonic.h>
#include <Servo.h>
#include <PID_v1.h>
#include <ResponsiveAnalogRead.h>

//! PIN DEFINITIONS
#define servoPin 9
#define pTrigger 12
#define pEcho 11

//! RAMP DEFINITIONS
#define distMax 30
#define distMin 3

//! PID
float Kp = 2.5; // P value
float Ki = 0.2; // I value
float Kd = 0.7; // D value

//! Variables
double setpoint, input, output; //? Create variables to PID object
//int correctionServo = 0;

//! PID & Sensor
PID myPID(&input, &output, &setpoint, Kp, Ki, Kd, DIRECT); //? Create a PID object
ResponsiveAnalogRead analog(0, true); //? Create a ResponsiveAnalogRead object
Servo myServo; //? Create a Servo object
Ultrasonic ultrasonic(pTrigger, pEcho); //? create a Ultrasonic object

//! Functions
float readUltrasonicSensor(){
  int rawDist=0, filteredDist=0;
  rawDist = ultrasonic.read();
  analog.update(rawDist);
  filteredDist = analog.getValue();
  return (filteredDist);
}

void printmonitor(){ //? plot the graph in the monitor
  Serial.print(ultrasonic.read());
  Serial.print("\t");
  Serial.print(analog.getValue());
  Serial.println("");
  delay(20);
}

//! Setup
void setup() {
      Serial.begin(9600);
      myServo.attach(servoPin); //? set the servo pin
      myPID.SetMode(AUTOMATIC); //? set PID to automatic
      myPID.SetOutputLimits(-90,90); //? set PID limit between -90 and 90
}

//! Loop
void loop() {
  setpoint = 8; //? set point
  input = readUltrasonicSensor(); //? save the current filtered distance to be the PID input
  myPID.Compute(); //? calculate the PID output
  // correctionServo = 90 + output //! use as servo angle if needed
  myServo.write(output); //? write the output to the Servomotor
}