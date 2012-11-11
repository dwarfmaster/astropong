#include "stdafx.h"
#include "joueur.h"
#include "balle.h"
#include <string>

using namespace std;
#define PI 3.14159265358979323846264338327950288419716939937510582


extern float g_coeffVitesse;

#pragma warning (disable:4244) // conversion int en float

Joueur::Joueur(float raquetteX, float raquetteY, float raquetteRotation, int score, int longueur)
{
	m_score = score;
	m_longueur = longueur;
	m_largeur = 15;
	m_raquetteX = raquetteX;
	m_raquetteY = raquetteY;
	m_raquetteRotation = raquetteRotation;
	m_raquetteVitesse = 0.02f;
	m_vivant = true;
}

float Joueur::getX()
{
	return m_raquetteX;
}
float Joueur::getY()
{
	return m_raquetteY;
}

float Joueur::getMinX()
{
	return (m_raquetteX - abs(m_longueur/2*cos(m_raquetteRotation*PI/180)));
}

float Joueur::getMaxX()
{
	return (m_raquetteX + abs(m_longueur/2*cos(m_raquetteRotation*PI/180)));
}

float Joueur::getRotation()
{
	return m_raquetteRotation;
}
int Joueur::getLargeur()
{
	return m_largeur;
}

int Joueur::getLongueur()
{
	return m_longueur;
}

void Joueur::setX(float x)
{
	m_raquetteX = x;
}
void Joueur::setY(float y)
{
	m_raquetteX = y;
}
void Joueur::setRotation(float r)
{
	m_raquetteRotation = r;
}

// Déplacement des raquettes
void Joueur::bougerDir1(float coeffVitesse)
{
	if (m_actuelDeplacement > 0 && m_vivant)
	{
		float vitesseX = cos(m_raquetteRotation*PI/180)*m_raquetteVitesse*m_totalDeplacement*coeffVitesse * g_coeffVitesse;
		float vitesseY = sin(m_raquetteRotation*PI/180)*m_raquetteVitesse*m_totalDeplacement*coeffVitesse * g_coeffVitesse;
		m_raquetteX -= vitesseX;
		m_raquetteY -= vitesseY;
		m_actuelDeplacement -= sqrt(vitesseX*vitesseX + vitesseY*vitesseY);

		if (m_actuelDeplacement < 0) // Raquette qui dépasse...
		{
			m_raquetteX -= cos(m_raquetteRotation * PI/180)*m_actuelDeplacement;
			m_raquetteY -= sin(m_raquetteRotation * PI/180)*m_actuelDeplacement;
			m_actuelDeplacement = 0;
		}
	}
}

void Joueur::bougerDir2(float coeffVitesse)
{
	if (m_actuelDeplacement < m_totalDeplacement && m_vivant)
	{
		float vitesseX = cos(m_raquetteRotation*PI/180)*m_raquetteVitesse*m_totalDeplacement*coeffVitesse * g_coeffVitesse;
		float vitesseY = sin(m_raquetteRotation*PI/180)*m_raquetteVitesse*m_totalDeplacement*coeffVitesse * g_coeffVitesse;;
		m_raquetteX += vitesseX;
		m_raquetteY += vitesseY;
		m_actuelDeplacement += sqrt(vitesseX*vitesseX + vitesseY*vitesseY);


		if (m_actuelDeplacement > m_totalDeplacement) // Raquette qui dépasse...
		{
			m_raquetteX -= cos(m_raquetteRotation * PI/180)*(m_actuelDeplacement - m_totalDeplacement );
			m_raquetteY -= sin(m_raquetteRotation * PI/180)*(m_actuelDeplacement - m_totalDeplacement);
			m_actuelDeplacement = m_totalDeplacement;
		}
	}
}

int Joueur::getScore()
{
	return m_score;
}

void Joueur::decrementerScore()
{
	m_score--;
}

void Joueur::incrementerScore()
{
	m_score++;
}

int Joueur::getScorePosX()
{
	return m_scorePosX;
}
int Joueur::getScorePosY()
{
	return m_scorePosY;
}
void Joueur::setPositionScore(int x, int y)
{
	m_scorePosX = x;
	m_scorePosY = y;
}

void Joueur::setTotalDeplacement(int d)
{
	m_totalDeplacement = d;
	m_actuelDeplacement = m_totalDeplacement/2;
}

bool Joueur::estVivant()
{
	return m_vivant;
}

void Joueur::tuerJoueur()
{
	m_vivant = false;
}

int Joueur::getIndicationTouchesPosX()
{
	return m_indicationTouchesPosX;
}
int Joueur::getIndicationTouchesPosY()
{
	return m_indicationTouchesPosY;
}
void Joueur::setIndicationTouchesPosition(int x, int y)
{
	m_indicationTouchesPosX = x;
	m_indicationTouchesPosY = y;

}
