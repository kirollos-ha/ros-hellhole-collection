#include<iostream>

#include "ros/ros.h"

// per la sincronizzazoine
// questo se si vuole usare ApproximateTimeSyncronizer,
// probabilmente si userà TimeSyncrhonizer, tanto mezzi include sono gli stessi
// 
// TimeSyncronizer richiede che i messaggi abbiano LO STESSO timestamp
// ema ha detto che probabilmente metterà i timestamp uguali prima di pubblicare, ma in caso
#include "message_filters/subscriber.h"
#include "message_filters/synchronizer.h"
#include "message_filters/time_synchronizer.h"
#include "message_filters/sync_policies/approximate_time.h"

#include "driverless_msgs/PoseStamped.h"
#include "driverless_msgs/curvature_throttle.h"
#include "driverless_msgs/can.h"

class Controllo {
public:
    Controllo(ros::NodeHandle nh, std::string sterzo_topic, std::string throttle_topic) {
	steer_pub = nh.advertise<driverless_msgs::can>(sterzo_topic, 100);
	throttle_pub = nh.advertise<driverless_msgs::can>(throttle_topic, 100);
    }

    void callback(const driverless_msgs::PoseStamped::ConstPtr &pose_msg,
                  const driverless_msgs::curvature_throttle::ConstPtr &curve_msg) {
	std::cout<<"non so come gestire un curvature_throttle fantoccio\n";
	std::cout<<"se vedi questo testo sta chiamando la callback\n";
	std::cout<<"per adesso basta questo" << std::endl;
	std::cout<<some_counter++<<std::endl;

	// qui fate qualche calcolo

	steer_pub.publish(build_steer_msg(3.141));
	throttle_pub.publish(build_throttle_msg(2.718));
    }

private:
    // la struttura che dovrebbe avere questa classe è tirata a indovinare,
    // perchè non so com'è che usiamo can a livello ros

    ros::Publisher steer_pub;
    ros::Publisher throttle_pub;
    int some_counter = 0;

    driverless_msgs::can build_steer_msg(double sterzo) {
	driverless_msgs::can msg;
	/* sto settando cose a caso
	 * non so cosa bisogna mettere in questi messaggi
	 * vedere con edo
	 */
	msg.dlc = sterzo;
	return msg;
    }

    driverless_msgs::can build_throttle_msg(double throttle) {
	driverless_msgs::can msg;
	msg.dlc = throttle;
	return msg;
    }
};

int main(int argc, char** argv) {
    // tutte le stringhe con nomi di topic andranno sostituite con nh.getParam("..."); quando questo nodo verrà messo con gli altri

    ros::init(argc, argv, "subscriber");
    ros::NodeHandle nh;

    message_filters::Subscriber<driverless_msgs::PoseStamped> sync_pose_sub(nh, "/tutorial/pose", 1);
    message_filters::Subscriber<driverless_msgs::curvature_throttle> sync_curve_sub(nh, "/tutorial/curvethrottle", 1);

    // per avere messaggi circa simultanei (ApproximateTimeSyncrhonizer) si fa sta monnezza
    typedef message_filters::sync_policies::ApproximateTime
	<driverless_msgs::PoseStamped, driverless_msgs::curvature_throttle>
        pose_curve_sync;

    // il parametro passato al costruttore di pose_curve_sync è la dimensione della coda utilizzata
    message_filters::Synchronizer<pose_curve_sync> approx_synco
	(pose_curve_sync(10), sync_pose_sub, sync_curve_sub);


    Controllo c(nh, "/qualche/topic/sterzo", "/qualche/topic/throttle");
    // registerCallback usa boost::bind nei tutorial, quindi uso boost bind
    // e visto che qui stiamo facendo con un metodo
    // https://stackoverflow.com/questions/2304203/how-to-use-boost-bind-with-a-member-function
    approx_synco.registerCallback(boost::bind(&Controllo::callback, &c, _1, _2));

    ros::spin();

    return 0;
}
