#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <../include/back/enet.h>
#include <time.h>

int main(int argc, char **argv) {
    if (enet_initialize() != 0) {
        fprintf(stderr, "Erreur d'initialisation d'ENet\n");
        return EXIT_FAILURE;
    }

    ENetHost *client = enet_host_create(NULL, 1, 2, 0, 0);
    if (!client) {
        fprintf(stderr, "Impossible de créer le client\n");
        return EXIT_FAILURE;
    }

    ENetAddress address;
    ENetEvent event;
    ENetPeer *serverPeer;

    enet_address_set_host(&address, "127.0.0.1");
    address.port = 1234;

    serverPeer = enet_host_connect(client, &address, 2, 0);
    if (!serverPeer) {
        fprintf(stderr, "Connexion échouée\n");
        return EXIT_FAILURE;
    }

    char mot_a_trouver[20] = "";

    while (1) {
        while (enet_host_service(client, &event, 1000) > 0) {
            switch (event.type) {
                case ENET_EVENT_TYPE_RECEIVE:
                    if (mot_a_trouver[0] == '\0') {
                        strncpy(mot_a_trouver, (char *)event.packet->data, sizeof(mot_a_trouver) - 1);
                        printf("Mot à trouver : %s\n", mot_a_trouver);
                    } else {
                        printf("%s\n", event.packet->data);
                    }
                    enet_packet_destroy(event.packet);
                    break;

                case ENET_EVENT_TYPE_DISCONNECT:
                    printf("Déconnecté du serveur\n");
                    return 0;

                default:
                    break;
            }
        }

        // Simulation : l'utilisateur entre un mot
        char mot_entre[20];
        printf("Tape un mot : ");
        scanf("%s", mot_entre);

        if (strcmp(mot_entre, mot_a_trouver) == 0) {
            ENetPacket *packet = enet_packet_create("Trouvé", strlen("Trouvé") + 1, ENET_PACKET_FLAG_RELIABLE);
            enet_peer_send(serverPeer, 0, packet);
            printf("Mot correct envoyé au serveur !\n");
        }
    }

    enet_peer_disconnect(serverPeer, 0);
    enet_host_destroy(client);
    enet_deinitialize();
    return 0;
}
