rosparam load ./src/my_robot/config/robot_controllers.yaml
rosrun controller_manager spawner my_robot_joint_state_controller my_robot_head_controller my_robot_body_to_right_shoulder_controller
