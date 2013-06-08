/*
 ============================================================================
 APPROACH
 ============================================================================
 1. Generate a random key, called the 'parent',  decipher the ciphertext
 using this key. Rate the fitness of the deciphered text, store the result.
 2. Change the key slightly (swap two characters in the key at random),
 measure the fitness of the deciphered text using the new key.
 3. If the fitness is higher with the modified key, store it as our current
 key and discard the parent.
 4. Go back to 2, unless no improvement in fitness occurred in the last 1000
 iterations.
 ============================================================================
 SOURCE
 ============================================================================
 http://practicalcryptography.com/cryptanalysis/stochastic-searching/cryptanalysis-playfair/
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define KEY_SIZE 6

void simulatedAnnealing(char* cipherText);
char* generateRandomKey(int numberOfCharacters);
char generateRandomCharacter();
int firstUse(char aChar, char* inString, int stringSize);
char* decipher(char* cipherText, char* key);
float rateFitness(char* text);
char* slightlyModifiedKey(char* originalKey, int keySize, int numberOfSwapOperations);
float checkForLetters(char* text);
char charToLowerCase(char c);
void stringToLowerCase(char* text, int sizeofText);
float punctuateLetterFrequency(int* hashOfLetterFrequency);

int main(int argc, char* argv[]) {

  char* cipherText;

  cipherText = calloc(1, 255);
  strcpy(cipherText, "ENTREINAFEIRADAFRUTA");
  stringToLowerCase(cipherText, strlen(cipherText));
  simulatedAnnealing(cipherText);

  free(cipherText);

}

void simulatedAnnealing(char* cipherText) {
  char* firstKey, *secondKey, *firstDecipheredText, *secondDecipheredText;
  float firstRate, secondRate;

  srand(time(NULL ));

  // ======================================================
  firstKey = generateRandomKey(KEY_SIZE);
  firstDecipheredText = decipher(cipherText, firstKey);
  firstRate = rateFitness(firstDecipheredText);
  // ======================================================

  // ======================================================
  secondKey = slightlyModifiedKey(firstKey, KEY_SIZE, 1);
  secondDecipheredText = decipher(cipherText, firstKey);
  secondRate = rateFitness(firstDecipheredText);
  // ======================================================

  puts(firstDecipheredText);

  puts(firstKey);
  puts(secondKey);

  free(firstDecipheredText);
  free(firstKey);
  free(secondKey);
  free(secondDecipheredText);

}

char* generateRandomKey(int numberOfCharacters) {
  char* output = calloc(1, numberOfCharacters);
  int i;
  char aChar;

  for (i = 0; i < numberOfCharacters; i++) {
    aChar = generateRandomCharacter();
    while (!firstUse(aChar, output, i)) {
      aChar = generateRandomCharacter();
    }
    output[i] = aChar;
  }
  return output;
}

char generateRandomCharacter() {
  int aRandomNumber = rand() % 26;
  char aChar = 97 + aRandomNumber;

  return aChar;
}

int firstUse(char aChar, char* inString, int stringSize) {
  int i;

  for (i = 0; i < stringSize; i++) {
    if (inString[i] == aChar) {
      return FALSE;
    }
  }
  return TRUE;
}

char* decipher(char* cipherText, char* key) {
  char* result = calloc(1, 255);
  strcpy(result, "something really cool will happen herec in <decipher>");
  return result;
}

float rateFitness(char* text) {
  // So we need to verify the quadgrams
  // but it's really hard to find a solution in portuguese
  // let's build a new rate fitness function, totally from scratch

  float totalScore = 0;

  totalScore = checkForLetters(text);

  return totalScore;

}

char* slightlyModifiedKey(char* originalKey, int keySize, int numberOfSwapOperations) {
  // TODO mudar para imperativo
  int i;
  int changeLetterFrom = 0;
  int changeLetterTo = 0;
  char* aCopy = calloc(1, keySize);
  char swap;

  strcpy(aCopy, originalKey);

  for (i = 0; i < numberOfSwapOperations; i++) {
    while (changeLetterFrom == changeLetterTo) {
      changeLetterFrom = rand() % keySize;
      changeLetterTo = rand() % keySize;
    }
    swap = aCopy[changeLetterFrom];
    aCopy[changeLetterFrom] = aCopy[changeLetterTo];
    aCopy[changeLetterTo] = swap;

  }

  return aCopy;
}

float checkForLetters(char* text) {
  /*
   * Count number of letters and gives a pontuation accordingly with the languague frequency
   */

  int* lettersHash, sizeofText, i;
  char aChar;
  float score;

  lettersHash = calloc(255, sizeof(int));
  sizeofText = strlen(text);

  for (i = 0; i < sizeofText; i++) {
    aChar = text[i];
    lettersHash[aChar] = lettersHash[aChar]++;
  }

  score = punctuateLetterFrequency(lettersHash);
  free(lettersHash);
  return score;

}

char charToLowerCase(char c) {
  if (c >= 'A' && c <= 'Z') {
    return c + 32;
  } else if (c >= 'a' && c <= 'z') {
    return c;
  } else {
    printf("Something went wrong in <toLowerCase>, not a valid character.");
    return '?';
  }

}

void stringToLowerCase(char* text, int sizeofText) {
  /*
   * Modifies the original text
   */
  int i;
  for (i = 0; i < sizeofText; i++) {
    text[i] = charToLowerCase(text[i]);
  }

}

float punctuateLetterFrequency(int* hashOfLetterFrequency) {
  /*
   * SOURCE
   * http://pt.wikipedia.org/wiki/Frequ%C3%AAncia_de_letras
   */

  int i = 0;
  float modifier[255], score = 0;

  modifier['a'] = 14.63;
  modifier['b'] = 1.04;
  modifier['c'] = 3.88;
  modifier['d'] = 4.99;
  modifier['e'] = 12.57;
  modifier['f'] = 1.02;
  modifier['g'] = 1.30;
  modifier['h'] = 1.28;
  modifier['i'] = 6.18;
  modifier['j'] = 0.40;
  modifier['k'] = 0.02;
  modifier['l'] = 2.78;
  modifier['m'] = 4.74;
  modifier['n'] = 5.05;
  modifier['o'] = 10.73;
  modifier['p'] = 2.52;
  modifier['q'] = 1.20;
  modifier['r'] = 6.53;
  modifier['s'] = 7.81;
  modifier['t'] = 4.74;
  modifier['u'] = 4.63;
  modifier['v'] = 1.67;
  modifier['w'] = 0.01;
  modifier['x'] = 0.21;
  modifier['y'] = 0.01;
  modifier['z'] = 0.47;

  for (i = 97; i < 123; i++) {
    score = score + hashOfLetterFrequency[i] * modifier[i];

  }

  return score;

}

