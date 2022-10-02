import rospy
from geometry_msgs.msg import Twist


def talker():
    pub = rospy.Publisher('/robot/cmd_vel', Twist, queue_size=10)
    rospy.init_node('talker', anonymous=True)
    rate = rospy.Rate(10) # 10hz
    while not rospy.is_shutdown():
        text = Twist()
        text.linear.x = 3.0
        text.linear.y = 0.0
        text.linear.z = 0.0
        text.angular.x = 1.0
        text.angular.y = 1.0
        text.angular.z = 1.0
        rospy.loginfo(text)
        pub.publish(text)
        rate.sleep()


if __name__ == '__main__':
    try:
        talker()
    except rospy.ROSInterruptException:
        pass