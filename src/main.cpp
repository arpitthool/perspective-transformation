#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>

using namespace std;
using namespace cv;

void showMenu(int noOfFaces, string windowName) {
    // show the menu
    // we use Mat::zeros to create a black image, with height and width as input
    // CV_8UC3 means 3 channels (RGB) and 8 bits per channel
    // we use CV_8UC3 because we want to display the text in color
    Mat menuImage = Mat::zeros(300, 300, CV_8UC3);
    vector<string> menuItems = {
        "*** Menu ***",
        "Press ESC or Q to quit"
    };  

    int menuItemsSize = menuItems.size();

    for (int i = 0; i < menuItemsSize; i++) {
        putText(menuImage, menuItems[i], Point(10, 30 + i * 30), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0), 1);
    }
    imshow(windowName, menuImage);
}

int main() {
    cout << "Hello World" << endl;
    return 0;
}