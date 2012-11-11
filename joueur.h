#pragma once
#include <complex>
#include "balle.h"


class Joueur
{

private:
	float m_raquetteX, m_raquetteY, m_raquetteRotation; // Position et rotation de la raquette
	int m_largeur, m_longueur;
	std::string m_boutonGauche, m_boutonDroite;
	float m_actuelDeplacement, m_totalDeplacement;
	float m_raquetteVitesse;
	int m_score, m_scorePosX, m_scorePosY;
	int m_indicationTouchesPosX, m_indicationTouchesPosY;
	bool m_vivant;

public:
	Joueur(float raquetteX, float raquetteY, float raquetteRotation, int score, int longueur = 150);
	float getX();
	float getY();
	float getRotation();
	int getLongueur();
	int getLargeur();
	float getMinX();
	float getMaxX();
	int getScore();
	int getScorePosX();
	int getScorePosY();
	int getIndicationTouchesPosX();
	int getIndicationTouchesPosY();
	void setIndicationTouchesPosition(int x, int y);

	bool estVivant();

	void bougerDir1(float coeffVitesse = 1);
	void bougerDir2(float coeffVitesse = 1);

	void setX(float x);
	void setY(float y);
	void setRotation(float rotation);
	void incrementerScore();
	void decrementerScore();
	void setPositionScore(int x, int y);
	void setTotalDeplacement(int d);
	void tuerJoueur();



};

