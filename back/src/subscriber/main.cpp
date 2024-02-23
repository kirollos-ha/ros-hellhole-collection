#include<iostream>

#include "ros/ros.h"
#include "std_msgs/String.h"
#include "driverless_msgs/PoseStamped.h"

void string_callback(const std_msgs::String::ConstPtr& msg) {
    std::string str = msg->data;
    ROS_INFO("ricevuto un [%s]", msg->data.c_str());
}

void pose_callback(const driverless_msgs::PoseStamped::ConstPtr& msg) {
    std::cout<< "yaw angle : " << msg->yaw << '\n';

    std::cout<< "posizione : "
	     << msg->pose.position.x << ", "
	     << msg->pose.position.y << ", "
	     << msg->pose.position.z << '\n';

    std::cout<< "orientazione : "
	     << msg->pose.orientation.x << ", "
	     << msg->pose.orientation.y << ", "
	     << msg->pose.orientation.z << ", "
	     << msg->pose.orientation.w << std::endl;
}

int main(int argc, char** argv) {
    ros::init(argc, argv, "subscriber");
    ros::NodeHandle nh;

    ros::Subscriber sub_string = nh.subscribe("/tutorial/stringhe", 100, string_callback);
    ros::Subscriber sub_pose = nh.subscribe("/tutorial/pose", 100, pose_callback);

    ros::spin();
    return 0;
}
	

				     
