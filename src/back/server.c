#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <../include/back/enet.h>
#include <time.h>

#define MAX_PLAYERS 4

typedef struct {
    ENetPeer *peer;
    double time_taken;
    int found;
} Player;

Player players[MAX_PLAYERS];
int num_players = 0;
clock_t start_time;

void broadcast(ENetHost *server, const char *message) {
    ENetPacket *packet = enet_packet_create(message, strlen(message) + 1, ENET_PACKET_FLAG_RELIABLE);
    enet_host_broadcast(server, 0, packet);
}

void serveur(int argc, char **argv) {
    if(enet_initialize() != 0) {
        fprintf(stderr, "Erreur d'initialisation d'ENet\n");
        return EXIT_FAILURE;
    }

    ENetAddress address;
    ENetHost *server;
    int port = 1234;
    int max_attempts = 10;
    int success = 0;

    for(int i = 0; i < max_attempts; i++) {
        address.host = ENET_HOST_ANY;
        address.port = port + i; // Essayer 1234, 1235, 1236, etc.

        server = enet_host_create(&address, MAX_PLAYERS, 2, 0, 0);
        if(server) {
            printf("Serveur démarré sur le port %d\n", port + i);
            success = 1;
            break;
        }
    }
    if(!success) {
        printf("Impossible de démarrer le serveur (tous les ports testés sont pris)\n");
        return EXIT_FAILURE;
    }

    printf("Serveur en attente de connexions...\n");
    char* secret_word = get_random_word(argv[1]);
        if(!secret_word) {
            printf("Impossible de recupérer un mot.\n");
            return EXIT_FAILURE;
        }
    while(1) {
        ENetEvent event;
        while (enet_host_service(server, &event, 1000) > 0) {
            switch (event.type) {
                case ENET_EVENT_TYPE_CONNECT:
                    if(num_players < MAX_PLAYERS) {
                        printf("Nouveau joueur connecté !\n");
                        players[num_players].peer = event.peer;
                        players[num_players].found = 0;
                        players[num_players].time_taken = 0;
                        num_players++;

                        if(num_players == 1) start_time = clock(); // Début de la partie

                        // Envoyer le mot à deviner
                        broadcast(server, secret_word);
                    } else {
                        printf("Serveur plein !\n");
                        enet_peer_disconnect(event.peer, 0);
                    }
                    break;

                case ENET_EVENT_TYPE_RECEIVE:
                    printf("Réponse reçue : %s\n", event.packet->data);

                    for(int i = 0; i < num_players; i++) {
                        if(event.peer == players[i].peer && !players[i].found) {
                            players[i].found = 1;
                            players[i].time_taken = ((double)(clock() - start_time)) / CLOCKS_PER_SEC;
                            printf("Joueur %d a trouvé en %.2f secondes\n", i, players[i].time_taken);
                        }
                    }

                    enet_packet_destroy(event.packet);
                    break;

                case ENET_EVENT_TYPE_DISCONNECT:
                    printf("Un joueur s'est déconnecté\n");
                    break;

                default:
                    break;
            }
        }

        // Vérifier si tout le monde a terminé
        int all_done = 1;
        for(int i = 0; i < num_players; i++) {
            if(!players[i].found) {
                all_done = 0;
                break;
            }
        }

        if(all_done && num_players > 0) {
            printf("Tous les joueurs ont terminé, classement final :\n");

            // Trier les joueurs par temps
            for(int i = 0; i < num_players - 1; i++) {
                for(int j = i + 1; j < num_players; j++) {
                    if(players[i].time_taken > players[j].time_taken) {
                        Player temp = players[i];
                        players[i] = players[j];
                        players[j] = temp;
                    }
                }
            }

            // Envoyer le classement
            char ranking[256] = "Classement :\n";
            for(int i = 0; i < num_players; i++) {
                char buffer[50];
                snprintf(buffer, sizeof(buffer), "Joueur %d: %.2f sec\n", i + 1, players[i].time_taken);
                strcat(ranking, buffer);
            }
            broadcast(server, ranking);
            break;
        }
    }

    enet_host_destroy(server);
    enet_deinitialize();
    free(secret_word);
    return EXIT_SUCCESS;
}
