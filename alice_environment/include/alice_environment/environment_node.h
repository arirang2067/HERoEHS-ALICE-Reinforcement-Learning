#include <ros/ros.h>
#include <rl_msgs/SensorData.h>
#include "robotis_controller_msgs/StatusMsg.h"
#include <geometry_msgs/Vector3.h>
#include <geometry_msgs/Pose2D.h>
using namespace std;

class Env
{
    public:
    Env(ros::NodeHandle n);
    ~Env();
    ros::Publisher pub_number;
    rl_msgs::SensorData count;
    
    ros::Subscriber walking_status_sub;
    bool            walking_flag;
    double          difference_E;
    void walkingCallback(const robotis_controller_msgs::StatusMsg::ConstPtr& msg);

    ros::Subscriber ref_zmp_sub;
    bool            ref_zmp_flag;
    double          ref_zmp_sum;
    double          ref_zmp_count;
    double          ref_zmp_E;
    void refZMPCallback(const geometry_msgs::Vector3::ConstPtr& msg);

    ros::Subscriber fsr_zmp_sub;
    bool            fsr_zmp_flag;
    double          fsr_zmp_sum;
    double          fsr_zmp_count;
    double          fsr_zmp_E;
    void fsrZMPCallback(const geometry_msgs::Pose2D::ConstPtr& msg);

    private:
};