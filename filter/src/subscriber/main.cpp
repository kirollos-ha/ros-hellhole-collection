#include<iostream>

#include "ros/forwards.h"
#include "ros/message_traits.h"
#include "ros/ros.h"

#include "ros/time.h"

#include "ros/header.h"

#include "message_filters/subscriber.h"
#include "message_filters/time_synchronizer.h"

// per la bestemmia approssimata
#include "message_filters/synchronizer.h"
#include "message_filters/sync_policies/approximate_time.h"

#include "driverless_msgs/PoseStamped.h"
#include "driverless_msgs/steering.h"


long ros_time_nanos(ros::Time t) {
    return (long)t.nsec + (1000000000 * (long)t.sec);
}

void normal_pose_call(const driverless_msgs::PoseStamped::ConstPtr &pose_msg) {
    std::cout << "ciao sono lucia e sono una sirena: " << pose_msg->pose.position.x << std::endl;
}

void normal_steer_call(const driverless_msgs::steering::ConstPtr &steer_msg) {
    std::cout << "ma chi è quel mona che batte la porta ed esce urlando: " << steer_msg->position << std::endl;
}

void callabacco(const driverless_msgs::PoseStamped::ConstPtr &pose_msg,
                const driverless_msgs::steering::ConstPtr &steer_msg) {

  std::cout << "yaw angle : " << pose_msg->yaw << '\n';

  std::cout << "posizione: "
	    << "<x :" << pose_msg->pose.position.x << ">, "
            << "<y :" << pose_msg->pose.position.y << ">, "
	    << "<z :" << pose_msg->pose.position.z << ">\n";

  std::cout << "orientazione: "
	    << "<x : " << pose_msg->pose.orientation.x << ">, "
            << "<x : " << pose_msg->pose.orientation.y << ">, "
            << "<x : " << pose_msg->pose.orientation.z << ">, "
            << "<x : " << pose_msg->pose.orientation.w << ">\n";

  std::cout << "steer position: " << steer_msg->position << '\n';
  std::cout << "steer priority: " << (int)steer_msg->priority << '\n';


  long pose_nsec_time = ros_time_nanos(pose_msg->header.stamp);
  long steer_nsec_time = ros_time_nanos(steer_msg->header.stamp);

  std::cout<< "pose time is " << pose_msg->header.stamp
           << "in nanoseconds " << pose_nsec_time << '\n';

  std::cout<< "steer time is " << steer_msg->header.stamp
	   << "in nanoseconds " << steer_nsec_time << '\n';

  std::cout << "nanosecond time delta is "
	    << std::abs(pose_nsec_time - steer_nsec_time) << std::endl;
}

int main(int argc, char** argv) {
    ros::init(argc, argv, "subscriber");
    ros::NodeHandle nh;

    ros::Subscriber normal_pose_sub
        = nh.subscribe("/tutorial/pose", 10, normal_pose_call);
    ros::Subscriber normal_steer_sub
        = nh.subscribe("/tutorial/steer",10, normal_steer_call);

    message_filters::Subscriber<driverless_msgs::PoseStamped>
        sync_pose_sub(nh, "/tutorial/pose", 1);
    message_filters::Subscriber<driverless_msgs::steering>
        sync_steer_sub(nh, "/tutorial/steer", 1);

    // timesynchronizer accetta solo messaggi con timestamp uguali

    // message_filters::TimeSynchronizer<driverless_msgs::PoseStamped,
    //                                   driverless_msgs::steering>
    //     exact_synco(sync_pose_sub, sync_steer_sub, 10);
    // exact_synco.registerCallback(boost::bind(&callabacco, _1, _2));
    // // perchè voglia un boost::bind chiedilo a the fucking manual

    // per avere messaggi circa uguali si può fare questa bestemmia approssiamta

    typedef message_filters::sync_policies::ApproximateTime
	<driverless_msgs::PoseStamped, driverless_msgs::steering>
        pose_steer_sync;

    // il parametro passato a pose_steer_sync è la dimensione della coda utilizzata
    message_filters::Synchronizer<pose_steer_sync> approx_synco
	(pose_steer_sync(10), sync_pose_sub, sync_steer_sub);

    approx_synco.registerCallback(boost::bind(&callabacco, _1, _2));
    

    ros::spin();

    return 0;
}
	

				     
