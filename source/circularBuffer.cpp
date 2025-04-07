#include "../include/circularBuffer.h"

CircularBuffer::CircularBuffer(size_t size)
    : maxSize(size), start(0), end(0), full(false) {
        buffer.resize(size);
}

void CircularBuffer::add(const cv::Mat& frame) {
    std::unique_lock<std::mutex> lock(mutex);
    
    // If buffer is full, wait for consumer to read some frames
    while (full) {
        condition.wait(lock);
    }
    
    buffer[end] = frame.clone();
    end = (end + 1) % maxSize;
    full = end == start;
    
    condition.notify_one();
}

cv::Mat CircularBuffer::get() {
    std::unique_lock<std::mutex> lock(mutex);
    
    // Wait until there's data to read
    while (!full && start == end) {
        condition.wait(lock);
    }
    
    // Get frame at current read position
    cv::Mat frame = buffer[start].clone();
    
    // Move read position forward
    start = (start + 1) % maxSize;
    full = false;
    
    // Notify producer that space is available
    condition.notify_one();
    
    return frame;
}

bool CircularBuffer::isEmpty() const {
    std::unique_lock<std::mutex> lock(mutex);
    return !full && start == end;
}
