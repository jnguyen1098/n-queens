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

int check_board(short board[N][N], char *state)
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
#define MAX 100000

typedef struct {
    char str[N + 1];
    int fitness;
} Result;

void generandom(char strings[MAX][N + 1], int x) {
    for (int i = 0; i < x; i++)
        for (int j = 0; j < N; j++)
            strings[i][j] = RANDOM(1, N) + '0';
}

int cmpfit(const void *a, const void *b)
{
    Result *_a = (Result *)a;
    Result *_b = (Result *)b;

    return _a->fitness - _b->fitness;
}

void swap(char *str, int x, int y)
{
    if (str[x] == str[y]) return;

    char tmp = str[x];
    str[x] = str[y];
    str[y] = tmp;
}

void reverse(char str[N + 1]) {
    char tmp[N + 1];
    for (int i = 0; i < N + 1; i++)
        tmp[i] = str[i];
    for (int i = N; i >= 0; i--)
        str[i] = tmp[i];
}

void scramble(char str[N + 1]) {
    char tmp[N + 1];
    
    for (int i = 0; i < N + 1; i++)
        tmp[i] = str[i];
        
    for (int i = 0;;i++) {
        char tmpc;
        int rando;
        
        do {
            rando = RANDOM(0, N - 1);
            tmpc = tmp[rando];
        } while (tmpc == 0);
        
        str[i] = tmpc;
        tmp[rando] = 0;

        int cont = 0;
        for (int j = 0; j < N + 1; j++)
            if (tmp[j] != 0)
                cont = 1;
        if (!cont)
            break;
    }
}

void sex(char mom[N + 1], char dad[N + 1], char kid[N + 1])
{
    int bound = (N + 1) / 2;
    for (int i = 0; i < bound; i++) {
        kid[i] = mom[i];
    }

    for (int i = bound; i < N + 1; i++) {
        kid[i] = dad[i];
    }

    if (RANDOM(0, 1000) < 100) {
        if (RANDOM(0, 1000) < 500) {
            kid[RANDOM(0, N - 1)] = RANDOM(0, N) + '0';
            scramble(kid);
        } else {
            reverse(kid);
        }
    }
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
    assert(!check_board(board, cool_state));
    PRINT(board);
    puts("");

    // generating random individuals
    char strings[MAX][N + 1] = { 0 };
    generandom(strings, MAX);
    for (int i = 0; i < MAX; i++) {
        char phony[N + 1] = "";
        strcpy(phony, strings[i]);
        CLEAR(board);
        TO_BOARD(phony);
        int fitness = check_board(board, phony);
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

    // store fitnesses
    Result results[MAX] = { 0 };

    Result champion = { 0 };
    strcpy(champion.str, "");
    champion.fitness = 1000;

    for (int gen = 0; gen < 10000; gen++) {

        for (int i = 0; i < MAX; i++) {
            strcpy(results[i].str, strings[i]);
            CLEAR(board);
            TO_BOARD(strings[i]);
            results[i].fitness = check_board(board, strings[i]);
        }

        qsort(results, MAX, sizeof(Result), cmpfit);

        int sum = 0;
        for (int i = 0; i < MAX; i++) {
            //printf("%s (%d)\n", results[i].str, results[i].fitness);
            sum += results[i].fitness;
        }
        printf("Generation #%5d, avg %2d, best %2d (%s)\n",
                gen + 1, sum / MAX, champion.fitness, champion.str);

        if (results[0].fitness < champion.fitness) {
            strcpy(champion.str, results[0].str);
            champion.fitness = results[0].fitness;
        }

#if 0
        puts("Champion:");
        for (int i = 0; i < 1; i++)
            printf("%s (%d)\n", results[i].str, results[i].fitness);
#endif

        for (int i = 0; i < MAX; i++)
            strings[i][0] = 0;

#define UBOUND ((MAX / 10) * 8)

        for (int i = 0; i < MAX; i++) {
            char tmp[N + 1] = "";
            sex(results[RANDOM(0, UBOUND)].str, results[RANDOM(0, UBOUND)].str, tmp);
            strcpy(strings[i], tmp);
        }

        if (champion.fitness == 0)
            break;
    }

    puts("Champion:");
    printf("%s (%d)\n", champion.str, champion.fitness);

    CLEAR(board);
    TO_BOARD(champion.str);
    PRINT(board);

    return 0;
}



















