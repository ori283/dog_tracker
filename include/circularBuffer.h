#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <opencv2/opencv.hpp>
#include <vector>
#include <mutex>
#include <condition_variable>

class CircularBuffer {
public:
    CircularBuffer(size_t size);
    
    void add(const cv::Mat& frame);
    
    cv::Mat get();
    
    bool isEmpty() const;
    
private:
    std::vector<cv::Mat> buffer;
    size_t maxSize;
    size_t start;
    size_t end;
    bool full;

    mutable std::mutex mutex;
    std::condition_variable condition;

};

#endif // CIRCULAR_BUFFER_H
