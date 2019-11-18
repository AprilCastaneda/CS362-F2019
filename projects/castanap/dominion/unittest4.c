/*
 * unittest4.c
 *
 
 */

/*
 * Include the following lines in your makefile:
 *
 * unittest4: unittest4.c dominion.o rngs.o
 *      gcc -o unittest4 -g  unittest4.c dominion.o rngs.o $(CFLAGS)
 */


#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
//#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "minion"

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
    int numHand = 0, numHandNew = 0;
    int numCoins = 0, numCoinsNew = 0;
    int actions = 0, actionsNew = 0;
    int minionCount = 0, minionCountNew = 0;
    int notEqual = 0;
    int otherPlayer = 1;
    int otherPHandCount = 0, otherPHandCountNew = 0;
    int tempHand[4];	// Store 4 of player's cards if player chooses to redraw
    int minReturn = 0;		// return after running minionCard()
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

	// ----------- TEST 1: numActions += 1. --------------
	printf("TEST 1: numActions += 1.\n");

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	handPos = 0;
	choice1 = 1;
	choice2 = 0;
	actions = testG.numActions;

	minReturn = minionCard(choice1, choice2, &testG, handPos);

	actionsNew = testG.numActions;
	
	printf("num actions before function call = %d\n", actions);
	printf("expected num actions after function call = %d + 1\n", actions);
	printf("actual = %d\n", actionsNew);
	printf("assert = %d\n\n", myAssert(actions + 1, actionsNew));

	// ----------- TEST 2: minion card discarded from player's hand. --------------
	printf("TEST 2: minion card discarded from player's hand. \n");

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	handPos = 0;
	choice1 = 1;
	choice2 = 0;

	testG.hand[currentPlayer][handPos] = minion;	// Set choice1 card to enum = minion
	numHand = numHandCards(&testG);
	for(i = 0; i < numHand; i++)				// Count minion cards before function call
		if(testG.hand[currentPlayer][i] == minion)
			minionCount++;

	minReturn = minionCard(choice1, choice2, &testG, handPos);

	numHandNew = numHandCards(&testG);
	for(i = 0; i < numHandNew; i++)				// Count minion cards after function call
		if(testG.hand[currentPlayer][i] == minion)
			minionCountNew++;
	
	printf("minion cards in hand before function call = %d\n", minionCount);
	printf("expected minion cards in hand after function call = %d - 1\n", minionCount);
	printf("actual = %d\n", minionCountNew);
	printf("assert = %d\n\n", myAssert(minionCount - 1, minionCountNew));

	// ----------- TEST 3: choice1 = 1, player chooses to gain two coins. --------------
	printf("TEST 3: choice1 = 1, player chooses to gain two coins.\n");

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	handPos = 0;
	choice1 = 1;
	choice2 = 0;

	numCoins = testG.coins;

	minReturn = minionCard(choice1, choice2, &testG, handPos);

	numCoinsNew = testG.coins;
	
	printf("num coins before function call = %d\n", numCoins);
	printf("expected num coins after function call = %d + 2\n", numCoins);
	printf("actual = %d\n", numCoinsNew);
	printf("assert = %d\n\n", myAssert(numCoins + 2, numCoinsNew));

	// -- TEST 4: choice1 = 0, choice2 = 1, player chooses to discard rest of their cards, 
	// -- 			and draw 4 new cards.--
	printf("TEST 4: choice1 = 0, choice2 = 1, player chooses to discard cards and draw new cards.\n");

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	handPos = 0;
	choice1 = 0;
	choice2 = 1;

	numHand = numHandCards(&testG);
	testG.hand[currentPlayer][handPos] = minion;

	for(i = 1; i < numHand; i++)			// Store old hand to tempHand
		tempHand[i-1] = testG.hand[currentPlayer][i];

	minReturn = minionCard(choice1, choice2, &testG, handPos);

	numHandNew = numHandCards(&testG);
	for(i = 0; i < numHandNew; i++)
		if(tempHand[i] != testG.hand[currentPlayer][i])
			notEqual = 1;
	
	printf("player's cards before and after function call should be not equal.\n");
	printf("expected comparison notEqual = 1\n");
	printf("actual = %d\n", notEqual);
	printf("assert = %d\n\n", myAssert(1, notEqual));

	// ----- TEST 5: choice1 = 0, choice2 = 1, player chooses to discard cards and draw new cards. -----
	printf("TEST 5: choice1 = 0, choice2 = 1, player chooses to discard cards and draw new cards.\n");

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	handPos = 0;
	choice1 = 0;
	choice2 = 1;

	testG.hand[currentPlayer][handPos] = minion;

	minReturn = minionCard(choice1, choice2, &testG, handPos);

	numHandNew = numHandCards(&testG);
	
	printf("expected num cards in hand after function call = 4\n");
	printf("actual = %d\n", numHandNew);
	printf("assert = %d\n\n", myAssert(4, numHandNew));

	// ----- TEST 6: choice1 = 0, choice2 = 1, other players have less than five cards and not made to discard. -----
	printf("TEST 6: choice1 = 0, choice2 = 1, other players have less than five cards and not made to discard.\n");

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	handPos = 0;
	choice1 = 0;
	choice2 = 1;
	otherPlayer = 1;
	i = 0;

	testG.hand[currentPlayer][handPos] = minion;

	// Make sure other player has three cards
	while(testG.handCount[otherPlayer] > 3){
		discardCard(i, otherPlayer, &testG, 1);
		i++;
	}
	while(testG.handCount[otherPlayer] < 3){
		gainCard(1, &testG, 2, otherPlayer);
	}

	otherPHandCount = testG.handCount[otherPlayer];

	minReturn = minionCard(choice1, choice2, &testG, handPos);

	otherPHandCountNew = testG.handCount[otherPlayer];
	
	printf("other player's cards in hand before function call = %d.\n", otherPHandCount);
	printf("expected other player's cards in hand doesn't change after function call = %d\n", otherPHandCount);
	printf("actual = %d\n", otherPHandCountNew);
	printf("assert = %d\n\n", myAssert(otherPHandCount, otherPHandCountNew));

	// ----- TEST 7: choice1 = 0, choice2 = 1, other player has more than four cards and have to redraw. -----
	printf("TEST 7: choice1 = 0, choice2 = 1, other player has more than four cards and have to redraw.\n");

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	handPos = 0;
	choice1 = 0;
	choice2 = 1;
	otherPlayer = 1;
	i = 0;

	testG.hand[currentPlayer][handPos] = minion;

	// Make sure other player has five cards
	while(testG.handCount[otherPlayer] > 5){
		discardCard(i, otherPlayer, &testG, 1);
		i++;
	}
	while(testG.handCount[otherPlayer] < 5){
		gainCard(1, &testG, 2, otherPlayer);
	}

	otherPHandCount = testG.handCount[otherPlayer];

	minReturn = minionCard(choice1, choice2, &testG, handPos);

	otherPHandCountNew = testG.handCount[otherPlayer];
	
	printf("other player's cards in hand before function call = %d.\n", otherPHandCount);
	printf("expected other player's cards in hand after function call = 4\n");
	printf("actual = %d\n", otherPHandCountNew);
	printf("assert = %d\n\n", myAssert(4, otherPHandCountNew));

	// ----- TEST 8: choice1 = 0, choice2 = 1, other player has more than four cards and have to redraw. -----
	printf("TEST 8: choice1 = 0, choice2 = 1, other player has more than four cards and have to redraw.\n");

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	handPos = 0;
	choice1 = 0;
	choice2 = 1;
	otherPlayer = 1;
	i = 0;
	notEqual = 0;

	testG.hand[currentPlayer][handPos] = minion;

	// Make sure other player has five cards
	while(testG.handCount[otherPlayer] > 5){
		discardCard(i, otherPlayer, &testG, 1);
		i++;
	}
	while(testG.handCount[otherPlayer] < 5){
		gainCard(1, &testG, 2, otherPlayer);
	}

	otherPHandCount = testG.handCount[otherPlayer];

	for(i = 0; i < otherPHandCount; i++)		// Change all cards to estate cards
		testG.hand[otherPlayer][i] = estate;

	minReturn = minionCard(choice1, choice2, &testG, handPos);

	otherPHandCountNew = testG.handCount[otherPlayer];

	for(i = 0; i < otherPHandCountNew; i++)	// See if other player drew cards and they're different
		if(testG.hand[otherPlayer][i] != estate)
			notEqual = 1;
	
	printf("other player's cards before and after function call should be not equal.\n");
	printf("expected comparison notEqual = 1\n");
	printf("actual = %d\n", notEqual);
	printf("assert = %d\n\n", myAssert(1, notEqual));

	// ----------- TESTS FINISHED. --------------
	printf("\n >>>>> SUCCESS: Testing complete %s <<<<<\n\n", TESTCARD);

	return 0;
}

