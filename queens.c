#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define N 8

#define PRINT(x)    for (int i = 0; i < N; i++, puts(""))               \
                        for (int j = 0; j < N; j++)                     \
                            printf("[%c]", x[i][j] == 0 ? ' ' : x[i][j])\

#define TO_BOARD(x) for (int i = 0; i < N; i++)                         \
                        board[i][x[i] - '0' - 1] = 'X'                  \

#define TO_STATE(x) for (int i = 0; i < N; i++)                         \
                        for (int j = 0; j < N; j++)                     \
                            if (board[i][j])                            \
                                x[i] = j + '0' + 1                      \

#define VERIFY(x)   __extension__({                                     \
                        int flag = 1;                                   \
                        if (strlen(x) != N)                             \
                            flag = 0;                                   \
                        for (int i = 0; i < N; i++)                     \
                            if (x[i] - '0' > N || x[i] - '0' <= 0)      \
                                flag = 0;                               \
                        flag;                                           \
                    })
#define CLEAR(x)    for (int i = 0; i < N; i++)                         \
                        for (int j = 0; j < N; j++)                     \
                            x[i][j] = 0                                 \

static short board[N][N];

int check_board(char *state)
{
    int pool = 0;
    for (int i = 0; i < N; i++) {
        int j = state[i] - '0';

        board[i][j - 1] = 'O';

        // vertical
        for (int c = 0; c < N; c++)
            if (board[c][j - 1] == 'X')
                pool++;

        // horizontal
        for (int c = 0; c < N; c++)
            if (board[i][c] == 'X')
                pool++;
        
        // NW diag
        for (int x = i, y = j - 1; x >= 0 && y >= 0; x--, y--)
            if (board[x][y] == 'X')
                pool++;

        // NE diag
        for (int x = i, y = j - 1; x >= 0 && y < N; x--, y++)
            if (board[x][y] == 'X')
                pool++;

        // SE diag
        for (int x = i, y = j - 1; x < N && y < N; x++, y++)
            if (board[x][y] == 'X')
                pool++;
        
        // SW diag
        for (int x = i, y = j - 1; x < N && y >= 0; x++, y--)
            if (board[x][y] == 'X')
                pool++;

        board[i][j - 1] = 'X';

    }
    return pool;
}

#define RANDOM(x, y) ((rand() % (y - x + 1)) + x)
#define MAX 1000

void generandom(char strings[MAX][N + 1], int x) {
    for (int i = 0; i < x; i++)
        for (int j = 0; j < N; j++)
            strings[i][j] = RANDOM(1, N) + '0';
}

int main(void)
{
    // seed
    srand(time(0));

    // create test state
    char base_state[N + 1] = "87654321";
    assert(VERIFY(base_state));
    TO_BOARD(base_state);

    // verify roundabout
    char test_state[N + 1] = "";
    TO_STATE(test_state);
    if (strcmp(test_state, base_state))
        printf("Fail: actually got %s\n", test_state);
    TO_BOARD(test_state);

    // checking
    char cool_state[N + 1] = "36814752";
    CLEAR(board);
    TO_BOARD(cool_state);
    assert(!check_board(cool_state));
    PRINT(board);
    puts("");

    // generating random boys
    char strings[MAX][N + 1] = { 0 };
    generandom(strings, MAX);
    for (int i = 0; i < MAX; i++) {
        char phony[N + 1] = "";
        strcpy(phony, strings[i]);
        CLEAR(board);
        TO_BOARD(phony);
        int fitness = check_board(phony);
#if 0
            printf("Fitness: %d\n", fitness);
            PRINT(board);
            puts("");
            usleep(1000 * 100);
#endif
        if (!fitness) {
            printf("%s master race\n", phony);
            exit(EXIT_SUCCESS);
        }
    }

    puts("heh...");
    return 0;
}



















