//#include <opencv2/opencv.hpp>
//#include <iostream>
//#include <vector>

//using namespace cv;
//using namespace std;

//// Global variables for image data and mouse state
//Mat img, display_img, hsv_img;
//Point center;
//int radius = 0;
//bool drawing = false;

//// Function to find similar colors and draw rectangles
//void findAndBoxObjects(Scalar targetHSV) {
//    // 1. Define color tolerance (Hue +/- 10, Sat/Val +/- 40)
//    Scalar lower(max(0.0, targetHSV[0] - 10), 40, 40);
//    Scalar upper(min(179.0, targetHSV[0] + 10), 255, 255);
////    cv::Scalar lower(0, 10, 120);
////    cv::Scalar upper(180, 50, 255);

//    // 2. Create a binary mask of matching colors
//    Mat mask;
//    inRange(hsv_img, lower, upper, mask);

//    // 3. Clean up noise (removes tiny stray pixels)
//    Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5));
//    morphologyEx(mask, mask, MORPH_OPEN, kernel);

//    // 4. Find boundaries of the matching areas
//    vector<vector<Point>> contours;
//    findContours(mask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

//    // 5. Draw rectangles on a fresh copy of the image
//    Mat output = img.clone();
//    int count = 0;

//    for (const auto& contour : contours) {
//        // Only box objects larger than 100 pixels to avoid noise
//        if (contourArea(contour) > 100) {
//            Rect rect = boundingRect(contour);
//            rectangle(output, rect, Scalar(0, 0, 255), 2); // Red Box
//            count++;
//        }
//    }

//    // Display count on the result window
//    string text = "Found: " + to_string(count);
//    putText(output, text, Point(20, 40), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 255, 0), 2);

//    imshow("Detection Results", output);
//}

//// Mouse callback to handle circle drawing
//void onMouse(int event, int x, int y, int flags, void* userdata) {
//    if (event == EVENT_LBUTTONDOWN) {
//        drawing = true;
//        center = Point(x, y);
//    }
//    else if (event == EVENT_MOUSEMOVE) {
//        if (drawing) {
//            display_img = img.clone();
//            radius = (int)norm(center - Point(x, y));
//            circle(display_img, center, radius, Scalar(0, 255, 0), 2);
//        }
//    }
//    else if (event == EVENT_LBUTTONUP) {
//        drawing = false;
//        radius = (int)norm(center - Point(x, y));
//        if (radius < 2) radius = 2; // Prevent invalid small circles

//        // Create a circular mask to sample the average color
//        Mat mask = Mat::zeros(img.size(), CV_8UC1);
//        circle(mask, center, radius, 255, -1);

//        Scalar avgColor = mean(hsv_img, mask);
//        findAndBoxObjects(avgColor);
//    }
//}

//int main(int argc, char** argv) {
//    // Check if user provided an image path
//    if (argc != 2) {
//        cout << "Usage: " << argv[0] << " <path_to_image>" << endl;
//        return -1;
//    }

//    // Load the image from the argument path
//    img = imread(argv[1]);
//    if (img.empty()) {
//        cout << "Error: Could not open or find the image: " << argv[1] << endl;
//        return -1;
//    }

//    display_img = img.clone();
//    cvtColor(img, hsv_img, COLOR_BGR2HSV);

//    namedWindow("Select Object");
//    setMouseCallback("Select Object", onMouse, NULL);

//    cout << "Loaded: " << argv[1] << endl;
//    cout << "1. Click and drag to circle the color you want to match." << endl;
//    cout << "2. Results will appear in a separate window." << endl;
//    cout << "3. Press ESC to quit." << endl;

//    while (true) {
//        imshow("Select Object", display_img);
//        if (waitKey(30) == 27) break;
//    }

//    return 0;
//}
