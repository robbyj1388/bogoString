#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

int main(int argc, char** argv){
  // Seed random via current time 
  srand(time(NULL));

  // Allocate user string
  char* word = malloc(sizeof(char) * 256); // 255 + 1 (\0) 
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
  long step = 0;
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

  printf("\n");

  printf("Selected WORD: %s\n", word);
  printf("STEPS: %ld\n", step);
  printf("%d", 'a');
  
  // Free up memory
  free(word);
  free(generatedWord);
  generatedWord = NULL;
  word = NULL;
  return 0;
}
