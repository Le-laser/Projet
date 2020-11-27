**_Séance 1 : 10/09/2020_**

Prise de connaissance du sujet et recherches sur internet de projet similaire pour voir les pistes de départ.
Recherches sur les caméras/laser.
Visionnage de documentaire sur les frelons asiatiques pour avoir des infos comme leur mode/période de chasse, taille etc.


**_Séance 2 : 25/09/2020_**

Recherches sur la température léthale d'un frelons (47°C) et des lasers permettant d'atteindre cette température.
Recherches sur la fréquence des battements d'ailes des abeilles (200-250Hz).
Lecture de forum sur la reconnaissance d'image avec le fonctionnement et les github.

Liens utiles :
https://www.inaturalist.org/ = image insectes
https://www.mca-ingenierie.fr/fr/blog/271
https://forum.arduino.cc/index.php?topic=579623.45
https://github.com/snoop2head/study_in_yellow


**_Séance 3 : 08/10/2020_**

Test du micro avec Mattei pour plus tard implémenter une fonction de reconnaissance des frelons par leur bruit afin de ne pas laisser la reconnaissance par image tourner tout le temps.
Le micro marche.
Implémentation d'un code FFT pour traiter le signal et l'adapter aux fréquences souhaitées.


**_Séance 4 : 22/10/2020_**

La FFT implentée marche.
Recherche de module laser arduino assez puissant pour brûler les ailes d'un frelons, pas trouvé -> solution faire un laser brûleur DIY avec des laser de lecteur blu-ray.
Test du contrôle des moteurs pas à pas avec un joystick (ça marche).
Réalisation d'une pièce 3D pour fixer le laser de test aux moteurs pas à pas.


**_Séance 5 : 19/11/2020_**

Prise de connaissances du fonctionnement des servomoteurs et suivi de l'assemblage fait par Lucas Bagrowski et test de la pièce 3D réalisé à la séance précédente.
Aidé Mattei avec la detection sonore des frelons, cela semble compliqué car les fréquences de battements d'ailes de frelons sont assez similaires à ceux des abeilles. Une optimisation du code et pourquoi un micro plus performant pourraient aider à avoir un système plus fiable.


**_Séance 6 : 26/11/2020_**

Prise de connaissance d'un laser assez puissant pour neutraliser un frelon. Il semblerait qu'un module laser assez puissant pour réaliser cela est assez cher.
J'ai donc visionné des vidéos sur des fabrications de laser DIY (DO IT YOURSELF) assez puissant pour neutraliser un frelon.
Car le problème est d'atteindre la température léthale (environ 47°C) le plus rapidement possible afin que le frelon n'ait pas le temps de tuer des abeilles.
Liens utiles : https://www.instructables.com/How-to-make-a-burning-laser-for-CHEAP/
Ce site explique pas à pas les étapes à suivre pour réaliser un laser capable de graver sur du bois. La prochaine étape sera l'élaboration du laser.
