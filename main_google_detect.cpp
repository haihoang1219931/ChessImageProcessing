//#include <opencv2/opencv.hpp>
//#include <vector>
//#include <algorithm>

//std::vector<cv::Rect> getChessGrid(cv::Mat board, int squareSize) {
//    std::vector<cv::Rect> squares;
//    for (int y = 0; y < 8; y++) {
//        for (int x = 0; x < 8; x++) {
//            squares.push_back(cv::Rect(x * squareSize, y * squareSize, squareSize, squareSize));
//        }
//    }
//    return squares;
//}

//int main(int argc, char **argv) {
//    if (argc < 3) return -1;
//    cv::Mat prevFrame = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);
//    cv::Mat currFrame = cv::imread(argv[2], cv::IMREAD_GRAYSCALE);
//    // Load a color version for drawing
//    cv::Mat displayImg = cv::imread(argv[2]);

//    if (prevFrame.empty() || currFrame.empty()) return -1;

//    cv::Mat diff;
//    cv::absdiff(prevFrame, currFrame, diff);
//    cv::threshold(diff, diff, 30, 255, cv::THRESH_BINARY);

//    int squareSize = prevFrame.cols / 8;
//    auto squares = getChessGrid(prevFrame, squareSize);

//    // Vector to store {change_percentage, square_index}
//    std::vector<std::pair<double, int>> detections;

//    for (int i = 0; i < 64; i++) {
//        if(squares[i].x + squares[i].width > diff.cols ||
//           squares[i].y + squares[i].height > diff.rows) continue;

//        cv::Mat roi = diff(squares[i]);
//        double ratio = (double)cv::countNonZero(roi) / squares[i].area();

//        if (ratio > 0.15) {
//            detections.push_back({ratio, i});
//        }
//    }

//    // Sort by most change to identify Start and Stop
//    std::sort(detections.rbegin(), detections.rend());

//    // Draw top 2 detections (Start and Stop)
//    for (int i = 0; i < std::min((int)detections.size(), 2); i++) {
//        int idx = detections[i].second;
//        cv::Rect r = squares[idx];

//        // Red for "From" (Start), Green for "To" (Stop)
//        // Usually, the higher change is the destination square in simple diffs
//        cv::Scalar color = (i == 0) ? cv::Scalar(0, 255, 0) : cv::Scalar(0, 0, 255);
//        std::string label = (i == 0) ? "STOP" : "START";

//        cv::rectangle(displayImg, r, color, 3);
//        cv::putText(displayImg, label, cv::Point(r.x + 5, r.y + 20),
//                    cv::FONT_HERSHEY_SIMPLEX, 0.5, color, 2);

//        int row = 8 - (idx / 8);
//        char col = 'a' + (idx % 8);
//        std::cout << label << " detected at: " << col << row << std::endl;
//    }

//    cv::imshow("Move Detection", displayImg);
//    cv::waitKey(0);
//    return 0;
//}
