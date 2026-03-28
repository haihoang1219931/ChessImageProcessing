//#include <opencv2/opencv.hpp>

//int main(int argc, char** argv) {
//    cv::VideoCapture cap(0);
//    cv::Mat src;
//    int keywait = 0;
//    do {
//        cap.read(src);
//        // 1. Load the original image
//    //    cv::Mat src = cv::imread(argv[1]);
//    //    if (src.empty()) return -1;

//        // 2. Pre-processing: Convert to Grayscale and Blur
//        cv::Mat gray, blurred;
//        cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
//        cv::GaussianBlur(gray, blurred, cv::Size(3, 3), 0);

//        // 3. Extract Edges using Canny
//        // threshold1 (low): 50, threshold2 (high): 150
//        cv::Mat edges;
//        cv::Canny(blurred, edges, 120, 150);

//        // 4. Display results
//        cv::imshow("Original", src);
//        cv::imshow("Edge Image", edges);
//        keywait = cv::waitKey(30);
//    } while (keywait != ' ');


//    return 0;
//}
