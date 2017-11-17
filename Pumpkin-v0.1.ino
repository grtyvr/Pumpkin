/* Pumpkin
 *   jiggle some servos, unless you get too close, then
 *   stare at you@
*/

#include <Servo.h>

Servo myservo1;  // create servo object to control a servo
Servo myservo2;
// twelve servo objects can be created on most boards
// that should be enough eyes for non-insects
//

#define trig1 13
#define echo1 12
#define trig2 11
#define echo2 10

#define interOcularDistance 14

int lastPosition1 = 0;
int lastPosition2 = 0;
int lastFix1 =40;           // start as if something is in it's field of view
int lastFix2 = 40;          //

int range1 = 20;
int range2 = 20;

void setup() {
  Serial.begin(9600);
  myservo1.attach(9);  // attaches the servo on pin 9 to the servo object
  myservo2.attach(8);
  pinMode(trig1, OUTPUT);
  pinMode(echo1, INPUT);
  pinMode(trig2, OUTPUT);
  pinMode(echo2, INPUT);
}

void loop() {
  int angle1, angle2;
  long curX;
  
// Look Left then Right;  
// ToDo: switch for Austrailia, Thailand and the UK.
//
  curX = getX(lastFix1, lastFix2);
  if (readDistance(1) <= 50 ) {
    // my good eye!
    // lets calculate the angle

    lastFix1 = lastPosition1;
    myservo1.write((int) getAngle(1,curX));
    delay(15);
  } else {
    lastPosition1=jiggle(lastPosition1, range1);
      myservo1.write(jiggle(lastPosition1, range1));
      delay(15);
  }
  if (readDistance(2) <= 50 ) {
    lastFix2 = lastPosition2;
    myservo2.write((int) getAngle(2,curX));
    delay(15);
  } else {
    lastPosition2=jiggle(lastPosition2, range2);
    myservo2.write(jiggle(lastPosition2, range2));
    delay(15);
  }
  delay(50);
}

long getX(int lastFix1, int lastFix2) {
/*         
 *  with :d = interOcularDistance
 *       :r = left radius
 *       :R = right radius
 *       
 *       x intercept of the perpendicular chord relative to the line formed by the centers of two circles
 *        
 *  
 *          2     2     2
 *         d   - R   + r
 *   x = -----------------
 *             2d
 *
 */
  long x;
  x = (sq(interOcularDistance) -sq(lastFix2) + sq(lastFix1))/(2*interOcularDistance);
  Serial.print("x: ");
  Serial.println(x);
  return x;
}

long getAngle(int sensor, int curX){
  long angle;
// calculate the angle based on the distance to object of int sensor  
  if (sensor = 1) {
    // get angle for sensor 1 = left
    angle = acos(curX/(lastFix1)); 
  } else {
    angle = acos((interOcularDistance - curX)/lastFix2);
  }
  return angle;
}

int readDistance(int sensor){
  long duration, distance;
  if (sensor == 1){
    digitalWrite(trig1, LOW);
    delayMicroseconds(2);
    digitalWrite(trig1, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig1, LOW);
    duration = pulseIn(echo1, HIGH);
    distance = (duration/2)/29.1;
    Serial.print(distance);
    Serial.print(" cm Right :");
  } else {
    digitalWrite(trig2, LOW);
    delayMicroseconds(2);
    digitalWrite(trig2, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig2, LOW);
    duration = pulseIn(echo2, HIGH);
    distance = (duration/2)/29.1;
    Serial.print(distance);
    Serial.println(" cm Left");
  }
  return distance; 
}
int jiggle(int lastPosition, int range){              // tell servo to go to position in variable 'pos'
    int twitch, result;
    twitch = (int) random(-range,range);
    result = lastPosition + twitch;
    if (lastPosition + twitch >= 180 ) {
      result = lastPosition - twitch;
    } else if (lastPosition + twitch <= 0 ) {
      result = lastPosition - twitch;
    }
    return result;
} // end jiggle

