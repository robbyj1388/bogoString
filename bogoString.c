#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdint.h>

// State can't be all zeros. Using time(NULL) makes it different every run.
uint64_t s[2] = {0x12345678, 0xABCDEF01}; 

uint64_t xorshift128plus(void) {
  uint64_t x = s[0];                   // Take the first half of the state
  uint64_t const y = s[1];             // Take the second half of the state
  s[0] = y;                            // Move the second half to the first (rotation)
  
  x ^= x << 23;                        // Scramble x: shift left and XOR
  
  // Mix everything together to create the new second half
  s[1] = x ^ y ^ (x >> 18) ^ (y >> 5); 
  
  // The range logic:
  // 1. (s[1] + y) is the raw random number.
  // 2. % 95 keeps it between 0 and 94.
  // 3. + 32 shifts it to the range 32 to 126.
  return ((s[1] + y) % 95) + 32;
}

// Global variables
char* word = NULL;
long step = 0;

volatile sig_atomic_t interrupted = 0;
// If signal is passed (ex: ctrl c) interrupted = 1
void onSignalPrintStats(int sig) {
  (void)sig; // explicitly mark as unused
  interrupted = 1;
}

int main(int argc, char** argv){
  // Get terminal size
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

  printf ("lines %d\n", w.ws_row); // ____________________________USE THIS TO PRINT IN MIDDLE OF SCREEN
  printf ("columns %d\n", w.ws_col);
  // On signal (ex: ctrl c) call method onSignalPrintStats
  signal(SIGINT, onSignalPrintStats);
  // Seed random via current time 
  uint64_t t = (uint64_t)time(NULL);
  s[0] = t;
  s[1] = t ^ 0xABCDEF0123456789ULL; // Ensure they aren't the same

  word = malloc(sizeof(char) * 256); // 255 + 1 (\0) 
  if (word == NULL){
    printf("WORD MEMORY ALLOCATION FAILED\n");
    return 1;
  }

  // Check amount of args
  if(argc == 2){
    strncpy(word, argv[1], 255);
    word[255] = '\0';
  }else{
    // Get word from user
    printf("Enter word: ");
    if (scanf("%255s", word) != 1){
      printf("ERROR ON INPUT!\n");
      return 1;
    }
  }
  
  long wordLength = strlen(word);

  // While word is not a substring of generatedWord
  int i = 0;
  char randomCharacter;
 
  while(1){
    if (interrupted){
      printf("\n");
      printf("PROGRAM CANCELLED!\n");
      printf("Selected WORD: %s\n", word);
      printf("CURRENT STEP COUNT: %ld\n", step);
      free(word);
      word = NULL;
      exit(0);
    }
    // if i == wordLength word found
    if(i == wordLength){
      break;
    }
    randomCharacter = (char)xorshift128plus();
    step++;
    printf("%c", randomCharacter); 

    if (word[i] == randomCharacter) {
        // Char matches the random char 
        i++;
    } else {
        // If random char is start of word, i=1 
        if (randomCharacter == word[0]) {
            i = 1;
        } else {
            i = 0; 
        }
    }
  }
  
  // Print out stats of found word
  printf("\n");
  printf("Selected WORD: %s\n", word);
  printf("STEPS: %ld\n", step);
  printf("Seed: %ld\n", time(NULL));

  // Free up memory
  free(word);
  word = NULL;
  return 0;
}
