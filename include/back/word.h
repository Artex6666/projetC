#ifndef WORD_H
#define WORD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <windows.h>

#define MAX_WORD_LENGTH 100
#define FILE_PATH "assets/words/liste_mots.txt"

char* get_random_word(const char* difficulty);
int is_valid_word(const char* word);
char check_word(const char* guess, const char* secret, int* result);

#endif