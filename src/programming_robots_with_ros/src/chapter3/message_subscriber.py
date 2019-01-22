#!/usr/bin/env python

import rospy
from programming_robots_with_ros.msg import complex

def callback(msg):
    rospy.loginfo("Real e Imaginario: (%.2f ,%.2f)", 
        msg.real,msg.imaginary)

rospy.init_node('message_subscriber')

sub = rospy.Subscriber('complex',complex,callback)

rospy.spin()
