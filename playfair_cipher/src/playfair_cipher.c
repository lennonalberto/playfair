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

#define TRUE 1
#define FALSE 0

void simulatedAnnealing();
char* generateRandomKey(int numberOfCharacters);
char generateRandomCharacter();
int firstUse(char aChar, char* inString, int stringSize);

int main(int argc, char* argv[]) {
	simulatedAnnealing();

}

void simulatedAnnealing() {
	char* aKey = generateRandomKey(6);

	puts(aKey);

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
	srand(time(NULL ));
	int randomNumber = rand() % 26;
	char aChar = 97 + randomNumber;

	return aChar;
}

int firstUse(char aChar, char* inString, int stringSize) {
	int i;

	for (i = 0; i < stringSize; i++) {
		if (inString[i] == aChar)
			return FALSE;

	}
	return TRUE;

}

