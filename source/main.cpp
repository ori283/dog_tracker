#include "../include/cameraController.h"
#include "../include/circularBuffer.h"
#include "../include/producerConsumer.h"
#include "../include/configLoader.h"
#include <opencv2/opencv.hpp>
#include <thread>
#include <atomic>
#include <iostream>
#include <chrono>

// Define the global variable
std::atomic<bool> running(true);

int main() {
    try {
        // Load configuration
        ConfigLoader config;
        if (!config.loadConfig()) {
            std::cerr << "Failed to load configuration" << std::endl;
            return 1;
        }

        // Initialize camera with URL from config
        CameraController camera(config.getCameraUrl());
        if (!camera.connect()) {
            std::cerr << "Failed to connect to camera" << std::endl;
            return 1;
        }

        // Initialize circular buffer
        CircularBuffer circularBuffer(100);

        // Create and start threads
        std::thread producerThread(producer, std::ref(circularBuffer), std::ref(camera));
        std::thread consumerThread(consumer, std::ref(circularBuffer), "output.avi", 30);

        // Wait for user input to stop recording
        std::cout << "Press 'q' to quit..." << std::endl;
        char input;
        while (running) {
            if (std::cin.get(input) && (input == 'q' || input == 'Q')) {
                running = false;
                break;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        
        // Signal threads to stop
        running = false;

        // Wait for threads to finish
        if (producerThread.joinable()) {
            producerThread.join();
        }
        if (consumerThread.joinable()) {
            consumerThread.join();
        }

        std::cout << "Program terminated successfully" << std::endl;
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        running = false;
        return 1;
    }
}
