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

------Séance du 2020/12/03--------------
J'ai essayé de faire fonctionner la diode laser avec le driver fournis par le professeur. Je n'ai pas reussi a la faire fonctionner, possiblement parce que la diode e'tait branchée a l'envers et du coup je l'ai grillé. Pour la prochaine fois je vais amener deux diodes laser rouges extraites d'un lecteur DVD. Cela permettra d'experimenter davantage pour voir comment les diodes laser fonctionnent.

------Séance du 2020/12/04--------------
En regardant sur internet, j'ai pu voir des exemples de branchement pour les diodes laser et chaque diode est différente, c.à.d qu'il n'y a pas une norme sur les broches. La diode peut etre Case Negative(boitier negatif) ou Case Positive(boitier positif), donc avoir la datasheet du composant est très important. On a tenté de bruler une feuille de papier noire avec les lasers mais sans succès. Les diodes rouges testés ont atteint leurs limites dans nos tests.

------Séance du 2020/12/10--------------
J'ai acheté deux diodes laser bleues(réf: PLPT9 450LA_E et PLPT5),des boitiers optiques pour heberger les diodes laser, des lunettes de protection et quelques regulateurs linéaires LM317 pour fabriquer un driver à courant constant plus puissant.
J'ai decidé de faire des simulations sur LTSpice pour m'aider à concevoir le driver. Le souci avec le LM317 est que la tension de headroom(Vin-Vout) minimale demandé est de 3V. Cela veut dire qu'a des courants elevés il ne peut pas etre très efficace. Si on pilote la diode laser avec 1.5A(courant max du LM317) alors la puissance dissipée par régulateur est de 3*1.5=4.5W. Donc, pour la solution finale, j'opterai plutot sur un driver switché et non pas linéaire , comme par exemple le NCL30160.

------Séance du 2020/12/17--------------
J'ai assemblé le laser dans son boitier avec Pierre Yves et je l'ai testé sur les frelons. Avec un courant de 500mA débité par l'alimentation, le laser arrive a cramer le frelon facilement. J'ai fait un code de test pour tester l'orientation de la tourelle avec le laser monté dessus a l'aide d'un joystick.

