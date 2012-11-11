#include "stdafx.h"
#include <complex>
#include "joueur.h"
#include "terrain.h"



using namespace std;
#define PI 3.14159265358979323846264338327950288419716939937510582

#pragma warning (disable:4244) // conversion int en float. Merde, quoi.

Terrain::Terrain(int nbJoueurs, std::vector<Joueur> &joueur, int largeur, int hauteur, int points, vector<Trait> &trait)
{

	// Mode solo
	if (nbJoueurs == 1)
	{

		// |--|
		// |  |
		// 0--1
		m_coordX.push_back(largeur/6); // 0
		m_coordY.push_back(hauteur - 50);

		m_coordX.push_back(largeur * 5/6);
		m_coordY.push_back(hauteur - 50);

		joueur.push_back(Joueur(largeur/2, hauteur - 100, 0, points, 250*largeur/1600)); // Raquette gauche
		joueur[0].setPositionScore(largeur/2,hauteur - 380);
		joueur[0].setIndicationTouchesPosition(largeur/2, hauteur - 200);
		joueur[0].setTotalDeplacement(largeur * 4/6 - joueur[0].getLongueur());

		trait.push_back(Trait(m_coordX[0], hauteur/2, 90, hauteur, 2));
		trait.push_back(Trait(m_coordX[1], hauteur/2, 90, hauteur, 2));
		trait.push_back(Trait(largeur/2, 1, 0, largeur * 4/6, 2));
	}
	

	// Partie 2 joueurs
	if (nbJoueurs == 2)  
	{
	// 0 : horizontal
	// 90 : tournée vers la droite


	// 1--2
	// |  |
	// 0--3

		m_coordX.push_back(150); // 0
		m_coordY.push_back(hauteur - 50);

		m_coordX.push_back(150);
		m_coordY.push_back(50);

		m_coordX.push_back(largeur - 150);
		m_coordY.push_back(50);

		m_coordX.push_back(largeur - 150);
		m_coordY.push_back(hauteur - 50);

		joueur.push_back(Joueur(150, hauteur/2, 90, points, 150*largeur/1600)); // Raquette gauche
		joueur.push_back(Joueur(largeur - 150, hauteur/2, 270, points, 150*largeur/1600)); // Raquette droite
		joueur[0].setPositionScore(450,200);
		joueur[1].setPositionScore(largeur - 450,200);
		
		joueur[0].setIndicationTouchesPosition(250, hauteur/2);
		joueur[1].setIndicationTouchesPosition(largeur - 250, hauteur/2);

		trait.push_back(Trait(largeur/2,50,0,largeur,2));
		trait.push_back(Trait(largeur/2,hauteur - 50,0,largeur,2));

		for (int i=0; i<2;i++)
			joueur[i].setTotalDeplacement(hauteur - joueur[i].getLongueur()- 100);

	}

	if (nbJoueurs == 3)  
	{

		//		1--2
		//	   /    \
		//	  0      3
		//	   \	/
		//		5--4

	// Les coordonnées des points 0 et 3 en x seront modifiées pour que ça forme un héxagone

		m_coordX.push_back(80);
		m_coordY.push_back(hauteur/2);

		m_coordX.push_back(largeur/3);
		m_coordY.push_back(50);

		m_coordX.push_back(largeur*2/3);
		m_coordY.push_back(50);

		m_coordX.push_back(largeur - 100);
		m_coordY.push_back(hauteur/2);

		m_coordX.push_back(largeur*2/3);
		m_coordY.push_back(hauteur - 50);

		m_coordX.push_back(largeur/3);
		m_coordY.push_back(hauteur - 50);

		// Redéfinition ds coordonnées des points 0 et 3 pour un hexagone parfait.

		int dist = calculeDistance(m_coordX[1],m_coordY[1], m_coordX[2],m_coordY[2]);
		float angl = asin((double)(hauteur/2 - 50) / dist);
		m_coordX[0] = largeur/3 - dist * sin(angl);
		m_coordX[3] = 2*largeur/3 + dist * sin(angl);
		
		// Ajout des joueurs
		float angle;
		for (int i=0; i<3;i++)
		{
			angle = calculeAngle(m_coordX[2*i],m_coordY[2*i],m_coordX[2*i+1],m_coordY[2*i+1]);
			joueur.push_back(Joueur((m_coordX[2*i]+m_coordX[2*i+1])/2,(m_coordY[2*i]+m_coordY[2*i+1])/2, angle, points, 150*largeur/1600)); // Raquette hg
			joueur[i].setTotalDeplacement(calculeDistance(m_coordX[2*i],m_coordY[2*i],m_coordX[2*i+1],m_coordY[2*i+1]) - joueur[i].getLongueur());
		}

		// Définition manuelle de la position des score
		joueur[0].setPositionScore(400,400);
		joueur[1].setPositionScore(largeur -400,400);
		joueur[2].setPositionScore(largeur/2,hauteur-350);

		//... et des indications de touches
		joueur[0].setIndicationTouchesPosition(largeur/4, hauteur/3);
		joueur[1].setIndicationTouchesPosition(3*largeur/4, hauteur/3);
		joueur[2].setIndicationTouchesPosition(largeur/2 + 10, hauteur - 150);

		// Ajout des traits
		for (int i=1; i<6;i+=2)
		{
			int j=i+1;
			if (j==6) j=0;

			angle = calculeAngle(m_coordX[i],m_coordY[i],m_coordX[j],m_coordY[j]);
			trait.push_back(Trait((m_coordX[i]+m_coordX[j])/2,(m_coordY[i]+m_coordY[j])/2, angle, 
								calculeDistance(m_coordX[i],m_coordY[i],m_coordX[j],m_coordY[j])+10,2));
		}


	}

	if (nbJoueurs == 4)
	{
		//		2--3
		//	   /    \
		//    1      4
		//	  |      |
		//	  0      5
		//	   \	/
		//		7--6

		m_coordX.push_back(100); //0
		m_coordY.push_back(hauteur/2 + largeur/6);

		m_coordX.push_back(100);
		m_coordY.push_back(hauteur/2 - largeur/6);

		m_coordX.push_back(largeur/4);
		m_coordY.push_back(50);

		m_coordX.push_back(largeur*3/4);
		m_coordY.push_back(50);

		m_coordX.push_back(largeur - 100); //4
		m_coordY.push_back(hauteur/2 - largeur/6);

		m_coordX.push_back(largeur - 100);
		m_coordY.push_back(hauteur/2 + largeur/6);

		m_coordX.push_back(largeur*3/4);
		m_coordY.push_back(hauteur - 50);

		m_coordX.push_back(largeur/4);
		m_coordY.push_back(hauteur - 50);

		float compensation = (m_coordX[4] - m_coordX[1]) / (m_coordY[7] - m_coordY[2]) - 1;
		compensation /= 4;

		float dist = m_coordY[0] - m_coordY[1];

		m_coordY[0] += compensation*dist; 
		m_coordY[1] -= compensation*dist;
		m_coordY[4] -= compensation*dist;
		m_coordY[5] += compensation*dist;

		// Ajout des joueurs
		float angle;
		for (int i=0; i<4;i++)
		{
			angle = calculeAngle(m_coordX[2*i],m_coordY[2*i],m_coordX[2*i+1],m_coordY[2*i+1]);
			joueur.push_back(Joueur((m_coordX[2*i]+m_coordX[2*i+1])/2,(m_coordY[2*i]+m_coordY[2*i+1])/2, angle, points, 150*largeur/1600)); // Raquette hg
			joueur[i].setTotalDeplacement(calculeDistance(m_coordX[2*i],m_coordY[2*i],m_coordX[2*i+1],m_coordY[2*i+1]) - joueur[i].getLongueur());
		}

		// Définition manuelle de la position des score
		joueur[0].setPositionScore(400,hauteur/2);
		joueur[1].setPositionScore(largeur /2, 250);
		joueur[2].setPositionScore(largeur-400,hauteur/2);
		joueur[3].setPositionScore(largeur/2, hauteur-250);
		
		//... et des indications de touches
		joueur[0].setIndicationTouchesPosition(250, hauteur/2);
		joueur[1].setIndicationTouchesPosition(largeur/2, 150);
		joueur[2].setIndicationTouchesPosition(largeur - 250, hauteur/2);
		joueur[3].setIndicationTouchesPosition(largeur/2, hauteur - 150);

		// Ajout des traits
		for (int i=1; i<8;i+=2)
		{
			int j=i+1;
			if (j==8) j=0;

			angle = calculeAngle(m_coordX[i],m_coordY[i],m_coordX[j],m_coordY[j]);
			trait.push_back(Trait((m_coordX[i]+m_coordX[j])/2,(m_coordY[i]+m_coordY[j])/2, angle, 
								calculeDistance(m_coordX[i],m_coordY[i],m_coordX[j],m_coordY[j])+10,2));
		}
		
	}


}





// Retourne un angle en degré.
float Terrain::calculeAngle(int x1, int y1, int x2, int y2)
{
	if (x1==x2) // vertical
	{
		if (y1 > y2)
			return 90;
		else 
			return 270;
	}
	else
	{
		int a = atan((double)(y2-y1)/(x2-x1))*180/PI;
		if (y1<y2 && x1>x2)
			a += 360;
		if (x1<x2)
			a += 180;

		return a;
	}
}

float Terrain::calculeDistance(int x1, int y1, int x2, int y2)
{
	return sqrt((float)((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)));
}

void Terrain::remplacerJoueur (int numJoueur, vector<Trait> &trait, vector<Balle> &balle)
{
	float angle;
	int n = 2*numJoueur;
	angle = calculeAngle(m_coordX[n],m_coordY[n],m_coordX[n+1],m_coordY[n+1]);
	trait.push_back(Trait((m_coordX[n]+m_coordX[n+1])/2,(m_coordY[n]+m_coordY[n+1])/2, angle,  
							calculeDistance(m_coordX[n],m_coordY[n],m_coordX[n+1],m_coordY[n+1])+10, 2));

	for (unsigned int i=0; i<balle.size(); i++)
		balle[i].ajouterCompteurTrait(1);
}	

vector<float> Terrain::getCoordX()
{
	return m_coordX;
}

vector<float> Terrain::getCoordY()
{
	return m_coordY;
}	
