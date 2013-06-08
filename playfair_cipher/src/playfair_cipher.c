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

int main(int argc, char* argv[]) {

  char* cipherText;

  cipherText = calloc(1, 255);
  strcpy(cipherText, "ENTREINAFEIRAFRUTA");
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
  strcpy(result, "something really cool will happen here");
  return result;
}

float rateFitness(char* text) {

  return 100;

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

