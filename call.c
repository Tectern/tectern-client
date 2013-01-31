#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>

unsigned write_cb(char *in, unsigned size, unsigned nmemb, void* out)
{
    uint r;
    r = size * nmemb;
    printf("Response: \n%s\n",in);
    return(r);
}

int main(int argc, char** argv) {
    if(argc != 2) {
        fprintf(stderr, "Sorry, buddy.  Try again.\n\tUSAGE: %s URL\n",argv[0]);
        return -1;
    }

    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_ALL);

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, argv[1]);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "id=1");
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);

        res = curl_easy_perform(curl);
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));

        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    return 0;
}
