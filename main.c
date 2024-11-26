#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "header.h"
#include <unistd.h>

// Buffer to store API response
#define RESPONSE_BUFFER_SIZE 8192


int main() {
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

    // First interaction with ChatGPT
    
    
    
    
    
    
    
    
    char currentQuestion[1024];
    strncpy(currentQuestion, initialQuestion, sizeof(currentQuestion));

    for (int i = 0; i < iterations; i++) {
        printf("\nIteration %d:\n", i + 1);
        // printf("\n\n\ncurrent question start is : %s\n\n\n\n", currentQuestion); qlongl test

        // Ask ChatGPT
        free(responseBuffer);
        responseBuffer = NULL;
        askChatGPT(apiKey, currentQuestion, &responseBuffer);

        if (responseBuffer) {
            // Extract content from the JSON response
            extractResponseContent(responseBuffer, content);

            // Print ChatGPT's response
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
           
            snprintf(currentQuestion, sizeof(currentQuestion),
                     "Based on your previous answer: %s, can you pose and answer a follow-up question about one aspect of this.", content);
            
           // printf("\n\n\ncurrent question is : %s\n\n\n\n", currentQuestion); qlongl test
            
            // it would probably be better to make it ask a question, then feed the question to itself rather than have in same response. That way question is hidden, so it looks like rambling.
            
            /////////////////////////////////////////////////////////////////////////////////////
            /////////////////////////////////////////////////////////////////////////////////////
            /////////////////////////////////////////////////////////////////////////////////////
            /////////////////////////////////////////////////////////////////////////////////////
        // the issue is that it is not asking chatgpt this current question after first attempt. This is likely due to incorrect sending of question to chatgpt.
            
        } else {
            printf("Error: No response received.\n");
            break;
        }
    }

    free(responseBuffer);
    return 0;
}


//devin -> sanfran startup. repeated loops in agi
// embeding space
