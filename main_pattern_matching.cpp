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

/**
 * @brief Pure function to find the best move between two board states.
 */
map<string, Point> findBestMove(const Mat& img_start, const Mat& img_end,
                               int threshold_val, int roi_percent,
                               int canny_low, int diff_thresh) {
    map<string, Point> result;
    result["from"] = Point(-1, -1);
    result["to"] = Point(-1, -1);
    result["occupied"] = Point(0, 0); // x=1 if capture, x=0 if empty move

    if (img_start.empty() || img_end.empty()) return result;

    // 1. Image Processing: Grayscale, Edges, and Binary Difference
    Mat gray1, gray2, diff_bin, edges1, edges2;
    cvtColor(img_start, gray1, COLOR_BGR2GRAY);
    cvtColor(img_end, gray2, COLOR_BGR2GRAY);

    Canny(gray1, edges1, canny_low, canny_low * 3);
    Canny(gray2, edges2, canny_low, canny_low * 3);

    absdiff(gray1, gray2, diff_bin);
    threshold(diff_bin, diff_bin, diff_thresh, 255, THRESH_BINARY);

    // 2. Grid Constants (Assuming 640x640 warped input)
    int sq = img_start.cols / 8;
    int sub = max(1, (sq * roi_percent) / 100);
    int off = (sq - sub) / 2;

    int max_diff_from = 0, max_diff_to = 0;

    // 3. Grid Analysis (8x8)
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            Rect roi(c * sq + off, r * sq + off, sub, sub);

            int p1 = countNonZero(edges1(roi));
            int p2 = countNonZero(edges2(roi));
            int diff_px = countNonZero(diff_bin(roi));

            // Identify START point (Piece removed)
            if (p1 > threshold_val && p2 < (p1 * 0.5)) {
                if (diff_px > max_diff_from) {
                    max_diff_from = diff_px;
                    result["from"] = Point(c, r);
                }
            }
            // Identify STOP point (Piece appeared or captured)
            else if (p2 > threshold_val && diff_px > (threshold_val / 2)) {
                if (diff_px > max_diff_to) {
                    max_diff_to = diff_px;
                    result["to"] = Point(c, r);
                    result["occupied"].x = (p1 > threshold_val) ? 1 : 0;
                }
            }
        }
    }
    return result;
}

void processAndDisplay() {
    if (warped1.empty() || warped2.empty()) return;

    // Call the core detection function
    map<string, Point> move = findBestMove(warped1, warped2, g_threshold_val, g_roi_percent, g_canny_low, g_diff_thresh);

    Mat display = warped2.clone();
    int sq = 80;

    // Draw START point (Blue)
    if (move["from"].x != -1) {
        Rect box(move["from"].x * sq, move["from"].y * sq, sq, sq);
        rectangle(display, box, Scalar(255, 0, 0), 4);
        putText(display, "FROM", box.tl() + Point(5, 25), 1, 0.9, Scalar(255, 0, 0), 2);
    }

    // Draw STOP point (Red Square or Circle if Occupied)
    if (move["to"].x != -1) {
        Point p = move["to"];
        Rect box(p.x * sq, p.y * sq, sq, sq);
        if (move["occupied"].x == 1) {
            circle(display, Point(p.x * sq + 40, p.y * sq + 40), 35, Scalar(0, 0, 255), 4);
            putText(display, "Occupied", box.tl() + Point(5, 25), 1, 0.8, Scalar(0, 0, 255), 2);
        } else {
            rectangle(display, box, Scalar(0, 0, 255), 4);
            putText(display, "STOP", box.tl() + Point(5, 25), 1, 0.9, Scalar(0, 0, 255), 2);
        }
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
