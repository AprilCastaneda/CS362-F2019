/*
 * unittest2.c
 *
 
 */

/*
 * Include the following lines in your makefile:
 *
 * unittest2: unittest2.c dominion.o rngs.o
 *      gcc -o unittest2 -g  unittest.c dominion.o rngs.o $(CFLAGS)
 */


#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
//#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "baron"

/* 	Custom assert function: Asserts if expected and actual results are the same.
	Parameters:	int expected = expected results
				int actual = actual results
	Return:		1 if they are the same
				0 if they are not the same
*/
int myAssert(int expected, int actual){
	return expected == actual;
}

int main() {

    int i;
    int handPos = 0, choice1 = 0;
    int seed = 1000;
    int numPlayers = 2;
    int currentPlayer = 0;
    int numCards = 0, numCardsNew = 0;
    int numEstate = 0, numEstateNew = 0;
    int numDuchy = 0, numDuchyNew = 0;
    int numCoins = 0, numCoinsNew = 0;
    int estateSupply = 0, estateSupplyNew = 0;
    int buysOld = 0, buysNew = 0;
    int bReturn = 0;		// return after running baronCard()
    int gameOver = 0, gameOverNew = 0;
	struct gameState G, testG;
	
	int k[10] = { adventurer, embargo, village, minion, mine, 
				baron, cutpurse, tribute, smithy, council_room};

	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);

	/* Print players' cards
	for(int i = 0; i < numPlayers; i++){
		for(int j = 0; j < 5; j++){
			printf("Player: %d, hand: %d\n", i, G.hand[i][j]);
		}
		printf("\n");
	}
	printf("\n\n");
	*/

	printf("----------------- Testing Card: %s ----------------\n", TESTCARD);

	// ----------- TEST 1: numBuys += 1. --------------
	printf("TEST 1: numBuys += 1.\n");

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	choice1 = 0;
	buysOld = testG.numBuys;	// Original numBuys	
	bReturn = baronCard(choice1, &testG);
	buysNew = testG.numBuys;	// New numBuys	
	
	printf("old numBuys = %d\n", buysOld);
	printf("expected new numBuys = %d + 1\n", buysOld);
	printf("actual = %d\n", buysNew);
	printf("assert = %d\n\n", myAssert(buysOld + 1, buysNew));

	// ----------- TEST 2: User chooses to discard estate card and has one in hand. --------------
	printf("TEST 2: User chooses to discard estate card and has one in hand.\n");

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	handPos = 0;
	choice1 = 1;

	currentPlayer = whoseTurn(&testG);
	numCards = numHandCards(&testG);
	numCoins = testG.coins;	// Original num coins
	numEstate = numEstateNew = 0;
	numDuchy = numDuchyNew = 0;

	testG.hand[currentPlayer][handPos] = baron;

	for(i = 1; i < numCards-1; i++)	// Change three cards in hand to estate.
		testG.hand[currentPlayer][i] = estate;
	testG.hand[currentPlayer][numCards-1] = duchy;	// Changed last card to duchy

	for(i = 0; i < numCards; i++){	// Count estate/duchy cards in hand.
		if(testG.hand[currentPlayer][i] == estate)
			numEstate++;
		if(testG.hand[currentPlayer][i] == duchy)
			numDuchy++;
	}

	bReturn = baronCard(choice1, &testG);

	numCardsNew = numHandCards(&testG);	// Get new numCards after estate discard
	numCoinsNew = testG.coins;

	for(i = 0; i < numCardsNew; i++){	// Count estate cards after one dicarded.
		if(testG.hand[currentPlayer][i] == estate)
			numEstateNew++;
		if(testG.hand[currentPlayer][i] == duchy)
			numDuchyNew++;
	}

	printf("num coins originally = %d\n", numCoins);
	printf("expected num coins after estate discarded = %d + 4\n", numCoins);
	printf("actual = %d\n", numCoinsNew);
	printf("assert = %d\n\n", myAssert(numCoins + 4, numCoinsNew));
	
	printf("num estate cards in hand = %d\n", numEstate);
	printf("expected num estate cards in hand after discard = %d - 1\n", numEstate);
	printf("actual = %d\n", numEstateNew);
	printf("assert = %d\n\n", myAssert(numEstate - 1, numEstateNew));
	
	printf("num duchy cards in hand = %d\n", numDuchy);
	printf("expected num duchy cards in hand after discard = %d\n", numDuchy);
	printf("actual = %d\n", numDuchyNew);
	printf("assert = %d\n\n", myAssert(numDuchy, numDuchyNew));
	
	printf("num cards in hand originally = %d\n", numCards);
	printf("expected num cards in hand after discard = %d - 1\n", numCards);
	printf("actual = %d\n", numCardsNew);
	printf("assert = %d\n\n", myAssert(numCards - 1, numCardsNew));

	// ----------- TEST 3: User chooses to discard estate card and has none in hand. --------------
	printf("TEST 3: User chooses to discard estate card and has none in hand.\n");

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	handPos = 0;
	choice1 = 1;

	currentPlayer = whoseTurn(&testG);
	numCards = numHandCards(&testG);
	estateSupply = supplyCount(estate, &testG);	// Original estate count in supply

	testG.hand[currentPlayer][handPos] = baron;

	for(i = 1; i < numCards; i++)	// Change all other cards in hand to duchy.
		testG.hand[currentPlayer][i] = duchy;

	bReturn = baronCard(choice1, &testG);

	numCardsNew = numHandCards(&testG);	// Get new numCards after estate discard
	estateSupplyNew = supplyCount(estate, &testG);	// New estate count in supply

	printf("num estate cards in supply = %d\n", estateSupply);
	printf("expected num estate cards in supply after one card drawn = %d - 1\n", estateSupply);
	printf("actual = %d\n", estateSupplyNew);
	printf("assert = %d\n\n", myAssert(estateSupply - 1, estateSupplyNew));
	
	// ----TEST 4: User chooses to discard estate card and has none in hand and one in supply. ------
	printf("TEST 4: User chooses to discard estate card and has none in hand and one in supply.\n");

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	handPos = 0;
	choice1 = 1;

	currentPlayer = whoseTurn(&testG);
	numCards = numHandCards(&testG);
	testG.supplyCount[estate] = 1;	// Set estate count to 1
	estateSupply = supplyCount(estate, &testG);	
	gameOver = isGameOver(&testG);

	testG.hand[currentPlayer][handPos] = baron;

	for(i = 1; i < numCards; i++)	// Change all other cards in hand to duchy.
		testG.hand[currentPlayer][i] = duchy;

	bReturn = baronCard(choice1, &testG);

	estateSupplyNew = supplyCount(estate, &testG);	// New estate count in supply
	gameOverNew = isGameOver(&testG);

	printf("num estate cards in supply = %d\n", estateSupply);
	printf("expected num estate cards in supply after one card drawn = %d - 1\n", estateSupply);
	printf("actual = %d\n", estateSupplyNew);
	printf("assert = %d\n\n", myAssert(estateSupply - 1, estateSupplyNew));

	printf("game over status before baronCard() = %d\n", gameOver);
	printf("expected game over status after baronCard() = 1\n");
	printf("actual = %d\n", gameOverNew);
	printf("assert = %d\n\n", myAssert(1, gameOverNew));

	// ---- TEST 5: User chooses not to discard an estate card and there are estate cards in the supply.-----
	printf("TEST 5: User chooses not to discard an estate card and there are estate cards in the supply.\n");

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	handPos = 0;
	choice1 = 0;

	currentPlayer = whoseTurn(&testG);
	numCards = numHandCards(&testG);
	estateSupply = supplyCount(estate, &testG);	// Original estate count in supply

	testG.hand[currentPlayer][handPos] = baron;

	bReturn = baronCard(choice1, &testG);

	numCardsNew = numHandCards(&testG);	// Get new numCards after estate drawn
	estateSupplyNew = supplyCount(estate, &testG);	// New estate count in supply

	printf("num estate cards in supply = %d\n", estateSupply);
	printf("expected num estate cards in supply after one card drawn = %d - 1\n", estateSupply);
	printf("actual = %d\n", estateSupplyNew);
	printf("assert = %d\n\n", myAssert(estateSupply - 1, estateSupplyNew));

	printf("num cards in hand = %d\n", numCards);
	printf("expected num cards in hand after estate drawn to hand = %d + 1\n", numCards);
	printf("actual = %d\n", numCardsNew);
	printf("assert = %d\n\n", myAssert(numCards + 1, numCardsNew));

	// ----TEST 6: User chooses not to discard estate card and there is one in supply. ------
	printf("TEST 6: User chooses not to discard estate card and there is one in supply.\n");

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	handPos = 0;
	choice1 = 0;

	currentPlayer = whoseTurn(&testG);
	numCards = numHandCards(&testG);
	testG.supplyCount[estate] = 1;	// Set estate count to 1
	estateSupply = supplyCount(estate, &testG);	
	gameOver = isGameOver(&testG);

	testG.hand[currentPlayer][handPos] = baron;

	bReturn = baronCard(choice1, &testG);

	gameOverNew = isGameOver(&testG);
	estateSupplyNew = supplyCount(estate, &testG);	// New estate count in supply

	printf("num estate cards in supply = %d\n", estateSupply);
	printf("expected num estate cards in supply after one card drawn = %d - 1\n", estateSupply);
	printf("actual = %d\n", estateSupplyNew);
	printf("assert = %d\n\n", myAssert(estateSupply - 1, estateSupplyNew));

	printf("game over status before baronCard() = %d\n", gameOver);
	printf("expected game over status after baronCard() = 1\n");
	printf("actual = %d\n", gameOverNew);
	printf("assert = %d\n\n", myAssert(1, gameOverNew));

	// ----------- TESTS FINISHED. --------------
	printf("\n >>>>> SUCCESS: Testing complete %s <<<<<\n\n", TESTCARD);

	return 0;
}

