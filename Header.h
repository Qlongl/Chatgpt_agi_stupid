//
//  Header.h
//  GPTAGI_C
//
//  Created by Quin Long Liang on 2024-11-20.
//

#ifndef Header_h
#define Header_h

// Struct to handle dynamic response buffer
typedef struct {
    char *data;
    size_t size;
} ResponseBuffer;

// Function to handle API response
size_t write_callback(void *ptr, size_t size, size_t nmemb, void *userdata) {
    size_t totalSize = size * nmemb;
    ResponseBuffer *response = (ResponseBuffer *)userdata;

    // Reallocate buffer to accommodate new data
    char *temp = realloc(response->data, response->size + totalSize + 1);
    if (temp == NULL) {
        fprintf(stderr, "Failed to allocate memory for response buffer.\n");
        return 0; // Returning 0 will signal an error to libcurl
    }
    response->data = temp;

    // Copy the new data into the buffer
    memcpy(response->data + response->size, ptr, totalSize);
    response->size += totalSize;

    // Null-terminate the string
    response->data[response->size] = '\0';

    return totalSize;
}

// Function to make a request to ChatGPT API
void askChatGPT(const char *apiKey, const char *message, char **responseBuffer) {
    CURL *curl;
    CURLcode res;

    const char *url = "https://api.openai.com/v1/chat/completions";

    // Prepare JSON payload
    char postFields[4096];
    snprintf(postFields, sizeof(postFields),
             "{\"model\":\"gpt-3.5-turbo\",\"messages\":[{\"role\":\"user\",\"content\":\"%s\"}],\"max_tokens\":200}",
             message);
    
    //printf("the snprinf function outputs %s\n", postFields); qlongl test

    curl = curl_easy_init();
    if (curl) {
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        char authHeader[256];
        snprintf(authHeader, sizeof(authHeader), "Authorization: Bearer %s", apiKey);
        headers = curl_slist_append(headers, authHeader);

        // Initialize response buffer
        ResponseBuffer response = {malloc(1), 0}; // Start with an empty buffer
        if (!response.data) {
            fprintf(stderr, "Failed to allocate memory for response.\n");
            curl_easy_cleanup(curl);
            return;
        }

        // Set CURL options
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postFields);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // Perform the request
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        } else {
            // Pass response data back to the caller
            *responseBuffer = strdup(response.data);
        }

        // Cleanup
        free(response.data);
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    } else {
        fprintf(stderr, "Failed to initialize CURL.\n");
    }
}

// Function to extract content from the API's JSON response

//incorrect inputs and outputs
void extractResponseContent(const char *big_str, char *result) {
    
    //printf("Json is: %s", big_str ); qlongl test
    
    const char * start_word = "\"content\": \"";
    const char *start = strstr(big_str, start_word);
    const char *end_word = "\",";
    
       // Check if the start_word exists in the string
       if (start == NULL) {
           printf("Start word not found.\n");
           return;
       }

       // Move the start pointer to the character right after the start_word
       start += strlen(start_word);

       // Find the ending point of the substring (first occurrence of end_word after start_word)
       const char *end = strstr(start, end_word);

       // Check if the end_word exists after the start_word
       if (end == NULL) {
           printf("End word not found.\n");
           return;
       }

       // Calculate the length of the substring
       size_t length = end - start;

       // Create a buffer for the extracted substring
       char extracted[length + 1];  // +1 for the null terminator

       // Copy the substring into the buffer
       strncpy(extracted, start, length);

       // Null-terminate the extracted string
       extracted[length] = '\0';

       // Print the extracted substring testing
       //printf("Extracted substring: %s\n", extracted);

        strcpy(result, extracted);
    
    }

#endif /* Header_h */
