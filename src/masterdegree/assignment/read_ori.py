#!/usr/bin/env python

import rospy
import math
from std_msgs.msg import Float32MultiArray

def chatter_callback(message):
    w = message.data[0]
    x = message.data[1]
    y = message.data[2]
    z = message.data[3]
    sinr_cosp = +2.0 * (w * x + y * z)
    cosr_cosp = +1.0 - 2.0 * (x * x + y * y)
    roll = math.atan2(sinr_cosp,cosr_cosp)
    sinp = +2.0 * (w * y - z * x)
    pitch = math.asin(sinp)
    siny_cosp = +2.0 * (w * z + x * y)
    cosy_cosp = +1.0 - 2.0 * (y * y + z * z)  
    yaw = math.atan2(siny_cosp, cosy_cosp)
    print 'Roll = %f, pitch = %f, yaw = %f' % (roll,pitch,yaw)
    
def listener():
    rospy.init_node('listener', anonymous=True)

    rospy.Subscriber("sensorimu", Float32MultiArray, chatter_callback)
    rospy.spin()

if __name__ == '__main__':
    listener()