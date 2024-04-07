// CapstoneSpring.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <Windows.h>

#include <iostream>

using namespace cv;
using namespace std;

/*
* This method checks if the vector is empty so that this code does not take up space in the main method each time it needs to be used
*/
bool vector_is_empty(Vec3b vector)
{
    if (vector[0] == 0)
    {
        if (vector[1] == 0)
        {
            if (vector[2] == 0)
            {
                return true;
            }
        }
    }
    return false;
}

//list all cameras since there are more than 1 - webcam + offboard
//only need to run this method once at very beginning
//my computer: port 0 is webcam and port 1 is external camera
int list_ports()
{
    //create arrays of size bigger than possible number of ports
    //must keep track of index for each array
    int working[5] = { NULL, NULL, NULL, NULL, NULL };
    int working_index = 0;

    int non_working[6] = { NULL, NULL, NULL, NULL, NULL, NULL };
    int non_working_index = 0;

    int available[5] = { NULL, NULL, NULL, NULL, NULL };
    int available_index = 0;

    int port = 0;
    VideoCapture camera;

    //variables to hold data for when the port is working
    Mat img;
    bool is_reading = true;
    double w = 0; double h = 0;

    //tests until finds more than 6 non working ports - last spot in non_working array is filled
    while (non_working[5] == NULL)
    {
        camera = VideoCapture(port);
        if (!camera.isOpened())
        {
            non_working[non_working_index] = port;
            cout << "Port " << port << " is not working" << endl;
            non_working_index++;
        }
        else
        {
            is_reading = camera.read(img);
            w = camera.get(3);
            h = camera.get(4);

            if (is_reading)
            {
                cout << "Port " << port << " is working and reads images (" << h << " x " << w << ")" << endl;
                working[working_index] = port;
                working_index++;
            }
            else
            {
                cout << "Port " << port << " for camera (" << h << " x " << w << ") is present but does not read" << endl;
                available[available_index] = port;
                available_index++;
            }
        }
        camera.release(); //release the camera each time
        port++; //move to the next port to be worked with
    }
    return 0;
}

int main()
{
    //Find coordinates of both displays - laptop (1) and monitor (2)
    int wDisplay1 = 1281;    //1920 / 2
    int hDisplay1 = 0;    //1080 / 2
    int wDisplay2 = 0;    
    int hDisplay2 = 0;

    //Start scene
    int xKnobRight = 359; int yKnobRight = 227;
    int xKnobLeft = 221; int yKnobLeft = 222;
    Vec3b knobLeftColor = { 0, 0, 0 }; Vec3b knobRightColor = { 0, 0, 0 };

    //Scene 1A
    int xChrys = 345; int yChrys = 94;
    int xViolet = 260; int yViolet = 88;
    Vec3b chrysColor = { 0, 0, 0 }; Vec3b violetColor = { 0, 0, 0 };

    //Scene 1B
    int xWinterKey = 237; int yWinterKey = 205;
    int xSpringKey = 318; int ySpringKey = 207;
    Vec3b winterKeyColor = { 0, 0, 0 }; Vec3b springKeyColor = { 0, 0, 0 };

    VideoCapture cam = VideoCapture(1); //assign to the correct port
    namedWindow("camOut");
    setWindowProperty("camOut", WND_PROP_FULLSCREEN, WINDOW_FULLSCREEN);
    int image_counter = 0; //number of images from the webcam saved to the computer
    bool returned; //boolean to check that an image was returned from the camera
    Mat frame; //frame from the webcam
    Mat vidFrame; //frame from when play a video
    int decision = 0; //holds decision of the user
    String video; //name of video to be played
    namedWindow("story", WINDOW_NORMAL);
    moveWindow("story", wDisplay1, 0); //position display
    setWindowProperty("story", WND_PROP_FULLSCREEN, WINDOW_FULLSCREEN);
    String imageName;
    int threshold = 50;
    bool selected1 = false;
    bool selected2 = false;
    Vec3b tmp1;
    Vec3b tmp2;
    Vec3b d1; Vec3b d2;
    bool playVideo = false;
    int currentScene = 0;
    int difference1 = 0;
    int difference2 = 0;
    
    //list_ports(); //FOR TESTING ONLY

    /*
    * This loop acts as the main loop for detecting decisions, displaying images, and playing videos
    */
    while (true)
    {
        playVideo = false;
        returned = cam.read(frame); //frame comes from external camera image
        if (!returned)
        {
            cout << "Failed to grab frame" << endl;
            break;
        }
        imshow("camOut", frame);

        //assign image name based on what scene we are at
        if (currentScene == 0) { imageName = "C:/Users/jilli/images/Start_A_0001.jpg"; } //start scene
        if (currentScene == 1) { imageName = "C:/Users/jilli/images/Scene 1A Flowers_0017.jpg"; } //scene 1A
        if (currentScene == 2) { imageName = "C:/Users/jilli/images/Scene 1B_0015.jpg"; } //scene 1B
        if (currentScene == 3) { imageName = "C:/Users/jilli/images/Scene 2A_0001.jpg"; } //scene 2A
        if (currentScene == 4) { imageName = "C:/Users/jilli/images/Scene 2B_0001.jpg"; } //scene 2B
        if (currentScene == 5) { imageName = "C:/Users/jilli/images/Scene 2C_0001.jpg"; } //scene 2C
        if (currentScene == 6) { imageName = "C:/Users/jilli/images/Scene 2D_0001.jpg"; } //scene 2D
        
        //display the scene
        Mat img = imread(imageName); //import image from file folder 
        //cout << img.size << endl; //FOR TESTING

        //set height and width to cols and rows respectively
        int hStory = img.cols;
        int wStory = img.rows;

        if (img.empty())
        {
            cout << "Could not read the image" << endl;
            return -1;
        }

        imshow("story", img); 
        cout << "display scene " << currentScene << endl;
        Sleep(500);
        
        //CHECK FOR DECISION
        //check to make sure you remembered to do calibration step 2
        if (currentScene == 0 && (vector_is_empty(knobLeftColor) || vector_is_empty(knobRightColor)))
        {
            cout << "not calibrated" << endl;
        }
        else if (currentScene == 1 && (vector_is_empty(violetColor) || vector_is_empty(chrysColor)))
        {
            cout << "not calibrated" << endl;
        }
        else if (currentScene == 2 && (vector_is_empty(springKeyColor) || vector_is_empty(winterKeyColor)))
        {
            cout << "not calibrated" << endl;
        }
        else //only calculate differences if it is calibrated
        {
            //assign variables needed to check for color disparities
            if (currentScene == 0)
            {
                tmp1 = frame.at<Vec3b>(yKnobLeft, xKnobLeft);
                tmp2 = frame.at<Vec3b>(yKnobRight, xKnobRight);
                d1 = knobLeftColor - tmp1;
                d2 = knobRightColor - tmp2;
            }
            else if (currentScene == 1)
            {
                tmp1 = frame.at<Vec3b>(yViolet, xViolet);
                tmp2 = frame.at<Vec3b>(yChrys, xChrys);
                d1 = violetColor - tmp1;
                d2 = chrysColor - tmp2;
            }
            else if (currentScene == 2)
            {
                tmp1 = frame.at<Vec3b>(yWinterKey, xWinterKey);
                tmp2 = frame.at<Vec3b>(ySpringKey, xSpringKey);
                d1 = winterKeyColor - tmp1;
                d2 = springKeyColor - tmp2;
            }
            //Checking for color disparities with option 1
            cout << "d1: " << d1 << endl;
            cout << "d2: " << d2 << endl;
            cout << "tmp1: " << tmp1 << "   tmp2: " << tmp2 << endl;

            //multiply each element by itself individually
            d1[0] = d1[0] * d1[0];
            d1[1] = d1[1] * d1[1];
            d1[2] = d1[2] * d1[2];

            difference1 = d1[0] + d1[1] + d1[2]; //calculate total difference by adding RGB values together

            cout << "diff 1 " << difference1 << endl;

            //Checking for color disparities with option 2
            //multiply each element by itself individually
            d2[0] = d2[0] * d2[0];
            d2[1] = d2[1] * d2[1];
            d2[2] = d2[2] * d2[2];

            difference2 = d2[0] + d2[1] + d2[2];
            cout << "diff 2 " << difference2 << endl;

            if (difference1 > threshold && difference1 > difference2)
            {
                selected1 = true;
            }
            if (difference2 > threshold && difference2 > difference1)
            {
                selected2 = true;
            }

            if (selected1 && selected2)
            {
                cout << "You must only choose 1 choice" << endl;
            }
            else if (selected1)
            {
                cout << "Option 1 was chosen" << endl;
                decision = 1;
            }
            else if (selected2)
            {
                cout << "Option 2 was chosen" << endl;
                decision = 2;
            }

            if (decision > 0) //CHANGE STRING OF VIDEO NAME HERE
            {
                if (currentScene == 0)
                {
                    if (decision == 1)
                    {
                        currentScene = 1;
                        video = "C:/Users/jilli/images/Scene1A.mp4";
                        playVideo = true;
                    }
                    if (decision == 2)
                    {
                        currentScene = 2;
                        video = "C:/Users/jilli/images/Scene1B.mp4";
                        playVideo = true;
                    }
                }
                else if (currentScene == 1)
                {
                    if (decision == 1)
                    {
                        currentScene = 3;
                        video = "Scene2A";
                    }
                    if (decision == 2)
                    {
                        currentScene = 4;
                        video = "Scene2B";
                    }
                }
                else if (currentScene == 2)
                {
                    if (decision == 1)
                    {
                        currentScene = 5;
                        video = "Scene2C";
                    }
                    if (decision == 2)
                    {
                        currentScene = 6;
                        video = "Scene2D";
                    }
                }
            }

            //reset decision variables
            selected1 = false;
            selected2 = false;
            decision = 0;
            d1 = { 0, 0, 0 };
            d2 = { 0, 0, 0 };
            difference1 = 0;
            difference2 = 0;
        }

        if (playVideo == true)
        {
            VideoCapture cap1(video);
            // Check if camera opened successfully
            if (!cap1.isOpened()) {
                cout << "Error opening video stream or file" << endl;
                return -1;
            }

            ////display video to the correct window
            //namedWindow("video", WINDOW_NORMAL);
            //moveWindow("video", wDisplay1, 0); //position display
            //setWindowProperty("video", WND_PROP_FULLSCREEN, WINDOW_FULLSCREEN);

            while (true)
            {
                // Capture frame-by-frame
                cap1 >> vidFrame;

                // If the frame is empty, break immediately
                if (vidFrame.empty())
                    break;

                // Display the resulting frame
                imshow("story", vidFrame);
                waitKey(3);
            }

            /*delete& vidFrame;
            cap1.release();*/
        }

        //setting up key strokes to do different actions
        int k = waitKey(1) & 0xff;
        if (k == 27 || k == ('q') || k == ('Q'))
        {
            //escape pressed
            cout << "Escape hit, closing..." << endl;
            break;
        }
        else if (k == 'c' || k == 'C')
        {
            //run calibration step 2 - this is here to do the calibration manually but it is automatically done in the main loop
            if (currentScene == 0)
            {
                knobLeftColor = frame.at<Vec3b>(yKnobLeft, xKnobLeft);
                knobRightColor = frame.at<Vec3b>(yKnobRight, xKnobRight);
            }
            else if (currentScene == 1)
            {
                violetColor = frame.at<Vec3b>(yViolet, xViolet);
                chrysColor = frame.at<Vec3b>(yChrys, xChrys);
            }
            else if (currentScene == 2)
            {
                winterKeyColor = frame.at<Vec3b>(yWinterKey, xWinterKey);
                springKeyColor = frame.at<Vec3b>(ySpringKey, xSpringKey);
            }
            cout << "knob left " << knobLeftColor << " and knob right " << knobRightColor << endl;
            cout << "winter: " << winterKeyColor << " and spring: " << springKeyColor << endl;
            cout << "violet " << violetColor << " and chrys " << chrysColor << endl;
        }
        else if (k % 256 == 32)
        {
            //space is pressed - save current frme from camera view
            string img_name = "C:/Users/jilli/images/savedImage.jpg";
            imwrite(img_name, frame);
            cout << "space pressed and image written" << endl;
            image_counter++;
        }
    }

    //avoid memory leaks at the very end
    /*delete &knobLeftColor;
    delete &knobRightColor;
    delete &chrysColor;
    delete &violetColor;
    delete &winterKeyColor;
    delete &springKeyColor;
    cam.release();
    destroyAllWindows();
    delete& tmp1;
    delete& tmp2;
    delete& d1;
    delete& d2;*/
}