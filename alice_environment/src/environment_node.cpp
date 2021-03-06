#include "alice_environment/environment_node.h"

Env::Env(ros::NodeHandle n)
{
    pub_number = n.advertise<rl_msgs::SensorData>("rl/test/topic", 10);

    walking_flag = false;
    walking_status_sub = n.subscribe("/heroehs/status", 10, &Env::walkingCallback, this);

    ref_zmp_flag = false;
    ref_zmp_sub = n.subscribe("/heroehs/alice_reference_zmp", 10, &Env::refZMPCallback, this);

    fsr_zmp_flag = false;
    fsr_zmp_sub = n.subscribe("/alice/zmp", 10, &Env::fsrZMPCallback, this);
}
Env::~Env(){}
void Env::walkingCallback(const robotis_controller_msgs::StatusMsg::ConstPtr& msg)
{
    if(msg->status_msg == "Walking_Started")
    {
        walking_flag = true;
    }
    else if(msg->status_msg == "Walking_Finished")
    {
        ref_zmp_E = ref_zmp_sum/ref_zmp_count;
        fsr_zmp_E = fsr_zmp_sum/fsr_zmp_count;
        difference_E = ref_zmp_E - fsr_zmp_E;
        similarity = 1 - (difference_E/ref_zmp_E);
        ROS_INFO("ref_zmp : %f", ref_zmp_E);
        ROS_INFO("fsr_zmp : %f", fsr_zmp_E);
        ROS_INFO("similarity : %f", similarity);
        walking_flag = false;
        ref_zmp_flag = false;
        fsr_zmp_flag = false;
    }
}
void Env::refZMPCallback(const geometry_msgs::Vector3::ConstPtr& msg)
{
    if(walking_flag == true)
    {
        if(ref_zmp_flag == false)
        {
            ref_zmp_flag = true;
            ref_zmp_sum = 0;
            ref_zmp_count = 0;
        }
        else if(ref_zmp_flag == true)
        {
            if(msg -> y > 0) ref_zmp_sum = ref_zmp_sum + msg -> y;
            else ref_zmp_sum = ref_zmp_sum - msg -> y;
            ref_zmp_count ++;
        }
    }
    //ROS_INFO("ref_zmp_sum : %f      ref_zmp_count : %f", ref_zmp_sum, ref_zmp_count);
}
void Env::fsrZMPCallback(const geometry_msgs::Pose2D::ConstPtr& msg)
{
    if(walking_flag == true)
    {
        if(fsr_zmp_flag == false)
        {
            fsr_zmp_flag = true;
            fsr_zmp_sum = 0;
            fsr_zmp_count = 0;
        }
        else if(fsr_zmp_flag == true)
        {
            if(msg -> y > 0) fsr_zmp_sum = fsr_zmp_sum + msg -> y;
	    else fsr_zmp_sum = fsr_zmp_sum - msg -> y;
            fsr_zmp_count ++;
        }
    }    
    //ROS_INFO("fsr_zmp_sum : %f      fsr_zmp_count : %f", fsr_zmp_sum, fsr_zmp_count);
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "environment_node");
    ros::NodeHandle n_;
    Env env1(n_);


    ros::Rate loop_rate(100);
	ROS_INFO("alice_environment_node start!!!");

    while(ros::ok())
    {
        env1.pub_number.publish(env1.count);
        env1.count.data++;
        //ROS_INFO("aaa");
        ros::spinOnce();
        loop_rate.sleep();
    }
}
