#include "../include/ptzController.h"
#include <iostream>

PTZController::PTZController(const std::string& ip, const std::string& username, const std::string& password)
    : baseUrl("http://" + ip + "/cgi-bin/hi3510/ptzctrl.cgi")
    , username(username)
    , password(password)
{
    curl = curl_easy_init();
    if (curl) {
        headers = curl_slist_append(nullptr, "Content-Type: application/x-www-form-urlencoded");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_USERNAME, username.c_str());
        curl_easy_setopt(curl, CURLOPT_PASSWORD, password.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    }
}

PTZController::~PTZController() {
    if (headers) {
        curl_slist_free_all(headers);
    }
    if (curl) {
        curl_easy_cleanup(curl);
    }
}

size_t PTZController::WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    return size * nmemb;  // We don't need to process the response
}

bool PTZController::sendCommand(const std::string& command) {
    if (!curl) {
        std::cerr << "CURL not initialized" << std::endl;
        return false;
    }

    std::string url = baseUrl + command;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "Failed to send command: " << curl_easy_strerror(res) << std::endl;
        return false;
    }

    return true;
}

bool PTZController::moveUp() {
    return sendCommand("?-step=1&-act=up");
}

bool PTZController::moveDown() {
    return sendCommand("?-step=1&-act=down");
}

bool PTZController::moveLeft() {
    return sendCommand("?-step=1&-act=left");
}

bool PTZController::moveRight() {
    return sendCommand("?-step=1&-act=right");
}

bool PTZController::startMovingUp() {
    return sendCommand("?-step=0&-act=up");
}

bool PTZController::startMovingDown() {
    return sendCommand("?-step=0&-act=down");
}

bool PTZController::startMovingLeft() {
    return sendCommand("?-step=0&-act=left");
}

bool PTZController::startMovingRight() {
    return sendCommand("?-step=0&-act=right");
}

bool PTZController::stopMovement() {
    return sendCommand("?-step=0&-act=stop");
} 