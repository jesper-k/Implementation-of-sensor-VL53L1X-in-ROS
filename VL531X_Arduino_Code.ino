#include <ComponentObject.h>
#include <RangeSensor.h>
#include <SparkFun_VL53L1X.h>
#include <vl53l1x_class.h>
#include <vl53l1_error_codes.h>
#include <Wire.h>
#include <ros.h>
#include <std_msgs/Float32.h>

//Optional interrupt and shutdown pins.
#define SHUTDOWN_PIN 2
#define INTERRUPT_PIN 3

std_msgs::Float32 vl_msg;
ros::Publisher pub_vl("VLdata", &vl_msg);
ros::NodeHandle nh;

SFEVL53L1X distanceSensor;
//Uncomment the following line to use the optional shutdown and interrupt pins.
//SFEVL53L1X distanceSensor(Wire, SHUTDOWN_PIN, INTERRUPT_PIN);

void setup(void)
{
  nh.initNode();
  nh.advertise(pub_vl);
  
  Wire.begin();

  Serial.begin(9600);
  Serial.println(F("VL53L1X Qwiic Test"));

  if (distanceSensor.begin() == 0) //Begin returns 0 on a good init
  {
    Serial.println(F("Sensor online!"));
  }
}

void loop(void)
{
  distanceSensor.startRanging(); //Write configuration bytes to initiate measurement
  int distance = distanceSensor.getDistance(); //Get the result of the measurement from the sensor
  distanceSensor.stopRanging();

  //Serial.print("Distance(mm): ");
  //Serial.print(distance);

  float distanceInches = distance * 0.0393701;
  float distanceFeet = distanceInches / 12.0;

  //Serial.print("\tDistance(ft): ");
  //Serial.print(distanceFeet, 2);

  //Serial.println();

  vl_msg.data = distance;
  pub_vl.publish(&vl_msg);
  nh.spinOnce();
  delay(1000);
 
}
