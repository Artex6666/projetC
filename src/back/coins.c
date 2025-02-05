#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define USER_DETAILS_PATH "features/usersDetails.txt"


int getUsersCoin(void) {
    FILE *fp = fopen(USER_DETAILS_PATH, "r");
    if (fp == NULL) {
        return 0;
    }
    char buffer[1024] = {0};
    size_t len = fread(buffer, 1, sizeof(buffer) - 1, fp);
    buffer[len] = '\0';
    fclose(fp);
    char *p = strstr(buffer, "coins:");
    if (p == NULL) {
        return 0;
    }
    int coins = 0;
    if (sscanf(p, "coins: %d;", &coins) != 1) {
        return 0;
    }
    return coins;
}

void addUsersCoin(int pieces){
    char buffer[1024];
    buffer[0] = '\0';
    FILE *fp = fopen(USER_DETAILS_PATH, "r");
    if (fp != NULL) {
        size_t len = fread(buffer, 1, sizeof(buffer) - 1, fp);
        buffer[len] = '\0';
        fclose(fp);
    }
    int currentCoins = 0;
    char *p = strstr(buffer, "coins:");
    if (p != NULL) {
        sscanf(p, "coins: %d;", &currentCoins);
    }
    currentCoins += pieces;
    char newCoinsStr[50];
    snprintf(newCoinsStr, sizeof(newCoinsStr), "coins: %d;", currentCoins);
    
    char newBuffer[1024];
    newBuffer[0] = '\0';
    if (p != NULL) {
        char *semicolon = strchr(p, ';');
        if (semicolon != NULL) {
            size_t prefixLen = p - buffer;
            strncpy(newBuffer, buffer, prefixLen);
            newBuffer[prefixLen] = '\0';
            strcat(newBuffer, newCoinsStr);
            strcat(newBuffer, semicolon + 1);
        } else {
            snprintf(newBuffer, sizeof(newBuffer), "%s%s", buffer, newCoinsStr);
        }
    } else {
        if (strlen(buffer) > 0) {
            snprintf(newBuffer, sizeof(newBuffer), "%s\n%s", buffer, newCoinsStr);
        } else {
            snprintf(newBuffer, sizeof(newBuffer), "%s", newCoinsStr);
        }
    }
    
    fp = fopen(USER_DETAILS_PATH, "w");
    if (fp == NULL) {
        fprintf(stderr, "Erreur d'ouverture du fichier %s pour écriture.\n", USER_DETAILS_PATH);
        return;
    }
    fputs(newBuffer, fp);
    fclose(fp);
}