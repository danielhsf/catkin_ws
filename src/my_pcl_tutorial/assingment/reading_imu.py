#!/usr/bin/env python

import rospy
from std_msgs.msg import Float32MultiArray

def chatter_callback(message):
    #get_caller_id(): Get fully resolved name of local node
    #rospy.loginfo(rospy.get_caller_id() + "I heard %f", message.data)
    print(message.data[:])
    
def listener():
    rospy.init_node('listener', anonymous=True)

    rospy.Subscriber("sensorimu", Float32MultiArray, chatter_callback)
    rospy.spin()

if __name__ == '__main__':
    listener()