Introduction
================
Nous parlerons ici de l'installation sous Linux. Pour l'installer sous windows, il faudra aller voir http://www.siteduzero.com/forum-83-785145-p1-jeu-video-astropong.html.

Téléchargement
================
Vous devez tout d'abord télécharger l'archive. Si vous utilisez git, vous pouvez faire :
git clone git://github.com/lucas8/astropong.git
Sinon, vous devez faire :
wget https://github.com/lucas8/astropong/archive/master.zip
unzip master.zip

Compilation
================
Vous devez ensuite vous placer dans le dossier des sources. Pour compiler, il faut que vous ayez sfml 1.6 d'installée.
Ouvrez le Makefile avec votre editeur de texte préféré et modifiez les deux lignes suivantes en fonction de votre système :

CXXFLAGS=-I/path/vers/includes/de/sfml

LDFLAGS=-lnom_sfml1.6-{audio,graphics,system,window,network}

Ensuite tapez 'make'. Normalement, vous devriez avoir un programme astropong. Si non, vous avez du vous tromper lors de la modification des deux lignes précédentes.

Les ressources
================
Afin de pouvoir lancer le jeu, vous devez télécharger les ressources. Vous pouvez les obtenir avec la commande suivante :
wget https://github.com/downloads/lucas8/astropong/res.tar.gz
Vous obtenez une archive .tar.gz qui contient un dossier res. Vous devez placer ce dossier dans le même dossier que le programme astropong.

Enjoy
================
Vous pouvez maintenant lancer astropong de cette manière :
./astropong
N'hésitez pas à faire part de vos retour à l'adresse suivante : http://www.siteduzero.com/forum-83-785145-p2-jeu-video-astropong.html

