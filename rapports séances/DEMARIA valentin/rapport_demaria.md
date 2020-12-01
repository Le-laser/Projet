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


