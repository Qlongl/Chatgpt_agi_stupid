#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "header.h"
#include <unistd.h>


// Explanation of what this is: This is a super basic program that just asks chatgpt an initial question
// then has chatgpt respond, then has chatgpt ask itself a question based off of its own response, then answers it
// ect. Please don't set a gazillion iterations in the loop, it costs me about 5 cents per 100 responses, not a huge amount
// but if you want to do extensive testing please use your own api key

// Also, most of this was programmed with chatgpt, lol


// Buffer to store API response
#define RESPONSE_BUFFER_SIZE 8192


int main() {
    
    // random setup stuff
    
    const char *apiKey = "<>"; // Replace with your OpenAI API key
    char initialQuestion[1024];
    char *responseBuffer = NULL;
    char content[2048] = {0};
    int iterations;

    // Prompt the user for the initial question
    printf("Enter your initial question: ");
    fgets(initialQuestion, sizeof(initialQuestion), stdin);

    // Remove newline character from the input
    size_t len = strlen(initialQuestion);
    if (len > 0 && initialQuestion[len - 1] == '\n') {
        initialQuestion[len - 1] = '\0';
    }

    // Prompt the user for the number of iterations
    printf("How many iterations do you want? ");
    scanf("%d", &iterations);

    // Clear stdin buffer
    getchar();
    
    
   
    
    //Start of the logic
    char currentQuestion[1024]; // creating a string to hold your initial question and pass it to chatgpt
    strncpy(currentQuestion, initialQuestion, sizeof(currentQuestion));

    // starting a loop of chatgpt answering itself based off of your question
    for (int i = 0; i < iterations; i++) {
        printf("\nIteration %d:\n", i + 1);
        // printf("\n\n\ncurrent question start is : %s\n\n\n\n", currentQuestion); qlongl test

        // Ask ChatGPT
        free(responseBuffer);
        responseBuffer = NULL;
        askChatGPT(apiKey, currentQuestion, &responseBuffer);

        if (responseBuffer) {
            // Extract content from the JSON response, (Chatgpt api sends it's responses in these files that are
            // of json type, so this extract response content function just pulls out the response from the file.
            // The function is defined in the header file
            
            extractResponseContent(responseBuffer, content);

            // Print ChatGPT's response to itself onto the screen
            printf("ChatGPT's Response:\n%s\n", content);
            
            // sleep to make it more human "thinking" like
            //sleep(7);
            
            
            // Prepare ChatGPT's follow-up question
            /////////////////////////////////////////////////////////////////////////////////////
            /////////////////////////////////////////////////////////////////////////////////////
            /////////////////////////////////////////////////////////////////////////////////////
            
            // this is where the magical prompts come up & logic tests
            // still dont like how it routes through the api, relying on unknown window. Instead would prefer new response to be opened each and every time
            strncpy(currentQuestion, content, sizeof(currentQuestion));
           
            
            // this takes chatgpt's previous response, adds the framing "Based on your previous answer : ... pose a follow up\
            // thus chatgpt is answering it's own responses
            snprintf(currentQuestion, sizeof(currentQuestion),
                     "Based on your previous answer: %s, can you pose and answer a follow-up question about one aspect of this.", content);
            
            
            /////////////////////////////////////////////////////////////////////////////////////
            /////////////////////////////////////////////////////////////////////////////////////
            /////////////////////////////////////////////////////////////////////////////////////
            /////////////////////////////////////////////////////////////////////////////////////
            
        } else {
            printf("Error: No response received.\n");
            break;
        }
    }

    free(responseBuffer);
    return 0;
}



// devin -> sanfran startup. repeated loops in agi
// embeding space
//
