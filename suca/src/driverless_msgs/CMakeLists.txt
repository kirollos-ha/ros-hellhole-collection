cmake_minimum_required(VERSION 3.0.2)
project(driverless_msgs)

find_package(catkin REQUIRED COMPONENTS
  std_msgs
  visualization_msgs
  sensor_msgs
  geometry_msgs
  message_generation
  message_runtime
)

add_message_files(
  FILES
  bounding_boxes.msg
  ImageArray.msg
  curvature_throttle.msg
  control_to_traj.msg
  trajectory.msg
  MarkerArrayStamped.msg
  can.msg
  PoseStamped.msg
  steering.msg
)

generate_messages(
  DEPENDENCIES
  std_msgs
  visualization_msgs
  sensor_msgs
  geometry_msgs
)


catkin_package(
    CATKIN_DEPENDS
    std_msgs
    message_runtime
    visualization_msgs
    sensor_msgs
    geometry_msgs
)

