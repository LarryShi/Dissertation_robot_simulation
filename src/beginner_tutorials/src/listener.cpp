#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Float64.h"
#include <sensor_msgs/JointState.h>
#include "gazebo_msgs/LinkStates.h"

/**
 * This tutorial demonstrates simple receipt of messages over the ROS system.
 */
int count=0;

void linkstatesCallback(const gazebo_msgs::LinkStates::ConstPtr& msg){
  ROS_INFO("Heard Name: %d[%s]",count, msg->name[1].c_str());
  ROS_INFO("Heard Name: %d[%s]",count, msg->name[2].c_str());
  ROS_INFO("Heard Name: %d[%s]",count, msg->name[3].c_str());
  ROS_INFO("Heard Name: %d[%s]",count, msg->name[4].c_str());
  ROS_INFO("Heard Name: %d[%s]",count, msg->name[5].c_str());
  ROS_INFO("Heard Name: %d[%s]",count, msg->name[6].c_str());
  ROS_INFO("Heard Name: %d[%s]",count, msg->name[7].c_str());
  ROS_INFO("Heard Name: %d[%s]",count, msg->name[8].c_str());
  ROS_INFO("Heard Name: %d[%s]",count, msg->name[9].c_str());
  ROS_INFO("Heard Name: %d[%s]",count, msg->name[10].c_str());
  ROS_INFO("Heard Name: %d[%s]",count, msg->name[11].c_str());
  ROS_INFO("Heard Name: %d[%s]",count, msg->name[12].c_str());
  ROS_INFO("Heard Name: %d[%s]",count, msg->name[13].c_str());
  ROS_INFO("Heard Name: %d[%s]",count, msg->name[14].c_str());
  ROS_INFO("Heard X: [%f]", msg->pose[1].position.x);
  ROS_INFO("Heard Y: [%f]", msg->pose[1].position.y);
  ROS_INFO("Heard Z: [%f]", msg->pose[1].position.z);
  ROS_INFO("====================================================/n");
}

void chatterCallback(const sensor_msgs::JointState::ConstPtr& msg)
{
  ROS_INFO("1. I heard: [%s]", msg->name[0].c_str());
  ROS_INFO("2. I heard: [%s]", msg->name[1].c_str());
}

void chatterCallback2(const std_msgs::Float64::ConstPtr& msg)
{
  ROS_INFO("I heard: [%f]", msg->data);
}

int main(int argc, char **argv)
{
  /**
   * The ros::init() function needs to see argc and argv so that it can perform
   * any ROS arguments and name remapping that were provided at the command line.
   * For programmatic remappings you can use a different version of init() which takes
   * remappings directly, but for most command-line programs, passing argc and argv is
   * the easiest way to do it.  The third argument to init() is the name of the node.
   *
   * You must call one of the versions of ros::init() before using any other
   * part of the ROS system.
   */
  ros::init(argc, argv, "listener");

  /**
   * NodeHandle is the main access point to communications with the ROS system.
   * The first NodeHandle constructed will fully initialize this node, and the last
   * NodeHandle destructed will close down the node.
   */
  ros::NodeHandle n;

  /**
   * The subscribe() call is how you tell ROS that you want to receive messages
   * on a given topic.  This invokes a call to the ROS
   * master node, which keeps a registry of who is publishing and who
   * is subscribing.  Messages are passed to a callback function, here
   * called chatterCallback.  subscribe() returns a Subscriber object that you
   * must hold on to until you want to unsubscribe.  When all copies of the Subscriber
   * object go out of scope, this callback will automatically be unsubscribed from
   * this topic.
   *
   * The second parameter to the subscribe() function is the size of the message
   * queue.  If messages are arriving faster than they are being processed, this
   * is the number of messages that will be buffered up before beginning to throw
   * away the oldest ones.
   */
  ROS_INFO("Info Recored Start____________________");
  //ros::Subscriber sub = n.subscribe("my_robot_head_controller/state", 1, chatterCallback);

  ros::Subscriber sub = n.subscribe<gazebo_msgs::LinkStates>("gazebo/link_states", 1, linkstatesCallback); //ROS gazebo publishes all joint states, you can use these.

  /**
   * ros::spin() will enter a loop, pumping callbacks.  With this version, all
   * callbacks will be called from within this thread (the main one).  ros::spin()
   * will exit when Ctrl-C is pressed, or the node is shutdown by the master.
   */
  ros::spin();
  count++;
  ROS_INFO("Info Recored Finished___________________");
  return 0;
}
