/**
 * file:   CapstoneSpringV2.cpp
 * author: jillian o'connell
 * desc.:  what does this do?
 */
#include <string>
#include <unordered_map>

#include <opencv2/opencv.hpp>

#define CAM 1  //camera port
#define HOME "C:/Users/jilli/images/"       //path to where images and videos are stored
#define wDisplay1 1281  //primary (laptop) display width


using namespace cv;
using namespace std;

#define MAXCAM 10  //max number of ports to scan for cameras
#define THRESHOLD 150  //sum of squared differences must exceed this to detect a selection/choice
#define VIDEO_FRAME_DELAY 5  //delay between each frame in video (in milliseconds)
#define verbose  true  //mucho messages (or not)

static string currentScene = "Start";  //start of show

class Scene;  //(defined below)
//hash of scene name (string), and actual scene data.
static unordered_map< string, Scene* > sc;
//----------------------------------------------------------------------
//class for a single target. each target must be added to a particular scene.
// each target has the name of the next scene (when the particular target
// is selected).
class Target {
public:
    int x, y;      //location
    int r, g, b;   //rgb value
    string name;   //target name aka where the decision is 
    string video;  //video file to play if this target is chosen
    string nextSceneName;  //after selecting this target and after video
    // plays, go to this scene number

//ctor
    Target(int x, int y, int r, int g, int b, string name, string video,
        string nextSceneName)
    {
        this->x = x;  this->y = y;
        this->r = r;  this->g = g;  this->b = b;
        this->name = name;
        this->video = video;
        this->nextSceneName = nextSceneName;
    }

    //pretty print the contents of an instance of Target to an output stream
    friend ostream& operator<< (ostream& os, const Target& t) {
        os << "(x,y)=(" << t.x << "," << t.y << ") "
            << "(r,g,b)=(" << t.r << "," << t.g << "," << t.b << ") "
            << "name=\"" << t.name << "\" video=\"" << t.video << "\" nextSceneName=\""
            << t.nextSceneName << "\""
            << endl;
        return os;
    }

};

//class for a single scene. each scene may have multiple targets.
class Scene {
public:
    string sceneName;  //name of scene (any unique string)
    string fname;      //image displayed with target(s)
    vector< Target* >* targets;  //zero or more targets

    //ctor
    Scene(string sceneName, string fname, vector< Target* >* targets) {
        this->sceneName = sceneName;
        this->fname = fname;
        this->targets = targets;
    }

    ~Scene(void) {  //dtor
        // \todo after everything else is working 100%
    }

    //pretty print the contents of an instance of Target to an output stream
    friend ostream& operator<< (ostream& os, const Scene& s) {
        os << " sceneName=\"" << s.sceneName << "\" fname=\"" << s.fname << "\"";
        if (s.targets == nullptr || s.targets->empty())
            os << " targets=none";
        else {
            int i = 0;
            for (auto& target : *s.targets) {
                os << " targets[" << i << "]=" << *target;
                i++;
            }
        }
        os << endl;
        return os;
    }
};
//----------------------------------------------------------------------
//init scenes and corresponding targets.
static void init_scenes(void) {
    vector< Target* >* targetsTmp = nullptr;
    Scene* scTmp = nullptr;

    //init start scene (targets first, then scene)
    targetsTmp = new vector< Target* >();
    //note: to make selection easier for user, you can add more than one (x,y)
    // and (rgb) for a particular target.
    targetsTmp->push_back(new Target(391, 199, 220, 177, 207, "knobRight", "Scene1B.mp4", "Scene 1B"));
    targetsTmp->push_back(new Target(391, 194, 219, 243, 253, "knobRight", "Scene1B.mp4", "Scene 1B"));
    targetsTmp->push_back(new Target(391, 204, 202, 153, 172, "knobRight", "Scene1B.mp4", "Scene 1B"));
    targetsTmp->push_back(new Target(253, 199, 242, 188, 211, "knobLeft", "Scene1A.mp4", "Scene 1A"));
    targetsTmp->push_back(new Target(253, 194, 211, 199, 221, "knobLeft", "Scene1A.mp4", "Scene 1A"));
    targetsTmp->push_back(new Target(253, 203, 235, 191, 218, "knobLeft", "Scene1A.mp4", "Scene 1A"));
    scTmp = new Scene("Start", "Start_A_0001.jpg", targetsTmp);
    sc.insert({ scTmp->sceneName, scTmp });

    //init the remaining scenes

    //scene 1A
    targetsTmp = new vector< Target* >();
    //add targets for this scene \todo set colors
    targetsTmp->push_back(new Target(389, 69, 222, 134, 114, "chrys", "", "Scene 2B"));
    targetsTmp->push_back(new Target(389, 57, 210, 161, 121, "chrys", "", "Scene 2B"));
    targetsTmp->push_back(new Target(378, 71, 198, 132, 84, "chrys", "", "Scene 2B"));
    targetsTmp->push_back(new Target(405, 67, 184, 113, 81, "chrys", "", "Scene 2B"));
    targetsTmp->push_back(new Target(395, 80, 202, 161, 115, "chrys", "", "Scene 2B"));
    targetsTmp->push_back(new Target(307, 66, 228, 243, 255, "violet", "", "Scene 2A"));
    targetsTmp->push_back(new Target(303, 54, 61, 52, 253, "violet", "", "Scene 2A"));
    targetsTmp->push_back(new Target(295, 72, 57, 38, 241, "violet", "", "Scene 2A"));
    targetsTmp->push_back(new Target(322, 62, 65, 52, 253, "violet", "", "Scene 2A"));
    targetsTmp->push_back(new Target(316, 78, 56, 41, 244, "violet", "", "Scene 2A"));
    scTmp = new Scene("Scene 1A", "Scene 1A Flowers_0015.jpg", targetsTmp);
    sc.insert({ scTmp->sceneName, scTmp });

    //scene 1B
    targetsTmp = new vector< Target* >();
    //add targets for this scene \todo set colors
    targetsTmp->push_back(new Target(288, 181, 76, 190, 244, "winterKey", "", "Scene 2C"));
    targetsTmp->push_back(new Target(288, 176, 29, 112, 128, "winterKey", "", "Scene 2C"));
    targetsTmp->push_back(new Target(283, 182, 186, 247, 242, "winterKey", "", "Scene 2C"));
    targetsTmp->push_back(new Target(294, 182, 170, 249, 255, "winterKey", "", "Scene 2C"));
    targetsTmp->push_back(new Target(367, 182, 74, 217, 155, "springKey", "", "Scene 2D"));
    targetsTmp->push_back(new Target(368, 177, 55, 163, 129, "springKey", "", "Scene 2D"));
    targetsTmp->push_back(new Target(368, 187, 21, 140, 98, "springKey", "", "Scene 2D"));
    scTmp = new Scene("Scene 1B", "Scene 1B_0015.jpg", targetsTmp);
    sc.insert({ scTmp->sceneName, scTmp });

    //scene 2A
    targetsTmp = new vector< Target* >();
    // \todo add targets for this scene
    scTmp = new Scene("Scene 2A", "Scene 2A_0001.jpg", targetsTmp);
    sc.insert({ scTmp->sceneName, scTmp });

    //scene 2B
    targetsTmp = new vector< Target* >();
    // \todo add targets for this scene
    scTmp = new Scene("Scene 2B", "Scene 2B_0001.jpg", targetsTmp);
    sc.insert({ scTmp->sceneName, scTmp });

    //scene 2C
    targetsTmp = new vector< Target* >();
    // \todo add targets for this scene
    scTmp = new Scene("Scene 2C", "Scene 2C_0001.jpg", targetsTmp);
    sc.insert({ scTmp->sceneName, scTmp });

    //scene 2D
    targetsTmp = new vector< Target* >();
    // \todo add targets for this scene
    scTmp = new Scene("Scene 2D", "Scene 2D_0001.jpg", targetsTmp);
    sc.insert({ scTmp->sceneName, scTmp });
}
//----------------------------------------------------------------------
//list all cameras. since there may be more than one (built-in webcam
// plus one or more offboard usb cams. one only needs to run this once,
// set CAM appropriately, and recompile.
static void list_cameras(void) {
    bool ok[MAXCAM] = {};  //init all to false
    for (int i = 0; i < MAXCAM; i++) {
        VideoCapture camera = VideoCapture(i);
        if (camera.isOpened()) {
            Mat img;
            ok[i] = camera.read(img);
            if (ok[i]) {
                double w = camera.get(CAP_PROP_FRAME_WIDTH);
                double h = camera.get(CAP_PROP_FRAME_HEIGHT);
                cout << "camera " << i << " is working. w=" << w
                    << " h=" << h << endl;
            }
        }
        camera.release();  //release the camera each time
    }

    cout << endl << "camera summary:" << endl;
    for (int i = 0; i < MAXCAM; i++) {
        if (ok[i])    cout << " camera " << i << " is working." << endl;
    }
}
//----------------------------------------------------------------------
//simply print out the name of the running program and args (if any).
static void print_args(int argc, char* argv[]) {
    cout << "running";
    for (int i = 0; i < argc; i++)    cout << " " << argv[i];
    cout << endl;
}
//----------------------------------------------------------------------
//frame is image from camera. check frame for changes indicating a choice
// made by the user.
static Target* check_for_decision(Mat frame) {
    //check to make sure we remembered to do calibration step 2
    if (sc[currentScene]->targets == nullptr) {
        cout << "scene " << currentScene << " is not calibrated"
            << endl;
        return nullptr;
    }
    if (sc[currentScene]->targets->empty()) {
        cout << "scene " << currentScene << " does not have any targets"
            << endl;
        return nullptr;
    }

    //check all targets in this scene for selection
    for (auto t : *sc[currentScene]->targets) {
        if (verbose)    cout << "check_for_decision(): " << "    " << *t << endl;
        Vec3b camRGB = frame.at<Vec3b>(Point(t->x, t->y));  //target rgb value from camera
        if (verbose)    cout << "color at target from camera: " << camRGB << endl;
        //calc diffs between target and camera color values
        int dr = t->r - camRGB[0];
        int dg = t->g - camRGB[1];
        int db = t->b - camRGB[2];
        int dist = abs(dr + dg + db);  //sum of squared diffs
        if (verbose)    cout << "dist**2=" << dist << endl;
        //anything chosen?
        if (dist >= THRESHOLD)    return t;
    }
    return nullptr;
}
//----------------------------------------------------------------------
//play video from file
static void playVideo(string video) {
    video = HOME + video;  //prepend path to videos
    VideoCapture vid(video);
    // check if video file opened successfully
    if (!vid.isOpened()) {
        cerr << "error opening video stream or file: " << video << endl;
        return;
    }

    Mat vidFrame;  // frame from video
    for (; ; ) {  // play video frame-by-frame
        vid >> vidFrame;
        // if the frame is empty, break
        if (vidFrame.empty())    break;
        // display the resulting frame
        imshow("story", vidFrame);
        waitKey(VIDEO_FRAME_DELAY);
    }
    vid.release();
}
//----------------------------------------------------------------------
//display each scene, capture image of it from camera, and save to file.
// use this to determine targets in images.
static void test_scenes(VideoCapture cam) {
    cout << "test in progress" << endl;
    Mat frame;  //frame from the webcam
    //for each scene ...
    for (const auto& tmp : sc) {
        if (verbose)    cout << "key=" << tmp.first << " val=" << *tmp.second;
        string imageName = tmp.second->fname;
        imageName = HOME + imageName;  //prepend path to image(s)
        Mat img = imread(imageName);  //read image from file
        if (img.empty()) {
            cerr << "could not read the image: " << imageName << endl;
            return;
        }
        imshow("story", img);  //show the image
        pollKey();  //required to keep in sync!

        //grab frame from camera & display it
        if (verbose)    cout << "grabbing frames" << endl;
        bool ret = false;
        //MUST repeat a few times. Otherwise, it gets all out of sync (viz.,
        // what is displayed and what is captured are not the same).
        for (int i = 0; i < 10; i++) {
            //cout << i << endl;
            ret = cam.read(frame);  //grab frame from camera
            if (!ret)    break;
            imshow("camOut", frame);  //display camera frame
            //check keypress (if any)
            pollKey();  //required to keep in sync!
        }
        cout << "done grabbing frames" << endl;
        if (!ret) {
            cerr << "can't read frame from camera" << endl;
            break;
        }
        imshow("camOut", frame);  //display camera frame
        //check keypress (if any)
        pollKey();  //required to keep in sync!
        imageName = tmp.second->fname;
        imageName = HOME + string("test_") + imageName;  //prepend path to image(s) plus test name
        cout << imageName << endl;
        imwrite(imageName, frame);
    }
    cout << "test complete" << endl;
}
//----------------------------------------------------------------------
//once targets have been determined, use this to check them.
static void test_scenes2(VideoCapture cam) {
    cout << "test 2 in progress" << endl;
    Mat frame;  //frame from the webcam
    //for each scene ...
    for (const auto& tmp : sc) {
        if (verbose)    cout << "key=" << tmp.first << " val=" << *tmp.second;
        string imageName = tmp.second->fname;
        imageName = HOME + imageName;  //prepend path to image(s)
        Mat img = imread(imageName);  //read image from file
        if (img.empty()) {
            cerr << "could not read the image: " << imageName << endl;
            return;
        }
        imshow("story", img);  //show the image
        pollKey();  //required to keep in sync!

        //grab frame from camera & display it
        if (verbose)    cout << "grabbing frames" << endl;
        bool ret = false;
        //MUST repeat a few times. Otherwise, it gets all out of sync (viz.,
        // what is displayed and what is captured are not the same).
        for (int i = 0; i < 5; i++) {
            //cout << i << endl;
            ret = cam.read(frame);  //grab frame from camera
            if (!ret)    break;
            imshow("camOut", frame);  //display camera frame
            //check keypress (if any)
            pollKey();  //required to keep in sync!
        }
        cout << "done grabbing frames" << endl;
        if (!ret) {
            cerr << "can't read frame from camera" << endl;
            break;
        }
        imshow("camOut", frame);  //display camera frame
        //check keypress (if any)
        pollKey();  //required to keep in sync!

        //check target data structure values against data from camera.
        // for each scene ...
        for (auto t : *sc[currentScene]->targets) {
            Vec3b camRGB = frame.at<Vec3b>(Point(t->x, t->y));  //target rgb value from camera
            //calc diffs between target and camera color values
            int dr = t->r - camRGB[0];
            int dg = t->g - camRGB[1];
            int db = t->b - camRGB[2];
            int dist = dr * dr + dg * dg + db * db;  //sum of squared diffs
            cout << "dist**2=" << dist << endl;
            //anything chosen?
            if (dist >= THRESHOLD)    cout << "target threshold exceeded!" << endl;
        }
    }
    cout << "test 2 complete" << endl;
}
//----------------------------------------------------------------------
int main(int argc, char** argv) {
    /*if (verbose) {
        print_args(argc, argv);
        list_cameras();
        cout << "using camera: " << CAM << endl;
    }*/
    init_scenes();

    namedWindow("story", WINDOW_NORMAL);
    moveWindow("story", wDisplay1, 0);  //position display
    setWindowProperty("story", WND_PROP_FULLSCREEN, WINDOW_FULLSCREEN);

    //set up camera window (on primary/laptop display)
    VideoCapture cam = VideoCapture(CAM);
    //best to set every time (seems to remember, otherwise).
    //cam.set(CAP_PROP_AUTOFOCUS, 0);  //turn autofocus off
    cam.set(CAP_PROP_AUTO_EXPOSURE, 1);  //manual exposure mode
    cam.set(CAP_PROP_EXPOSURE, -7);  //set exposure
    cam.set(CAP_PROP_BUFFERSIZE, 2);  //does anything useful?

    namedWindow("camOut");
    setWindowProperty("camOut", WND_PROP_FULLSCREEN, WINDOW_FULLSCREEN);

    int image_counter = 0;  //no. of images from the webcam saved to the computer
    Mat frame;  //frame from the webcam

    bool started = false;  //wait for keypress to start processing decisions
    bool timeToQuit = false;
    while (!timeToQuit) {
        //cout << "NOT STARTED YET!" << endl;
        //display current scene
        String imageName = sc[currentScene]->fname;
        imageName = HOME + imageName;  //prepend path to image(s)
        Mat img = imread(imageName);  //read image from file
        if (img.empty()) {
            cout << "Could not read the image" << endl;
            return -1;
        }
        imshow("story", img);
        pollKey();  //required to keep in sync!
        cout << "display scene " << currentScene << endl;

        //grab frame from camera & display it
        bool returned = cam.read(frame);  //grab frame from camera
        if (!returned) {
            cerr << "can't read frame from camera" << endl;
            break;
        }
        imshow("camOut", frame);  //display camera frame
        //check keypress (if any)
        int ch = waitKey(200) & 0xFF;  //milliseconds
        char buff[1024];
        //handle keypress (if any)
        switch (ch) {
            case ' ':
                //space is pressed - save current frame from camera view
                sprintf_s(buff, "savedImage.%d.jpg", image_counter);
                imwrite(buff, frame);
                cout << "space pressed. image written to " << buff << endl;
                image_counter++;
                break;
            case 27:  //esc (escape) key
            case 'q':  case 'Q':  //quit
            case 'e':  case 'E':  //exit
            case 'x':  case 'X':  //ditto
                timeToQuit = true;
                break;
            case 's':  case 'S':
                started = true;
                break;
            case 't':  case 'T':
                test_scenes(cam);
                break;
            case '2':
                test_scenes2(cam);
                break;
            case 'v':  case 'V':
                playVideo("test.mp4");  //playVideo( "Scene1A.mp4" );
                break;
            case -1:
                break;  //ignore (idle; no key pressed)
            default:
                cout << "unrecognized key pressed: " << (char)ch << endl;
                break;
        }

        //test_scenes(cam);
        //timeToQuit = true;

        //wait until started (to begin checking for decisions)
        if (!started)    continue;
        //determine if something was selected
        Target* s = check_for_decision(frame);
        if (s != nullptr) {
            cout << "***" << *s << "***" << endl;
            //play video
            playVideo(s->video);  //prepend path to video(s)
            //get the next scene
            currentScene = s->nextSceneName;
            if (currentScene.empty())    cerr << "can't find next scene!";
            waitKey(500);
        }
    }  //end while

    return 0;
}
