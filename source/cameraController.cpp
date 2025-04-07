#include "../include/cameraController.h"
#include <opencv2/tracking.hpp>
#include <iostream>
#include <thread>

CameraController::CameraController(const std::string& cameraUrl)
    : m_cameraUrl(cameraUrl), m_isConnected(false) {}

CameraController::~CameraController() {
    disconnect();
}

bool CameraController::connect() {
    // Set RTSP transport to TCP
    m_capture.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('H', '2', '6', '4'));
    m_capture.set(cv::CAP_PROP_BUFFERSIZE, 1);  // Minimize buffering
    
    // Add RTSP-specific parameters
    std::string rtspUrl = m_cameraUrl;
    if (rtspUrl.find("rtsp://") == 0) {
        // Add TCP parameters with increased buffer sizes
        rtspUrl += "?tcp&buffer_size=1048576&max_delay=500000"; // 1MB buffer, 500ms max delay
    }
    
    // Increase OpenCV's internal buffer size
    m_capture.set(cv::CAP_PROP_BUFFERSIZE, 10); // Increase internal OpenCV buffer
    
    m_capture.open(rtspUrl);
    if (!m_capture.isOpened()) {
        std::cerr << "Failed to connect to camera at " << m_cameraUrl << std::endl;
        return false;
    }
    
    // Set additional properties for better RTSP handling
    m_capture.set(cv::CAP_PROP_FPS, 30);
    
    m_isConnected = true;
    std::cout << "Connected to camera at " << m_cameraUrl << std::endl;
    return true;
}

bool CameraController::isConnected() const {
    return m_isConnected;
}

bool CameraController::captureFrame(cv::Mat& frame) {
    if (!m_isConnected) {
        std::cerr << "Camera is not connected" << std::endl;
        return false;
    }
    
    // Try to read frame with retry
    for (int i = 0; i < 3; ++i) {
        if (m_capture.read(frame)) {
            return true;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    // If we get here, reading failed
    std::cerr << "Failed to read frame after retries" << std::endl;
    return false;
}

void CameraController::disconnect() {
    if (m_isConnected) {
        m_capture.release();
        m_isConnected = false;
        std::cout << "Disconnected from camera" << std::endl;
    }
}

bool CameraController::detectMotion(const cv::Mat& prevFrame, const cv::Mat& currFrame) {
    // Convert to grayscale
    cv::Mat grayPrev, grayCurr;
    cv::cvtColor(prevFrame, grayPrev, cv::COLOR_BGR2GRAY);
    cv::cvtColor(currFrame, grayCurr, cv::COLOR_BGR2GRAY);

    // Apply moderate Gaussian blur to reduce noise
    cv::GaussianBlur(grayPrev, grayPrev, cv::Size(5, 5), 0);
    cv::GaussianBlur(grayCurr, grayCurr, cv::Size(5, 5), 0);

    // Calculate absolute difference
    cv::Mat diff;
    cv::absdiff(grayPrev, grayCurr, diff);

    // Apply lower threshold - decreased from 50 to 25
    cv::Mat thresh;
    cv::threshold(diff, thresh, 25, 255, cv::THRESH_BINARY);

    // Apply moderate morphological operations
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    cv::morphologyEx(thresh, thresh, cv::MORPH_OPEN, kernel);  // Remove noise
    cv::morphologyEx(thresh, thresh, cv::MORPH_CLOSE, kernel); // Fill holes

    // Find contours of motion regions
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(thresh, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // Calculate total area of motion and check for significant contours
    double totalMotionArea = 0;
    int significantContours = 0;
    
    for (const auto& contour : contours) {
        double area = cv::contourArea(contour);
        // Lower minimum contour size from 500 to 200 pixels
        if (area > 200) {
            totalMotionArea += area;
            significantContours++;
        }
    }

    // Return true if:
    // 1. Total motion area is significant (decreased from 5000 to 1000 pixels)
    // 2. There is at least 1 significant contour (decreased from 2 to 1)
    return (totalMotionArea > 1000) && (significantContours >= 1);
}

bool CameraController::initializeTracker(const cv::Mat& frame, const cv::Rect& boundingBox) {
    m_tracker = cv::TrackerCSRT::create(); // Use CSRT tracker
    m_tracker->init(frame, boundingBox);

    return m_tracker != nullptr;
}

bool CameraController::updateTracker(const cv::Mat& frame, cv::Rect& boundingBox) {
    return m_tracker->update(frame, boundingBox);
}
