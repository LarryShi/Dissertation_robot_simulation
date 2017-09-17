#include <ros/ros.h>
//引入的是ROS Service的type
//因此在開新專案的時候，就需要引入Service端的Package相依性
//格式為<Server的Package名稱/Service名稱.h>
#include "gazebo_msgs/SetModelState.h"

using namespace std;

int main(int argc,char **argv)
{
    //初使化ROS的Client端
    ros::init(argc,argv,"move_pr2_by_magic_node"); //此字串不能有空白
    //建立一個Client端呼叫/gazebo/set_model_state來瞬移PR2的位置
    ros::NodeHandle n;
    ros::ServiceClient client = n.serviceClient<gazebo_msgs::SetModelState>("/gazebo/set_model_state");

    //設定PR2 Position
    geometry_msgs::Point pr2_position;
    pr2_position.x = 1.0;
    pr2_position.y = 0.0;
    pr2_position.z = 0.0;
    //設定PR2 orientation
    geometry_msgs::Quaternion pr2_orientation;
    pr2_orientation.x = 0.0;
    pr2_orientation.y = 0.0;
    pr2_orientation.z = 0.0;
    pr2_orientation.w = 1.0;

    //設定PR2 pose (Pose + Orientation)
    geometry_msgs::Pose pr2_pose;
    pr2_pose.position = pr2_position;
    pr2_pose.orientation = pr2_orientation;

    //設定ModelState
    gazebo_msgs::ModelState pr2_modelstate;
    pr2_modelstate.model_name = (std::string) "pr2";
    pr2_modelstate.pose = pr2_pose;

    //準備設定gazebo中PR2瞬移後的位置pose
    gazebo_msgs::SetModelState srv;
    srv.request.model_state = pr2_modelstate;

    //跟Server端連線，送出PR2要瞬移的位置
    if(client.call(srv))
    {
        ROS_INFO("PR2's magic moving success!!");
    }
    else
    {
        ROS_ERROR("Failed to magic move PR2! Error msg:%s",srv.response.status_message.c_str());
    }
    return 0;
}
