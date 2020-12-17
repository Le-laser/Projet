**10/09/2020**

documentation sur les abeilles et les frelons asiatiques : 
 tailles
 mode de vie
 température
 ...
 
 recherche de moyen pour repérer les frelons et les abeilles : 
 
 caméra thermique+ caméra ?
 
 capteur d'ondes ( battements d'ailes ) ? 
 
 
 
 visionnage de plusieurs vidéo sur les frelons, et lectures d'articles sur le même sujet.
 
 **25/09/2020**
 
 renseignement à propos du machine learning : 

c'est surement la meilleure solution pour la détection des frelons, avec déjà des travaux existant trouvés

j'ai contacté monsieur Miramond pour qu'il nous donne des informations sur le machine learning.

j'ai également inscrit notre équipe au concours electro-apicole. 

**08/10/2020**

installation de linux sur raspberry, et documentation sur tensor flow, qui nous servira pour faire le machine learning.
j'ai aussi aidé bagrow pour le branchement de son moteur.

**22/10/2020**

installation de pytorche et son envirronement, et visionnage de plusieurs tutoriels.

La SD du rapsberry est surement trop petite pour accueillir tous les envirronements et faire fonctionner les applciations, car je ne peut rien faire avec depuis que j'ai
installé pytorche.

**19/11/2020**

le machine learning semble vraiment trop lourd en terme de puissance de calcul, il nous faut une autre solution.

Avec Micha, nous avons téléchargé le projet github de MR. Barrois, que nous avons étudié.

nous avons commenté les codes, pour pouvoir par la suite mieux le comprendre, coder les points qu'il manque, et modifier le code pour que notre application
puisse fonctionner.

la stratégie consiste à prendre une photo,la mettre en noir et blanc, regrouper ensuite les pixels par groupe de 4x4, puis étudier la couleur de  chaque "blob" ( groupe de pixel)
pour trouver le plus sombre ( ce qui correspond au frelon ).

**26/11/2020**

téléchargement des différentes bibliothèques manquantes pour comprendre comment fonctionne le code de Mr. Barrois. 

Il nous manque du matériel pour pouvoir le tester, notament un ESP32 et la caméra.


une fois que nous l'aurons, il sera plus simple de comprendre le code, et nous pourrons commencer les premiers tests.

nous avons également pu constater que le code n'était pas assez optimisé, il est possible de gagner de précieuses ms dans notre future application.

**03/12/2020 et 04/12/2020**

Avec l'ESP32 Cam nous avons pu faire plusieurs tests. Nous pouvons maintenant prendre des photos, et nous avons commencé à réfléchir à l'algorithme
qui permettra de détecter un frelon.

Le principe est de regarder 1 pixel sur 4, de comparer son niveau de gris avec une variable, et de ce point, former dans une matrice de position une tache noire qui représentera le frelon. Il nous suffira de prendre la tâche la plus grande, de définir son point centrale, et de tirer un bon gros coup de laser sur le frelon, qui fera beaucoup moins le malin par la suite.

Nous avons d'autres idées pour l'algorithme de détection du frelon, et sommes très intéressés par l'optimisation de code ( condition ternaire, limiter les boucles For et appels de fonctions ).

Je pense que si Mr. Barrois, qui n'est pas un informaticien, a réussi à faire son traitement d'image en 300ms, nous pouvons largement améliorer ce temps et permettre par la suite une meilleure réactivité pour être sur de toucher le frelon pendant son vol stationaire.



**10/12/2020**

nous avons terminé un algorithme qui permet de donner la position en X Y et la taille de la plus grosse tache. Cette tâhe est une accumulation de tâches fonçées, qui représentent le frelon. Cependant, nous avons testé notre programme uniquement avec une image en 50x50, car nous n'avons pas assez de place mémoire sur notre ESP32 pour avoir des tableaux suffisament grands pour utiliser des images prises par l'espCAM.
plusieurs idées sont à explorer pour le moment.

**17/12/2020**
aujourd'hui le temps est à l'optimisation du code, et la diminution de l'espace mémoire prise par les tableaux de pixels.

Après discussion avec Mr. Bigot après mon épreuve de rattrapage, la solution du masquaque pour ranger non plus 1 mais 8 pixels dans un octet de données semble être la meilleure solution pour réduire notre consommation de stokage.

Cette optimisation permet de pouvoir multiplier la taille de notre image par x ( plusieurs tableaux sont utilisés, donc pas par 8 ). 

en reprenant l'algorithme de la semaine passée, nous avons décidé de chronométrer notre traitement d'image : 300µs !! 

nous avons donc préféré nous pencher sur la gestion de la mémoire plutôt que sur l'optimisation du temps.

des solutions simple éxistent pour palier notre problème, cependant, cela demande des utilisations d'allocation dynamique et de beaucoup de fonction récursives. Les allocation dynamiques sont à bannir en embarqué, et les fonctions récursives vivement déconseillées. 

en dépitde ça, pour notre traitement d'image nous sommes obligés d'utiliser la récursivité.

il nous faudra encore pas mal de tests et de modifications de notre algorithme pour le rendre utilisable avec une image de 320/240, mais nous sommes en bonne voie, c'est motivant ! 

