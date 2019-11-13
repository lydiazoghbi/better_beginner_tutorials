# Minimum version of CMake needed
cmake_minimum_required (VERSION 2.8.3)
# Compile based on cpp11 or later
add_compile_options (-std=c++11)
# Include the project working on, otherwise catkin_package will cause errors
project (mastering_ros)

# If you don't add this then ros/ros.h won't be found
find_package (catkin REQUIRED COMPONENTS roscpp rospy std_msgs actionlib actionlib_msgs message_generation)
find_package(Boost REQUIRED COMPONENTS system)

add_message_files (FILES demo_msg.msg)

add_service_files (FILES demo_srv.srv)

add_action_files (FILES Demo_action.action)

generate_messages (DEPENDENCIES std_msgs actionlib_msgs)

# Add catkin package otherwise the run files won't be found
catkin_package (CATKIN_DEPENDS roscpp rospy std_msgs actionlib actionlib_msgs message_runtime)

# To build publisher and subscriber
include_directories (
	include
	${catkin_INCLUDE_DIRS}
	${Boost_INCLUDE_DIRS}
)

# Create executables of the nodes, needs both inputs
add_executable (demo_service_server src/demo_service_server.cpp)
add_executable (demo_service_client src/demo_service_client.cpp)
add_executable (demo_action_server src/demo_action_server.cpp)
add_executable (demo_action_client src/demo_action_client.cpp)

# Generate message header file before building target, should be the name of the file
# and then the main package name_generate_messages_cpp
add_dependencies (demo_service_client
	mastering_ros_generate_messages_cpp)
add_dependencies (demo_service_server
	mastering_ros_generate_messages_cpp)
add_dependencies (demo_action_server
	mastering_ros_generate_messages_cpp)
add_dependencies (demo_action_client
	mastering_ros_generate_messages_cpp)

# Link executables to appropriate libraries
target_link_libraries (demo_service_client ${catkin_LIBRARIES})
target_link_libraries (demo_service_server ${catkin_LIBRARIES})
target_link_libraries (demo_action_server ${catkin_LIBRARIES})
target_link_libraries (demo_action_client ${catkin_LIBRARIES})
