
#pragma once

#include <memory>

// ros
#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>
#include <nav_msgs/Odometry.h>
#include <ros/ros.h>

// rpg quadrotor
#include <autopilot/autopilot_helper.h>
#include <autopilot/autopilot_states.h>
#include <quadrotor_common/parameter_helper.h>
#include <quadrotor_msgs/AutopilotFeedback.h>

// flightlib
#include "flightlib/bridges/unity_bridge.hpp"
#include "flightlib/bridges/unity_message_types.hpp"
#include "flightlib/common/quad_state.hpp"
#include "flightlib/common/types.hpp"
#include "flightlib/objects/quadrotor.hpp"
#include "flightlib/sensors/rgb_camera.hpp"

#include "geometry_msgs/Quaternion.h"

using namespace flightlib;

namespace flightros {

class FlightPilotCustom {
 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
  FlightPilotCustom(const ros::NodeHandle& nh, const ros::NodeHandle& pnh);
  ~FlightPilotCustom();

  // callbacks
  void mainLoopCallback(const ros::TimerEvent& event);
  void poseCallback(const nav_msgs::Odometry::ConstPtr& msg);
  void gimbalOrientationCallback(const geometry_msgs::Quaternion::ConstPtr& msg);

  bool setUnity(const bool render);
  bool connectUnity(void);
  bool loadParams(void);

 private:
  // ros nodes
  ros::NodeHandle nh_;
  ros::NodeHandle pnh_;

  // publisher
  // camera publishers
  image_transport::Publisher rgb_pub;
  image_transport::Publisher depth_pub;
  image_transport::Publisher segmentation_pub;
  image_transport::Publisher opticalflow_pub;

  // subscriber
  ros::Subscriber sub_state_est_;
  ros::Subscriber gimbal_orientation_sub_;

  // main loop timer
  ros::Timer timer_main_loop_;

  // unity quadrotor
  std::shared_ptr<Quadrotor> quad_ptr_;
  std::shared_ptr<RGBCamera> rgb_camera_;
  QuadState quad_state_;

  std::shared_ptr<Quadrotor> gimbal_ptr_;
  QuadState gimbal_state_;

  std::shared_ptr<RGBCamera> rgb_camera2;

  Vector<3>  gimbal_position;
  Quaternion gimbal_orientation;

  // Flightmare(Unity3D)
  std::shared_ptr<UnityBridge> unity_bridge_ptr_;
  SceneID scene_id_{UnityScene::WAREHOUSE};
  bool unity_ready_{false};
  bool unity_render_{false};
  RenderMessage_t unity_output_;
  uint16_t receive_id_{0};

  // auxiliary variables
  Scalar main_loop_freq_{50.0};
  FrameID frame_id{0};
};
}  // namespace flightros