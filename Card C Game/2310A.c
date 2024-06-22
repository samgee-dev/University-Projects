#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

int main(int argc, char** argv) {
    // Let's do the error checking for the player
    // check for the incorrect number of args
    if (argc != 3) {
        fprintf(stderr, "Usage: player pcount ID\n");
        return 1;
    }

    // player count is less than 1
    int playerCount;
    sscanf(argv[1], "%d", &playerCount);
   // printf("count: %d\n", playerCount);
    if (playerCount < 1) {
        fprintf(stderr, "Invalid player count\n");
        return 2;
    }

    // check the player id is correct
    int playerID;
    sscanf(argv[2], "%d", &playerID);
    //printf("current: %d\n", playerID);
    if (0 > playerID || playerID >= playerCount) {
        fprintf(stderr, "Invalid ID\n");
        return 3;
    }

    // print a ^ to stdout if all the above tests were passed
    printf("^");

    // Get the path
    char* empty = "";
    char* path = malloc(sizeof(char) * 100 + 1);

    fscanf(stdin, "%s", path);
    path[strlen(path)] = '\0';

    char* sitesAndMaxPlayers[path[0] - '0'][2];
    int sitesCurrentPlayers[path[0] - '0'];
    //memset(sitesAndMaxPlayers, 32, 2);

    // fill all the sitesCurrentPlayers with 0s
    for (int i = 0; i < (path[0] - '0'); i++) {
        sitesCurrentPlayers[i] = 0;
    }

    // All the variables which will be used in the loop
    //FILE* pathName2 = fopen(argv[2], "r");
    int firstPathValue = 0;
    int contentInvalid = 1;
    int currentStateNumber = 0;
    int isLetter = 1;
    char lastLetter;
    int checkMax = 0;
    int currentMax = 0;
    int numberOfSites = path[0] - '0';

    // Set up the malloc used for checking and sending
    char* currentState = malloc(sizeof(char) * 15);
    char* lastState = malloc(sizeof(char) * 30);
    char* pathToSend = malloc(sizeof(char) * 100);
    char maxToAdd[5];
    int barrierList[numberOfSites];
    int barrierNumber = 0;
    strcpy(currentState, empty);
    strcpy(lastState, empty);
    strcpy(maxToAdd, empty);
    strcpy(pathToSend, empty);

    // Make char* for all the states
    char* barrier = "::-";
    char* stateMo = "Mo";
    char* stateV1 = "V1";
    char* stateV2 = "V2";
    char* stateDo = "Do";
    char* stateRi = "Ri";

    int length = strlen(path);

    fprintf(stderr, "%d\n", length);

    // Check the path contains the correct content
    for (int i = 0; i < strlen(path); i++) {
        // Get the first character
        char c = path[i];

        // Check if c is the first or second letter
        if ((c > 64 && c < 123) || lastLetter == 'V') {
            // Change isLetter to the opposite number
            if (isLetter == 1 || isLetter == 2) {
                isLetter = 0;
            } else {
                isLetter = 1;
            }
        } else {
            // set it to be different for the number
            isLetter = 2;
        }

        // save the last char
        lastLetter = c;

        // Check if we are at the first character and see how many 
        // states are there
        if (firstPathValue == 0) {
            // Set it to not enter this loop if a ; has occured
            if (c == ';') {
                firstPathValue++;
            }

            // Append to the path that will send to player
            strncat(pathToSend, &c, 1);

        } else if (c == 10) { 
            // do nothing

        } else {
            // append to the current state
            strncat(currentState, &c, 1);

            // Append to the path that will send to player
            strncat(pathToSend, &c, 1);

            // Go through each state and check that it is correct
            // First check the ::-
            if (strcmp(currentState, barrier) == 0) {
                // Reset the currentState string
                strcpy(lastState, currentState);
                strcpy(currentState, empty);

                // Add to the string array
                sitesAndMaxPlayers[currentStateNumber][0] = "::";
                sitesAndMaxPlayers[currentStateNumber][1] = "-";

                // add the number to barrier list
                barrierList[barrierNumber] = currentStateNumber;
                barrierNumber++;

                // count the state
                currentStateNumber++;

                // change last and current state
                strcpy(lastState, currentState);
                strcpy(currentState, empty);

            } else if (isLetter == 1 && currentStateNumber != 0) {
                // Check this is a valid state and if not, flag it
                if (strcmp(currentState, stateMo) != 0 && 
                        strcmp(currentState, stateDo) != 0 
                        && strcmp(currentState, stateRi) != 0 && 
                        strcmp(currentState, stateV1) != 0 && 
                        strcmp(currentState, stateV2) != 0) {
                    contentInvalid = 1;
                    break;
                }

                // Add the string to the array storing the sites
                if (strcmp(currentState, stateMo) == 0) {
                    sitesAndMaxPlayers[currentStateNumber][0] = "Mo";

                } else if (strcmp(currentState, stateDo) == 0) {
                    sitesAndMaxPlayers[currentStateNumber][0] = "Do";

                } else if (strcmp(currentState, stateRi) == 0) {
                    sitesAndMaxPlayers[currentStateNumber][0] = "Ri";

                } else if (strcmp(currentState, stateV1) == 0) {
                    sitesAndMaxPlayers[currentStateNumber][0] = "V1";

                } else if (strcmp(currentState, stateV2) == 0) {
                    sitesAndMaxPlayers[currentStateNumber][0] = "V2";
                    
                }

                // count the state
                currentStateNumber++;
                checkMax = 1;
                isLetter = 1;

                // reset max to add here
               // strcpy(maxToAdd, empty);

                // change last and current state
                strcpy(lastState, currentState);
                strcpy(currentState, empty);

            } else if (isdigit(c)) {
                // Change from ascii to int
                c = c - '0';

                // change last and current state
                strcpy(lastState, currentState);
                strcpy(currentState, empty);

                // Check if we have another digit to add
                if (checkMax == 0) {
                    // Calculate new total and convert it to the correct format
                    currentMax = (currentMax * 10) + c;
                    snprintf(maxToAdd, 5, "%d", currentMax);
                    sitesAndMaxPlayers[currentStateNumber - 1][1] = maxToAdd;

                    // set checkMax to 0 to check if states have been changed
                    checkMax = 0;
                } else {
                    // wipe the variable holding max
                    currentMax = c;

                    // Check what the value in c is and add it to the array
                    switch(c) {
                        case 0:
                            sitesAndMaxPlayers[currentStateNumber - 1][1] = "0";
                            break;
                        case 1:
                            sitesAndMaxPlayers[currentStateNumber - 1][1] = "1";
                            break;
                        case 2:
                            sitesAndMaxPlayers[currentStateNumber - 1][1] = "2";
                            break;
                        case 3:
                            sitesAndMaxPlayers[currentStateNumber - 1][1] = "3";
                            break;
                        case 4:
                            sitesAndMaxPlayers[currentStateNumber - 1][1] = "4";
                            break;
                        case 5:
                            sitesAndMaxPlayers[currentStateNumber - 1][1] = "5";
                            break;
                        case 6:
                            sitesAndMaxPlayers[currentStateNumber - 1][1] = "6";
                            break;
                        case 7:
                            sitesAndMaxPlayers[currentStateNumber - 1][1] = "7";
                            break;
                        case 8:
                            sitesAndMaxPlayers[currentStateNumber - 1][1] = "8";
                            break;
                        case 9:
                            sitesAndMaxPlayers[currentStateNumber - 1][1] = "9";
                            break;
                    }

                    // set checkMax to 0 to check if states have been changed
                    checkMax = 0;
                }
            }

            // Check the number of states are equal and if so unflag the flag
            if (currentStateNumber == numberOfSites) {
                contentInvalid = 0;
            } else {
                // If we go over, reflag the flag
                contentInvalid = 1;
            }
        }
    }

    // close the file and free mallocs
    free(currentState);
    free(lastState);
    //free(maxToAdd);

    // check if contentInvalid was raised
    if (contentInvalid == 1) {
        fprintf(stderr, "Invalid path\n");
        return 4;
    }

    // Let's make the array which will store the player's
    char* playerUnder[playerCount - 1][numberOfSites - 1];
    char playerDisplay[playerCount - 1][numberOfSites * 3];

    // Add something to playerUnder so my code doesn't crash. I just don't even 
    // know why

    // Let's fill the display with blank spaces
    for (int i = 0; i < playerCount; i++) {
        for (int j = 0; j < numberOfSites * 3; j++) {
            playerDisplay[i][j] = ' ';
        }
    }

    // Let's put the player's starting positions into the board
    for (int i = 0; i < playerCount; i++) {
        for (int j = 0; j < numberOfSites * 3; j++) {
            // Check if we are at starting position and add the players
            if (j == 0) {
                // If player number is over 10, add two chars
                if (i > 9) {
                    // Add the player to the array
                    char playerSymbol = (playerCount - 1 - i) + '0';
                    playerDisplay[i][0] = playerSymbol;
                }

                // Add the player to the array
                char playerSymbol = (playerCount - 1 - i) + '0';
                playerDisplay[i][0] = playerSymbol;
            } 
        }
    }

    //fprintf(stderr, "A%sA\n", )

    // make a loop for the game
    int isGameOver = 0;
    int playerMoney = 0;
    int currentSite = 0;
    int playerPoint = 0;
    int nextBarrier = 1;
    int totalV1 = 0;
    int totalV2 = 0;
    int playerMoveID;
    int additionalPoints;
    int moneyChange;
    int cardDrawn;

    int pointStorage[playerCount - 1][8];
    int pointStorage3[playerCount - 1][8];
    int pointStorage2[playerCount - 1][8];

    // Save my code
    //if (pointStorage[0][0] == 0) {
        // do nothing
    //}

    // Let's fill the storage with the appropriate details
    for (int i = 0; i < playerCount; i++) {
        for (int j = 0; j < 8 * 3; j++) {
            if (j == 0) {
                pointStorage2[i][j] = i;
            } else {
                pointStorage2[i][j] = 0;
            }
        }
    }

    //char* empty = "";
    char* yourTurn = "YT";
    char* early = "EARLY";
    char* done = "DONE";
    char* moveMade = "HAP";

    while (isGameOver == 0) {
        // print the path using a for loop
        //printf("%s", ":: ");
        for (int i = 0; i < numberOfSites; i++) {
            fprintf(stderr, "%s", sitesAndMaxPlayers[i][0]);
            fprintf(stderr, " ");
        }
        fprintf(stderr, "\n");

        // Count which column has the most numbers
        int largestColumn = 0;
        for (int j = 0; j < numberOfSites * 3; j++) {
            int tempLargestColumn = 0;
            for (int i = 0; i < playerCount; i++) {
                if (playerDisplay[i][j] != ' ') {
                    tempLargestColumn++;
                }
            }

            // Check for new max
            if (tempLargestColumn > largestColumn) {
                largestColumn = tempLargestColumn;
            }
        }


        // now, let's print the player underneath
        for (int i = 0; i < largestColumn; i++) {
            for (int j = 0; j < numberOfSites * 3; j++) {
                fprintf(stderr, "%c", playerDisplay[i][j]);
            }
            fprintf(stderr, "\n");
        }

        // Get the input from the dealer
        char* dealerInput = malloc(sizeof(char) * 100);
        strcpy(dealerInput, empty);
        fscanf(stdin, "%s", dealerInput);

        // If it is the player's turn
        int oldSite = pointStorage2[playerID][1];
        currentSite = pointStorage2[playerID][1];
        if (strcmp(dealerInput, yourTurn) == 0) {
            // Now we need to see what site the player will move to
            // Let's check if the player is at a new barrier
            if (currentSite == barrierList[nextBarrier]) {                  // check this
                nextBarrier++;
            } 

            int newSiteSet = 0;
            for (int i = currentSite + 1; i < barrierList[nextBarrier]; i++) {
                // first let's check if the player can move to the Do site
                int siteMax = strtol(sitesAndMaxPlayers[i][1], (char **)NULL, 10);
                int siteMax2 = strtol(sitesAndMaxPlayers[currentSite + 1][1], (char **)NULL, 10);
                if (playerMoney > 0 && strcmp(sitesAndMaxPlayers[i][0], stateDo) == 0 && siteMax > sitesCurrentPlayers[i] && newSiteSet == 0) {
                    // first convert all money to points
                    if (playerMoney % 2 == 0) {
                        // money is even so don't round
                        playerPoint = playerPoint + (playerMoney / 2);
                    } else {
                        // money is odd so round down
                        playerPoint = playerPoint = ((playerMoney - 1) / 2);
                    }

                    // Reset the money back to 0
                    playerMoney = 0;
                    currentSite = i;
                    newSiteSet = 1;

                } else if (strcmp(sitesAndMaxPlayers[currentSite + 1][0], stateMo) == 0 && siteMax2 > sitesCurrentPlayers[currentSite + 1] && newSiteSet == 0) {
                    // Check if Mo is the next site and if so, move to it and add money
                    playerMoney = playerMoney + 3;
                    currentSite = currentSite + 1;
                    newSiteSet = 1;
                } 
            }

            // If nothing was set, look for closest V1, V2 or ::
            if (newSiteSet == 0) {
                // Go through the for loop and check for V1, v2 or ::
                for (int i = currentSite + 1; i <= barrierList[nextBarrier]; i++) {
                    int siteMax = strtol(sitesAndMaxPlayers[i][1], (char **)NULL, 10);
                    if (strcmp(sitesAndMaxPlayers[i][0], stateV1) == 0 && newSiteSet == 0 && siteMax > sitesCurrentPlayers[i]) {
                        // Add V1 and move to site
                        totalV1 = totalV1 + 1;
                        currentSite = i;
                        newSiteSet = 1;

                    } else if (strcmp(sitesAndMaxPlayers[i][0], stateV2) == 0 && newSiteSet == 0 && siteMax > sitesCurrentPlayers[i]) {
                        // Add V1 and move to site
                        totalV2 = totalV2 + 1;
                        currentSite = i;
                        newSiteSet = 1;

                    } 
                    
                }

                // Move to barrier
                if (newSiteSet == 0) {
                    newSiteSet = 1;
                    currentSite = barrierList[nextBarrier];

                }
            }

            // Return the command back to the dealer
            printf("DO%d\n", currentSite);
            fflush(stdout);

        } else if (strcmp(dealerInput, early) == 0) {
            // break loop if dealer gives the input of early
            fprintf(stderr, "Early game over\n");
            return 5;

        } else if (strcmp(dealerInput, done) == 0) {
            // break loop if game is over
            return 0;

        } else if (dealerInput[0] == 'H' && dealerInput[1] == 'A' && dealerInput[2] == 'P') {
            // Process the hap message
            // first let's break up the message. Check for 4 commas
            int numberOfCommas = 0;
            for (int i = 3; i < strlen(dealerInput); i++) {
                char b = dealerInput[i];

                // check the character is an int
                if (isdigit(b) > 0) {
                    // do nothing. Just doesn't go to the else
                } else if (b == ',') {
                    // check if the char is a comma
                    numberOfCommas++;
                } else if (b == '0') {
                    // do nothing. Just doesn't go to the else
                }else {
                    // raise the communication error
                    fprintf(stderr, "Communications error\n");
                    return 6;
                }
            }

            // Check if the commas doesn't equal 4
            if (numberOfCommas != 4) {
                fprintf(stderr, "Communications error\n");
                return 6;
            }

            // Put the variables from the HAP into messages
            sscanf(dealerInput, "HAP%d,%d,%d,%d,%d", &playerMoveID, &currentSite, &additionalPoints, &moneyChange, &cardDrawn);

            // Check some of the parameters
            if (playerMoveID > playerCount || currentSite > numberOfSites) {
                fprintf(stderr, "Communications error\n");
                return 6;
            }

            // Erase the old spot and place the player at new spot
            for (int i = 0; i < playerCount; i++) {
                if (playerDisplay[i][currentSite * 3 - 1] == playerMoveID) {
                    playerDisplay[i][currentSite * 3 - 1] = ' ';
                }
            }

            // Erase the old spot and place the player at new spot
            char playerDisNum = playerMoveID + '0';
                for (int i = 0; i < playerCount; i++) {
                    if (playerDisplay[i][(pointStorage2[playerMoveID][1]) * 3] == playerDisNum) {
                        playerDisplay[i][(pointStorage2[playerMoveID][1]) * 3] = ' ';
                    }
                }

            // Now, let's update the board information
            sitesCurrentPlayers[oldSite] = sitesCurrentPlayers[oldSite] - 1;
            pointStorage2[playerMoveID][1] = currentSite;
            sitesCurrentPlayers[currentSite] = sitesCurrentPlayers[currentSite] + 1;
            pointStorage2[playerMoveID][2] = pointStorage2[playerMoveID][2] + additionalPoints;
            pointStorage2[playerMoveID][3] = pointStorage2[playerMoveID][3] + moneyChange;
            pointStorage2[playerMoveID][4] = cardDrawn;

            // Let's now update its spot on the board
            if (currentSite > 9) {
                // DO THIS                                                                                              // RIGHT HERE!!!!
            } else {
            
                // Go down the new spot checking for vacant spots
                
                int playerSymbolMove = 0;
                int spotFound = 0;
                // Go down the new spot checking for vacant spots
                for (int i = 0; i < currentMax + 1; i++) {
                    if (playerDisplay[currentMax - i][currentSite * 3] == ' ' && spotFound == 0) {
                        playerSymbolMove = playerMoveID + '0';
                        playerDisplay[i][currentSite * 3] = playerSymbolMove;
                        spotFound = 1;
                    }
                }
            }
        } else {
            // raise the communication error
            fprintf(stderr, "Communications error\n");
            return 6;

        }

       












        // free mallocs
        free (dealerInput);
    }


    // Free malloc
    free(path);


    

    return 0;

}