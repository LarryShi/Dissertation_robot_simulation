cmake_minimum_required(VERSION 2.8.3)
project(test_robot)

find_package(catkin REQUIRED roslaunch)
catkin_package()

roslaunch_add_file_check(launch)

install(DIRECTORY config images meshes launch rviz urdf
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION})
