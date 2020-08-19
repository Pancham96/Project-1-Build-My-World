#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>

// Define a global client that can request services
ros::ServiceClient client;

// This function calls the command_robot service to drive the robot in the specified direction
void drive_robot(float lin_x, float ang_z)
{
    
    ROS_INFO_STREAM("Moving robot towards the white ball");

    ball_chaser::DriveToTarget srv;
    srv.request.linear_x = lin_x;
    srv.request.angular_z = ang_z;

    if(!client.call(srv))
       ROS_ERROR("Failed to call service command_robot");
}

// This callback function continuously executes and reads the image data
void process_image_callback(const sensor_msgs::Image img)
{

    int white_pixel = 255;
    int column = 0;
    int l = 0; //left
    int m = 0; //middle
    int r = 0; //right

    for(int i = 0; i < img.height * img.step; i += 3)
    {

        if(img.data[i] == white_pixel && img.data[i+1] == white_pixel && img.data[i+2] == white_pixel)
        {

            column = i % img.step;

            if(column >= 0 && column < img.step / 3)
            {
                l++;
            }  

            else if(column >= img.step / 3 && column < img.step * 2 / 3)
            {
                m++;
            }
        
            else if(column >= img.step * 2 / 3)
            {
                r++;
            }

        }
    }

    if(l > m && l > r)
    {
        drive_robot(0.0,0.5);
    }
    else if(m > l && m > r)
    {
        drive_robot(0.5,0.0);
    }
    else if(r>l && r>m)
    {
        drive_robot(0.0, -0.5);
    }
    else
    {
        drive_robot(0,0);
    }

}

int main(int argc, char** argv)
{
    // Initialize the process_image node and create a handle to it
    ros::init(argc, argv, "process_image");
    ros::NodeHandle n;

    // Define a client service capable of requesting services from command_robot
    client = n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");

    // Subscribe to /camera/rgb/image_raw topic to read the image data inside the process_image_callback function
    ros::Subscriber sub1 = n.subscribe("/camera/rgb/image_raw", 10, process_image_callback);

    // Handle ROS communication events
    ros::spin();

    return 0;
}
