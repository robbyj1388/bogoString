#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/ioctl.h>
#include <unistd.h>

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
  srand(time(NULL));

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
    randomCharacter = (rand() % 126) + 32; // 31 < rand < 127 ascii character
    step++;
    printf("%c", randomCharacter);

    // Check if character i in word == randomCharacter
    if(word[i] == randomCharacter){
      i++;
    }else{ // if not word reset pointer 
      i=0;
    }
  }
  
  // Print out stats of found word
  printf("\n");
  printf("Selected WORD: %s\n", word);
  printf("STEPS: %ld\n", step);

  // Free up memory
  free(word);
  word = NULL;
  return 0;
}
