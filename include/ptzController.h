#pragma once
#include <string>
#include <curl/curl.h>

class PTZController {
public:
    PTZController(const std::string& ip, const std::string& username, const std::string& password);
    ~PTZController();

    // Single step movements
    bool moveUp();
    bool moveDown();
    bool moveLeft();
    bool moveRight();

    // Continuous movements
    bool startMovingUp();
    bool startMovingDown();
    bool startMovingLeft();
    bool startMovingRight();
    bool stopMovement();

private:
    bool sendCommand(const std::string& command);
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);

    std::string baseUrl;
    std::string username;
    std::string password;
    CURL* curl;
    struct curl_slist* headers;
}; 