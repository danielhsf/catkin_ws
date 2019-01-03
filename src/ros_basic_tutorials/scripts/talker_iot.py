#!/usr/bin/env python
import rospy
from ros_essentials_cpp.msg import IoTSensor
import random

def talker():
    pub = rospy.Publisher('iot_sensor_topic', IoTSensor, queue_size=10)
    rospy.init_node('iot_sensor_topic', anonymous=True)
    rate = rospy.Rate(1)
    iot_sensor = IoTSensor()
    i = 0
    while not rospy.is_shutdown():
        iot_sensor.id = 1
        iot_sensor.name = "iot_parking_01"
        iot_sensor.temperature = 24.33 + random.random()*2
        iot_sensor.humidity = 33.41 + random.random()*2
        rospy.loginfo("I Publish")
        rospy.loginfo(iot_sensor)
        pub.publish(iot_sensor)
        rate.sleep()

if __name__ == '__main__':
    try:
        talker()
    except rospy.ROSInterruptException:
        pass