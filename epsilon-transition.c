
// studnet names : Abdellah Maamra, Abdelouahed Fellah

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_STATES 20
#define MAX_SYMBOLS 10

typedef struct
{
    int num_states;
    int num_symbols;
    char symbols[MAX_SYMBOLS];
    bool transitions[MAX_STATES][MAX_SYMBOLS + 1][MAX_STATES]; // last symbol index is for epsilon
    bool is_final[MAX_STATES];
    int start_state;
} NFA;

// Task 1: Read the NFA
void read_nfa(NFA *nfa)
{
    if (scanf("%d", &nfa->num_states) != 1)
        return;
    if (scanf("%d", &nfa->num_symbols) != 1)
        return;

    char sym_str[MAX_SYMBOLS];
    if (scanf("%s", sym_str) != 1)
        return;
    for (int i = 0; i < nfa->num_symbols; i++)
    {
        nfa->symbols[i] = sym_str[i];
    }

    memset(nfa->transitions, 0, sizeof(nfa->transitions));

    for (int i = 0; i < nfa->num_states; i++)
    {
        for (int j = 0; j <= nfa->num_symbols; j++)
        {
            int next_state;
            while (scanf("%d", &next_state) == 1 && next_state != -1)
            {
                if (next_state >= 0 && next_state < nfa->num_states)
                {
                    nfa->transitions[i][j][next_state] = true;
                }
            }
        }
    }

    if (scanf("%d", &nfa->start_state) != 1)
        return;

    int num_final;
    if (scanf("%d", &num_final) != 1)
        return;
    for (int i = 0; i < num_final; i++)
    {
        int f;
        if (scanf("%d", &f) == 1)
        {
            if (f >= 0 && f < nfa->num_states)
            {
                nfa->is_final[f] = true;
            }
        }
    }
}

// Task 2: Calculate the epsilon-closure of each state
void calculate_epsilon_closure(NFA *nfa, bool closure[MAX_STATES][MAX_STATES])
{
    for (int i = 0; i < nfa->num_states; i++)
    {
        for (int j = 0; j < nfa->num_states; j++)
            closure[i][j] = false;
        closure[i][i] = true;

        bool changed = true;
        while (changed)
        {
            changed = false;
            for (int s = 0; s < nfa->num_states; s++)
            {
                if (closure[i][s])
                {
                    for (int next = 0; next < nfa->num_states; next++)
                    {
                        if (nfa->transitions[s][nfa->num_symbols][next] && !closure[i][next])
                        {
                            closure[i][next] = true;
                            changed = true;
                        }
                    }
                }
            }
        }
    }
}

// Task 3 & 4: Compute new transitions and final states
void convert_to_nfa(NFA *enfa, NFA *nfa, bool closure[MAX_STATES][MAX_STATES])
{
    nfa->num_states = enfa->num_states;
    nfa->num_symbols = enfa->num_symbols;
    memcpy(nfa->symbols, enfa->symbols, sizeof(enfa->symbols));
    nfa->start_state = enfa->start_state;
    memset(nfa->transitions, 0, sizeof(nfa->transitions));
    memset(nfa->is_final, 0, sizeof(nfa->is_final));

    for (int i = 0; i < nfa->num_states; i++)
    {
        for (int s = 0; s < nfa->num_states; s++)
        {
            if (closure[i][s] && enfa->is_final[s])
            {
                nfa->is_final[i] = true;
            }
        }

        for (int j = 0; j < nfa->num_symbols; j++)
        {
            for (int s1 = 0; s1 < nfa->num_states; s1++)
            {
                if (closure[i][s1])
                {
                    for (int s2 = 0; s2 < nfa->num_states; s2++)
                    {
                        if (enfa->transitions[s1][j][s2])
                        {
                            for (int s3 = 0; s3 < nfa->num_states; s3++)
                            {
                                if (closure[s2][s3])
                                {
                                    nfa->transitions[i][j][s3] = true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

// Task 5: Display the new automaton
void display_nfa(NFA *nfa, const char *title)
{
    printf("\n--- %s ---\n", title);
    printf("States: %d\n", nfa->num_states);
    printf("Symbols: ");
    for (int i = 0; i < nfa->num_symbols; i++)
        printf("%c ", nfa->symbols[i]);
    printf("\nStart State: %d\n", nfa->start_state);
    printf("Final States: ");
    for (int i = 0; i < nfa->num_states; i++)
    {
        if (nfa->is_final[i])
            printf("%d ", i);
    }
    printf("\nTransitions:\n");
    for (int i = 0; i < nfa->num_states; i++)
    {
        for (int j = 0; j < nfa->num_symbols; j++)
        {
            printf("  d(%d, %c) = { ", i, nfa->symbols[j]);
            bool first = true;
            for (int next = 0; next < nfa->num_states; next++)
            {
                if (nfa->transitions[i][j][next])
                {
                    if (!first)
                        printf(", ");
                    printf("%d", next);
                    first = false;
                }
            }
            printf(" }\n");
        }
    }
}

int main()
{
    NFA enfa = {0}, nfa = {0};
    bool closure[MAX_STATES][MAX_STATES];

    read_nfa(&enfa);
    calculate_epsilon_closure(&enfa, closure);

    printf("\nEpsilon Closures:\n");
    for (int i = 0; i < enfa.num_states; i++)
    {
        printf("  E-closure(%d) = { ", i);
        bool first = true;
        for (int j = 0; j < enfa.num_states; j++)
        {
            if (closure[i][j])
            {
                if (!first)
                    printf(", ");
                printf("%d", j);
                first = false;
            }
        }
        printf(" }\n");
    }

    convert_to_nfa(&enfa, &nfa, closure);
    display_nfa(&nfa, "NFA without Epsilon Transitions");

    return 0;
}
