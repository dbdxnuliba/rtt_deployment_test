cmake_minimum_required(VERSION 2.8.3)
project(deployment_test)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rtt_ros
  actionlib
  actionlib_msgs
  genmsg
  rtt_roscomm
)


include_directories(
  ${catkin_INCLUDE_DIRS}
)

#add_action_files(DIRECTORY action FILES SomeAction.action)
#generate_messages(DEPENDENCIES actionlib_msg)

ros_generate_rtt_typekit(${PROJECT_NAME})
catkin_package()


find_package(catkin REQUIRED)

find_package(OROCOS-RTT REQUIRED)

#find_package(conman_hook)

include(${OROCOS-RTT_USE_FILE_PATH}/UseOROCOS-RTT.cmake )

set(CMAKE_BUILD_TYPE Debug)

###########
## Build ##
###########

orocos_use_package( ocl-taskbrowser )
orocos_use_package( ocl-deployment )
orocos_use_package( ocl-logging )

include_directories(include ${catkin_INCLUDE_DIRS} ${BOOST_INCLUDE_DIRS}, ${USE_OROCOS_INCLUDE_DIRS})
#orocos_plugin(conman_hook
#    src/hook_service.cpp )
#target_link_libraries(conman_hook conman)

#orocos_component(conman_components
#    src/conman_components.cpp )
#target_link_libraries(conman_components conman)

add_definitions(-DRTT_COMPONENT)

# add_library(plant src/Plant.cpp)
# add_library(controller src/Controller.cpp)

orocos_component(Plant src/Plant.cpp) # ...you may add multiple source files
set_property(TARGET Plant APPEND PROPERTY COMPILE_DEFINITIONS RTT_COMPONENT)
add_dependencies(Plant ${catkin_EXPORTED_TARGETS})

orocos_component(Controller src/Controller.cpp) # ...you may add multiple source files
set_property(TARGET Controller APPEND PROPERTY COMPILE_DEFINITIONS RTT_COMPONENT)
add_dependencies(Controller ${catkin_EXPORTED_TARGETS})


target_link_libraries(Plant
    ${catkin_LIBRARIES}
    ${GMOCK_LIBRARY}
    ${OROCOS-RTT_RTT-SCRIPTING_LIBRARY}
    ${USE_OROCOS_LIBRARIES})

target_link_libraries(Controller
    ${catkin_LIBRARIES}
    ${GMOCK_LIBRARY}
    ${OROCOS-RTT_RTT-SCRIPTING_LIBRARY}
    ${USE_OROCOS_LIBRARIES})


orocos_install_headers(DIRECTORY include/${PROJECT_NAME})
orocos_generate_package(INCLUDE_DIRS include)

install(DIRECTORY launch DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION})
install(DIRECTORY scripts DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION})
#
# Generates and installs our package. Must be the last statement such
# that it can pick up all above settings.
#
orocos_generate_package()






#############
## Install ##
#############

## Mark cpp header files for installation
install(DIRECTORY include/${PROJECT_NAME}/
  DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
  )



