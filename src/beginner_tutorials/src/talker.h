#ifndef TALKER
#define TALKER
#include "ros/ros.h"
#include "std_msgs/Float64.h"
#include "gazebo_msgs/LinkStates.h"
#include "std_srvs/Empty.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <string>
#include <stdio.h>      /* printf */
#include <math.h>       /* ceil */

//right arm
ros::Publisher body_to_right_shoulder;
ros::Publisher right_shoulder_to_right_up_arm;
ros::Publisher right_up_arm_to_right_down_arm;
//left arm
ros::Publisher body_to_left_shoulder;
ros::Publisher left_shoulder_to_left_up_arm;
ros::Publisher left_up_arm_to_left_down_arm;
//right leg
ros::Publisher body_to_right_leg_joint;
ros::Publisher right_leg_joint_to_right_up_leg;
ros::Publisher right_up_leg_to_right_down_leg;
ros::Publisher right_down_leg_to_right_foot;
//left leg
ros::Publisher body_to_left_leg_joint;
ros::Publisher left_leg_joint_to_left_up_leg;
ros::Publisher left_up_leg_to_left_down_leg;
ros::Publisher left_down_leg_to_left_foot;

std::vector<std::vector<std::vector<float> > > population;

int EXPTIME=40;
int TARGET_EXP_TIME=500;
int ONE_CHUNK_SIZE=17;
int BONUS_FOR_DISTANCE=3;
int ORGIN_POPULATION_SIZE=500;
int DECREASE_RATE_POP=1;

int POPULATION_SIZE=ORGIN_POPULATION_SIZE;
int count=0;
int pop_index=0;
int generation_index=0;
float mutation_rate=0.8f;


void mutation();
void saveFile(int generation);
void readFile();
void crossOver();
void selection();
void fitCal();
void resetExp();

#endif