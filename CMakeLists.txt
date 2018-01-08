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
)

find_package(OpenCV)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES visao_obr
#  CATKIN_DEPENDS cv_bridge geometry_msgs image_stream_opencv image_transport roscpp rospy std_msgs video_stream_opencv
#  DEPENDS system_lib
)

add_library( visao_obr
	src/FindRectangle.cpp
	src/FindCircles.cpp
	src/FindGeometric.cpp
)

include_directories(
# include
  ${catkin_INCLUDE_DIRS}
	${OpenCV_INCLUDE_DIRS}
)

add_executable(search_circles src/find_circles.cpp)
target_link_libraries(search_circles visao_obr ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})

add_executable(search_rectangle src/find_rectangle.cpp)
target_link_libraries(search_rectangle visao_obr ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})

