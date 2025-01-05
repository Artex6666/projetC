#ifndef WORD_GAME_H
#define WORD_GAME_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_WORD_LENGTH 100
#define FILE_PATH "features/liste_mots.txt"

char* get_random_word();

int is_valid_word(const char* word);

void check_word(const char* guess, const char* secret, int* result);

void print_result(const char* guess, const int* result, int length);

#endif 
