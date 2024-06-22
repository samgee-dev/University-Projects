#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>


int main(int argc, char** argv) {
    // Check that there are the correct amount of args
    if (argc < 5) {
        //Output a usage message to stderr and return 1
        fprintf(stderr, "Usage: 2310dealer deck path p1 {p2}\n");
        return 1;
    }

    // Let's read the deck and the path
    FILE* deckName = fopen(argv[1], "r");
    FILE* pathName = fopen(argv[2], "r");

    // Check if the file does not exist
    if (deckName == NULL) {
        fprintf(stderr, "Error reading deck\n");
        return 2;
    }

    // All the variables which will be used in the loop
    int firstDeckValue = 0;
    int numberOfCards = 0;
    int contentInvalid = 1;
    int countedCards = 0;
    char* cardList = malloc(sizeof(char) * 100);

    // Check the path contains the correct content
    do {
        // Get the first character
        char c = getc(deckName);

        // Check if we are at the first character and see how many 
        // cards are there
        if (firstDeckValue == 0) {
            // Check if it is not a digit
            if (!(isdigit(c))) {
                contentInvalid = 1;
                break;
            }

            // Change from ascii to int
            c = c - '0';

            // Set the number of sites and set firstDeckValue to be 1
            numberOfCards = c;
            firstDeckValue = 1;
        } else if (c == 10 || feof(deckName)) {
            // //Do nothing for these ones
        } else {
            // Check that the letters are between the ASCII of A and E
            if (c < 65 || c > 69) {
                contentInvalid = 1;
                break;
            }

            // Add 1 to the countedCards
            countedCards++;
            strncat(cardList, &c, 1);

            // Check if the counted cards have been reached
            if (countedCards == numberOfCards) {
                contentInvalid = 0;
            }

            // Check if we are over the counted cards
            if (countedCards > numberOfCards) {
                contentInvalid = 1;
                break;
            }
        }
    } while (!feof(deckName));

    // Close the file
    fclose(deckName);

    // If there is an incorrect thing in the deck, this should be flagged
    if (contentInvalid == 1) {
        fprintf(stderr, "Error reading deck\n");
        return 2;
    }
    
    // Check if there is no file
    if (pathName == NULL) {
        fprintf(stderr, "Error reading path\n");
        return 3;
    }
    
    // Find the number of sites
    int numberOfSites;
    int firstNumber = 0;
    contentInvalid = 1;

    do {
        // Get the first character
        char c = getc(pathName);

        // break if c equals ; and its not the first number
        if (c == ';' && firstNumber != 0) {
            contentInvalid = 0;
            break;
        }

        // Check if c is not a digit and if so, break
        if (!(isdigit(c))) {
            contentInvalid = 1;
            break;
        }

        // Change from ascii to int
        c = c - '0';

        // Make a switch statement which sets the number of sites
        switch (firstNumber) {
            case 0:
                numberOfSites = c;
                break;
            default:
                numberOfSites = (numberOfSites * 10) + c;
                break;
        }

        // Add one to the heightCount for digits
        firstNumber++;

    } while(!feof(pathName));

    // close the file
    fclose(pathName);

    // check if contentInvalid was raised
    if (contentInvalid == 1) {
        fprintf(stderr, "Error reading path\n");
        return 3;
    }

    // Let's now set up the array of arrays that will be used to store the 
    // state and how many players can fit in
    //int** sitesAndMaxPlayers = malloc(sizeof(int*) * numberOfStates);
   // for (int i = 0; i < playerCount * 2; i++) {
  //      sitesAndMaxPlayers[i] = (int*)malloc(sizeof(int) * 2);
    //}
    char* sitesAndMaxPlayers[numberOfSites][2];
    //memset(sitesAndMaxPlayers, 32, 2);

    // All the variables which will be used in the loop
    FILE* pathName2 = fopen(argv[2], "r");
    int firstPathValue = 0;
    contentInvalid = 1;
    int currentStateNumber = 0;
    int isLetter = 1;
    char lastLetter;
    int checkMax = 0;
    int currentMax = 0;

    // Set up the malloc used for checking and sending
    char* currentState = malloc(sizeof(char) * 15);
    char* lastState = malloc(sizeof(char) * 30);
    char* pathToSend = malloc(sizeof(char) * 100);
    char maxToAdd[5];
    char* empty = "";
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

    // Check the path contains the correct content
    do {
        // Get the first character
        char c = getc(pathName2);

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

        } else if (c == 10 || feof(pathName2)) { 
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
    } while (!feof(pathName2));

    // close the file and free mallocs
    fclose(pathName2);
    free(currentState);
    free(lastState);
    //free(maxToAdd);

    // check if contentInvalid was raised
    if (contentInvalid == 1) {
        fprintf(stderr, "Error reading path\n");
        return 3;
    }

    // Let's make the playerPipe by making an array of arrays (from lecture)
    int playerCount = argc - 3;
    int** playerPipe = malloc(sizeof(int*) * playerCount * 2);
    for (int i = 0; i < playerCount * 2; i++) {
        playerPipe[i] = malloc(sizeof(int) * 2);

        // Make the pipe
        if (pipe(playerPipe[i]) == -1) {
            fprintf(stderr, "Error starting one of the players\n");
            return 4;
        }
    }

    // Now, let's start the forking process to create the players
    // Here, lets create the player
    char* playerCountNumber = malloc(sizeof(playerCount) * 3);
    snprintf(playerCountNumber, 10, "%d", playerCount);
    char* currentPlayerNumber = malloc(sizeof(playerCount) * 3);
    
    for (int i = 0; i < playerCount; i++) {

        // Here we fork, creating the child process
        pid_t pid = fork();
        if (pid == -1) {
            // if it equals -1, raise the Communication error
            fprintf(stderr, "Error starting process\n");
            return 4;

        } else if (pid == 0) {
            // Set up the pipes for the child process
            dup2(playerPipe[i * 2][0], 0);
            dup2(playerPipe[i * 2 + 1][1], 1);

            // Put the currentPlayerNumber into a string
            snprintf(currentPlayerNumber, 10, "%d", i);

            // Supress output to the stderr
            freopen("/dev/null", "w", stderr);

            // Convert the child process into a player
            int k = execlp(argv[i + 3], argv[i + 3], playerCountNumber, currentPlayerNumber, NULL);

            // Check the child process converts
            if (k == -1) {
                fprintf(stderr, "Error starting process\n");
                return 4;
            }
        }

        // code for the parent to execute
        //wpid = wait(&status);
        //printf("wpid: %d\n", wpid);
    }

   // FILE* file;

   // file = fdopen(playerPipe[1][0], "r");
   // char* test = malloc(sizeof(int) * 50);;
   // fgets(test, 100, file);
    
    //printf("%s\n", test);


   // char* dab = "dab\n";
   // FILE* sendPath;
   // sendPath = fdopen(playerPipe[0 * 2][1], "w");
   // int check = fprintf(sendPath, "%s", dab);

   // printf("%d\n", check);
    //int check2 = fflush(sendPath);
  //  printf("%d\n", check2);

    

    // free the malloc
    free(playerCountNumber);
    free(currentPlayerNumber);

    // Let's make the malloc that will store the player's under the board
    //int** playerUnder = malloc(sizeof(int*) * playerCount);
    //for (int i = 0; i < playerCount; i++) {
    //    playerUnder[i] = (int*)malloc(sizeof(int) * numberOfSites);
    //}

    // Let's make the array which will store the player's
    char* playerUnder[playerCount - 1][numberOfSites - 1];
    char playerDisplay[playerCount - 1][numberOfSites * 3];

    // Add something to playerUnder so my code doesn't crash. I just don't even 
    // know why
    playerUnder[0][0] = "don't crash";
    char* savingMyCode = playerUnder[0][0];
    if (savingMyCode == 0) {
        // do nothing
    }

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

    // Send the path to the players
    char terminator = '\n';
    strncat(pathToSend, &terminator, 1);
    for (int i = 0; i < playerCount; i++) {
        FILE* sendPath;
        sendPath = fdopen(playerPipe[i * 2][1], "w");
        fprintf(sendPath, "%s", pathToSend);
        fflush(sendPath);
    }

    
    // Free the path malloc
    free(pathToSend);

    // make variables to use in loop
    int isGameOver = 0;
    int playerTurn = 0;
    int playerValidCheck = 1;
    int newSite = 0;
    int pointStorage[playerCount - 1][8];
    int pointStorage2[playerCount - 1][8];
    int pointStorage3[playerCount - 1][8];

    // Save my code
    if (pointStorage[0][0] == 0 && pointStorage2[0][0] == 0) {
        // do nothing
    }

    // Let's fill the storage with blank spaces
    // Order is playerID, site, points, money, card, V1, V2
    for (int i = 0; i < playerCount; i++) {
        for (int j = 0; j < 8; j++) {
            pointStorage3[i][j] = '0';
        }
    }

    // Let's make a loop which will only be broken if the game ends
    while (isGameOver == 0) {
        // print the path using a for loop
        printf("%s", ":: ");
        for (int i = 1; i < numberOfSites; i++) {
            printf("%s", sitesAndMaxPlayers[i][0]);
            printf(" ");
        }
        printf("\n");

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
                printf("%c", playerDisplay[i][j]);
            }
            printf("\n");
        }

        // Check who's turn it is in a for loop
        int playerObtained = 0;
        int playerX = 0;
        int playerY = 0;
        for (int j = 0; j < numberOfSites * 3; j++) {
            for (int i = 0; i < playerCount; i++) {
                // Find a spot that is not taken by a space. Start
                // at the bottom left
                if (isdigit(playerDisplay[playerCount - 1 - i][j]) > 0) {
                    if (playerObtained == 0) {
                        playerTurn = playerDisplay[playerCount - i - 1][j] - '0';                                  // THIS MIGHT FAIL
                        playerObtained = 1;

                        playerY = playerCount - i - 1;
                        playerX = j;

                        // Add check if the players are at the final barrier
                        if (j == (numberOfSites * 3) - 1) {
                            //Sprintf("game over\n");
                            isGameOver = 1;
                        }

                        // Check if any players are still in the first column
                        if (j != 0) {
                            playerValidCheck = 0;
                        }

                        // check if there is a number next to it on the right
                        if (isdigit(playerDisplay[playerCount - 1 - i][j + 1])) {
                            playerTurn = (playerTurn * 10) + playerDisplay[playerCount - i][j + 1];
                        }
                    }
                }
            }
        }

        // check if gameOver was flagged, break
        if (isGameOver == 1) {
            // Print done to all children
            for (int i = 0; i < playerCount; i++) {
                FILE* sendEnd;
                sendEnd = fdopen(playerPipe[i * 2][1], "w");
                fprintf(sendEnd, "%s", "DONE\n");
                fflush(sendEnd);
            }
            
            break;
        }

        //printf("player: %d\n", playerTurn);

        // Let the player know its their turn by sending code and flushing
        FILE* sendCommand;
        sendCommand = fdopen(playerPipe[playerTurn * 2][1], "w");
        fprintf(sendCommand, "%s", "YT\n");
        fflush(sendCommand);

        // Receive move from player
        FILE* receiveCommand;
        receiveCommand = fdopen(playerPipe[playerTurn * 2 + 1][0], "r");
        char* playerMove = malloc(sizeof(int) * 50);;
        fgets(playerMove, 100, receiveCommand);

        // If the player is still at the start position, check if it is valid
        newSite = 0;
        if (playerValidCheck == 1) {
            // Check if the first char is a ^ and if it is not, return error. Also 
            // check DO
            if (playerMove[0] != '^' || playerMove[1] != 'D' || playerMove[2] != 'O') {
                for (int i = 0; i < playerCount; i++) {
                    FILE* sendEarly;
                    sendEarly = fdopen(playerPipe[i * 2][1], "w");
                    fprintf(sendEarly, "%s", "DONE\n");
                    fflush(sendEarly);
                }

                fprintf(stderr, "Communications error\n");
                return 5;
            }

            // check the number at the end is a digit
            for (int i = 1; i < (strlen(playerMove) - 3); i++) {
                if (i == 0) {
                    // Check if it is not a digit. If so, raise error
                    if (isdigit(playerMove[2 + i]) == 0) {
                        for (int i = 0; i < playerCount; i++) {
                            FILE* sendEarly;
                            sendEarly = fdopen(playerPipe[i * 2][1], "w");
                            fprintf(sendEarly, "%s", "DONE\n");
                            fflush(sendEarly);
                        }
                        
                        fprintf(stderr, "Communications error\n");
                        return 5;
                    } 

                    // Set the site move
                    newSite = playerMove[2] - '0';

                } else {
                    if (isdigit(playerMove[2 + i]) == 0) {
                        for (int i = 0; i < playerCount; i++) {
                            FILE* sendEarly;
                            sendEarly = fdopen(playerPipe[i * 2][1], "w");
                            fprintf(sendEarly, "%s", "DONE\n");
                            fflush(sendEarly);
                        }
                        
                        fprintf(stderr, "Communications error\n");
                        return 5;
                    }

                    newSite = (newSite * 10) + (playerMove[2 + i] - '0');
                } 
            }
        } else {
            // Goes here if we are not in the first column
            for (int i = 0; i < (strlen(playerMove) - 3); i++) {
                if (i == 0) {
                    // Check if it is not a digit. If so, raise error
                    if (isdigit(playerMove[2]) == 0) {
                        for (int i = 0; i < playerCount; i++) {
                            FILE* sendEarly;
                            sendEarly = fdopen(playerPipe[i * 2][1], "w");
                            fprintf(sendEarly, "%s", "DONE\n");
                            fflush(sendEarly);
                        }
                        
                        fprintf(stderr, "Communications error\n");
                        return 5;
                    } 

                    // Get new site
                    sscanf(playerMove, "DO%d", &newSite);

                } else {
                    if (isdigit(playerMove[1 + i]) == 0) {
                        for (int i = 0; i < playerCount; i++) {
                            FILE* sendEarly;
                            sendEarly = fdopen(playerPipe[i * 2][1], "w");
                            fprintf(sendEarly, "%s", "DONE\n");
                            fflush(sendEarly);
                        }
                        
                        fprintf(stderr, "Communications error\n");
                        return 5;
                    }

                    // Get new site 
                    sscanf(playerMove, "DO%d", &newSite);
                } 
            }
        } 

        // Update current site
        int oldSite = pointStorage3[playerTurn][1] - '0';

        // check if the site wasn't changed
        if (newSite <= oldSite) {
            for (int i = 0; i < playerCount; i++) {
                FILE* sendEarly;
                sendEarly = fdopen(playerPipe[i * 2][1], "w");
                fprintf(sendEarly, "%s", "DONE\n");
                fflush(sendEarly);
            }

            fprintf(stderr, "Communications error\n");
            return 5;
        }

        // If there was no error, let's find the new position
        pointStorage3[playerTurn][1] = newSite;

        // Put the new player position on the board
        if (newSite > 9) {
            // DO THIS                                                                                              // RIGHT HERE!!!!
        } else {
            // Erase the old spot and place the player at new spot
            playerDisplay[playerY][playerX] = ' ';
            
            int playerSymbolMove = 0;
            int spotFound = 0;
            // Go down the new spot checking for vacant spots
            for (int i = 0; i < currentMax + 1; i++) {
                if (playerDisplay[currentMax - 1 - i][newSite * 3 - 1] == ' ' && spotFound == 0) {
                    playerSymbolMove = playerTurn + '0';
                    playerDisplay[i][newSite * 3] = playerSymbolMove;
                    spotFound = 1;
                }
            }
        }
        
        // Extract all the player information and make variables
        int moneyChange = 0;
        int pointChange = 0;
        int cardDrawn = 0;

        // Now, let's get the site that this is from
        if (strcmp(sitesAndMaxPlayers[newSite][0], stateDo) == 0) {                                           
            // Compute all the things to return to player for Do
            moneyChange = 0 - pointStorage3[playerTurn][3];

            // first convert all money to points
            if (pointStorage3[playerTurn][3] % 2 == 0) {
                // money is even so don't round
                pointChange = pointChange + (pointStorage3[playerTurn][3] / 2);
            } else {
                // money is odd so round down
                pointChange = pointChange = ((pointStorage3[playerTurn][3] - 1) / 2);
            }
            pointStorage3[playerTurn][2] = pointStorage3[playerTurn][2] + pointChange;
            pointStorage3[playerTurn][3] = 0;

        } else if (strcmp(sitesAndMaxPlayers[newSite][0], stateMo) == 0) {
            // Compute all the things to return to player for Mo
            moneyChange = 3;
            pointStorage3[playerTurn][3] = pointStorage3[playerTurn][3] + 3;

        } else if (strcmp(sitesAndMaxPlayers[newSite][0], stateRi) == 0) {                   // hmmmmmmm x
            // Compute all the things to return to player for Ri


        } else if (strcmp(sitesAndMaxPlayers[newSite][0], stateV1) == 0) {
            // Compute all the things to return to player for V1
            pointStorage3[playerTurn][5] = pointStorage3[playerTurn][5] + 1;

        } else if (strcmp(sitesAndMaxPlayers[newSite][0], stateV2) == 0) {
            // Compute all the things to return to player for V2
            pointStorage3[playerTurn][6] = pointStorage3[playerTurn][6] + 1;

        }

        // Send message to the other players containing this ones move
        for (int i = 0; i < playerCount; i++) {
            FILE* moveMade;
            moveMade = fdopen(playerPipe[i * 2][1], "w");
            fprintf(moveMade, "HAP%d,%d,%d,%d,%d\n", playerTurn, newSite, pointChange, moneyChange, cardDrawn);
            fflush(moveMade);
        }

        // print player thing to out Order is playerID, site, points, money, card, V1, V2
        printf("Player %d Money=%d V1=%d V2=%d Points=%d A=%d B=%d C=%d D=%d E=%d\n", playerTurn, pointStorage3[playerTurn][3], pointStorage3[playerTurn][5] - '0', 
        pointStorage3[playerTurn][6] - '0', pointStorage3[playerTurn][2], 0, 0, 0, 0, 0);

        // free mallocs
        free (playerMove);
    }

    //for (int i = 0; i < numberOfSites; i++) {
    //    for (int j = 0; j < 2; j++) {
    //        printf("%s", sitesAndMaxPlayers[i][j]);
    //    }
    //    printf("\n");
    //}
    

    // free the playerPipe malloc using a for loop
    for (int i = 0; i < playerCount * 2; ++i) {
        free(playerPipe[i]);
    }
    free(playerPipe);

    // free the playerUnder malloc using a for loop

    //for (int i = 0; i < playerCount; ++i) {
    //    free(playerUnder[i]);
    //}
    //free(playerUnder);

    // free mallocs which were used throughout
    free(cardList);
    //free(test);

    // Close the child processes
    int wpid, status;
    while ((wpid = wait(&status)) > 0);

    // print player score
    printf("Scores: ");
    printf("%d", pointStorage3[0][2]);
    for (int i = 1; i < playerCount; i++) {
        printf("%d", pointStorage3[i][2]);
        printf(",");
    }
    printf("\n");

    // Normal end
    return 0;

}