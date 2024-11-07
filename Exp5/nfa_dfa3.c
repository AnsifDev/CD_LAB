#include <stdio.h>
#include <stdbool.h>

double pow(double x, double y) {
    int result = 1;
    for (int i = 0; i < y; i++) result *= x;
    return result;
}

int index_of(int size, int array[size], int value) {
    for (int i = 0; i < size; i++) if (array[i] == value) return i;
    return -1;
}

void extract_old_name(int id, int max, char* string) {
    string[0] = '\0';
    for (int i = 0; i < max; i++) {
        if ((id & (1 << (max-1-i))) == 0) continue;
        sprintf(string, "%sq%d", string, i);
    }
}

int main() {
    int nfa_states_size = 0;
    printf("Enter number of states (> 0): ");
    scanf("%d", &nfa_states_size);

    int symbols_size = 0;
    printf("Enter number of symbols (> 0): ");
    scanf("%d", & symbols_size);

    int nfa[symbols_size][nfa_states_size];
    for (int state = 0; state < nfa_states_size; state++) {
        printf("For State q%d:\n", state);

        for (int symbol = 0; symbol < symbols_size; symbol++) {
            printf("    %c: ", 'a'+symbol);

            nfa[symbol][state] = 0;
            for (int k = 0; true; k++) {
                int state_id;
                scanf("%d", &state_id);
                if (state_id < 0) break;

                nfa[symbol][state] |= 1 << nfa_states_size-1-state_id;
            }
        }
    }

    printf("Enter the final states: ");

    int nfa_final_states = 0;
    for (int k = 0; true; k++) {
        int state_id;
        scanf("%d", &state_id);
        if (state_id < 0) break;

        nfa_final_states |= 1 << nfa_states_size-1-state_id;
    }

    int dfa_states_size = pow(2, nfa_states_size)-1;
    int dfa_discovered_states = 0;
    int state_names[dfa_states_size];
    int dfa[dfa_states_size][symbols_size];

    state_names[dfa_discovered_states++] = 1 << nfa_states_size-1;

    for (int i = 0; i < dfa_discovered_states; i++) {
        for (int symbol = 0; symbol < symbols_size; symbol++) {
            int new_state_name = 0;
            for (int state = 0; state < nfa_states_size; state++) {
                if ((state_names[i] & (1 << nfa_states_size-1-state)) == 0) continue; // No need to include

                new_state_name |= nfa[symbol][state];
            }

            int index = index_of(dfa_discovered_states, state_names, new_state_name);
            if (index == -1) {
                index = dfa_discovered_states++;
                state_names[index] = new_state_name;
            }

            dfa[i][symbol] = index;
        }
    }

    printf("Info: Symbols of Transition Table are renamed as follows:\n");
    for (int i = 0; i < dfa_discovered_states; i++)
    {
        char old_name[25];
        extract_old_name(state_names[i], nfa_states_size, old_name);
        printf("Q%d -> %s\n", i, old_name);
    }
    

    printf("DFA Transition Table:\n");
    for (int i = 0; i < symbols_size; i++) printf("\t%c", 'a'+i);
    printf("\n");

    for (int state = 0; state < dfa_discovered_states; state++)
    {
        if ((state_names[state] & nfa_final_states) > 0) printf("*");
        printf("Q%d\t", state);
        for (int symbol = 0; symbol < symbols_size; symbol++)
        {
            printf("Q%d\t", dfa[state][symbol]);
        }
        printf("\n");
    }

    return 0;
}