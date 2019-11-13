#include "ros/ros.h"
#include <iostream>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include "mastering_ros/Demo_actionAction.h"

int main (int argc, char **argv) {
	ros::init(argc, argv, "demo_action_client");
	
	if (argc!=3) {
		ROS_INFO("%d", argc);
		ROS_WARN("Usage: demo_action_client <goal> <time_to_preempt_in_sec>");
		return 1;
	}

	// Create an action client instance
	actionlib::SimpleActionClient<mastering_ros::Demo_actionAction> ac("demo_action", true);
	
	ROS_INFO("Waiting for action server to start");

	// Will wait infinitely for the server
	ac.waitForServer();

	ROS_INFO("Action server started, sending goal");

	// Create an instance of a goal and send the value from the first command line argument
	mastering_ros::Demo_actionGoal goal;
	goal.count = atoi(argv[1]);
	ac.sendGoal(goal);

	// Wait for the results
	bool finished_before_timeout = ac.waitForResult(ros::Duration(atoi(argv[2])));
	
	// If not finished, preempt the action
	ac.cancelGoal();

	if (finished_before_timeout) {
		actionlib::SimpleClientGoalState state = ac.getState();
		ROS_INFO("Action finished: %s", state.toString().c_str());
		ac.cancelGoal();
	}
	else ROS_INFO("Action did not finish before the time out.");

	return 0;
}
