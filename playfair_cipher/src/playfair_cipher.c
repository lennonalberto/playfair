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
#define MAX_ITERATIONS 10000

#define LOG_SHOW_TEXT_AND_PUNCTUATION 0
int LOG_COUNT;
float modifier[255];

void simulatedAnnealing(char* cipherText);
char* generateRandomKey(char* key, int numberOfCharacters);
char generateRandomCharacter();
int firstUse(char aChar, char* inString, int stringSize);
char* decipher(char* cipherText, char* key);
float rateFitness(char* text);
void slightlyModifiedKey(char* originalKey, char* modifiedKey, int keySize, int numberOfSwapOperations);
float checkForLetters(char* text);
char charToLowerCase(char c);
void stringToLowerCase(char* text, int sizeofText);
float punctuateLetterFrequency(int* hashOfLetterFrequency);
void initialize();

int main(int argc, char* argv[]) {

  initialize();

  char* cipherText;

  cipherText = calloc(1, 255);
  strcpy(cipherText, "ENTREINAFEIRADAFRUTA");
  stringToLowerCase(cipherText, strlen(cipherText));
  simulatedAnnealing(cipherText);

  free(cipherText);

}

void simulatedAnnealing(char* cipherText) {
  char* firstDecipheredText;
  float firstRate;
  char* topKey, *topText;
  float topScore = 0;

  srand(time(NULL ));

  char* aKey = calloc(1, sizeof(KEY_SIZE));
  char* modifiedKey = calloc(1, sizeof(KEY_SIZE));
  topKey = calloc(1, KEY_SIZE);
  topText = calloc(1, strlen(cipherText));

  int needToModify = TRUE;

  int numberOfIterations;

  while (numberOfIterations < MAX_ITERATIONS) {
    // ======================================================
    generateRandomKey(aKey, KEY_SIZE);
    firstDecipheredText = decipher(cipherText, aKey);
    firstRate = rateFitness(firstDecipheredText);
    printf("<LOG-%d> 1-Deci-Text: <%s> Punctuation: <%f> with key: <%s>\n", LOG_COUNT, firstDecipheredText, firstRate, aKey);
    LOG_COUNT++;
    // ======================================================
    needToModify = TRUE;

    while (needToModify) {
      if (firstRate > topScore) {
        strcpy(topKey, aKey);
        strcpy(topText, firstDecipheredText);
        topScore = firstRate;
        numberOfIterations = 0;

        needToModify = TRUE;

        // ======================================================
        slightlyModifiedKey(aKey, modifiedKey, KEY_SIZE, 1);
        firstDecipheredText = decipher(cipherText, modifiedKey);
        firstRate = rateFitness(firstDecipheredText);
        printf("<LOG-%d> 1-Deci-Text: <%s> Punctuation: <%f> with key: <%s>\n", LOG_COUNT, firstDecipheredText, firstRate, modifiedKey);
        LOG_COUNT++;
        // ======================================================

      } else {
        needToModify = FALSE;
      }

    }

    numberOfIterations++;

  }

  printf("<BEST SOLUTION>\n");
  printf("Text: <%s>\n", topText);
  printf("Score: <%f>\n", topScore);
  printf("With Key: <%s>", topKey);

}

char* generateRandomKey(char* key, int numberOfCharacters) {
  int i;
  char aChar;

  for (i = 0; i < numberOfCharacters; i++) {
    aChar = generateRandomCharacter();
    while (!firstUse(aChar, key, i)) {
      aChar = generateRandomCharacter();
    }
    key[i] = aChar;
  }
  return key;
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
  // for tests purposes only

  char* result = calloc(1, 255);

  generateRandomKey(result, 10);
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

void slightlyModifiedKey(char* originalKey, char* modifiedKey, int keySize, int numberOfSwapOperations) {
  // TODO mudar para imperativo
  int i;
  int changeLetterFrom = 0;
  int changeLetterTo = 0;
  char swap;

  strcpy(modifiedKey, originalKey);

  for (i = 0; i < numberOfSwapOperations; i++) {
    while (changeLetterFrom == changeLetterTo) {
      changeLetterFrom = rand() % keySize;
      changeLetterTo = rand() % keySize;
    }
    swap = modifiedKey[changeLetterFrom];
    modifiedKey[changeLetterFrom] = modifiedKey[changeLetterTo];
    modifiedKey[changeLetterTo] = swap;

  }

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
  float score = 0;

  for (i = 97; i < 123; i++) {
    score = score + hashOfLetterFrequency[i] * modifier[i];

  }

  return score;

}

void initialize() {
  /*
   * for performance
   */
  LOG_COUNT = 0;
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

}

