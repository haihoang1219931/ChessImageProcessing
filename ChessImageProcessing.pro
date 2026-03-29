TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

## OPENCV
INCLUDEPATH += /usr/local/include/opencv4
DEPENDPATH += /usr/local/include/opencv4
unix:!macx: LIBS += -L/usr/local/lib/  \
    -lopencv_objdetect \
    -lopencv_shape -lopencv_stitching -lopencv_superres -lopencv_features2d -lopencv_calib3d \
    -lopencv_videostab \
    -lopencv_video \
    -lopencv_core \
    -lopencv_highgui \
    -lopencv_imgcodecs \
    -lopencv_imgproc \
    -lopencv_videoio

SOURCES += \
        main.cpp \
    main_check_move_piece.cpp \
    ChessImageProcessing.cpp \
    main_color_object.cpp \
    main_google_detect.cpp \
    main_pattern_matching.cpp \
    main_visual_hsv.cpp

HEADERS += \
    ChessImageProcessing.h
