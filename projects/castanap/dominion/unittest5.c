/*
 * unittest5.c
 *
 
 */

/*
 * Include the following lines in your makefile:
 *
 * unittest5: unittest5.c dominion.o rngs.o
 *      gcc -o unittest5 -g  unittest5.c dominion.o rngs.o $(CFLAGS)
 */


#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
//#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "tribute"

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
    int seed = 1000;
    int numPlayers = 2;
    int numHand = 0, numHandNew = 0;
    int numCoins = 0, numCoinsNew = 0;
    int numDiscard = 0, numDiscardNew = 0;
    int numDeck = 0, numDeckNew = 0;
    int actions = 0, actionsNew = 0;
    int notEqual = 0;
    int otherPlayer = 1;
    int tempHand[5];	// Store 4 of player's cards if player chooses to redraw
    int tReturn = 0;		// return after running minionCard()
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

	// ----------- TEST 1: nextPlayer has one card in deck (copper) and no cards in discard. --------------
	printf("TEST 1: nextPlayer has one card in deck (copper) and no cards in discard.\n");

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	otherPlayer = 1;

	// Make next player only have one card in deck (copper) and no cards in discard
	testG.deckCount[otherPlayer] = 1;
	testG.discardCount[otherPlayer] = 0;
	testG.deck[otherPlayer][0] = copper;	// Causes add two coins
	numCoins = testG.coins;					// Coins before function call

	tReturn = tributeCard(&testG);

	numCoinsNew = testG.coins;
	
	printf("num coins before function call = %d\n", numCoins);
	printf("expected num actions after function call = %d + 2\n", numCoins);
	printf("actual = %d\n", numCoinsNew);
	printf("assert = %d\n\n", myAssert(numCoins + 2, numCoinsNew));

	// ----------- TEST 2: nextPlayer has no cards in deck and one card in dicard(copper). --------------
	printf("TEST 1: nextPlayer has no cards in deck and one card in dicard(copper).\n");

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	otherPlayer = 1;

	// Make next player have no cards in deck and one card (copper) in discard
	testG.deckCount[otherPlayer] = 0;
	testG.discardCount[otherPlayer] = 1;
	testG.discard[otherPlayer][0] = copper;	// Causes add two coins
	numCoins = testG.coins;					// Coins before function call

	tReturn = tributeCard(&testG);

	numCoinsNew = testG.coins;
	
	printf("num coins before function call = %d\n", numCoins);
	printf("expected num actions after function call = %d + 2\n", numCoins);
	printf("actual = %d\n", numCoinsNew);
	printf("assert = %d\n\n", myAssert(numCoins + 2, numCoinsNew));

	// ----------- TEST 3: nextPlayer has no cards in deck or dicard. Bug that adds actions.--------------
	printf("TEST 3: nextPlayer has no cards in deck or discard. Bug that adds actions\n");

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	otherPlayer = 1;

	// Make next player have no cards in deck and one card (copper) in discard
	testG.deckCount[otherPlayer] = 0;
	testG.discardCount[otherPlayer] = 0;
	actions = testG.numActions;		// Coins before function call

	tReturn = tributeCard(&testG);

	actionsNew = testG.numActions;
	
	printf("num actions before function call = %d\n", actions);
	printf("expected num actions after function call = %d\n", actions);
	printf("actual = %d\n", actionsNew);
	printf("assert = %d\n\n", myAssert(actions, actionsNew));

	// ----------- TEST 4: nextPlayer has no cards in deck and two cards in discard. --------------
	printf("TEST 4: nextPlayer has no cards in deck and two cards in discard.\n");

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	otherPlayer = 1;

	// Make next player have no cards in deck and two cards in discard
	testG.deckCount[otherPlayer] = 0;
	testG.discardCount[otherPlayer] = 2;
	testG.discard[otherPlayer][0] = copper;	// Discard has two copper cards only
	testG.discard[otherPlayer][1] = copper;	
	numDiscard = testG.discardCount[otherPlayer];
	numDeck = testG.deckCount[otherPlayer];

	tReturn = tributeCard(&testG);

	numDiscardNew = testG.discardCount[otherPlayer];
	numDeckNew = testG.deckCount[otherPlayer];
	
	printf("num discard before function call = %d\n", numDiscard);
	printf("expected num discard after function call = 0\n");
	printf("actual num discard = %d\n", numDiscardNew);	
	printf("assert = %d\n\n", myAssert(0, numDiscardNew));

	printf("num deck before function call = %d\n", numDeck);
	printf("expected num deck after function call = 0\n");
	printf("actual num deck = %d\n", numDeckNew);
	printf("assert = %d\n\n", myAssert(0, numDeckNew));

	// ----------- TEST 5: nextPlayer has two cards in deck, which are the same cards (copper). --------------
	printf("TEST 5: nextPlayer has two cards in deck, which are the same cards (copper).\n");

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	otherPlayer = 1;

	// Make next player have no cards in deck and two cards in discard
	testG.deckCount[otherPlayer] = 2;
	testG.discardCount[otherPlayer] = 2;
	testG.deck[otherPlayer][0] = copper;	// Deck has two copper cards only
	testG.deck[otherPlayer][1] = copper;	
	numDeck = testG.deckCount[otherPlayer];

	tReturn = tributeCard(&testG);

	numDeckNew = testG.deckCount[otherPlayer];
	
	printf("num deck before function call = %d\n", numDeck);
	printf("expected num deck after function call = 0\n");
	printf("actual num deck = %d\n", numDeckNew);	
	printf("assert = %d\n\n", myAssert(0, numDeckNew));

	// ----------- TEST 6: nextPlayer has two cards in deck, which are the same cards (copper).--------------
	printf("TEST 6: nextPlayer has two cards in deck, which are the same cards (copper).\n");

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	otherPlayer = 1;

	// Make next player have no cards in deck and two cards in discard
	testG.deckCount[otherPlayer] = 2;
	testG.discardCount[otherPlayer] = 2;
	testG.deck[otherPlayer][0] = copper;	// Deck has two copper cards only
	testG.deck[otherPlayer][1] = copper;	
	numCoins = testG.coins;					// Coins before function call

	tReturn = tributeCard(&testG);

	numCoinsNew = testG.coins;
	
	printf("num coins before function call = %d\n", numCoins);
	printf("expected num coins after function call = %d + 2\n", numCoins);
	printf("actual num coins = %d\n", numCoinsNew);	
	printf("assert = %d\n\n", myAssert(numCoins + 2, numCoinsNew));

	// ----------- TEST 7: nextPlayer has two cards in deck, which are two diff treasure cards (copper,silver). --------------
	printf("TEST 7: nextPlayer has two cards in deck, which are two diff treasure cards (copper,silver).\n");

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	otherPlayer = 1;

	// Make next player have no cards in deck and two cards in discard
	testG.deckCount[otherPlayer] = 2;
	testG.discardCount[otherPlayer] = 2;
	testG.deck[otherPlayer][0] = copper;	// Deck has two different treasure cards
	testG.deck[otherPlayer][1] = silver;	
	numCoins = testG.coins;					// Coins before function call

	tReturn = tributeCard(&testG);

	numCoinsNew = testG.coins;
	
	printf("num coins before function call = %d\n", numCoins);
	printf("expected num coins after function call = %d + 4\n", numCoins);
	printf("actual num coins = %d\n", numCoinsNew);	
	printf("assert = %d\n\n", myAssert(numCoins + 4, numCoinsNew));

	// ----------- TEST 8: nextPlayer has two cards in deck, which are two diff victory cards (estate, duchy). --------------
	printf("TEST 8: nextPlayer has two cards in deck, which are two diff victory cards (estate, duchy).\n");

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	otherPlayer = 1;

	// Make next player have no cards in deck and two cards in discard
	testG.deckCount[otherPlayer] = 2;
	testG.discardCount[otherPlayer] = 2;
	testG.deck[otherPlayer][0] = estate;	// Deck has two different treasure cards
	testG.deck[otherPlayer][1] = duchy;	
	numHand = numHandCards(&testG);				// hand count before function call

	tReturn = tributeCard(&testG);

	numHandNew = numHandCards(&testG);			// hand count after function call
	
	printf("num hand before function call = %d\n", numHand);
	printf("expected num hand after function call = %d + 2\n", numHand);
	printf("actual num hand = %d\n", numHandNew);	
	printf("assert = %d\n\n", myAssert(numHand + 2, numHandNew));

	// ----------- TEST 9: nextPlayer has two cards in deck, which are two diff action cards (minion, steward). --------------
	printf("TEST 9: nextPlayer has two cards in deck, which are two diff action cards (minion, steward).\n");

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	otherPlayer = 1;

	// Make next player have no cards in deck and two cards in discard
	testG.deckCount[otherPlayer] = 2;
	testG.discardCount[otherPlayer] = 2;
	testG.deck[otherPlayer][0] = minion;	// Deck has two different action cards
	testG.deck[otherPlayer][1] = steward;	
	actions = testG.numActions;				// action count before function call

	tReturn = tributeCard(&testG);

	actionsNew = testG.numActions;		// action count after function call
	
	printf("num actions before function call = %d\n", actions);
	printf("expected num actions after function call = %d + 2\n", actions);
	printf("actual num actions = %d\n", actionsNew);	
	printf("assert = %d\n\n", myAssert(actions + 2, actionsNew));

	// ----------- TEST 10: nextPlayer has no cards in deck and five diff cards in discard. --------------
	printf("TEST 10: nextPlayer has no cards in deck and five diff cards in discard.\n");

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	otherPlayer = 1;
	notEqual = 0;

	// Make next player have no cards in deck and two cards in discard
	testG.deckCount[otherPlayer] = 0;
	testG.discardCount[otherPlayer] = 5;
	testG.discard[otherPlayer][0] = estate;	// Discard has random cards
	testG.discard[otherPlayer][1] = province;
	testG.discard[otherPlayer][2] = silver;	
	testG.discard[otherPlayer][3] = adventurer;	
	testG.discard[otherPlayer][4] = feast;
	numDiscard = testG.discardCount[otherPlayer];
	numDeck = testG.deckCount[otherPlayer];

	// Store discard in temp
	for(i = 0; i < numDiscard; i++)
		tempHand[i] = testG.discard[otherPlayer][i];

	tReturn = tributeCard(&testG);

	numDeckNew = testG.deckCount[otherPlayer];
	
	// Check if cards in deck are shuffled from discard
	for(i = 0; i < numDeckNew; i++)
		if(tempHand[i] != testG.deck[otherPlayer][i])
			notEqual = 1;
	
	printf("deck before function call expected different from discard, notEqual = 1\n");
	printf("actual notEqual = %d\n", notEqual);
	printf("assert = %d\n\n", myAssert(1, notEqual));

	// ----------- TEST 11: nextPlayer has two cards in deck, which are two same victory cards (estate, estate). --------------
	printf("TEST 11: nextPlayer has two cards in deck, which are two same victory cards (estate, estate).\n");

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	otherPlayer = 1;

	// Make next player have no cards in deck and two cards in discard
	testG.deckCount[otherPlayer] = 2;
	testG.discardCount[otherPlayer] = 2;
	testG.deck[otherPlayer][0] = estate;	// Deck has two different treasure cards
	testG.deck[otherPlayer][1] = estate;	
	numHand = numHandCards(&testG);				// hand count before function call

	tReturn = tributeCard(&testG);

	numHandNew = numHandCards(&testG);			// hand count after function call
	
	printf("num hand before function call = %d\n", numHand);
	printf("expected num hand after function call = %d + 1\n", numHand);
	printf("actual num hand = %d\n", numHandNew);	
	printf("assert = %d\n\n", myAssert(numHand + 1, numHandNew));

	// ----------- TESTS FINISHED. --------------
	printf("\n >>>>> SUCCESS: Testing complete %s <<<<<\n\n", TESTCARD);

	return 0;
}

