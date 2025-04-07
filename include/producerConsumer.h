#ifndef PRODUCER_CONSUMER_H
#define PRODUCER_CONSUMER_H

#include "CircularBuffer.h"
#include "CameraController.h"
#include <atomic>
#include <string>

// Declare the global variable
extern std::atomic<bool> running;

// Function prototypes for producer and consumer
void producer(CircularBuffer& buffer, CameraController& camera);
void consumer(CircularBuffer& buffer, const std::string& outputFile, int fps);

#endif // PRODUCER_CONSUMER_H
