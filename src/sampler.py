#!/usr/bin/env python

#This sample code provides an easy to user interface to trigger sample taking

import rospy
from std_msgs.msg import Int8
from std_msgs.msg import Byte
flag = 0

def callback(button):
    global flag
    if button.data == 1: 
        flag = 450
        rospy.loginfo(button.data)

def sample_driver():
    pub = rospy.Publisher('pump_ctl', Int8, queue_size=1)
    rospy.init_node('sample_driver', anonymous=True)
    rospy.Subscriber("samples", Byte, callback)
    rate = rospy.Rate(10) # 10hz
    controller = 0
    
    global flag
    while not rospy.is_shutdown():
        if flag > -6: flag = flag -1
        if flag >= 0: 
            print flag
            controller = 9
            pub.publish(controller)
        if flag >= 350: 
            controller = -9
            pub.publish(controller)
        if (flag <= -1) and (flag >= -5): #Ensure stop message is received 
            pub.publish(0)
        #rospy.loginfo(controller)
        rate.sleep()

if __name__ == '__main__':
    try:
        sample_driver()
    except rospy.ROSInterruptException:
        pass
