#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>

using namespace std;
using namespace cv;
bool pointSelected[2] = {false, false};
// Point points[2] = {Point(0, 0), Point(0, 0)};
Point2f points[2] = {Point2f(0, 0), Point2f(0, 0)};
Mat frame;
bool isTransformed = false;
Point2f trapezoidPoints[4];
Point2f rectanglePoints[4];
Mat transformMatrix;

void showMenu(string windowName) {
    // show the menu
    // we use Mat::zeros to create a black image, with height and width as input
    // CV_8UC3 means 3 channels (RGB) and 8 bits per channel
    // we use CV_8UC3 because we want to display the text in color
    Mat menuImage = Mat::zeros(300, 300, CV_8UC3);
    vector<string> menuItems = {
        "> Press ESC or Q to quit",
        "> Press T to reset and click and select 2",
        "points (upper left and bottom",
        "right) to define a rectangle",
        "for perspective transformation",
        "> Press V to see the top view of the cropped frame"
    };  

    int menuItemsSize = menuItems.size();

    for (int i = 0; i < menuItemsSize; i++) {
        putText(menuImage, menuItems[i], Point(10, 30 + i * 30), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0), 1);
    }
    imshow(windowName, menuImage);
}

void createTrapezoidFromRectangle() {
    // Keep top left and bottom right points intact
    Point2f topLeft = points[0];
    Point2f bottomRight = points[1];
    
    trapezoidPoints[0] = topLeft;  // Top left (unchanged)
    trapezoidPoints[2] = bottomRight;  // Bottom right (unchanged)
    
    // Calculate rectangle dimensions
    int width = bottomRight.x - topLeft.x;
    int height = bottomRight.y - topLeft.y;
    
    // Create trapezoid by moving top right and bottom left points inward
    // Top right: move left by 20% of width
    trapezoidPoints[1] = Point2f(bottomRight.x - width * 0.2, topLeft.y);
    
    // Bottom left: move right by 20% of width
    trapezoidPoints[3] = Point2f(topLeft.x - width * 0.2, bottomRight.y);
}

void mouseCallback(int event, int x, int y, int flags, void* userdata) {
    // Debug: Print all mouse events
    if (event == EVENT_LBUTTONDOWN) {
        if (!pointSelected[0]) {
            points[0] = Point2f(x, y);
            pointSelected[0] = true;
            cout << "Upper left point selected: " << points[0] << endl;
        } else if (!pointSelected[1]) {
            points[1] = Point2f(x, y);
            pointSelected[1] = true;
            cout << "Bottom right point selected: " << points[1] << endl;
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
    
    int frameWidth = 640;
    int frameHeight = 480;
    // note the points need to be in the order of top left, top right, bottom right, bottom left same as the trapezoid points
    Point2f framePoints[4] = {Point2f(0, 0), Point2f(frameWidth, 0), Point2f(frameWidth, frameHeight), Point2f(0, frameHeight)};

    while (true) {
        video >> frame;
        // resize the frame to 640x480
        resize(frame, frame, Size(frameWidth, frameHeight));
        
        if (frame.empty()) break;
        // Draw the two selected points
        for (int i = 0; i < 2; i++) {
            if (pointSelected[i]) {
                circle(frame, points[i], 5, Scalar(0, 0, 255), -1);
            }
        }
        
        // Draw rectangle or trapezoid if both points are selected
        if (pointSelected[0] && pointSelected[1]) {
            if (isTransformed) {
                // Draw trapezoid
                for (int i = 0; i < 4; i++) {
                    line(frame, trapezoidPoints[i], trapezoidPoints[(i + 1) % 4], Scalar(0, 255, 0), 2);
                }
                // Draw corner points of trapezoid
                for (int i = 0; i < 4; i++) {
                    circle(frame, trapezoidPoints[i], 3, Scalar(255, 0, 0), -1);
                }
                
                // Apply perspective transformation with proper dimensions for the cropped frame
                Mat transformedFrame;
                warpPerspective(frame, transformedFrame, transformMatrix, Size(frameWidth, frameHeight));
                
                imshow("Top View", transformedFrame);
            } else {
                // Draw rectangle
                rectangle(frame, points[0], points[1], Scalar(0, 255, 0), 2);
            }
        }
        

        imshow(windowName, frame);
        showMenu(menuWindowName);

        int key = waitKey(30);
        if (key == 27 || key == 'q' || key == 'Q') {
            break;
        } else if (key == 't' || key == 'T') {
            cout << "T key pressed. Click on 2 points (upper left and bottom right) in the video window." << endl;
            for (int i = 0; i < 2; i++) {
                pointSelected[i] = false;
            }
            isTransformed = false;
        } else if (key == 'v' || key == 'V') {
            if (pointSelected[0] && pointSelected[1]) {
                cout << "V key pressed. Applying perspective transformation..." << endl;
                createTrapezoidFromRectangle();
                transformMatrix = getPerspectiveTransform(trapezoidPoints, framePoints);
                isTransformed = true;
            } else {
                cout << "Please select 2 points first before applying transformation." << endl;
            }
        }
    }
    
    return 0;
}