#include "rclcpp/rclcpp.hpp"


#include <nav_msgs/msg/odometry.hpp>
#include "tf2/LinearMath/Quaternion.h"
#include "geometry_msgs/msg/pose.hpp"
#include "tf2_ros/transform_broadcaster.h"
#include <string>
#include <geometry_msgs/msg/pose_stamped.hpp>
#include <tf2/LinearMath/Matrix3x3.h>

// using namespace std::placeholders;

class OdomSubscriber : public rclcpp::Node {
    public:
        OdomSubscriber(): Node("odom_sub") {
            // Initialize the transform broadcaster
            m_tf_broadcaster = std::make_shared<tf2_ros::TransformBroadcaster>(*this);
            // Create odom subscriber
            mOdomSub = create_subscription<nav_msgs::msg::Odometry>("/robot1/odom", 10, std::bind(&OdomSubscriber::odomCallback, this, std::placeholders::_1) );
        }

    protected:
        void broadcast_odom_footprint(geometry_msgs::msg::TransformStamped t)
        {
            
            // This function is used for send the tf, I write this function is because the pdf said we need to call a function
            
            m_tf_broadcaster->sendTransform(t);
        }
        void odomCallback(const nav_msgs::msg::Odometry::SharedPtr msg) {
            // initialize the tf
            geometry_msgs::msg::TransformStamped t;

            // save the odom value to TransformStamped
            t.header.stamp = this->get_clock()->now();
            t.header.frame_id = "/robot1/odom";
            t.child_frame_id = "/robot1/base_footprint";

            t.transform.translation.x = msg->pose.pose.position.x;
            t.transform.translation.y = msg->pose.pose.position.y;
            t.transform.translation.z = msg->pose.pose.position.z;

            t.transform.rotation.x = msg->pose.pose.orientation.x;
            t.transform.rotation.y = msg->pose.pose.orientation.y;
            t.transform.rotation.z = msg->pose.pose.orientation.z;
            t.transform.rotation.w = msg->pose.pose.orientation.w;

            // send the transformation
            // m_tf_broadcaster->sendTransform(t);
            
            // use this one should be fine?  
            OdomSubscriber::broadcast_odom_footprint(t);

        }

    private:
        // tf broadcaster
        std::shared_ptr<tf2_ros::TransformBroadcaster> m_tf_broadcaster;
        // odom subscriber
        rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr mOdomSub;
    };



int main(int argc, char *argv[])
{
    // init
    rclcpp::init(argc, argv);
    // node

    // create the odom subscriber node
    auto node = std::make_shared<OdomSubscriber>();
    // run the node
    rclcpp::spin(node);
    // rclcpp::spin(std::make_shared<OdomSubscriber>());


    // shutdown
    rclcpp::shutdown();
}