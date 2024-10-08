#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

bool intArrCmp(int size, int arr1[size], int arr2[size]) {
    for (int i = 0; i < size; i++)
        if (arr1[i] == -1) return arr1[i] == arr2[i];
        else if (arr1[i] != arr2[i]) return false;
    
    return true;
}

void intArrCpy(int size, int arr1[size], int arr2[size]) {
    for (int i = 0; i < size; i++) { 
        arr1[i] = arr2[i];
        if (arr2[i] == -1) break;
    }
}

int indexOfArr(int size1, int size2, int base[size1][size2], int arr[size2]) {
    for (int i = 0; i < size1; i++)
        if (intArrCmp(size2, base[i], arr)) return i;
    
    return -1;
}

int main() {
    int states_size = 0;
    printf("Enter number of states (> 0): ");
    scanf("%d", &states_size);

    int symbol_count = 0;
    printf("Enter number of symbols (> 0): ");
    scanf("%d", & symbol_count);
    
    int nfa_transition_table[states_size][symbol_count][states_size+1];

    for (int i = 0; i < states_size; i++) {
        printf("For State q%d:\n", i);

        for (int j = 0; j < symbol_count; j++) {
            printf("    %c: ", 'a'+j);
            
            for (int k = 0; true; k++) {
                int next_state;
                scanf("%d", &next_state);
                nfa_transition_table[i][j][k] = next_state;
                if (next_state < 0) break;

                for (int l = k; l > 0 && nfa_transition_table[i][j][l-1] > nfa_transition_table[i][j][l]; l--) {
                    int swap_temp = nfa_transition_table[i][j][l-1];
                    nfa_transition_table[i][j][l-1] = nfa_transition_table[i][j][l];
                    nfa_transition_table[i][j][l] = swap_temp;
                }
            }
        }
    }
    
    int dfa_transition_table[2*states_size][symbol_count];
    int state_names[2*states_size][states_size+1];

    state_names[0][0] = 0;
    state_names[0][1] = -1;
    int states_discovered = 1;

    for (int i = 0; i < states_discovered; i++) {
        for (int symbol = 0; symbol < symbol_count; symbol++) {
            int top = 0;
            int new_state[states_size+1];
            for (int j = 0; true; j++) {
                // states in state
                int state = state_names[i][j];
                if (state < 0) break;

                for (int k = 0; true; k++) {
                    int s = nfa_transition_table[state][symbol][k];
                    
                    if (s < 0) break;
                    new_state[top++] = s;

                    for (int l = top-1; l > 0 && new_state[l-1] > new_state[l]; l--) {
                        int swap_temp = new_state[l-1];
                        new_state[l-1] = new_state[l];
                        new_state[l] = swap_temp;
                    }
                }
            }
            new_state[top++] = -1;

            int index = indexOfArr(2*states_size, states_size+1, state_names, new_state);
            if (index == -1) {
                index = states_discovered++;
                intArrCpy(2*states_size, state_names[index], new_state);
            }

            dfa_transition_table[i][symbol] = index;
        }
        
    }

    for (int i = 0; i < symbol_count; i++) printf("\t%c", 'a'+i);
    printf("\n");

    for (int i = 0; i < states_discovered; i++)
    {
        printf("q%d\t", i);
        for (int j = 0; j < symbol_count; j++)
        {
            printf("q%d\t", dfa_transition_table[i][j]);
        }
        printf("\n");
        
    }
    

    return 0;
}