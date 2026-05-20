#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <map>

using namespace cv;
using namespace std;

// --- GLOBAL STORAGE FOR SETUP ---
Mat img1, img2, warped1, warped2;
vector<Point2f> corners;

// --- ADJUSTABLE PARAMETERS (Controlled by Trackbars) ---
int g_threshold_val = 500;
int g_roi_percent = 50;
int g_canny_low = 50;
int g_diff_thresh = 30;

// Helper: Convert board coordinates to chess notation
string coordToNotation(Point pt, const string& playerSide) {
    if (pt.x < 0 || pt.x > 7 || pt.y < 0 || pt.y > 7) return "";
    char file, rank;
    if (playerSide == "black") {
        file = 'a' + pt.x;
        rank = '8' - pt.y;
    } else { // black at bottom
        file = 'h' - pt.x;
        rank = '1' + pt.y;
    }
    return string(1, file) + string(1, rank);
}

/**
 * @brief Pure function to find the best move between two board states.
 * @param playerSide: "white" or "black" at the bottom
 * @param moveStr: output move string in chess notation (e.g., e2e4)
 */
map<string, Point> findBestMove(const Mat& img_start, const Mat& img_end,
                               int threshold_val, int roi_percent,
                               int canny_low, int diff_thresh,
                               vector<Point>* top3cells,
                               const string& playerSide = "white",
                               string* moveStr = nullptr) {
    // No longer identify start/stop/occupied, just collect top 3 cells
    map<string, Point> result;
    if (img_start.empty() || img_end.empty()) return result;

    Mat gray1, gray2, diff_bin, edges1, edges2;
    cvtColor(img_start, gray1, COLOR_BGR2GRAY);
    cvtColor(img_end, gray2, COLOR_BGR2GRAY);

    Canny(gray1, edges1, canny_low, canny_low * 3);
    Canny(gray2, edges2, canny_low, canny_low * 3);

    absdiff(gray1, gray2, diff_bin);
    threshold(diff_bin, diff_bin, diff_thresh, 255, THRESH_BINARY);
    imshow("diff_bin",diff_bin);
    int sq = img_start.cols / 8;
    int sub = max(1, (sq * roi_percent) / 100);
    int off = (sq - sub) / 2;

    vector<pair<int, Point>> topCells;
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            Rect roi(c * sq + off, r * sq + off, sub, sub);
            int diff_px = countNonZero(diff_bin(roi));
            topCells.push_back({diff_px, Point(c, r)});
        }
    }
    sort(topCells.begin(), topCells.end(), [](const pair<int, Point>& a, const pair<int, Point>& b){ return a.first > b.first; });
    if (top3cells) {
        top3cells->clear();
        for (int i = 0; i < 3 && i < (int)topCells.size(); ++i) {
            top3cells->push_back(topCells[i].second);
        }
    }
    // Output move string in chess notation for all 2-cell combinations from top 3 cells
    if (moveStr && top3cells) {
        *moveStr = "";
        int n = (int)top3cells->size();
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (i == j) continue;
                string fromStr = coordToNotation((*top3cells)[i], playerSide);
                string toStr = coordToNotation((*top3cells)[j], playerSide);
                if (!fromStr.empty() && !toStr.empty()) {
                    if (!moveStr->empty()) *moveStr += ",";
                    *moveStr += fromStr + toStr;
                }
            }
        }
    }
    return result;
}

void processAndDisplay(const string& playerSide = "white") {
    if (warped1.empty() || warped2.empty()) return;
    vector<Point> top3cells;
    string moveStr;
    findBestMove(warped1, warped2, g_threshold_val, g_roi_percent, g_canny_low, g_diff_thresh, &top3cells, playerSide, &moveStr);
    Mat display = warped2.clone();
    int sq = 80;
    // Draw rectangles on top 3 cells (Yellow)
    for (const auto& pt : top3cells) {
        Rect box(pt.x * sq, pt.y * sq, sq, sq);
        rectangle(display, box, Scalar(0, 255, 255), 2); // Yellow
    }
    // Show possible moves in chess notation
    if (!moveStr.empty()) {
        cout << "Possible moves: " << moveStr << endl;
        putText(display, moveStr, Point(10, 30), 1, 1.5, Scalar(0, 255, 0), 3);
    }
    imshow("Analysis", display);
}

void onMouse(int event, int x, int y, int flags, void* userdata) {
    if (event == EVENT_LBUTTONDOWN && corners.size() < 4) {
        corners.push_back(Point2f(x, y));
        circle(img1, Point(x, y), 5, Scalar(0, 255, 0), -1);
        imshow("Setup", img1);
        if (corners.size() == 4) {
            Mat trans = getPerspectiveTransform(corners, vector<Point2f>{{0,0},{640,0},{640,640},{0,640}});
            warpPerspective(img1, warped1, trans, Size(640, 640));
            warpPerspective(img2, warped2, trans, Size(640, 640));
            processAndDisplay();
        }
    }
}

int main(int argc, char** argv) {
    if (argc < 3) return -1;
    img1 = imread(argv[1]); img2 = imread(argv[2]);
    if (img1.empty() || img2.empty()) return -1;

    namedWindow("Setup"); namedWindow("Controls");
    createTrackbar("Min Pixels", "Controls", &g_threshold_val, 2000, [](int, void*){ processAndDisplay(); });
    createTrackbar("ROI %", "Controls", &g_roi_percent, 100, [](int, void*){ processAndDisplay(); });
    createTrackbar("Canny Low", "Controls", &g_canny_low, 255, [](int, void*){ processAndDisplay(); });
    createTrackbar("Diff Thresh", "Controls", &g_diff_thresh, 255, [](int, void*){ processAndDisplay(); });

    setMouseCallback("Setup", onMouse);
    imshow("Setup", img1);
    while(waitKey(1) != 27);
    return 0;
}
