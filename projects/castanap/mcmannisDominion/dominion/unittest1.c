/*
 * unittest1.c
 *
 
 */

/*
 * Include the following lines in your makefile:
 *
 * unittest1: unittest1.c dominion.o rngs.o
 *      gcc -o cardtest1 -g  cardtest4.c dominion.o rngs.o $(CFLAGS)
 */


#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
//#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "ambassador"

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
    int numCards = 0;
    int card = 0;
    int choice1Supply = 0;
    int choice1SupplyNew = 0;
    int aReturn = 0;		// return after running ambassadorCard()
	struct gameState G, testG;
	
	int k[10] = { adventurer, embargo, village, minion, mine, 
				ambassador, cutpurse, tribute, smithy, council_room};

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

	// ----------- TEST 1: choice1 == handpos --------------
	printf("TEST 1: choice1 == handpos = invalid; choosing own card.\n");

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	choice1 = handPos;
	aReturn = ambassadorEffect(choice1, choice2, currentPlayer, handPos, &testG);
	
	printf("choice1 = %d, handpos = %d, expected return = -1\n", choice1, handPos);
	printf("actual = %d\n", aReturn);
	printf("assert = %d\n\n", myAssert(-1, aReturn));

	// ----------- TEST 2: choice2 < 0 --------------
	printf("TEST 2: choice2 < 0 = invalid; choosing < 0 copies.\n");

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	handPos = 0;
	choice1 = 1;
	choice2 = -1;
	currentPlayer = whoseTurn(&testG);
	aReturn = ambassadorEffect(choice1, choice2, currentPlayer, handPos, &testG);
	
	printf("choice2 = %d, expected return = -1\n", choice2);
	printf("actual = %d\n", aReturn);
	printf("assert = %d\n\n", myAssert(-1, aReturn));

	// ----------- TEST 3: choice2 > 2 --------------
	printf("TEST 3: choice2 > 2 = invalid; choosing > 2 copies.\n");

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	handPos = 0;
	choice1 = 1;
	choice2 = 3;
	currentPlayer = whoseTurn(&testG);
	aReturn = ambassadorEffect(choice1, choice2, currentPlayer, handPos, &testG);
	
	printf("choice2 = %d, expected return = -1\n", choice2);
	printf("actual = %d\n", aReturn);
	printf("assert = %d\n\n", myAssert(-1, aReturn));
	
	// ----------- TEST 4: choice2 > num of choice1 cards in hand. --------------
	printf("TEST 4: choice2 > num of choice1 cards in hand. \n");

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	handPos = 0;
	choice1 = 1;
	choice2 = 2;
	currentPlayer = whoseTurn(&testG);
	numCards = numHandCards(&testG);
	card = testG.hand[currentPlayer][choice1];	// Get card at choice1 position
	card++;	// Go to next card from choice1 card.
	for(i = 2; i < numCards; i++)	// Change all cards in hand to a diff card.
		testG.hand[currentPlayer][i] = card;

	aReturn = ambassadorEffect(choice1, choice2, currentPlayer, handPos, &testG);

	printf("choice2 = 1, num of choice1 cards = 0, expected return = -1 \n");
	printf("actual = %d\n", aReturn);
	printf("assert = %d\n\n", myAssert(-1, aReturn)); // Should reveal first bug

	// ----------- TEST 5: Supply count of choice1 card increased by choice2. --------------
	printf("TEST 5: supply of choice1 card increased by choice2 amount. \n");

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	handPos = 0;
	choice1 = 1;
	choice2 = 2;
	currentPlayer = whoseTurn(&testG);
	card = testG.hand[currentPlayer][choice1];	// Get card at choice1 position
	choice1Supply = supplyCount(card, &testG);	// Original supplyCount of choice1

	for(i = 2; i < choice2; i++)	// Change choice2 num cards to choice1.
		testG.hand[currentPlayer][i] = card;
	
	aReturn = ambassadorEffect(choice1, choice2, currentPlayer, handPos, &testG);
	choice1SupplyNew = testG.supplyCount[card];	// New supplyCount of choice1

	printf("original num of choice1 cards in supply = %d\n", choice1Supply);
	printf("num of choice1 cards to be added to supply = %d\n", choice2);
	printf("expected new num of choice1 cards in supply %d + %d\n", choice1Supply, choice2);
	printf("actual = %d\n", choice1SupplyNew);
	printf("assert = %d\n\n", myAssert(choice1Supply + choice2, choice1SupplyNew));

	// ----------- TEST 6: Ambassador card discarded from player's hand --------------
	printf("TEST 6: Ambassador card discarded from player's hand.\n");

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	handPos = 0;
	choice1 = 1;
	choice2 = 2;
	currentPlayer = whoseTurn(&testG);
	numCards = numHandCards(&testG);	
	card = testG.hand[currentPlayer][choice1];	// Get card at choice1 position
	testG.hand[currentPlayer][handPos] = ambassador; // Make sure handPos is an ambassdor card
	
	for(i = 2; i < choice2; i++)	// Change choice2 num cards to choice1.
		testG.hand[currentPlayer][i] = card;

	int numAmbassador = 0;
	for(i = 0; i < numCards; i++)
		if(testG.hand[currentPlayer][i] == ambassador)
			numAmbassador++;	// Original count of ambassador cards in p1's hand.
	
	aReturn = ambassadorEffect(choice1, choice2, currentPlayer, handPos, &testG);
	int numCardsNew = numHandCards(&testG);
	int numAmbassadorNew = 0;	// New ambassador card count

	for(i = 0; i < numCardsNew; i++)
		if(testG.hand[currentPlayer][i] == ambassador)
			numAmbassadorNew++;	// New count of choice1 cards in p1's hand.

	printf("p1's original num of ambassador cards in hand = %d\n", numAmbassador);
	printf("expected new num of ambassador cards after one is discarded = %d - 1\n", numAmbassador);
	printf("actual = %d\n", numAmbassadorNew);
	printf("assert = %d\n\n", myAssert(numAmbassador - 1, numAmbassadorNew));

	// ----------- TEST 7: choice1 cards and p2's hand count increased by choice2 amount. --------------
	printf("TEST 7: choice1 cards and p2's hand count increased by choice2 amount. \n");

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	handPos = 0;
	choice1 = 1;
	choice2 = 2;
	currentPlayer = whoseTurn(&testG);
	card = testG.hand[currentPlayer][choice1];	// Get card at choice1 position
	
	for(i = 2; i < choice2; i++)	// Change choice2 num cards to choice1.
		testG.hand[currentPlayer][i] = card;

	int otherPHandCount = testG.handCount[1];	// Get otherPlayer's handCount
	int choice1p2Count = 0;
	for(i = 0; i < otherPHandCount; i++)
		if(testG.hand[1][i] == choice1)
			choice1p2Count++;	// Original count of choice1 cards in p2's hand.
	
	aReturn = ambassadorEffect(choice1, choice2, currentPlayer, handPos, &testG);
	int choice1p2CountNew = 0;	// New choice1 supplyCount
	int otherPHandCountNew = testG.handCount[1];	// Get otherPlayer's new handCount

	for(i = 0; i < otherPHandCountNew; i++)
		if(testG.hand[1][i] == choice1)
			choice1p2CountNew++;	// New count of choice1 cards in p2's hand.

	printf("p2's original hand count = %d\n", otherPHandCount);		// Should reveeal second bug
	printf("choice2 num of choice1 cards added to p2's hand = %d\n", choice2);
	printf("expected new p2's hand count %d + %d\n", otherPHandCount, choice2);
	printf("actual = %d\n", otherPHandCountNew);
	printf("assert = %d\n\n", myAssert(otherPHandCount + choice2, otherPHandCountNew));

	printf("num of choice1 cards in p2's hand = %d\n", choice1p2Count);
	printf("choice2 num of choice1 cards added to p2's hand = %d\n", choice2);
	printf("expected new num of choice1 cards in p2's hand to be %d + %d\n", choice1p2Count, choice2);
	printf("actual = %d\n", choice1p2CountNew);
	printf("assert = %d\n\n", myAssert(choice1p2Count + choice2, choice1p2CountNew));

	// ----------- TEST 8: choice2 == 0 --------------
	printf("TEST 8: choice2 == 0 = valid; Should be able to choose to not use any cards.\n");

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	handPos = 0;
	choice1 = 1;
	choice2 = 0;
	currentPlayer = whoseTurn(&testG);	
	aReturn = ambassadorEffect(choice1, choice2, currentPlayer, handPos, &testG);
	
	printf("choice2 = %d, expected return = 0\n", choice2);
	printf("actual = %d\n", aReturn);
	printf("assert = %d\n\n", myAssert(0, aReturn));

	// ----------- TEST 9: choice1 card(s) discarded from player's hand --------------
	printf("TEST 9: choice1 card(s) discarded from player's hand.\n");

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	handPos = 0;
	choice1 = 1;
	choice2 = 2;
	currentPlayer = whoseTurn(&testG);
	numCards = numHandCards(&testG);	
	card = testG.hand[currentPlayer][choice1];	// Get card at choice1 position
	
	for(i = 2; i < choice2; i++)	// Change choice2 num cards to choice1.
		testG.hand[currentPlayer][i] = card;

	int numChoice1 = 0;
	for(i = 0; i < numCards; i++)
		if(testG.hand[currentPlayer][i] == card)
			numChoice1++;	// Original count of choice1 cards in p1's hand.
	
	aReturn = ambassadorEffect(choice1, choice2, currentPlayer, handPos, &testG);
	numCardsNew = numHandCards(&testG);
	int numChoice1New = 0;	// New choice1 card count

	for(i = 0; i < numCardsNew; i++)
		if(testG.hand[currentPlayer][i] == choice1)
			numChoice1New++;	// New count of choice1 cards in p1's hand.

	printf("p1's original num of choice1 cards in hand = %d\n", numChoice1);
	printf("expected new num of choice1 cards after choice2 amount are discarded = %d - %d\n", numChoice1, choice2);
	printf("actual = %d\n", numChoice1New);
	printf("assert = %d\n\n", myAssert(numChoice1 - choice2, numChoice1New));

	// ----------- TESTS FINISHED. --------------
	printf("\n >>>>> SUCCESS: Testing complete %s <<<<<\n\n", TESTCARD);

	return 0;
}

