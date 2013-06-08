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

void simulatedAnnealing();

int main(int argc, char* argv[]) {
	simulatedAnnealing();

}

void simulatedAnnealing() {

}
