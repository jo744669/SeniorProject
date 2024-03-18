// CapstoneSpring.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>

using namespace cv;
using namespace std;

int currentScene; //global variable to hold what scene user is at

int main()
{
    //Find coordinates of both displays - laptop and monitor
    int wDisplay1 = int(1920 / 2);    
    int hDisplay1 = int(1080 / 2);
    int wDisplay2 = int(1920 / 2);    
    int hDisplay2 = int(1080 / 2);
    currentScene = 0; //keeps track of what scene user is currently at

    //Calibration Step 1 - do this each time for each image after setting up the camera
    /*
    * Run this program
    * Aim the camera at the display (where the story will be projected)
    * In the other window on the other display where the camera is:
    *   move the mouse cursor to the center of each decision and record the positions
    * Repeat manually for each decision image
    * Stop this program
    * Set those (x,y) values below
    * Do not set the colors
    * Repeat this every time the camera or display is moved
    */

    //Start scene
    int xKnobRight = 396; int yKnobRight = 221;
    int xKnobLeft = 209; int yKnobLeft = 219;
    Vec3b knobLeftColor = { 0, 0, 0 }; Vec3b knobRightColor = { 0, 0, 0 };

    //Scene 1A
    int xChrys = 0; int yChrys = 0;
    int xViolet = 0; int yViolet = 0;
    Vec3b chrysColor = { 0, 0, 0 }; Vec3b violetColor = { 0, 0, 0 };

    //Scene 1B
    int xWinterKey = 0; int yWinterKey = 0;
    int xSpringKey = 0; int ySpringKey = 0;
    Vec3b winterKeyColor = { 0, 0, 0 }; Vec3b springKeyColor = { 0, 0, 0 };

    VideoCapture cam = VideoCapture(1); //assign to the correct port
    namedWindow("camOut");
    setWindowProperty("camOut", WND_PROP_FULLSCREEN, WINDOW_FULLSCREEN);
    int image_counter = 0;
    bool returned; //boolean to check that an image was returned from the camera
    Mat frame;
    int decision;
    
    /*
    * This loop acts as the main loop for detecting decisions, displaying images, and playing videos
    */
    while (true)
    {
        returned = cam.read(frame); //frame comes from external camera image
        if (!returned)
        {
            cout << "Failed to grab frame" << endl;
            break;
        }
        imshow("camOut", frame);

        //calibrate automatically based on scene - only calibrate if it hasn't been done yet before
        if (currentScene == 0 && vector_is_empty(knobLeftColor) && vector_is_empty(knobRightColor))
        {
            calibrate(xKnobLeft, yKnobLeft, xKnobRight, yKnobRight, knobLeftColor, knobRightColor, frame);
            cout << "calibrated scene " << currentScene << endl;
        }
        else if (currentScene == 1 && vector_is_empty(chrysColor) && vector_is_empty(violetColor))
        {
            calibrate(xViolet, yViolet, xChrys, yChrys, violetColor, chrysColor, frame);
            cout << "calibrated scene " << currentScene << endl;
        }
        else if (currentScene == 2 && vector_is_empty(winterKeyColor) && vector_is_empty(springKeyColor))
        {
            calibrate(xWinterKey, yWinterKey, xSpringKey, ySpringKey, winterKeyColor, springKeyColor, frame);
            cout << "calibrated scene " << currentScene << endl;
        }

        //check what scene currently at to know what decisions are the choices
        //detect decisions accordingly
        if (currentScene == 0)
        {
            decision = detect_decision(xKnobLeft, yKnobLeft, xKnobRight, yKnobRight, knobLeftColor, knobRightColor, frame);
        }
        else if (currentScene == 1)
        {
            decision = detect_decision(xViolet, yViolet, xChrys, yChrys, violetColor, chrysColor, frame);
        }
        else if (currentScene == 2)
        {
            decision = detect_decision(xWinterKey, yWinterKey, xSpringKey, ySpringKey, winterKeyColor, springKeyColor, frame);
        }
        
        //check if scene needs to change and call playVideo method for correct video
        if (decision != 0 && decision != -1)
        {
            //CHANGE THESE SCENE NAMES BASED ON FILE NAMES
            if (currentScene == 0)
            {
                if (decision == 1) 
                { 
                    currentScene = 1;
                    playVideo("Scene1A", wDisplay1); 
                }
                if (decision == 2) 
                {
                    currentScene = 2;
                    playVideo("Scene1B", wDisplay1); 
                }
            }
            if (currentScene == 1)
            {
                if (decision == 1) 
                { 
                    currentScene = 3;
                    playVideo("Scene2A", wDisplay1); 
                }
                if (decision == 2) 
                { 
                    currentScene = 4;
                    playVideo("Scene2B", wDisplay1);
                }
            }
            if (currentScene == 2)
            {
                if (decision == 1) 
                { 
                    currentScene = 5;
                    playVideo("Scene2C", wDisplay1); 
                }
                if (decision == 2) 
                { 
                    currentScene = 6;
                    playVideo("Scene2D", wDisplay1); 
                }
            }
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
                calibrate(xKnobLeft, yKnobLeft, xKnobRight, yKnobRight, knobLeftColor, knobRightColor, frame);
            }
            if (currentScene == 1)
            {
                calibrate(xViolet, yViolet, xChrys, yChrys, violetColor, chrysColor, frame);
            }
            if (currentScene == 2)
            {
                calibrate(xWinterKey, yWinterKey, xSpringKey, ySpringKey, winterKeyColor, springKeyColor, frame);
            }
        }
        else if (k % 256 == 32)
        {
            //space is pressed - save current frme from camera view
            string img_name = "frame_" + image_counter;
            imwrite(img_name, frame);
            cout << "space pressed and image writter" << endl;
            image_counter++;
        }
    }

    //avoid memory leaks at the very end
    delete &knobLeftColor;
    delete &knobRightColor;
    delete &chrysColor;
    delete &violetColor;
    delete &winterKeyColor;
    delete &springKeyColor;
    cam.release();
    destroyAllWindows();
}

/*
* This method will be used to find the colors of the different decision options
*/
int calibrate(int xOne, int yOne, int xTwo, int yTwo, Vec3b& oneColor, Vec3b& twoColor, Mat image)
{
    //uses reference variables for  colors to edit the color variables in main method without returning multiple values
    //assigns RGB value at location of decision to color vector
    oneColor = image.at<Vec3b>(yOne, xOne);
    twoColor = image.at<Vec3b>(yTwo, xTwo);
    cout << "calibrate: " << oneColor << " and " << twoColor << endl;
    return 1;
}

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

/*
* This method will be used to find and display the image the program should be looking for.
* This method should be called in the beginning for the start scene and for the pause scenes when waiting for user's decision
* @args imageName = name of image file to look for with .jpg extension
* @args wDisplay1
*/
int getImage(String imageName, int wDisplay1)
{
    //import image from file folder 
    string image_path = samples::findFile(imageName); //FIX THIS FILE PATH
    Mat img = imread(image_path, IMREAD_COLOR);
    cout << img.size << endl;

    //set height and width to cols and rows respectively
    int hStory = img.cols;
    int wStory = img.rows;

    if (img.empty())
    {
        cout << "Could not read the image: " << image_path << endl;
        return -1;
    }

    //display the image to the correct window
    namedWindow("story", WINDOW_NORMAL);
    moveWindow("story", wDisplay1, 0); //position display
    setWindowProperty("story", WND_PROP_FULLSCREEN, WINDOW_FULLSCREEN);
    imshow("story", img); //displays the image

    delete &img; //delete to avoid memory leaks
    return 0;
}

/*
* This method will be used to display the video of the correct scene 
*/
int playVideo(String videoName, int wDisplay1)
{
    VideoCapture cap(videoName);
    // Check if camera opened successfully
    if (!cap.isOpened()) {
        cout << "Error opening video stream or file" << endl;
        return -1;
    }

    //display video to the correct window
    namedWindow("video", WINDOW_NORMAL);
    moveWindow("video", wDisplay1, 0); //position display
    setWindowProperty("video", WND_PROP_FULLSCREEN, WINDOW_FULLSCREEN);
    Mat frame;

    while (1) 
    {
        // Capture frame-by-frame
        cap >> frame;

        // If the frame is empty, break immediately
        if (frame.empty())
            break;

        // Display the resulting frame
        imshow("video", frame);
    }

    delete &frame;
    cap.release();
    return 0;
    //DO I NEED TO CALL GET IMAGE VARIABLE HERE
}

/*
* This method will be used to detect if the user has made their decision in a given scene
* @args xOne, yOne - the x and y coordinates of option 1
* @args xTwo, yTwo - the x and y coordinates of option 2
* @args oneColor, twoColor - the original colors of option 1 and 2 determined in calibration step 2
* @args image - the current image displayed on the screen
*/
int detect_decision(int xOne, int yOne, int xTwo, int yTwo, Vec3b oneColor, Vec3b twoColor, Mat image)
{
    //check to make sure you remembered to do calibration step 2
    if (vector_is_empty(oneColor) || vector_is_empty(twoColor))
    {
        cout << "not calibrated" << endl;
        return -1;
    }

    //assign variables needed to check for color disparities
    Vec3b& tmp1 = image.at<Vec3b>(yOne, xOne);
    Vec3b& tmp2 = image.at<Vec3b>(yTwo, xTwo);
    int threshold = 50;
    bool selected1 = false;
    bool selected2 = false;

    //Checking for color disparities with option 1
    Vec3b d = oneColor - tmp1;
    //multiply each element by itself individually
    d[0] = d[0] * d[0];
    d[1] = d[1] * d[1];
    d[2] = d[2] * d[2];

    int difference = d[0] + d[1] + d[2]; //calculate total difference by adding RGB values together

    if (difference > threshold)
    {
        selected1 = true;
    }

    //Checking for color disparities with option 2
    d = twoColor - tmp1;
    //multiply each element by itself individually
    d[0] = d[0] * d[0];
    d[1] = d[1] * d[1];
    d[2] = d[2] * d[2];

    difference = d[0] + d[1] + d[2];
    if (difference > threshold)
    {
        selected2 = true;
    }

    //delete vectors to avoid memory leaks - before checking for decision or would never be deleted when decision is found
    delete &tmp1;
    delete &tmp2;
    delete &d;

    if (selected1 && selected2)
    {
        cout << "You must only choose 1 choice" << endl;
        return -1;
    }
    else if (selected1)
    {
        cout << "Option 1 was chosen" << endl;
        return 1;
    }
    else if (selected2)
    {
        cout << "Option 2 was chosen" << endl;
        return 2;
    }
    else
    {
        return 0;
    }

    return 0; //only gets here if no option chosen and exits if statement for some reason
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
