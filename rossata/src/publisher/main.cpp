#include<iostream>
#include<vector>
#include<thread>
#include<chrono>

#include "ros/ros.h"
#include "ros/time.h"
#include "geometry_msgs/Quaternion.h"
#include "driverless_msgs/PoseStamped.h"
//#include "driverless_msgs/steering.h"
#include "driverless_msgs/curvature_throttle.h"

int main(int argc, char** argv) {
    ros::init(argc, argv, "publisher");
    ros::NodeHandle nh;

    ros::Publisher curve_pub = nh.advertise<driverless_msgs::curvature_throttle>("/tutorial/curvethrottle", 100);
    ros::Publisher pose_pub = nh.advertise<driverless_msgs::PoseStamped>("/tutorial/pose", 100);

    ros::Rate hertz_sleeper(10);

    float f = 0;

    while(ros::ok()) {
	driverless_msgs::curvature_throttle curve_messaggio;

        driverless_msgs::PoseStamped pose_messaggio;
        pose_messaggio.yaw = f + 0.1;
	pose_messaggio.pose.position.x = f + 0.2;
	pose_messaggio.pose.position.y = f + 0.3;
	pose_messaggio.pose.position.z = f + 0.4;
	pose_messaggio.pose.orientation.x = f + 0.5;
	pose_messaggio.pose.orientation.y = f + 0.6;
	pose_messaggio.pose.orientation.z = f + 0.7;
	pose_messaggio.pose.orientation.w = f + 0.8;

	ROS_INFO("fatto posa con %f", f);

	f = f+1;

	ROS_INFO("mo si manda la robetta con %f", f);

	pose_messaggio.header.stamp = ros::Time::now();
	pose_pub.publish(pose_messaggio);

	curve_messaggio.header.stamp = pose_messaggio.header.stamp;
	int offset = std::rand()%10;
	std::cout<<"with offset " << offset << '\n';
	curve_messaggio.header.stamp.nsec += offset;
	curve_pub.publish(curve_messaggio);

        ros::spinOnce();
	hertz_sleeper.sleep();
    }

    return 0;
}
