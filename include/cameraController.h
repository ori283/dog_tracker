#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H


#include <opencv2/opencv.hpp>
#include <string>

class CameraController {
public:
    CameraController(const std::string& cameraUrl);
    ~CameraController();

    bool connect();
    bool isConnected() const;
    bool captureFrame(cv::Mat& frame);
    void disconnect();

    bool detectMotion(const cv::Mat& prevFrame, const cv::Mat& currFrame);
    bool initializeTracker(const cv::Mat& frame, const cv::Rect& boundingBox);
    bool updateTracker(const cv::Mat& frame, cv::Rect& boundingBox);

private:
    cv::VideoCapture m_capture;
    std::string m_cameraUrl;
    bool m_isConnected;
    cv::Ptr<cv::Tracker> m_tracker; // Object tracker
};

#endif // CAMERA_CONTROLLER_H
