#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>
#include <boost/asio.hpp>
#include <std_msgs/UInt16.h>
#include <math.h>

#define PI 3.14159265

void addDummyData(sensor_msgs::LaserScan::Ptr scan);



int main(int argc, char **argv)
{
  //ROS Node initialization
  ros::init(argc,argv,"scan_publisher");
  ros::NodeHandle n;
  ros::NodeHandle ns_nh("~");

  std::string frame_id;

  std_msgs::UInt16 rpms;
 
  //assign frame_id
  ns_nh.param("frame_id", frame_id, std::string("robot_frame/world"));

    //publish the LaserScan msg in /scan topic
    ros::Publisher laser_pub = n.advertise<sensor_msgs::LaserScan>("scan", 1000);

    //publish the rpm data of scanner in /rpms topic
    ros::Publisher motor_pub = n.advertise<std_msgs::UInt16>("rpms",1000);
    
    //Create LaserScan pointer and allocate space dynamically
    sensor_msgs::LaserScan::Ptr scan(new sensor_msgs::LaserScan);

      //Initializing the constant data in LaserScan msg  
      scan->header.frame_id = frame_id;         
      scan->angle_min = 0.0;
      scan->angle_max = 2.0*M_PI;
      scan->angle_increment = (2.0*M_PI/360.0);
      scan->range_min = 0.06;
      scan->range_max = 5.0;
      scan->ranges.reserve(360);
      scan->intensities.reserve(360);
      rpms.data=50;
 
 //Loop for publishing scanner data
 while (ros::ok()) {
      
      scan->header.stamp = ros::Time::now();
      scan->time_increment = 1000/1e8;
      addDummyData(scan);
      laser_pub.publish(scan);
      motor_pub.publish(rpms);

    }
    return 0;
}




//Function to add Dummy values to the LaserScan msg members
void addDummyData(sensor_msgs::LaserScan::Ptr scan)
{
	scan->ranges.resize(360);
	scan->intensities.resize(360);
	for(int i = 0; i < 360; i++)
	{
		uint16_t intensity=0;
		if(i <= 180)
			intensity = 1000*sin(i*PI/180);
		else
		        intensity = 1000*sin((360-i)*PI/180);
		uint16_t range = 2;
	      	scan->ranges.push_back(range);
	        scan->intensities.push_back(intensity); 
	}


}
