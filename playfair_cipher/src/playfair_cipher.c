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
#include <math.h>

#define TRUE 1
#define FALSE 0
#define KEY_SIZE 6
#define MAX_ITERATIONS 100000
#define DIGRAPH_IMPLICIT_MODIFIER 1.3
#define TRIGRAPH_IMPLICIT_MODIFIER 1.6
#define TEMPERATURE 40
#define STEP 0.2

int LOG_COUNT;
float letterModifier[255];
float digraphModifier[255][255];
char trigrams[255][255];
float trigramsValues[255];
int trigramsQuantity;

void hillClimbing(char* cipherText);
char* generateRandomKey(char* key, int numberOfCharacters);
char generateRandomCharacter();
int firstUse(char aChar, char* inString, int stringSize);
char* decipher(char* cipherText, char* key);
float rateFitness(char* text);
void slightlyModifiedKey(char* originalKey, char* modifiedKey, int keySize, int numberOfSwapOperations);
float checkForLetters(char* text);
char charToLowerCase(char c);
void stringToLowerCase(char* text, int sizeofText);
float scoreLetterFrequency(int* hashOfLetterFrequency);
void initialize();
void digraphInitialize();
float checkForDigraphs(char* text);
void reverseKey(char* key);
void replaceCharacter(char* key);
void swapCharacters(char* key);
void simulatedAnnealing(char* cipherText);
void modifyKey(char *originalKey, char *modifiedKey);
void newDecipher(char* decipheredText, char* cipheredText, char* key);
void trigraphInitialize();
void trim(char* dest);
float getTrigramScore(char* trigram);
float checkForTrigraphs(char* text);

int main(int argc, char* argv[]) {

  initialize();

  char* cipherText;

  cipherText = calloc(1, 255);
  strcpy(cipherText, "ouviramdoipirangaasmargensplacidasdeumpovoheroicobradoretumbante");
  stringToLowerCase(cipherText, strlen(cipherText));
//  hillClimbing(cipherText);
  printf("esse e o score de uma frase em portugues <%f>\n", rateFitness("ouviramdoipirangaasmargensplacidasdeumpovoheroicobradoretumbante"));
  printf("esse e o score de uma frase em portugues <%f>\n", rateFitness("umafraseemportuguesbemgrandeporxemplocomdiversoscaracteresbanana"));
  printf("esse e o score de uma frase em portugues <%f>\n", rateFitness("quequequequequequequequequequequequequequequequequequequequequea"));
//  printf("esse e o score de uma frase em portugues <%f>\n", rateFitness("umtextomuitogenerico"));
//  printf("esse e o score de uma frase em portugues <%f>\n", rateFitness("praveroquefeiradafru"));
  simulatedAnnealing(cipherText);


  free(cipherText);

  return 0;

}

void simulatedAnnealing(char* cipherText) {

  /*
   1. Generate a random key, called the 'parent', decipher the ciphertext
   using this key.
   2. Rate the fitness of the deciphered text, store the result.
   3. for(TEMP = 10;TEMP >= 0; TEMP = TEMP - STEP)
   for (count = 50,000; count>0; count--)
   Change the parent key slightly (e.g. swap two characters in the
   key at random) to get child key,
   Measure the fitness of the deciphered text using the child key
   set dF = (fitness of child - fitness of parent)
   If dF > 0 (fitness of child is higher than parent key),
   set parent = child
   If dF < 0 (fitness of child is worse than parent),
   set parent = child with probability e^(dF/T).
   */

  char* keyParent, *testKey, *topText, *maxKey, *decipheredText, *bestKey;
  float dF = 0, fitnessParent = 0, fitnessChild = 0, T, topScore, score, prob, maxscore, bestscore, mystery;
  int count, sizeofText;

  bestKey = calloc(1, KEY_SIZE);
  maxKey = calloc(1, KEY_SIZE);
  keyParent = calloc(1, KEY_SIZE);
  testKey = calloc(1, KEY_SIZE);
  decipheredText = calloc(1, 1024);
  sizeofText = strlen(cipherText);

  generateRandomKey(maxKey, KEY_SIZE);
  strcpy(bestKey, maxKey);
  newDecipher(decipheredText, cipherText, maxKey);
  maxscore = rateFitness(decipheredText);
  bestscore = maxscore;

  for (T = TEMPERATURE; T >= 0; T -= STEP) {
    for (count = 0; count < MAX_ITERATIONS; count++) {
      modifyKey(maxKey, testKey);
      newDecipher(decipheredText, cipherText, testKey);
      score = rateFitness(decipheredText);
      dF = score - maxscore;
      if (dF >= 0) {
        maxscore = score;
        strcpy(maxKey, testKey);
      } else if (T > 0) {
        prob = exp(dF / T);
        mystery = 1.0 * rand() / RAND_MAX;
        if (prob > mystery) {
          maxscore = score;
          strcpy(maxKey, testKey);
        }
      }
      if (maxscore > bestscore) {
        bestscore = maxscore;
        strcpy(bestKey, maxKey);
        printf("best text so far <%s> with score <%f> and key <%s>\n", decipheredText, bestscore, bestKey);
      }
    }
  }

  free(keyParent);
  free(testKey);

}

void hillClimbing(char* cipherText) {
  char* firstDecipheredText;
  float firstRate;
  char* topKey, *topText;
  float topScore = 0;

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
    printf("<LOG-%d> 1-Deci-Text: <%s> Score: <%f> with key: <%s>\n", LOG_COUNT, firstDecipheredText, firstRate, aKey);
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
        printf("<LOG-%d> 1-Deci-Text: <%s> Score: <%f> with key: <%s>\n", LOG_COUNT, firstDecipheredText, firstRate, modifiedKey);
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
  totalScore = totalScore + checkForDigraphs(text);
  totalScore = totalScore + checkForTrigraphs(text);

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

  score = scoreLetterFrequency(lettersHash);
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

float scoreLetterFrequency(int* hashOfLetterFrequency) {
  /*
   * SOURCE
   * http://pt.wikipedia.org/wiki/Frequ%C3%AAncia_de_letras
   */

  int i = 0;
  float score = 0;

  for (i = 97; i < 123; i++) {
    score = score + hashOfLetterFrequency[i] * letterModifier[i];

  }

  return score;

}

void initialize() {
  /*
   * performance purposes
   */
  srand(time(NULL ));
  LOG_COUNT = 0;
  letterModifier['a'] = 14.63;
  letterModifier['b'] = 1.04;
  letterModifier['c'] = 3.88;
  letterModifier['d'] = 4.99;
  letterModifier['e'] = 12.57;
  letterModifier['f'] = 1.02;
  letterModifier['g'] = 1.30;
  letterModifier['h'] = 1.28;
  letterModifier['i'] = 6.18;
  letterModifier['j'] = 0.40;
  letterModifier['k'] = 0.02;
  letterModifier['l'] = 2.78;
  letterModifier['m'] = 4.74;
  letterModifier['n'] = 5.05;
  letterModifier['o'] = 10.73;
  letterModifier['p'] = 2.52;
  letterModifier['q'] = 1.20;
  letterModifier['r'] = 6.53;
  letterModifier['s'] = 7.81;
  letterModifier['t'] = 4.74;
  letterModifier['u'] = 4.63;
  letterModifier['v'] = 1.67;
  letterModifier['w'] = 0.01;
  letterModifier['x'] = 0.21;
  letterModifier['y'] = 0.01;
  letterModifier['z'] = 0.47;

  digraphInitialize();

  trigraphInitialize();

}

void digraphInitialize() {
  FILE *inputFile;
  char i, j;

  /*
   * SOURCE: http://www.dcc.fc.up.pt/~rvr/naulas/tabelasPT/
   */

  inputFile = fopen("/home/lwf09/digraphsInput.txt", "r");
  float value;

  for (i = 'a'; i <= 'z'; i++) {
    for (j = 'a'; j <= 'z'; j++) {
      char line[128]; /* or other suitable maximum line size */
      fgets(line, sizeof line, inputFile);
      value = atof(line);
      digraphModifier[i][j] = value;
    }
  }

  fclose(inputFile);

}

void modifyKey(char *originalKey, char *modifiedKey) {
  /*
   * 10% new char, 10% reverse key, 80% swap characters
   */

  strcpy(modifiedKey, originalKey);

  int decision = rand() % 10;

  switch (decision) {
  case 1:
    reverseKey(originalKey);

    break;
  case 2:
    replaceCharacter(originalKey);

    break;
  default:
    swapCharacters(originalKey);
    break;
  }

}

void swapCharacters(char* key) {
  int keySize = KEY_SIZE, changeLetterFrom = 0, changeLetterTo = 0;
  char swap;
  while (changeLetterFrom == changeLetterTo) {
    changeLetterFrom = rand() % keySize;
    changeLetterTo = rand() % keySize;
  }
  swap = key[changeLetterFrom];
  key[changeLetterFrom] = key[changeLetterTo];
  key[changeLetterTo] = swap;
}

void replaceCharacter(char* key) {
  int replaceCharacterWithIndex = 0;
  char replaceWith;
  replaceCharacterWithIndex = rand() % KEY_SIZE;
  replaceWith = generateRandomCharacter();
  while (!firstUse(replaceWith, key, KEY_SIZE)) {
    replaceWith = generateRandomCharacter();
  }

  key[replaceCharacterWithIndex] = replaceWith;

}

void reverseKey(char* key) {
  int i = 0, currentPosition = 0;
  char swap;
  for (i = KEY_SIZE - 1; i >= 0; i--) {
    swap = key[currentPosition];
    key[currentPosition] = key[i];
    key[i] = swap;
  }
}

void newDecipher(char* decipheredText, char* cipheredText, char* key) {
  int i = 0, sizeofText = strlen(cipheredText);

  for (i = 0; i < sizeofText; i++) {
    decipheredText[i] = generateRandomCharacter();

  }

}

void trigraphInitialize() {

  FILE *inputFile;
  char numberOfTrigrams = 0;
  float value;

  /*
   * SOURCE: http://www.dcc.fc.up.pt/~rvr/naulas/tabelasPT/
   */

  inputFile = fopen("/home/lwf09/trigraphsInput.txt", "r");

  char line[128]; /* or other suitable maximum line size */
  while (fgets(line, sizeof line, inputFile) != NULL ) {
    trim(line);
    value = atof(line);
    // ok.. bad practice but...
    if (!value) {
      strcpy(trigrams[numberOfTrigrams], line);

    } else {
      trigramsValues[numberOfTrigrams] = value;
      numberOfTrigrams++;
    }
  }
  trigramsQuantity = numberOfTrigrams;

  fclose(inputFile);

}

void trim(char* dest) {
  int size = strlen(dest);
  dest[size - 1] = '\0';
}

float getTrigramScore(char* trigram) {
  float score = 0;
  char* currentTrigram;
  int i = 0;

  for (i = 0; i < trigramsQuantity; i++) {
    currentTrigram = trigrams[i];
    if (!strcmp(currentTrigram, trigram)) {
      score = trigramsValues[i];
    }

  }
  return score;

}

float checkForTrigraphs(char* text) {
  int sizeofText = strlen(text), i = 0, j = 0;
  float score[3], bestScore;
  char *aTrigraph;

  aTrigraph = calloc(1, 10);

  for (i = 0; i < 3; i++) {
    score[i] = 0;
  }

  for (i = 0; i < sizeofText; i++) {
    memset(aTrigraph, 0, 3);
    for (j = 0; j < 3; j++) {
      if ((i + j) < sizeofText) {
        aTrigraph[j] = text[i + j];
      }

    }
    if (strlen(aTrigraph) == 3) {
      score[i % 3] = score[i % 3] + getTrigramScore(aTrigraph);
    }

  }
  bestScore = 0;
  for (i = 0; i < 3; i++) {
    if (score[i] > bestScore) {
      bestScore = score[i];
    }
  }

  free(aTrigraph);
  return bestScore * TRIGRAPH_IMPLICIT_MODIFIER;

}

float checkForDigraphs(char* text) {
  int i = 0, sizeofText;
  char firstChar, secondChar, thirdChar;
  float firstScore = 0, secondScore = 0, result = 0;

  sizeofText = strlen(text);

  for (i = 0; i < sizeofText; i = i + 2) {
    firstChar = text[i];
    secondChar = text[i + 1];
    firstScore = firstScore + digraphModifier[firstChar][secondChar];
    if ((i + 2) < sizeofText) {
      thirdChar = text[i + 2];
      secondScore = secondScore + digraphModifier[secondChar][thirdChar];
    }

  }

  result = firstScore > secondScore ? firstScore : secondScore;

  return result * DIGRAPH_IMPLICIT_MODIFIER;

}

