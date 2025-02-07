//#include "opencv2/imgproc.hpp"
//#include "opencv2/imgcodecs.hpp"
//#include "opencv2/highgui.hpp"
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/core/core.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/features2d/features2d.hpp>
//#include <opencv2/xfeatures2d/nonfree.hpp>
//#include "opencv2/features2d/features2d.hpp"
//#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/calib3d/calib3d.hpp"
//#include <iostream>
//#include <vector>
//using std::cout;


//int threshold_value = 1;
//int threshold_level = 1;
//int threshold_type = 3;
//int const max_value = 255;
//int const max_type = 4;
//int const max_binary_value = 255;
//int chessBoardRow = 8;
//int chessBoardBox = 60;

//int chessBoardSize = chessBoardBox * chessBoardRow;
//cv::Mat src, src_gray, dst;
//const char* window_name = "Threshold Demo";

//const char* trackbar_type = "Type: \n 0: Binary \n 1: Binary Inverted \n 2: Truncate \n 3: To Zero \n 4: To Zero Inverted";
//const char* trackbar_value = "Thresh";

//const char* trackbar_level = "Level";

//std::vector<cv::Point> listPoints;
//std::vector<cv::Point2f> chessBoardPoints = {cv::Point2f (0,0),
//                                             cv::Point2f (chessBoardSize,0),
//                                             cv::Point2f (chessBoardSize,chessBoardSize),
//                                             cv::Point2f (0,chessBoardSize)};
//static double angle( cv::Point pt1, cv::Point pt2, cv::Point pt0 )
//{
//    double dx1 = pt1.x - pt0.x;
//    double dy1 = pt1.y - pt0.y;
//    double dx2 = pt2.x - pt0.x;
//    double dy2 = pt2.y - pt0.y;
//    return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
//}
//// returns sequence of squares detected on the image.
//static void findSquares( const cv::Mat& image, std::vector<std::vector<cv::Point> >& squares , int thresh = 50, int N = 11)
//{
//    std::cout << "thresh: " << thresh << std::endl;
//    squares.clear();
//    cv::Mat pyr, timg, gray0(image.size(), CV_8U), gray;
//    // down-scale and upscale the image to filter out the noise
//    pyrDown(image, pyr, cv::Size(image.cols/2, image.rows/2));
//    pyrUp(pyr, timg, image.size());
//    std::vector<std::vector<cv::Point> > contours;
//    // find squares in every color plane of the image
//    for( int c = 0; c < 1; c++ )
//    {
//        int ch[] = {c, 0};
//        //        mixChannels(&timg, 1, &gray0, 1, ch, 1);
//        gray0 = image.clone();
//        // try several threshold levels
//        for( int l = 0; l < N; l++ )
//        {
//            // hack: use Canny instead of zero threshold level.
//            // Canny helps to catch squares with gradient shading
//            if( l == 0 )
//            {
//                // apply Canny. Take the upper threshold from slider
//                // and set the lower to 0 (which forces edges merging)
//                Canny(gray0, gray, 0, thresh, 5);
//                // dilate canny output to remove potential
//                // holes between edge segments
//                dilate(gray, gray, cv::Mat());
//                dilate(gray, gray, cv::Mat());
//                dilate(gray, gray, cv::Mat());
//            }
//            else
//            {
//                // apply threshold if l!=0:
//                //     tgray(x,y) = gray(x,y) < (l+1)*255/N ? 255 : 0
//                gray = gray0 >= (l+1)*255/N;
//            }
//            // find contours and store them all as a list
//            cv::findContours(gray, contours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);
//            std::vector<cv::Point> approx;
//            // test each contour
//            for( size_t i = 0; i < contours.size(); i++ )
//            {
//                // approximate contour with accuracy proportional
//                // to the contour perimeter
//                approxPolyDP(contours[i], approx, arcLength(contours[i], true)*0.02, true);
//                // square contours should have 4 vertices after approximation
//                // relatively large area (to filter out noisy contours)
//                // and be convex.
//                // Note: absolute value of an area is used because
//                // area may be positive or negative - in accordance with the
//                // contour orientation
//                if( approx.size() == 4 &&
//                        fabs(contourArea(approx)) > 1000 &&
//                        isContourConvex(approx) )
//                {
//                    double maxCosine = 0;
//                    for( int j = 2; j < 5; j++ )
//                    {
//                        // find the maximum cosine of the angle between joint edges
//                        double cosine = fabs(angle(approx[j%4], approx[j-2], approx[j-1]));
//                        maxCosine = MAX(maxCosine, cosine);
//                    }
//                    // if cosines of all angles are small
//                    // (all angles are ~90 degree) then write quandrange
//                    // vertices to resultant sequence
//                    if( maxCosine < 0.3 )
//                        squares.push_back(approx);
//                }
//            }
//        }
//    }
//}

//static void Threshold_Demo( int, void* )
//{
//    /* 0: Binary
//     1: Binary Inverted
//     2: Threshold Truncated
//     3: Threshold to Zero
//     4: Threshold to Zero Inverted
//    */
//    threshold( src_gray, dst, threshold_value, max_binary_value, threshold_type);
//    cv::Mat output = src_gray.clone();

//    cv::Mat edges = dst.clone();
//    Canny(dst, edges, 0, threshold_value, 5);
//    //    cv::dilate(edges, edges, cv::Mat());
//    //    cv::dilate(edges, edges, cv::Mat());
//    //    cv::dilate(edges, edges, cv::Mat());

//    //    cv::erode(edges, edges, cv::Mat());
//    //    cv::erode(edges, edges, cv::Mat());
//    //    cv::erode(edges, edges, cv::Mat());

//    std::vector<cv::Vec4i> lines;
//    cv::HoughLinesP(edges, lines, 1, CV_PI/180, threshold_value,10, 250);
//    std::cout << "Line number: " << lines.size() << std::endl;
//    for( size_t i = 0; i < lines.size(); i++ )
//    {
//        cv::Vec4i l = lines[i];

//        cv::line( output, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0,0,255), 3);

//    }

//    cv::Mat outputContour = src_gray.clone();
//    std::vector<std::vector<cv::Point> > squares;
//    findSquares(src_gray, squares, threshold_value, threshold_level);
//    polylines(outputContour, squares, true, cv::Scalar(0, 255, 0), 3, cv::LINE_AA);
//    cv::imshow("Edges", edges);
//    cv::imshow("Output", output);
//    cv::imshow("outputContour", outputContour);
//    cv::Mat drawPoints = src.clone();
//    polylines(drawPoints,std::vector<std::vector<cv::Point>>{listPoints},true,cv::Scalar(0, 255, 0), 3, cv::LINE_AA);
//    imshow( window_name, drawPoints );
//}

//static void onMouse2( int event, int x, int y, int, void* )
//{
//    if( event != cv::EVENT_LBUTTONDOWN )
//        return;
//    std::cout << "x:" << x << ",y:" << y << std::endl;
//    listPoints.push_back(cv::Point(x,y));
//}
//int main1( int argc, char** argv )
//{
//    cv::String imageName("stuff.jpg"); // by default
//    if (argc > 1)
//    {
//        imageName = argv[1];
//    }
//    src = imread( cv::samples::findFile( imageName ), cv::IMREAD_COLOR ); // Load an image

//    if (src.empty())
//    {
//        cout << "Cannot read the image: " << imageName << std::endl;
//        return -1;
//    }

//    cvtColor( src, src_gray, cv::COLOR_BGR2GRAY ); // Convert the image to Gray

//    namedWindow( window_name, cv::WINDOW_AUTOSIZE ); // Create a window to display results
//    cv::setMouseCallback( window_name, onMouse2, 0 );
//    cv::createTrackbar( trackbar_type,
//                        window_name, &threshold_type,
//                        max_type, Threshold_Demo ); // Create a Trackbar to choose type of Threshold

//    cv::createTrackbar( trackbar_value,
//                        window_name, &threshold_value,
//                        max_value, Threshold_Demo ); // Create a Trackbar to choose Threshold value

//    cv::createTrackbar( trackbar_level,
//                        window_name, &threshold_level,
//                        max_value, Threshold_Demo ); // Create a Trackbar to choose Threshold value

////    Threshold_Demo( 0, 0 ); // Call the function to initialize
//    char keyPressed = 0;
//    do {
//        keyPressed = cv::waitKey(30);
////        std::cout << "Key:" << keyPressed << std::endl;
//        switch (keyPressed) {
//        case 'p':
//        {
//            std::cout << "Perspective" << std::endl;
//            if(listPoints.size() == 4) {
//                cv::Point2f source_points[4];
//                cv::Point2f dest_points[4];
//                for(int i=0; i< listPoints.size(); i++) {
//                    source_points[i].x = (float)(listPoints[i].x);
//                    source_points[i].y = (float)(listPoints[i].y);
//                    dest_points[i].x = (float)(chessBoardPoints[i].x);
//                    dest_points[i].y = (float)(chessBoardPoints[i].y);
//                }
//                cv::Mat transformMatrix = cv::getPerspectiveTransform(source_points, dest_points);
//                std::cout << "transformMatrix :" << transformMatrix << std::endl;
//                cv::Mat warpImage;
//                cv::warpPerspective(src, warpImage,transformMatrix,cv::Size(480,480));
//                for(int row = 1; row < 8; row ++) {
//                    cv::line(warpImage,
//                             cv::Point(row*chessBoardBox,0),
//                             cv::Point(row*chessBoardBox,chessBoardSize*chessBoardBox),cv::Scalar(255,0,0));
//                }
//                for(int col = 1; col < 8; col ++) {
//                    cv::line(warpImage,
//                             cv::Point(0,col*chessBoardBox),
//                             cv::Point(chessBoardSize*chessBoardBox,col*chessBoardBox),cv::Scalar(255,0,0));
//                }
//                cv::imshow("warpImage",warpImage);
//            }
//        }break;
//        case 'c':
//        {
//            std::cout << "Clear points" << std::endl;
//            listPoints.clear();
//        }break;
//        default:
//            Threshold_Demo( 0, 0 );
//        }
//    } while (keyPressed != ' ');

//    return 0;
//}
