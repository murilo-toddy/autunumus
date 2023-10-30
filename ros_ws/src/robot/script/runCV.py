#!/usr/bin/env python3

from __future__ import print_function

import roslib
roslib.load_manifest('robot')
import sys
import rospy
import cv2
from std_msgs.msg import String
from sensor_msgs.msg import Image
from cv_bridge import CvBridge, CvBridgeError

class showImage:
	def __init__(self):
		self.bridge = CvBridge()
		self.image_sub = rospy.Subscriber("/camera/rgb/image_raw",Image,self.callback)

	def callback(self,data):
		try:
			cv_image = self.bridge.imgmsg_to_cv2(data, "bgr8")
		except CvBridgeError as e:
			print(e)

		(rows,cols,channels) = cv_image.shape
		cv2.circle(cv_image, (int(rows/2),int(cols/2)), 10, 255)
		cv2.imshow("Image window", cv_image)
		cv2.waitKey(3)

def main(args):
	ic = showImage()
	rospy.init_node('image_converter', anonymous=True)
	try:
		rospy.spin()
	except KeyboardInterrupt:
		print("Shutting down")
	cv2.destroyAllWindows()

if __name__ == '__main__':
	main(sys.argv)