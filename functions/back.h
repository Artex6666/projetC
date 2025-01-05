#ifndef WORD_GAME_H
#define WORD_GAME_H

char* get_random_word(const char* difficulty);

int is_valid_word(const char* word);

void check_word(const char* guess, const char* secret, int* result);

#endif 
