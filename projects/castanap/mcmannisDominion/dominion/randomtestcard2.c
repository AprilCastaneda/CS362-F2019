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

void checkMinion(int p, int choice1, int choice2, int handPos, struct gameState *G) {
    int i;

    struct gameState testG;
    memcpy (&testG, G, sizeof(struct gameState));

    // Test 1: numActions += 1
    int numActionsOld = testG.numActions;
    int currentPlayer = 0;
    int mReturn = minionEffect(choice1, choice2, currentPlayer, handPos, &testG);
    int numActionsNew = testG.numActions;

    int assert = myAssert(numActionsOld + 1, numActionsNew);
    if(NOISY_TEST && !assert){
        printf("Test 1: numActions += 1 fail\n");
        printf("numActionsOld + 1: %d != numActionsNew: %d\n", numActionsOld+1, numActionsNew);
    }

    // Test 2: Discard minion card from hand.
    memcpy (&testG, G, sizeof(struct gameState)); 
    int numCardsOld = numHandCards(&testG);
    int curPlayer = whoseTurn(&testG);
    mReturn = minionEffect(choice1, choice2, currentPlayer, handPos, &testG);
    int numCardsNew = numHandCards(&testG);

    assert = myAssert(numCardsOld + 1, numCardsNew);
    if(NOISY_TEST && !assert){
        printf("Test 2: minion card not discarded\n");
        printf("numCardsOld + 1: %d != numCardsNew: %d\n", numCardsOld+1, numCardsNew);
    }

    // Test 3: Player choose to gain two coins.
    memcpy (&testG, G, sizeof(struct gameState)); 
    if(choice1 == 1){
        int numCoinsOld = testG.coins;
        mReturn = minionEffect(choice1, choice2, currentPlayer, handPos, &testG);
        int numCoinsNew = testG.coins;
        assert = myAssert(numCoinsOld + 2, numCoinsNew);
        if(NOISY_TEST && !assert){
            printf("Test 3: choice1 = 1, Player chooses to gain two coins\n");
            printf("numCoinsOld + 2: %d != numCoinsNew: %d\n", numCoinsOld+2, numCoinsNew);
        }
    }

    // Test 4: choice2 = 1, player chooses to discard cards and draw new cards
    memcpy (&testG, G, sizeof(struct gameState)); 
    if(choice2 == 1){
        numCardsOld = numHandCards(&testG);
        int tempHand[numCardsOld];
        int notEqual = 0;

        for(i = 0; i < numCardsOld; i++){
            tempHand[i] = testG.hand[whoseTurn(&testG)][i];
        }

        mReturn = minionEffect(choice1, choice2, currentPlayer, handPos, &testG);

        numCardsNew = numHandCards(&testG);

        for(i = 0; i < numCardsNew; i++){
            if(tempHand[i] == testG.hand[whoseTurn(&testG)][i])
                notEqual = 1;
        }

        assert = myAssert(1, notEqual);
        if(NOISY_TEST && !assert){
            printf("Test 4: choice2 = 1, player chooses to discard cards and draw new cards\n");
            printf("newCardsDrawn: %d\n", assert);
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

    printf ("Testing minionCard.\n");

    printf ("RANDOM TESTS.\n");

//    SelectStream(2);
//    PutSeed(3);

    for (n = 0; n < 5000000; n++) {

        seed = (rand() % (5000 - 2000) + 2000);
        numPlayers = (rand() % (4-1) + 1);
        handPos = (rand() % 4);
        choice1 = (rand() % 2);
        if(choice1 == 0)
            choice2 = 1;
        else
            choice2 = 0;

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
        if(start == 0)      
            checkMinion(numPlayers, choice1, choice2, handPos, &G);
    }

    printf ("ALL TESTS OK\n");

    exit(0);

}
