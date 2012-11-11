#pragma once
#include <vector>
#include "trait.h"
#include "joueur.h"


class Terrain
{

private:
	std::vector<float> m_coordX;
	std::vector<float> m_coordY;


public:
	Terrain(int nbJoueurs, std::vector<Joueur> &joueur, int largeur, int hauteur, int points, std::vector<Trait> &trait);
	static float calculeAngle(int x1, int y1, int x2, int y2);
	static float calculeDistance (int x1, int y1, int x2, int y2);
	void remplacerJoueur (int numJoueur, std::vector<Trait> &trait, std::vector<Balle> &balle);
	std::vector<float> getCoordX();
	std::vector<float> getCoordY();
};
