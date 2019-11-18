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

void checkBaron(int p, int choice1, int handP, struct gameState *G) {
    struct gameState testG;
    memcpy (&testG, &G, sizeof(struct gameState));

    // Test 1: numBuys += 1
    int buysOld = testG.numBuys;
    int bReturn = baronCard(choice1, &testG);
    int buysNew = testG.numBuys;

    int assert = myAssert(buysOld + 1, buysNew);
    if(NOISY_TEST && !assert){
        printf("Test 1: numBuys += 1 fail\n");
        printf("buysOld + 1: %d != buysNew: %d\n", buysOld+1, buysNew);
    }

    // Test 2: Discard estate.
    memcpy (&testG, &G, sizeof(struct gameState)); 
    int numCards = numHandCards(&testG);
    int curPlayer = whoseTurn(&testG);
    int found = 0;
    int numCardsNew = 0;

    // Player chooses to discard estate; Find if there's an estate in hand.
    if(choice1 == 1){
        for(int i = 0; i < numCards; i++){
            if(testG.hand[curPlayer][i] == estate){ 
                found = 1;
                i = numCards;
            }
        }

        if(found){
            bReturn = baronCard(choice1, &testG);
            numCardsNew = numHandCards(&testG);
            assert = myAssert(numCards-1, numCardsNew);
            if(NOISY_TEST && !assert){
                printf("Test 2: User chooses to discard estate card and has one in hand.\n");
                printf("numCards - 1: %d != numCardsNew: %d\n", numCards-1, numCardsNew);
            }
        }
        // Test 3: Discard estate. But none in hand.
        else{
            int estateSupply = supplyCount(estate, &testG);
            bReturn = baronCard(choice1, &testG); 
            int estateSupplyNew = supplyCount(estate, &testG);
            assert = myAssert(estateSupply-1, estateSupplyNew);
            if(NOISY_TEST && !assert){
                printf("Test 3: User chooses to discard estate card and has none in hand.\n");
                printf("estateSupply - 1: %d != estateSupplyNew: %d\n", estateSupply-1, estateSupplyNew);
            }
        }    
    }

    else{
        // Test 4
    }


}

int main () {

    int i, j, n, found, randCard, choice1, handPos, numPlayers, k[10];
    int seed = 1000;
    srand(time(0));
/*
    int k[10] = {adventurer, council_room, feast, gardens, mine,
                 remodel, smithy, village, baron, great_hall
                };
*/
    struct gameState G;

    printf ("Testing baronCard.\n");

    printf ("RANDOM TESTS.\n");

//    SelectStream(2);
//    PutSeed(3);

    for (n = 0; n < 5000000; n++) {

        seed = (rand() % (5000 - 2000) + 2000);
        numPlayers = (rand() % 5);
        handPos = (rand() % 5);
        choice1 = (rand() % 2);

        // Initialize kingdom cards
        for(i = 0; i < 10; i++){
            if(i == handPos)
                k[i] = 15;
            else
                k[i] = 0;
        }
        found = 0;

        for(i = 0; i < 10; i++){
            if(i != handPos){
                randCard = (rand() % (26-7) + 7);
                k[i] = randCard;
            }
        }
        /*
        if(n == 1){
            for(i = 0; i < 10; i++){
                printf("k card i: %d %d ", i, k[i]);
            }
        }
        */
        //printf("choice1: %d, handPos: %d\n", choice1, handPos);
        initializeGame(numPlayers, k, seed, &G);
        
        // Random supplyCount for estate
        G.supplyCount[estate] = (rand() % 3);

        // Random hand cards
        for(i = 0; i < numHandCards(&G); i++){
            if(i != handPos){
                G.hand[whoseTurn(&G)][i] = (rand() % 27);
            }
        }       
        checkBaron(numPlayers, choice1, handPos, &G);
        /*
        if(n == 100){
            for(i = 0; i < numHandCards(&G); i++){
                printf("hand card i: %d %d ", i, G.hand[whoseTurn(&G)][i]);
            }
        }
        */
    }

    printf ("ALL TESTS OK\n");

    exit(0);

}
