#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define NUM_WORDS 12
#define NUM_PUZZLES 7
#define MAX_LOGBOOK 100

char *words[3][4] = {
    {"claim", "trait", "creek", "risky"},
    {"rogue", "blossom", "trench", "threaten"},
    {"warrior", "nepotism", "shocked", "freedom"}
};

char *passwords[8] = {"apple", "brick", "words", "train", "drone", "chair", "flute", "eagle"};

int fight_items[4][2] = {{1, 0}, {2, 0}, {3, 0}, {4, 0}};

char survivor_names[MAX_LOGBOOK][50];
char survivor_dates[MAX_LOGBOOK][30];
double survivor_times[MAX_LOGBOOK];
int log_count = 0;
int game_success = 0;
int lives = 0;

void waitForUser() {
    printf("\nPress Enter to continue...");
    while (getchar() != '\n');
    getchar();
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void endGame() {
    printf("\nYou have no lives left or ran out of time! Game over.\n");
    exit(0);
}

void loseLifeOrEnd() {
    lives--;
    if (lives <= 0) endGame();
    else printf("\nWrong answer! You have %d lives remaining.\n", lives);
    waitForUser();
}

void viewInventory() {
    printf("\nNote: The rank of your item indicates higher rarity for a lesser value.\n");
    printf("\nLevel  ~~  Rank of Item obtained\n");
    for (int i = 0; i < 4; i++) {
        printf("\n%d     ~~    %d\n", fight_items[i][0], fight_items[i][1]);
    }
}

void showLogbook() {
    printf("\n--- SURVIVOR'S LOGBOOK ---\n");
    if (log_count == 0) {
        printf("No survivors yet...\n");
        return;
    }
    for (int i = 0; i < log_count; i++) {
        printf("%d. %s - Time: %.2f seconds - Date: %s\n", i + 1, survivor_names[i], survivor_times[i], survivor_dates[i]);
    }
}

void shuffleWord(char *word, char *shuffled) {
    int len = strlen(word);
    int used[len];
    for (int i = 0; i < len; i++) used[i] = 0;

    for (int i = 0; i < len; i++) {
        int index;
        do {
            index = rand() % len;
        } while (used[index]);
        shuffled[i] = word[index];
        used[index] = 1;
    }
    shuffled[len] = '\0';
}

void shuffleArray(char *array[], int size) {
    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        char *temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

void playRoom(int room) {
    time_t start, end;
    double elapsed;
    
    retry_room:
    clearScreen();

    if (room == 1) {
        printf("\n--- Room 1: Unscramble the Words ---\n");
        char *flat_words[NUM_WORDS];
        int index = 0;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 4; j++) {
                flat_words[index++] = words[i][j];
            }
        }
        shuffleArray(flat_words, NUM_WORDS);
        start = time(NULL);

        for (int i = 0; i < 5; i++) {
            char shuffled[20];
            shuffleWord(flat_words[i], shuffled);
            printf("\nUnscramble this word: %s\n", shuffled);

            char userinput[20];
            scanf("%s", userinput);
            if (strcasecmp(userinput, flat_words[i]) == 0) {
                printf("\nCorrect!\n");
            } else {
                loseLifeOrEnd();
                goto retry_room;
            }

            end = time(NULL);
            elapsed = difftime(end, start);
            if (elapsed >= 45) endGame();
        }

        end = time(NULL);
        elapsed = difftime(end, start);
        if (elapsed <= 30) fight_items[0][1] = 1;
        else if (elapsed <= 45) fight_items[0][1] = 2;
        else fight_items[0][1] = 3;

        viewInventory();
        waitForUser();
    }

    if (room == 2) {
        printf("\n--- Room 2: Guess the 5-Letter Password ---\n");
        char *password = passwords[rand() % 8];
        int attempts = 7;

        start = time(NULL);
        while (attempts > 0) {
            char guess[10];
            printf("\nAttempts left: %d\nEnter your guess: ", attempts);
            scanf("%s", guess);

            if (strlen(guess) != 5) {
                printf("Password must be exactly 5 letters!\n");
                continue;
            }

            if (strcasecmp(guess, password) == 0) {
                printf("\nCorrect! Escaping Room 2...\n");
                break;
            } else {
                printf("\nIncorrect guess.\n");

                // Wordle-style feedback:
                for (int i = 0; i < 5; i++) {
                    int letter_found = 0;
                    for (int j = 0; j < 5; j++) {
                        if (tolower(guess[i]) == tolower(password[j])) {
                            letter_found = 1;
                            if (i == j) {
                                printf("Letter '%c' is in the password and in the correct position.\n", guess[i]);
                            } else {
                                printf("Letter '%c' is in the password but in the wrong position.\n", guess[i]);
                            }
                            break;
                        }
                    }
                    if (!letter_found) {
                        printf("Letter '%c' is not in the password.\n", guess[i]);
                    }
                }
            }

            attempts--;
            end = time(NULL);
            elapsed = difftime(end, start);
            if (elapsed >= 60) endGame();
            if (attempts == 0) {
                loseLifeOrEnd();
                goto retry_room;
            }
        }

        end = time(NULL);
        elapsed = difftime(end, start);
        if (elapsed <= 20) fight_items[1][1] = 1;
        else if (elapsed <= 40) fight_items[1][1] = 2;
        else fight_items[1][1] = 3;

        viewInventory();
        waitForUser();
    }

    if (room == 3) {
        printf("\n--- Room 3: Solve the Puzzles ---\n");

        char puzzles[NUM_PUZZLES][2][100] = {
            {"What has keys but can't open locks?", "keyboard"},
            {"What is so fragile that saying its name breaks it?", "silence"},
            {"What has to be filled before it can be used?", "cup"},
            {"What approaches but never occurs?", "tomorrow"},
            {"I have cities but no houses, forests but no trees, and rivers but no water. What am I?", "map"},
            {"The more you take, the more you leave behind. What am I?", "footsteps"},
            {"What comes once in a minute, twice in a moment, but never in a thousand years?", "m"}
        };

        int used[NUM_PUZZLES] = {0};
        start = time(NULL);

        for (int i = 0; i < NUM_PUZZLES; i++) {
            int k;
            do { k = rand() % NUM_PUZZLES; } while (used[k]);
            used[k] = 1;

            printf("\nPuzzle: %s\n", puzzles[k][0]);
            char userInput[50];
            scanf("%s", userInput);

            if (strcasecmp(userInput, puzzles[k][1]) == 0) {
                printf("Correct!\n");
            } else {
                loseLifeOrEnd();
                goto retry_room;
            }

            end = time(NULL);
            elapsed = difftime(end, start);
            if (elapsed >= 91) endGame();
        }

        end = time(NULL);
        elapsed = difftime(end, start);
        if (elapsed <= 45) fight_items[2][1] = 1;
        else if (elapsed <= 75) fight_items[2][1] = 2;
        else fight_items[2][1] = 3;

        viewInventory();
        waitForUser();
    }

    if (room == 4) {
        printf("\n--- Room 4: Find the Hidden Key ---\n");
        char *locations[] = {"desk", "bookshelf", "cabinet", "rug", "drawer", "trunk"};
        int keyLocation = rand() % 6, attempts = 5, found = 0;

        start = time(NULL);
        while (attempts > 0) {
            char choice[20];
            printf("\nLocations:\n");
            for (int i = 0; i < 6; i++) printf("%s\n", locations[i]);
            printf("\nChoose a location to search: ");
            scanf("%s", choice);

            if (strcasecmp(choice, locations[keyLocation]) == 0) {
                printf("\nYou found the key! Escaping Room 4...\n");
                found = 1;
                break;
            } else {
                printf("\nNo key here!\n");
                attempts--;
            }

            end = time(NULL);
            elapsed = difftime(end, start);
            if (elapsed >= 60) endGame();
        }

        if (!found) {
            loseLifeOrEnd();
            goto retry_room;
        }

        end = time(NULL);
        elapsed = difftime(end, start);
        if (elapsed <= 20) fight_items[3][1] = 1;
        else if (elapsed <= 40) fight_items[3][1] = 2;
        else fight_items[3][1] = 3;

        viewInventory();
        waitForUser();
    }

    if (room == 5) {
        printf("\n--- Room 5: The Final Battle ---\n");
        int totalBonus = fight_items[0][1] + fight_items[1][1] + fight_items[2][1] + fight_items[3][1];
        int winChance = 100 - totalBonus * 15;
        if (winChance < 5) winChance = 5;

        printf("\nYou encounter the Entity!\nDo you want to:\n1. Fight\n2. Hide\nChoose (1/2): ");
        int choice;
        scanf("%d", &choice);

        if (choice == 1) {
            printf("Your chance of victory is %d%%!\n", winChance);
            if (rand() % 100 < winChance) {
                printf("You defeated the Entity! You escaped!\n");
                game_success = 1;
            } else {
                printf("The Entity defeated you...\n");
                endGame();
            }
        } else if (choice == 2) {
            if (rand() % 2 == 0) {
                printf("You hid successfully and survived...\n");
                game_success = 1;
            } else {
                printf("The Entity found you! You're dead.\n");
                endGame();
            }
        } else {
            printf("\nInvalid choice. The Entity finds you!\n");
            endGame();
        }
    }
}

int main() {
    srand(time(NULL));
    int choice;

    do {
        clearScreen();
        printf("\n====== ESCAPE ROOM SIMULATOR ======\n");
        printf("1. View Survivor's Logbook\n");
        printf("2. Start Game\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            showLogbook();
            waitForUser();
        } else if (choice == 2) {
            char name[50];
            printf("\nEnter your name: ");
            scanf("%s", name);

            printf("\nSelect difficulty:\n1. Easy (3 lives)\n2. Medium (2 lives)\n3. Hard (1 life)\nChoice: ");
            int diff;
            scanf("%d", &diff);
            lives = (diff == 1) ? 3 : (diff == 2) ? 2 : 1;

            game_success = 0;

            time_t game_start = time(NULL);
            for (int i = 1; i <= 5; i++) {
                playRoom(i);
                if (lives <= 0 || game_success) break;
            }
            time_t game_end = time(NULL);
            double total_time = difftime(game_end, game_start);

            if (game_success) {
                printf("\nYou escaped successfully in %.2f seconds!\n", total_time);
                if (log_count < MAX_LOGBOOK) {
                    strcpy(survivor_names[log_count], name);
                    survivor_times[log_count] = total_time;

                    time_t now = time(NULL);
                    struct tm *local = localtime(&now);
                    strftime(survivor_dates[log_count], sizeof(survivor_dates[log_count]), "%Y-%m-%d", local);

                    log_count++;
                    FILE *logfile = fopen("srvlog.txt", "a");
                    if (logfile) {
                        if (log_count == 1) fprintf(logfile, "Name Time Date\n");
                        fprintf(logfile, "%-20s %6.2f %s\n", name, total_time, survivor_dates[log_count - 1]);
                        fclose(logfile);
                    }
                }
                waitForUser();
            }
        }

    } while (choice != 3);

    return 0;
}