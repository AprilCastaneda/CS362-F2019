#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
//#include <time.h>

#define DEBUG 0
#define NOISY_TEST 0

int myAssert(int expected, int actual){
	return expected == actual;
}

void checkTribute(int p, int handPos, struct gameState *G) {
    int i, tReturn = 0, numCoinsOld = 0, numCoinsNew = 0, numActionsOld = 0, numActionsNew = 0, numDeckOld = 0, numDeckNew = 0, assert = 0;

    struct gameState testG;


    // Test 1 - nextPlayer has one card in deck(copper) and no cards in discard
    memcpy (&testG, G, sizeof(struct gameState));
    int nextPlayer = whoseTurn(&G) + 1;
    int currentPlayer = whoseTurn(&G);
    if(testG.deckCount[nextPlayer] == 1 && testG.discardCount[nextPlayer] == 0 && testG.deck[nextPlayer][0] == copper){
        numCoinsOld = testG.coins;
        tReturn = tributeEffect(currentPlayer, nextPlayer, &testG);
        numCoinsNew = testG.coins;
        assert = myAssert(numCoinsOld + 2, numCoinsNew);
        if(NOISY_TEST && !assert){
            printf("Test 1: nextPlayer has one card in deck (copper), and no cards in discard.\n");
            printf("numCoinsOld + 2: %d != numCoinsNew: %d\n", numCoinsOld+2, numCoinsNew);
        }
    }

    // Test 2 - nextPlayer has no cards in deck and one card in discard (copper)
    memcpy (&testG, G, sizeof(struct gameState));
    nextPlayer = whoseTurn(&G) + 1;
    if(testG.deckCount[nextPlayer] == 0 && testG.discardCount[nextPlayer] == 1 && testG.discard[nextPlayer][0] == copper){
        numCoinsOld = testG.coins;
        currentPlayer = whoseTurn(&G);
        tReturn = tributeEffect(currentPlayer, nextPlayer, &testG);
        numCoinsNew = testG.coins;
        assert = myAssert(numCoinsOld + 2, numCoinsNew);
        if(NOISY_TEST && !assert){
            printf("Test 2: nextPlayer has no cards in deck, and one card in discard(copper).\n");
            printf("numCoinsOld + 2: %d != numCoinsNew: %d\n", numCoinsOld+2, numCoinsNew);
        }
    }

    // Test 3 - nextPlayer has no cards in deck or discard. Bug that adds actions.
    memcpy (&testG, G, sizeof(struct gameState));
    nextPlayer = whoseTurn(&G) + 1;
    if(testG.deckCount[nextPlayer] == 0 && testG.discardCount[nextPlayer] == 0){
        numActionsOld = testG.numActions;
        currentPlayer = whoseTurn(&G);
        tReturn = tributeEffect(currentPlayer, nextPlayer, &testG);
        numActionsNew = testG.numActions;
        assert = myAssert(numActionsOld, numActionsNew);
        if(NOISY_TEST && !assert){
            printf("Test 3: nextPlayer has no cards in deck or in discard. numActions shouldn't change.\n");
            printf("numActionsOld: %d != numActionsNew: %d\n", numActionsOld, numActionsNew);
        }
    }

    // Test 4 - nextPlayer has more than two cards in deck or discard.
    memcpy (&testG, G, sizeof(struct gameState));
    nextPlayer = whoseTurn(&G) + 1;
    if(testG.deckCount[nextPlayer] > 2){
        numDeckOld = testG.deckCount[nextPlayer];
        currentPlayer = whoseTurn(&G);
        tReturn = tributeEffect(currentPlayer, nextPlayer, &testG);
        numDeckNew = testG.deckCount[nextPlayer];;
        assert = myAssert(numDeckOld-2, numDeckNew);
        if(NOISY_TEST && !assert){
            printf("Test 4: nextPlayer has more than two cards in deck. Deck Count should decrease by 2.\n");
            printf("numDeckOld-2: %d != numDeckNew: %d\n", numDeckOld-2, numDeckNew);
        }
    }
}

int main () {

    int i, j, n, found, randCard, choice1, choice2, handPos, numPlayers, k[10];
    int seed = 1000;
    srand(time(0));
/*
    int k[10] = {adventurer, council_room, feast, gardens, mine,
                 remodel, smithy, village, baron, great_hall
                };
*/
    struct gameState G;

    printf ("Testing tributeCard.\n");

    printf ("RANDOM TESTS.\n");

//    SelectStream(2);
//    PutSeed(3);

    for (n = 0; n < 5000000; n++) {

        seed = (rand() % (5000 - 2000) + 2000);
        numPlayers = (rand() % (4-1) + 1);
        handPos = (rand() % 4);

        // Initialize kingdom cards
        for(i = 0; i < 10; i++){
            k[i] = 0;
        }
        found = 0;

        for(i = 0; i < 10; i++){
            randCard = (rand() % (26-7) + 7);
            k[i] = randCard;
        }
        
        int start = initializeGame(numPlayers, k, seed, &G);

        //Random hand cards
        for(i = 0; i < numHandCards(&G); i++){
            if(i != handPos){
                G.hand[whoseTurn(&G)][i] = (rand() % 27);
            }
            else{
                G.hand[whoseTurn(&G)][i] = 17;                
            }
        }

        // Random deckCount
        for(i = 0; i < numPlayers; i++){
            if(i != whoseTurn(&G)){
                G.deckCount[i] = (rand() % MAX_DECK);
                G.discardCount[i] = (rand() % MAX_DECK); 
            }
        }

        if(start == 0)      
            checkTribute(numPlayers, handPos, &G);
    }

    printf ("ALL TESTS OK\n");

    exit(0);

}
