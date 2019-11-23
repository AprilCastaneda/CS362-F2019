/*
 * unittest3.c
 *
 
 */

/*
 * Include the following lines in your makefile:
 *
 * unittest3: unittest3.c dominion.o rngs.o
 *      gcc -o unittest3 -g  unittest3.c dominion.o rngs.o $(CFLAGS)
 */


#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
//#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "mine"

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
    int handPos = 0, choice1 = 0, choice2 = 0;
    int seed = 1000;
    int numPlayers = 2;
    int currentPlayer = 0;
    int numCards = 0, numCardsNew = 0;
    int numMine = 0, numMineNew = 0;
    int numChoice1 = 0, numChoice1New = 0;
    int choice1Card = 0, choice1CardCost = 0;
    int choice2CardCost = 0;
    int mReturn = 0;		// return after running baronCard()
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

	// ----------- TEST 1: choice1 - player chose to trash card less than enum 4. --------------
	printf("TEST 1: choice1 - player chose to trash card less than enum 4.\n");

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	handPos = 0;
	choice1 = 1;
	choice2 = 4;
	currentPlayer = whoseTurn(&testG);
	testG.hand[currentPlayer][choice1] = 0;	// Set choice1 card to enum = 0	or curse card

	mReturn = mineEffect(choice1, choice2, currentPlayer, handPos, &testG);
	
	printf("choice1 enum = %d\n", testG.hand[currentPlayer][choice1]);
	printf("invalid choice; expected return = -1\n");
	printf("actual = %d\n", mReturn);
	printf("assert = %d\n\n", myAssert(-1, mReturn));

	// ----------- TEST 2: choice1 - player chose to trash card more than enum 6. --------------
	printf("TEST 2: choice1 - player chose to trash card more than enum 6.\n");

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	handPos = 0;
	choice1 = 1;
	choice2 = 4;
	currentPlayer = whoseTurn(&testG);
	testG.hand[currentPlayer][choice1] = 7;	// Set choice1 card to enum = 6	or adventurer card

	mReturn = mineEffect(choice1, choice2, currentPlayer, handPos, &testG);
	
	printf("choice1 enum = %d\n", testG.hand[currentPlayer][choice1]);
	printf("invalid choice; expected return = -1\n");
	printf("actual = %d\n", mReturn);
	printf("assert = %d\n\n", myAssert(-1, mReturn));

	// ----------- TEST 3: choice2 - player chose to gain card more than enum 26. --------------
	printf("TEST 3: choice2 - player chose to gain card more than enum 26.\n");

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	handPos = 0;
	choice1 = 1;
	choice2 = 27;
	currentPlayer = whoseTurn(&testG);
	mReturn = mineEffect(choice1, choice2, currentPlayer, handPos, &testG);
	
	printf("choice2 enum = %d\n", choice2);
	printf("invalid choice; expected return = -1\n");
	printf("actual = %d\n", mReturn);
	printf("assert = %d\n\n", myAssert(-1, mReturn));

	// ----------- TEST 4: choice2 - player chose to gain card less than enum 0. --------------
	printf("TEST 4: choice2 - player chose to gain card less than enum 0.\n");

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	handPos = 0;
	choice1 = 1;
	choice2 = -1;
	currentPlayer = whoseTurn(&testG);

	mReturn = mineEffect(choice1, choice2, currentPlayer, handPos, &testG);

	printf("choice2 enum = %d\n", choice2);
	printf("invalid choice; expected return = -1\n");
	printf("actual = %d\n", mReturn);
	printf("assert = %d\n\n", myAssert(-1, mReturn));

	// ----- TEST 5: Player chose to gain card more than +3 of trashed card's cost. -----
	printf("TEST 5: Player chose to gain card more than +3 of trashed card's cost.\n");

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	handPos = 0;
	choice1 = 1;
	choice2 = 6;
	currentPlayer = whoseTurn(&testG);

	testG.hand[currentPlayer][choice1] = 4;	// Set choice1 card to enum = 4	or copper card
	choice1Card = testG.hand[currentPlayer][choice1];
	choice1CardCost = getCost(choice1Card);
	choice2CardCost = getCost(choice2);

	mReturn = mineEffect(choice1, choice2, currentPlayer, handPos, &testG);
	
	printf("choice1 card cost = %d\n", choice1CardCost);
	printf("choice1 card can afford = %d + 3\n", choice1CardCost);
	printf("choice2 card cost = %d\n", choice2CardCost);
	printf("invalid choice - cannot afford; expected return = -1\n");
	printf("actual = %d\n", mReturn);
	printf("assert = %d\n\n", myAssert(-1, mReturn));

	// ----- TEST 6: Player chose to gain card less than or = +3 of trashed card's cost. -----
	printf("TEST 6: Player chose to gain card less than or = +3 of trashed card's cost.\n");

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	handPos = 0;
	choice1 = 1;
	choice2 = 5;	// Silver card
	currentPlayer = whoseTurn(&testG);

	testG.hand[currentPlayer][handPos] = mine;
	testG.hand[currentPlayer][choice1] = 4;	// Set choice1 card to enum = 4	or copper card
	numCards = numHandCards(&testG);
	choice1Card = testG.hand[currentPlayer][choice1];
	choice1CardCost = getCost(choice1Card);
	choice2CardCost = getCost(choice2);

	for(i = 0; i < numCards; i++){		// Count how many mine/choice1 cards in hand
		if(testG.hand[currentPlayer][i] == mine)
			numMine++;
		if(testG.hand[currentPlayer][i] == choice1Card)
			numChoice1++;
	}

	mReturn = mineEffect(choice1, choice2, currentPlayer, handPos, &testG);

	numCardsNew = numHandCards(&testG);
	for(i = 0; i < numCards; i++){	// Count how many mine/choice1 cards in hand after one discarded
		if(testG.hand[currentPlayer][i] == mine)
			numMineNew++;
		if(testG.hand[currentPlayer][i] == choice1Card)
			numChoice1New++;
	}

	printf("choice1 card cost = %d\n", choice1CardCost);
	printf("choice1 card can afford = %d + 3\n", choice1CardCost);
	printf("choice2 card cost = %d\n", choice2CardCost);
	printf("valid choice - expected return = 0\n");
	printf("actual = %d\n", mReturn);
	printf("assert = %d\n\n", myAssert(0, mReturn));

	printf("original num choice1 cards in hand = %d\n", numChoice1);
	printf("expected num choice1 cards after one discarded = %d - 1\n", numChoice1);
	printf("actual = %d\n", numChoice1New);
	printf("assert = %d\n\n", myAssert(numChoice1 - 1, numChoice1New));

	printf("original num mine cards in hand = %d\n", numMine);
	printf("expected num mine cards after one discarded = %d - 1\n", numMine);
	printf("actual = %d\n", numMineNew);
	printf("assert = %d\n\n", myAssert(numMine - 1, numMineNew));

	printf("original hand count = %d\n", numCards);
	printf("expected hand count = %d + 1 - 1 - 1\n", numCards);
	printf("actual = %d\n", numCardsNew);
	printf("assert = %d\n\n", myAssert(numCards + 1 - 1 - 1, numCardsNew));

	// ----------- TESTS FINISHED. --------------
	printf("\n >>>>> SUCCESS: Testing complete %s <<<<<\n\n", TESTCARD);

	return 0;
}

