//#include <opencv2/opencv.hpp>
//#include <opencv2/viz.hpp>

//using namespace cv;

//int main() {
//    // 1. Load image and convert to HSV
//    Mat img = imread("input.jpg");
//    if(img.empty()) return -1;

//    Mat hsv;
//    cvtColor(img, hsv, COLOR_BGR2HSV); // OpenCV HSV: H[0,179], S[0,255], V[0,255]

//    // 2. Prepare 3D points and colors
//    std::vector<Vec3f> points;
//    std::vector<Vec3b> colors;

//    for (int y = 0; y < hsv.rows; y += 2) { // Step to reduce density
//        for (int x = 0; x < hsv.cols; x += 2) {
//            Vec3b p = hsv.at<Vec3b>(y, x);
//            float h = p[0] * 2.0f; // Scale H to [0, 360]
//            float s = p[1] / 255.0f;
//            float v = p[2] / 255.0f;

//            // Convert Cylindrical (H,S,V) to Cartesian (X,Y,Z)
//            float angle = h * (CV_PI / 180.0f);
//            float nx = s * cos(angle);
//            float ny = s * sin(angle);
//            float nz = v;

//            points.push_back(Vec3f(nx, ny, nz));
//            colors.push_back(img.at<Vec3b>(y, x)); // Store original BGR for display
//        }
//    }

//    // 3. Create Viz3d window and show point cloud
//    viz::Viz3d window("3D HSV Color Space");
//    window.showWidget("Coordinate System", viz::WCoordinateSystem(0.5));

//    // Create point cloud widget
//    viz::WCloud cloud(points, colors);
//    window.showWidget("HSV_Cloud", cloud);

//    window.spin(); // Start event loop
//    return 0;
//}
