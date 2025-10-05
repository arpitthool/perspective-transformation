#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>

using namespace std;
using namespace cv;
bool pointSelected[4] = {false, false, false, false};
Point points[4] = {Point(0, 0), Point(0, 0), Point(0, 0), Point(0, 0)};
Mat frame;

void showMenu(string windowName) {
    // show the menu
    // we use Mat::zeros to create a black image, with height and width as input
    // CV_8UC3 means 3 channels (RGB) and 8 bits per channel
    // we use CV_8UC3 because we want to display the text in color
    Mat menuImage = Mat::zeros(300, 300, CV_8UC3);
    vector<string> menuItems = {
        "Press ESC or Q to quit",
        "Press T to click and select 4",
        "points on the image to get the ",
        "perspective transformation"
    };  

    int menuItemsSize = menuItems.size();

    for (int i = 0; i < menuItemsSize; i++) {
        putText(menuImage, menuItems[i], Point(10, 30 + i * 30), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0), 1);
    }
    imshow(windowName, menuImage);
}

void mouseCallback(int event, int x, int y, int flags, void* userdata) {
    // Debug: Print all mouse events
    if (event == EVENT_MOUSEMOVE) {
        // Only print every 10th mouse move to avoid spam
        static int moveCount = 0;
        if (++moveCount % 10 == 0) {
            cout << "Mouse moved to (" << x << ", " << y << ")" << endl;
        }
    }
    
    if (event == EVENT_LBUTTONDOWN) {
        if (!pointSelected[0]) {
            points[0] = Point(x, y);
            pointSelected[0] = true;
            cout << "First point selected: " << points[0] << endl;
        } else if (!pointSelected[1]) {
            points[1] = Point(x, y);
            pointSelected[1] = true;
            cout << "Second point selected: " << points[1] << endl;
        } else if (!pointSelected[2]) {
            points[2] = Point(x, y);
            pointSelected[2] = true;
            cout << "Third point selected: " << points[2] << endl;
        } else if (!pointSelected[3]) {
            points[3] = Point(x, y);
            pointSelected[3] = true;
            cout << "Fourth point selected: " << points[3] << endl;
        }
    }
}



int main() {
    string windowName = "Perspective Transformation", menuWindowName = "Menu";

    string videoPath = "../resources/videos/LaneVideo.mp4";
    VideoCapture video(videoPath);
    if (!video.isOpened()) {
        cout << "Error: Could not open video file" << endl;
        return -1;
    }
    
    // Create the window first
    namedWindow(windowName, WINDOW_AUTOSIZE);
    namedWindow(menuWindowName, WINDOW_AUTOSIZE);
    
    // Then set the mouse callback
    setMouseCallback(windowName, mouseCallback, NULL);
    cout << "Mouse callback set for window: " << windowName << endl;
    
    while (true) {
        video >> frame;
        
        if (frame.empty()) break;
        for (int i = 0; i < 4; i++) {
            if (pointSelected[i]) {
                circle(frame, points[i], 5, Scalar(0, 0, 255), -1);
            }
        }
        

        imshow(windowName, frame);
        showMenu(menuWindowName);

        int key = waitKey(30);
        if (key == 27 || key == 'q' || key == 'Q') {
            break;
        } else if (key == 't' || key == 'T') {
            cout << "T key pressed. Click on 4 points in the video window." << endl;
            for (int i = 0; i < 4; i++) {
                pointSelected[i] = false;
            }
        }
    }
    
    return 0;
}