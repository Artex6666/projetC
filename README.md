TRAVAIL COMMUN - CREATION MOTUS EN C



Commande pour compiler le back:


`gcc -Wall -Wextra -Werror main.c functions/back.c -o modus`


libs nécessaires : SDL2, SDL2_ttf et SDL2_mixer 

commande pour compiler le front:

`gcc main.c front.c -lSDL2 -lSDL2_ttf -lSDL2_mixer -o motus`
