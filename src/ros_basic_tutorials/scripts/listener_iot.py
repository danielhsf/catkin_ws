#!/usr/bin/env python

import rospy
from ros_essentials_cpp.msg import IoTSensor

def callback(data):
    rospy.loginfo("I heard (%d, %s,%.2f, %.2f)",data.id,data.name, data.temperature,data.humidity)

def listener():
    rospy.init_node('iot_sensor_subscriber_node',anonymous=True)
    rospy.Subscriber('iot_sensor_topic',IoTSensor,callback)
    rospy.spin()

if __name__ == '__main__':
    listener()