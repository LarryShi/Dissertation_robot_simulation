#include "talker.h"


void saveFile(int generation){
  std::ofstream out;
  std::stringstream ss;

  ss<<generation;
  ss<<".txt";
  std::string fileName = "/home/zqshi/ExperimentData/Generation";
  fileName+=ss.str();
  out.open (fileName.c_str());

  if (out.is_open()) {
      for(int n=0;n<POPULATION_SIZE;n++){
        for(int m=0;m<EXPTIME;m++){
          for(int i=0;i<ONE_CHUNK_SIZE;i++){
              out << population[n][m][i];
              out <<" ";
          }
          out<<"\n";
         }
       }
      out<<"-999";
      out.close();  
     }  

  out.close();
}

void readFile(){
  ROS_INFO("Read File Start");
  std::stringstream ss;
  std::ifstream fin2("/home/zqshi/ExperimentData/config.txt");
  int index=0;
  int START_GENERATION=0;
  while (!fin2.eof()){
    int idata;
    fin2 >> idata;
    switch(index){
      case 0:EXPTIME=idata;break;
      case 1:TARGET_EXP_TIME=idata;break;
      case 2:ONE_CHUNK_SIZE=idata;break;
      case 3:BONUS_FOR_DISTANCE=idata;break;
      case 4:ORGIN_POPULATION_SIZE=idata;POPULATION_SIZE=ORGIN_POPULATION_SIZE;break;
      case 5:DECREASE_RATE_POP=idata;break;
      case 6:START_GENERATION=idata;break;
    }
    index++;
  }
  ss<<"/home/zqshi/ExperimentData/Generation";
  ss<<START_GENERATION;
  ss<<".txt";
  generation_index=START_GENERATION+1;
  POPULATION_SIZE=ORGIN_POPULATION_SIZE*ORGIN_POPULATION_SIZE/(ORGIN_POPULATION_SIZE+DECREASE_RATE_POP*generation_index);
  ROS_INFO("Config File Done");
  std::ifstream fin(ss.str().c_str());
  population.resize(POPULATION_SIZE);
  for (int i = 0; i < POPULATION_SIZE; ++i){
    population[i].resize(EXPTIME);
    for(int n=0;n<EXPTIME;n++)
      population[i][n].resize(ONE_CHUNK_SIZE);
  }

  int i=0,n=0,m=0;
  while (!fin.eof()){
    float idata;
    fin >> idata;
    population[i][m][n]=idata;
    //std::cout<<idata<<" ";
    n++;
    if(n==ONE_CHUNK_SIZE){
      m++;
      n=0;
    }
    if(m==EXPTIME){
      i++;
      m=0;
    }
    if(i==POPULATION_SIZE){
      break;
    }
  }
  ROS_INFO("Generation File Done");
  pop_index=0;
}


void mutation(){
  //When start Mutation, it is down with the crossover, still old generation number
  ROS_INFO("Start Mutation For Generation %d", generation_index);
  for(int i=0;i<POPULATION_SIZE;i++){
    for(int n=0;n<EXPTIME;n++){
      for(int m=0;m<14;m++){
          if(rand()%1024/1024<mutation_rate){
            population[i][n][m]+=(rand()%80-40)/10*mutation_rate;
          }
      }
    }
  }
  float temp = ((TARGET_EXP_TIME/10)*generation_index+2*TARGET_EXP_TIME);
  mutation_rate=TARGET_EXP_TIME/temp;
  ROS_INFO("END Mutation For Generation %d, next time Mutation Rate is %f", generation_index, mutation_rate);
}

void crossOver(){
  //When start Cross Over, it is according to new generation popluation, not older one
  ROS_INFO("Start Crossover For Generation %d", generation_index);
  for(int i=POPULATION_SIZE/2;i<POPULATION_SIZE;i++){
    int parent_index1=rand() % (POPULATION_SIZE/32+1);
    int parent_index2=rand() % (POPULATION_SIZE/8+1);
    while(parent_index1==parent_index2){
      parent_index1 = rand() % (POPULATION_SIZE/4+1);
      parent_index2 = rand() % (POPULATION_SIZE/2+1);
    }
    for(int n=0;n<EXPTIME;n++){
      for(int m=0;m<14;m++){
        if(population[parent_index1][0][16]>population[parent_index2][0][16])
          population[i][n][m]=0.75*population[parent_index1][n][m]+0.25*population[parent_index2][n][m];
        else
          population[i][n][m]=0.25*population[parent_index1][n][m]+0.75*population[parent_index2][n][m];
      }
    }
  }

  ROS_INFO("End Crossover For Generation %d", generation_index);
}

void selection(){
  ROS_INFO("Start Selection For Generation %d", generation_index);
  for (int i = 0; i < POPULATION_SIZE; ++i){
    for (int j = 0; j < POPULATION_SIZE-i-1; ++j){
      // Comparing consecutive data and switching values if value at j > j+1.
      if (population[j][0][16] < population[j+1][0][16] ){
        std::vector<std::vector<float> > temp = population[j];
        population[j] = population[j+1];
        population[j+1] = temp;
      }
    }
    // Value at n-i-1 will be maximum of all the values below this index.
  } 

  for (int i = 0; i < POPULATION_SIZE; ++i){

      for(int j=population[i][0][14];j<population[i][0][14]*2;j++){
        //Exchange left arm and right arm info
        if(j>=EXPTIME)
          break;
        population[i][j][0]=population[i][j-population[i][0][14]][0];
        population[i][j][1]=population[i][j-population[i][0][14]][1];
        population[i][j][2]=population[i][j-population[i][0][14]][2];
        population[i][j][3]=population[i][j-population[i][0][14]][3];
        population[i][j][4]=population[i][j-population[i][0][14]][4];
        population[i][j][5]=population[i][j-population[i][0][14]][5];

        //Exchange left leg and right leg info
        population[i][j][6]=population[i][j-population[i][0][14]][6];
        population[i][j][7]=population[i][j-population[i][0][14]][7];
        population[i][j][8]=population[i][j-population[i][0][14]][8];
        population[i][j][9]=population[i][j-population[i][0][14]][9];
        population[i][j][10]=population[i][j-population[i][0][14]][10];
        population[i][j][11]=population[i][j-population[i][0][14]][11];
        population[i][j][12]=population[i][j-population[i][0][14]][12];
        population[i][j][13]=population[i][j-population[i][0][14]][13];

      }
    // Value at n-i-1 will be maximum of all the values below this index.
  } 
  // for (int i = 0; i < POPULATION_SIZE; ++i){

  //     for(int j=population[i][0][14];j<population[i][0][14]*2;j++){
  //       //Exchange left arm and right arm info
  //       if(j>=EXPTIME)
  //         break;
  //       population[i][j][3]=-population[i][j-population[i][0][14]][0];
  //       population[i][j][4]=population[i][j-population[i][0][14]][1];
  //       population[i][j][5]=population[i][j-population[i][0][14]][2];
  //       population[i][j][0]=-population[i][j-population[i][0][14]][3];
  //       population[i][j][1]=population[i][j-population[i][0][14]][4];
  //       population[i][j][2]=population[i][j-population[i][0][14]][5];

  //       //Exchange left leg and right leg info
  //       population[i][j][6]=-population[i][j-population[i][0][14]][10];
  //       population[i][j][7]=-population[i][j-population[i][0][14]][11];
  //       population[i][j][8]=population[i][j-population[i][0][14]][12];
  //       population[i][j][9]=population[i][j-population[i][0][14]][13];
  //       population[i][j][10]=-population[i][j-population[i][0][14]][6];
  //       population[i][j][11]=-population[i][j-population[i][0][14]][7];
  //       population[i][j][12]=population[i][j-population[i][0][14]][8];
  //       population[i][j][13]=population[i][j-population[i][0][14]][9];

  //     }
  //   // Value at n-i-1 will be maximum of all the values below this index.
  // } 
  ROS_INFO("END Selection For Generation %d", generation_index);
}

void fitCal(){
    for(int n=0;n<POPULATION_SIZE;n++){
      population[n][0][16]=population[n][0][14]+BONUS_FOR_DISTANCE*population[n][0][15];
      //Time passed before falling down + distance grade
    }
}

void resetExp(){
    std_srvs::Empty resetWorldSrv;
   
    if(ros::service::call("/gazebo/reset_simulation", resetWorldSrv)){
      ROS_INFO("Individual:%d finished,Reset World",pop_index); 
    }
    else { 
      ROS_INFO("Failed topic reset the world"); 
    }
    population[pop_index][0][14]=count/8;

    count=0;
    pop_index++;

}



void linkstatesCallback(const gazebo_msgs::LinkStates::ConstPtr& msg){

  //ROS_INFO("%f",msg->pose[2].position.z);
  
  //population[pop_index][0][15]=msg->pose[2].position.x;
  population[pop_index][0][15]=msg->pose[13].position.x;
  if(msg->pose[1].position.z<0.7){
    resetExp();
  }
}

/**
 * This tutorial demonstrates simple sending of messages over the ROS system.
 */
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
  ros::init(argc, argv, "talker");
  ROS_INFO("Start Simulation, God Bless Me!");
  /**
   * NodeHandle is the main access point to communications with the ROS system.
   * The first NodeHandle constructed will fully initialize this node, and the last
   * NodeHandle destructed will close down the node.
   */
  ros::NodeHandle n;

  /**
   * The advertise() function is how you tell ROS that you want to
   * publish on a given topic name. This invokes a call to the ROS
   * master node, which keeps a registry of who is publishing and who
   * is subscribing. After this advertise() call is made, the master
   * node will notify anyone who is trying to subscribe to this topic name,
   * and they will in turn negotiate a peer-to-peer connection with this
   * node.  advertise() returns a Publisher object which allows you to
   * publish messages on that topic through a call to publish().  Once
   * all copies of the returned Publisher object are destroyed, the topic
   * will be automatically unadvertised.
   *
   * The second parameter to advertise() is the size of the message queue
   * used for publishing messages.  If messages are published more quickly
   * than we can send them, the number here specifies how many messages to
   * buffer up before throwing some away.
   */

  //right arm
  body_to_right_shoulder = n.advertise<std_msgs::Float64>("my_robot_body_to_right_shoulder_controller/command", 1);
  right_shoulder_to_right_up_arm = n.advertise<std_msgs::Float64>("my_robot_right_shoulder_to_right_up_arm_controller/command", 1);
  right_up_arm_to_right_down_arm = n.advertise<std_msgs::Float64>("my_robot_right_up_arm_to_right_down_arm_controller/command", 1);
  //left arm
  body_to_left_shoulder = n.advertise<std_msgs::Float64>("my_robot_body_to_left_shoulder_controller/command", 1);
  left_shoulder_to_left_up_arm = n.advertise<std_msgs::Float64>("my_robot_left_shoulder_to_left_up_arm_controller/command", 1);
  left_up_arm_to_left_down_arm = n.advertise<std_msgs::Float64>("my_robot_left_up_arm_to_left_down_arm_controller/command", 1);
  //right leg
  body_to_right_leg_joint = n.advertise<std_msgs::Float64>("my_robot_body_to_right_leg_joint_controller/command", 1);
  right_leg_joint_to_right_up_leg = n.advertise<std_msgs::Float64>("my_robot_right_leg_joint_to_right_up_leg_controller/command", 1);
  right_up_leg_to_right_down_leg = n.advertise<std_msgs::Float64>("my_robot_right_up_leg_to_right_down_leg_controller/command", 1);
  right_down_leg_to_right_foot = n.advertise<std_msgs::Float64>("my_robot_right_down_leg_to_right_foot_controller/command", 1);
  //left leg
  body_to_left_leg_joint = n.advertise<std_msgs::Float64>("my_robot_body_to_left_leg_joint_controller/command", 1);
  left_leg_joint_to_left_up_leg = n.advertise<std_msgs::Float64>("my_robot_left_leg_joint_to_left_up_leg_controller/command", 1);
  left_up_leg_to_left_down_leg = n.advertise<std_msgs::Float64>("my_robot_left_up_leg_to_left_down_leg_controller/command", 1);
  left_down_leg_to_left_foot = n.advertise<std_msgs::Float64>("my_robot_left_down_leg_to_left_foot_controller/command", 1);

  //get the status
  ros::Subscriber sub = n.subscribe<gazebo_msgs::LinkStates>("gazebo/link_states", 1, linkstatesCallback); //ROS gazebo publishes all joint states, you can use these.

  ros::Rate loop_rate(32);

  readFile();

  /**
   * A count of how many messages we have sen This is used to create
   * a unique string for each message.
   */
  int expTime=count/8;
  for(;generation_index<TARGET_EXP_TIME;generation_index++){
    ROS_INFO("New Generation %d Started!\n POPULATION SIZE:%d",generation_index,POPULATION_SIZE);
    while (ros::ok()&&pop_index<POPULATION_SIZE){

      /**
       * This is a message object. You stuff it with data, and then publish it.
       */
      std_msgs::Float64 msg;

      /**
       * The publish() function is how you send messages. The parameter
       * is the message object. The type of this object must agree with the type
       * given as a template parameter to the advertise<>() call, as was done
       * in the constructor above.
       */
      expTime=count/8;
      if(count==0){
        ROS_INFO("Individual %d Started!",pop_index );  
        msg.data=0.0f;
        
        body_to_right_shoulder.publish(msg);
        body_to_right_leg_joint.publish(msg);
        //right arm
        body_to_right_shoulder.publish(msg);
        right_shoulder_to_right_up_arm.publish(msg);
        right_up_arm_to_right_down_arm.publish(msg);
        //left arm
        body_to_left_shoulder.publish(msg);
        left_shoulder_to_left_up_arm.publish(msg);
        left_up_arm_to_left_down_arm.publish(msg);
        //right leg
        body_to_right_leg_joint.publish(msg);
        right_leg_joint_to_right_up_leg.publish(msg);
        right_up_leg_to_right_down_leg.publish(msg);
        right_down_leg_to_right_foot.publish(msg);
        //left leg
        body_to_left_leg_joint.publish(msg);
        left_leg_joint_to_left_up_leg.publish(msg);
        left_up_leg_to_left_down_leg.publish(msg);
        left_down_leg_to_left_foot.publish(msg);
        count++;
      }else{  
        //ROS_INFO("Individual %d Testing, with %d",pop_index, expTime);  
        //right arm
        msg.data= population[pop_index][expTime][0];
        body_to_right_shoulder.publish(msg);
        msg.data= population[pop_index][expTime][1];
        right_shoulder_to_right_up_arm.publish(msg);
        msg.data= population[pop_index][expTime][2];
        right_up_arm_to_right_down_arm.publish(msg);

        //left arm
        msg.data= population[pop_index][expTime][3];
        body_to_left_shoulder.publish(msg);
        msg.data= population[pop_index][expTime][4];
        left_shoulder_to_left_up_arm.publish(msg);
        msg.data= population[pop_index][expTime][5];
        left_up_arm_to_left_down_arm.publish(msg);

        //right leg
        msg.data= population[pop_index][expTime][6];
        body_to_right_leg_joint.publish(msg);
        msg.data= population[pop_index][expTime][7];
        right_leg_joint_to_right_up_leg.publish(msg);
        msg.data= population[pop_index][expTime][8];
        right_up_leg_to_right_down_leg.publish(msg);
        msg.data= population[pop_index][expTime][9];
        right_down_leg_to_right_foot.publish(msg);
        //left leg

        msg.data= population[pop_index][expTime][10];
        body_to_left_leg_joint.publish(msg);
        msg.data= population[pop_index][expTime][11];
        left_leg_joint_to_left_up_leg.publish(msg);
        msg.data= population[pop_index][expTime][12];
        left_up_leg_to_left_down_leg.publish(msg);
        msg.data= population[pop_index][expTime][13];
        left_down_leg_to_left_foot.publish(msg);
        //ROS_INFO("count:%d, data:%f",count, msg.data);

        ros::spinOnce();

        loop_rate.sleep();
        count++;

        if(count>=EXPTIME*8){
          resetExp();
        }
      }
    }
    ROS_INFO("END OF Simulation, Start Generation");
    saveFile(generation_index);
    fitCal();
    selection();
    saveFile(generation_index);
    //get new Generation Population Size
    POPULATION_SIZE=ORGIN_POPULATION_SIZE*ORGIN_POPULATION_SIZE/(ORGIN_POPULATION_SIZE+DECREASE_RATE_POP*generation_index);
    crossOver();
    mutation();
    pop_index=0;

  }
  return 0;
}
