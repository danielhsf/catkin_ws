#!/usr/bin/env python

import rospy
from programming_robots_with_ros.msg import complex
from random import random

rospy.init_node('message_publisher')

pub = rospy.Publisher('complex',complex,queue_size=10)

rate = rospy.Rate(2)

while not rospy.is_shutdown():
    msg = complex()
    msg.real = random()
    msg.imaginary = random()
    pub.publish(msg)
    rate.sleep()