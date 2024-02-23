#include<iostream>
#include<cstdlib> // per rand()
#include<vector> // per rand()

#include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/Quaternion.h"
#include "driverless_msgs/PoseStamped.h"

int main(int argc, char** argv) {
    ros::init(argc, argv, "publisher");
    ros::NodeHandle nh;

    ros::Publisher strings = nh.advertise<std_msgs::String>("/tutorial/stringhe", 100);
    ros::Publisher poses = nh.advertise<driverless_msgs::PoseStamped>("/tutorial/pose", 100);

    ros::Rate hertz_sleeper(10);

    // testo a cazzo da mandare
    std::vector<std::string> errori = { "track lost",
					"camera fucked",
					"controls fucked",
					"god is dead",
					"god remains dead",
					"kernel panic",
					"kernel manic episode",
					"kernel acute psychosis",
					"kernel proletariat revolution",
					"erectile disfunction" };

    float f = 0;

    while(ros::ok()) {
	int ind = rand()%(errori.size());
        std_msgs::String str_messaggio;
        str_messaggio.data = errori[ind];
        ROS_INFO("mando stringa %s", str_messaggio.data.c_str());
        strings.publish(str_messaggio);

        driverless_msgs::PoseStamped pose_messaggio;
        pose_messaggio.yaw = f + 0.1;
	pose_messaggio.pose.position.x = f + 0.2;
	pose_messaggio.pose.position.y = f + 0.3;
	pose_messaggio.pose.position.z = f + 0.4;
	pose_messaggio.pose.orientation.x = f + 0.5;
	pose_messaggio.pose.orientation.y = f + 0.6;
	pose_messaggio.pose.orientation.z = f + 0.7;
	pose_messaggio.pose.orientation.w = f + 0.8;

	ROS_INFO("mando posa con %f", f);
	poses.publish(pose_messaggio);

        ros::spinOnce();
	hertz_sleeper.sleep();
    }

    return 0;
}
	

				     
