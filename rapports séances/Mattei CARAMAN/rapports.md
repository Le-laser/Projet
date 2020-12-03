# Rapports de séances


----------Séance du 10/09/2020-------------

Création d'une equipe et d'un repository GitHub.
Participé à l'etablissement du cahier de charges initial.
Comment identifier un frelon et qu'est-ce qui le différencie de l'abeille(pour le traitement d'images). Deux élements permettent de l'identifier facilement. 
C'est la fréquence de battement des ailes et la température corporelle.


----------Séance du 25/09/2020--------------

J'ai regardé les avantages/ inconvenients des servomoteurs et moteurs pas a pas


----------Séance du 2020/10/08--------------

J'ai regardé les différences entre frelons et abeilles en termes d'acoustique. 
J'ai pu trouver que les frelons ont une fréquence des bttement des ailes autour de 230Hz avec des harmoniques a 351Hz, 703Hz et 820Hz


---------Séance du 2020/10/22---------------
J'arrive a appliquer la fonction FFT sur le signal du microphone et de voir les raies du spectre sous forme de tableur.
Je suis en train de trouver un moyen pour transmettre les donnes du tableur contenant les informations spectrales. 
Au final j'utilise Processing pour etablir le lien entre l'Arduino et l'ordinateur.
Je n'arrive pas a stoquer les informations de l'arduino sur un fichier .csv pour l'instant.


---------Séance du 2020/11/19---------------
J'ai créée un visualisateur de spectre sous Processing(Java)
J'ai implementé un algorithme de detection de frelon basé sur celui-ci : https://github.com/Barrois/Detecteur-ESP32-Cam-Blob-Detector
Il a l'air de fonctionner mais il y a possibilité de confondre abeilles et frelons.
J'ai rencontré un probleme lié au CAN de l'arduino(MEGA32U4). Celui ci est assez limité en fréquence d'echantillonage (9600Hz) et donc en nombre d'echantillons (128).
Ca nous laisse avec 75Hz d'espacement entre chaque raie du spectre, ce qui n'est pas assez pour distinguer frelons des abeilles. 
Il faut donc prendre un uC avec un CAN plus rapide.

------Séance du 2020/11/26--------------

J'ai optimisé l'algorithme de détection acoustique de frelons pour avoir moins de fausses détections.
