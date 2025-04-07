#include <curl/curl.h>
#include <stdio.h>

int main() {
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if(curl) {
        printf("CURL initialized successfully!\n");
        
        // Test a simple HTTP request
        curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");
        res = curl_easy_perform(curl);
        
        if(res != CURLE_OK) {
            printf("curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        
        curl_easy_cleanup(curl);
    } else {
        printf("Failed to initialize CURL!\n");
    }
    
    return 0;
} 