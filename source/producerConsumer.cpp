#include "../include/producerConsumer.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <chrono>
#include <thread>

std::atomic<bool> motionDetected(false); // Flag to track if motion is detected

void producer(CircularBuffer& buffer, CameraController& camera) {
    std::cout << "Producer started" << std::endl;
    cv::Mat prevFrame;
    
    while (running) {
        cv::Mat frame;
        if (!camera.captureFrame(frame)) {
            std::cerr << "Failed to capture frame" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }
        
        if (frame.empty()) {
            std::cerr << "Received empty frame" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }
        
        if (prevFrame.empty()) {
            prevFrame = frame.clone();
            std::cout << "Initial frame captured" << std::endl;
            continue;
        }
        
        // Check for motion
        if (camera.detectMotion(prevFrame, frame)) {
            if (!motionDetected) {
                motionDetected = true;
                std::cout << "Motion detected, starting buffering" << std::endl;
            }
            buffer.add(frame);
        } else {
            if (motionDetected) {
                motionDetected = false;
                std::cout << "Motion stopped, stopping buffering" << std::endl;
            }
        }
        
        prevFrame = frame.clone();
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }
}

void consumer(CircularBuffer& buffer, const std::string& outputFile, int fps) {
    std::cout << "Consumer started" << std::endl;
    
    // Change output file extension to .avi
    std::string aviFile = outputFile;
    if (aviFile.substr(aviFile.length() - 4) == ".mp4") {
        aviFile = aviFile.substr(0, aviFile.length() - 4) + ".avi";
    }
    
    // Use MJPG codec which is widely supported
    int codec = cv::VideoWriter::fourcc('M', 'J', 'P', 'G');
    
    // Wait for first frame to get video dimensions
    cv::Mat firstFrame = buffer.get();
    while(firstFrame.empty());
    
    cv::VideoWriter writer(aviFile, codec, fps, firstFrame.size());
    if (!writer.isOpened()) {
        std::cerr << "Failed to open video writer" << std::endl;
        return;
    }

    while (running) {
        // If motion is detected, keep writing frames
        if (motionDetected) {
            cv::Mat frame = buffer.get();
            if (!frame.empty()) {
                writer.write(frame);
            }
        }
        // If motion is not detected and buffer is empty, wait
        else if (buffer.isEmpty()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        // If motion is not detected but buffer has frames, write remaining frames
        else {
            cv::Mat frame = buffer.get();
            if (!frame.empty()) {
                writer.write(frame);
            }
        }
    }

    writer.release();
}
