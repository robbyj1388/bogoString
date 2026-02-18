#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>

// Global variables
char* word = NULL;
long step = 0;

// Print current incomplete stats
void onSignalPrintStats(int sig){
  printf("\n");
  printf("PROGRAM CANCELLED!\n");
  printf("Selected WORD: %s\n", word);
  printf("CURRENT STEP COUNT: %ld\n", step);
  exit(sig);
}

int main(int argc, char** argv){
  // Get terminal size
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

  printf ("lines %d\n", w.ws_row); // ____________________________USE THIS TO PRINT IN MIDDLE OF SCREEN
  printf ("columns %d\n", w.ws_col);
  // Handle ctrl c quitting program
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
    strcpy(word, argv[1]); // set word to passed arg
  }else{
    // Get word from user
    printf("Enter word: ");
    if (scanf("%s", word) != 1){
      printf("ERROR ON INPUT!\n");
      return 1;
    }
  }
  
  long wordLength = strlen(word);

  // While word is not a substring of generatedWord
  int i = 0;
  char randomCharacter;
 
  while(1){
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
