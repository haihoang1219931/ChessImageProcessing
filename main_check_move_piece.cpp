#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "ChessImageProcessing.h"

static void onMouse( int event, int x, int y, int flags, void* param);
void presentMoves(cv::Mat src, ChessImageProcessing* chessController, std::vector<cv::Point> moves);
int main( int argc, char** argv )
{
    if (argc <3)
    {
        std::cout << "Need 2 pictures and 1 threshold" << std::endl;
        return -1;
    }
    int threshold = atoi(argv[3]);
    cv::Mat src1 = imread( cv::samples::findFile( argv[1] ), cv::IMREAD_COLOR ); // Load an image

    if (src1.empty())
    {
        std::cout << "Cannot read the image: " << argv[2] << std::endl;
        return -1;
    }

    cv::Mat src2 = imread( cv::samples::findFile( argv[2] ), cv::IMREAD_COLOR ); // Load an image

    if (src2.empty())
    {
        std::cout << "Cannot read the image: " << argv[2] << std::endl;
        return -1;
    }
    ChessImageProcessing chessController;
    std::vector<cv::Rect> moves;
    chessController.extractMove(src1, src2, threshold, moves);
    cv::Mat drawImage = src2.clone();
    for( size_t i = 0; i < moves.size(); i++ )
    {
        //        printf("rect[%d](%d,%d)\r\n",i,moves[i].width,moves[i].height);
        rectangle( drawImage, moves[i].tl(), moves[i].br(), cv::Scalar(0,0,255), 2 );
    }
    cv::imshow("Contours",drawImage);
    cv::setMouseCallback( "Contours", onMouse, &chessController );
    char keyPressed = 0;

    int numPoint = 0;
    do {
        keyPressed = cv::waitKey(30);
        switch (keyPressed){
        case 'c':
        {
            std::cout << "Clear points" << std::endl;
            chessController.corners().clear();
        }break;
        case 'p':
        {
            std::cout << "Perspective" << std::endl;
            std::vector<cv::Point> chessMoves;
            chessController.convertChessMove(moves, chessMoves);
            presentMoves(src2,&chessController,chessMoves);
        }break;
        }
        // Draw Chess Board
        if(numPoint != chessController.corners().size()) {
            numPoint = chessController.corners().size();
            cv::Mat drawPoints = drawImage.clone();
            polylines(drawPoints,std::vector<std::vector<cv::Point>>{chessController.corners()},true,
                      cv::Scalar(0, 255, 0), 3, cv::LINE_AA);
            cv::imshow("Contours",drawPoints);
        }
    } while (keyPressed != ' ');
    return 0;
}

void onMouse( int event, int x, int y, int flags, void* param)
{
    if( event != cv::EVENT_LBUTTONDOWN )
        return;
    std::cout << "x:" << x << ",y:" << y << std::endl;
    ((ChessImageProcessing*)param)->corners().push_back(cv::Point(x,y));
}

void presentMoves(cv::Mat src, ChessImageProcessing* chessController, std::vector<cv::Point> moves) {
    // Draw information
    cv::Mat transformMatrix = chessController->getTranformMatrix();
    cv::Mat chessBoard;

    int chessBoardBox = chessController->chessBoardBox();
    int chessBoardSize = chessController->chessBoardSize();
    cv::warpPerspective(src,chessBoard,transformMatrix,cv::Size(chessBoardSize,chessBoardSize));
    for(int row = 1; row < 8; row ++) {
        cv::line(chessBoard,
                 cv::Point(row*chessBoardBox,0),
                 cv::Point(row*chessBoardBox,chessBoardSize*chessBoardBox),cv::Scalar(255,0,0));
    }
    for(int col = 1; col < 8; col ++) {
        cv::line(chessBoard,
                 cv::Point(0,col*chessBoardBox),
                 cv::Point(chessBoardSize*chessBoardBox,col*chessBoardBox),cv::Scalar(255,0,0));
    }

    for(int i = 0; i< moves.size(); i++) {
        cv::rectangle(chessBoard,
                      cv::Rect(moves[i].x*chessBoardBox,
                               moves[i].y*chessBoardBox,
                               chessBoardBox,
                               chessBoardBox),
                      cv::Scalar(0,0,255),3);
    }
    cv::imshow("Perspective",chessBoard);
}
