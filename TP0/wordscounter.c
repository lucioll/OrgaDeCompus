#include "wordscounter.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define STATE_WAITING_WORD 0
#define STATE_IN_WORD 1
#define STATE_FINISHED 2
#define DELIM_WORDS " ,.;:\n"

// Compara el caracter leído c y define el nuevo estado.
static char wordscounter_next_state(wordscounter_t *self, char state, char c);

void wordscounter_create(wordscounter_t *self) {
    self->words = 0;
}

void wordscounter_destroy(wordscounter_t *self) {
    //do nothing
}

size_t wordscounter_get_words(wordscounter_t *self) {
    return self->words;
}

void wordscounter_process(wordscounter_t *self, FILE *text_file) {
    char state = STATE_WAITING_WORD;
    do {
        int c = getc(text_file);
        state = wordscounter_next_state(self, state, c);
    } while (state != STATE_FINISHED);
}

static char wordscounter_next_state(wordscounter_t *self, char state, char c) {
    char next_state = state;

    if (state == STATE_WAITING_WORD) {
        if (c == EOF) { 
            next_state = STATE_FINISHED;
        } else if (strchr(DELIM_WORDS, c) == NULL) {
            next_state = STATE_IN_WORD;
        }
    } else if (state == STATE_IN_WORD) {
        if (c == EOF) { 
            next_state = STATE_FINISHED;
            self->words++;
        } else if (strchr(DELIM_WORDS, c) != NULL) {
            self->words++;
            next_state = STATE_WAITING_WORD;
        }
    }

    return next_state;
}

