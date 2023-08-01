from math import sin, cos

import rosbag
from tf.transformations import euler_from_quaternion


def format_lidar_data():
    with open("lidar.info", "w") as lidar_info_file:
        for topic, msg, t in bag.read_messages(topics="/scan"):
            lidar_info_file.write(f"angle_max: {msg.angle_max}\n")
            lidar_info_file.write(f"angle_min: {msg.angle_min}\n")
            lidar_info_file.write(f"angle_increment: {msg.angle_increment}\n")
            break

    with open("lidar_prov.txt", "w") as lidar_file:
        for topic, msg, t in bag.read_messages(topics="/scan"):
            lidar_data = list(msg.ranges)
            lidar_filtered = ["S", f"{len(msg.ranges)}"]
            for l in lidar_data:
                # print(l)
                if str(l) != 'inf':
                    lidar_filtered.append(str(l))
                else:
                    lidar_filtered.append("30.0")
            lidar_text = " ".join(lidar_filtered)
            lidar_file.write(f"{lidar_text}\n")
    
    with open("lidar_prov.txt", "r") as lidar_file:
        lidar_lines = lidar_file.readlines()
        skip = 4  # Whole division was giving 3 while value is 3.9
        with open("lidar.txt", "w") as lidar_filtered_file:
            for idx, line in enumerate(lidar_lines):
                if idx % skip == 0:
                    lidar_filtered_file.write(line)


def format_odometry_data_and_calculate_motor_tick():
    with open("odom.txt", "w") as odom_file:
        with open("motor.txt", "w") as motor_file:
            w = 0.4
            old_pose = None
            for topic, msg, t in bag.read_messages(topics="/atom/odom"):
                o = msg.pose.pose.orientation
                orientation_list = [o.x, o.y, o.z, o.w]
                roll, pitch, yaw = euler_from_quaternion(orientation_list)
                p = msg.pose.pose.position
                x, y = p.x, p.y
                if old_pose:
                    ox, oy, oyaw = old_pose
                    if abs(oyaw - yaw) < 0.001:
                        l = (x - ox) / cos(yaw)
                        r = l
                    else:
                        l = (yaw - oyaw) * ((x - ox) / (sin(yaw) - sin(oyaw)) - w/2)
                        r = l + w*(yaw-oyaw)
                    motor_file.write(f"M {l} {r}\n")
                    odom_file.write(f"F {x+25} {y+10} {yaw}\n")

                old_pose = [x, y, yaw]

    with open("odom.txt", "r") as odom_file:
        return len(odom_file.readlines())


if __name__ == "__main__":
    bag = rosbag.Bag("./2022-10-01-16-05-01.bag")
    
    # Odom formatting
    format_odometry_data_and_calculate_motor_tick()

    # Lidar formatting
    format_lidar_data()

