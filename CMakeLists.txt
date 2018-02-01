cmake_minimum_required(VERSION 2.8.3)
project(visao_obr)

find_package(catkin REQUIRED COMPONENTS
  cv_bridge
  image_transport
  geometry_msgs
  video_stream_opencv
  image_stream_opencv

  roscpp
  rospy
  std_msgs
	message_generation
  dynamic_reconfigure
)

add_message_files(
	FILES
	CaliRect.msg
	CaliCirc.msg
	CaliGeometricCam.msg
)

generate_messages(
	DEPENDENCIES
	std_msgs
)

find_package(OpenCV)

## Generate dynamic reconfigure parameters in the 'cfg' folder
generate_dynamic_reconfigure_options(
  cfg/CalibracaoData.cfg
#   cfg/DynReconf2.cfg
)


catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES visao_obr
#  CATKIN_DEPENDS cv_bridge geometry_msgs image_stream_opencv image_transport roscpp rospy std_msgs video_stream_opencv
	CATKIN_DEPENDS message_runtime
#  DEPENDS system_lib
)

add_library( visao_obr
	src/FindRectangle.cpp
	src/FindCircles.cpp
	src/FindGeometric.cpp
	src/Calibration.cpp
	node/cali_server.cpp
)

include_directories(
# include
  ${catkin_INCLUDE_DIRS}
	${OpenCV_INCLUDE_DIRS}
)

add_executable(search_circles src/find_circles.cpp)
target_link_libraries(search_circles visao_obr ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})
add_dependencies(search_circles ${PROJECT_NAME}_gencfg)

add_executable(search_rectangle src/find_rectangle.cpp)
target_link_libraries(search_rectangle visao_obr ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})
add_dependencies(search_rectangle ${PROJECT_NAME}_gencfg)

add_executable(cali src/CaliClient.cpp)
target_link_libraries(cali visao_obr ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})
add_dependencies(cali ${PROJECT_NAME}_gencfg)

add_executable(cali_server node/cali_server.cpp)
target_link_libraries(cali_server visao_obr ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})
add_dependencies(cali_server ${PROJECT_NAME}_gencfg)


