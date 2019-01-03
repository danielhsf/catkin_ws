#!/usr/bin/env python
import rospy
from geometry_msgs.msg import Twist
from turtlesim.msg import Pose

def move():
    pub = rospy.Publisher('/turtle1/cmd_vel', Twist, queue_size=10)
    rospy.init_node('robot_topic_move', anonymous=True)
    rate = rospy.Rate(1)
    twist = Twist()
    while not rospy.is_shutdown():
        twist.linear.x = 0.5
        rospy.loginfo(twist)
        pub.publish(twist)
        rate.sleep()
        twist.linear.x = -0.4
        rospy.loginfo(twist)
        pub.publish(twist)
        rate.sleep()

def callback(data):
    rospy.loginfo(data)

def pose():
    rospy.init_node('robot_topic_pose',anonymous=True)
    rospy.Subscriber('/turtle1/pose',Pose,callback)
    rospy.spin()

if __name__ == '__main__':
    try:
        pose()
    except rospy.ROSInterruptException:
        pass