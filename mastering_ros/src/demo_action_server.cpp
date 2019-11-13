#include "ros/ros.h"
#include "std_msgs/Int32.h"
#include <actionlib/server/simple_action_server.h>
#include "mastering_ros/Demo_actionAction.h"
#include <iostream>
#include <sstream>

// Class containing the action server definition
class Demo_actionAction {
	protected:
		ros::NodeHandle nh_;

		// Create an action server instance
		actionlib::SimpleActionServer<mastering_ros::Demo_actionAction> as;
		// Create feedback instance for sending feedback during the operation
		mastering_ros::Demo_actionFeedback feedback;
		// Create result instance for sendint the final result
		mastering_ros::Demo_actionResult result;

		std::string action_name;
		int goal;
		int progress;

	public:
		
		// Action constructor and action server
		Demo_actionAction (std::string name):
			as(nh_, name, boost::bind(&Demo_actionAction::executeCB, this, _1), false), action_name(name) {
		
			// Register a callback when the action is preempted
			as.registerPreemptCallback(boost::bind(&Demo_actionAction::preemptCB, this));
			as.start();
		}

	~Demo_actionAction (void) {
	}

	// Action callback executed when there is a preempt request
	void preemptCB () {
		ROS_WARN("%s got preempted!", action_name.c_str());
		result.final_count = progress;
		as.setPreempted(result, "I got preempted");
	}

	// Callback definition, executed when the action server receives a goal value
	void executeCB (const mastering_ros::Demo_actionGoalConstPtr &goal) {
		if (!as.isActive() || as.isPreemptRequested()) return;
		ros::Rate rate(5);
		ROS_INFO("%s Shutting down", action_name.c_str());

		// Loop which will keep executing until goal value is reached
		for (progress = 0; progress < goal ->count; progress++) {
			if(!ros::ok()) {
				result.final_count = progress;
				as.setAborted(result, "I failed!");
				ROS_INFO("%s Shutting down", action_name.c_str());
				break;
			}
		if (!as.isActive() || as.isPreemptRequested()) {
			return;
		}

		// If current value reaches goal value, final result is published
		if (goal->count == progress) {
			ROS_INFO("%s Succeeded at getting to goal %d", action_name.c_str(), goal->count);
			result.final_count = progress;
			as.publishFeedback(feedback);
		} else {
			ROS_INFO("Setting to goal %d / %d", feedback.current_number, goal->count);
			feedback.current_number = progress;
			as.publishFeedback(feedback);
		}
		
		rate.sleep();
		}
	}
};

int main (int argc, char** argv) {
	ros::init(argc, argv, "demo_action");
	ROS_INFO("Starting Demo Action Server");

	// Create an instance of Demo_actionAction which will start the action server
	Demo_actionAction demo_action_obj(ros::this_node::getName());
	ros::spin();
	return 0;
}
