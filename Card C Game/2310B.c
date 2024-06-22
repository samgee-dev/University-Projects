#include <stdio.h> 

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
    if (playerCount < 1) {
        fprintf(stderr, "Invalid player count\n");
        return 2;
    }

    // check the player id is correct
    int playerID;
    sscanf(argv[2], "%d", &playerID);
    if (0 > playerID || playerID >= playerCount) {
        fprintf(stderr, "Invalid ID\n");
        return 3;
    }

    return 0;
}