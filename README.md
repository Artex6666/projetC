TRAVAIL COMMUN - CREATION MOTUS EN C

Commande pour compiler le back:

`gcc -Wall -Wextra -Werror main.c functions/back.c -o motus_back`

libs nécessaires : SDL2 2.30.11, SDL2_ttf 2.24.0, SDL2_mixer 2.8.0 et SDL2_image2.8.4

commande pour compiler le front:

`gcc main.c front.c -lSDL2 -lSDL2_ttf -lSDL2_mixer -o motus_front`
