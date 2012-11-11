#include "stdafx.h"
#include "balle.h"
#include <complex>

extern float g_coeffVitesse;

#define PI 3.14159265358979323846264338327950288419716939937510582
#pragma warning (disable:4244) // conversion int en float

Balle::Balle(float posX, float posY, float vitesse, float angle)
:m_posX(posX), m_posY(posY), m_vitesse(vitesse), m_angle(angle)
{
	convertirEnCart();
	m_rayon = 8;
	m_estPerdue = -1;
	m_active = false;
	m_etat = 0;
	m_joueur = -1; // Dernier joueur qui touche la balle.
}

// Getters

float Balle::getPosX()
{
	return m_posX;
}
float Balle::getPosY()
{
	return m_posY;
}
float Balle::getVitX()
{
	return m_vitX;
}
float Balle::getVitY()
{
	return m_vitY;
}

float Balle::getVitesse()
{
	return m_vitesse;
}

float Balle::getAngle()
{
	return m_angle;
}

int Balle::getRayon()
{
	return m_rayon;
}

// Setters

void Balle::setPosX(float x)
{
	if (!m_active)
		return;
		
	m_posX = x;
	convertirEnPol();
}
void Balle::setPosY(float y)
{
	if (!m_active)
		return;

	m_posY = y;
	convertirEnPol();
}
void Balle::setVitX(float x)
{
	if (!m_active)
		return;

	m_vitX = x;
	convertirEnPol();
}
void Balle::setVitY(float y)
{
	if (!m_active)
		return;

	m_vitY = y;
	convertirEnPol();
}

void Balle::ajouterVitesseX(float vit)
{
	if (!m_active)
		return;

	m_vitX += vit;
	convertirEnPol();
}

void Balle::ajouterVitesseY(float vit)
{
	if (!m_active)
		return;

	m_vitY += vit;
	convertirEnPol();
}

void Balle::setVitesse(float vit)
{
	if (!m_active)
		return;

	m_vitesse = vit;
	convertirEnCart();
}

void Balle::setAngle(float angle)
{
	if (!m_active)
		return;

	m_angle = (int)(angle)%360;
	while (m_angle < 0)
		m_angle += 360;
	convertirEnCart();
}

void Balle::ajouterVitesse(float vit)
{
	if (!m_active)
		return;

	m_vitesse += vit;
	convertirEnCart();
}

void Balle::ajouterAngle(float angle)
{
	if (!m_active)
		return;

	m_angle +=angle;
	m_angle = (int)m_angle % 360;
	while (m_angle < 0)
		m_angle += 360;
	convertirEnCart();
}

void Balle::convertirEnCart()
{
	m_vitX = m_vitesse * cos(m_angle*PI/180);
	m_vitY = m_vitesse * sin(m_angle*PI/180);
}

void Balle::convertirEnPol()
{
	m_angle = (int)(atan(m_vitY/m_vitX)*180/PI)%360 ;
	if (m_vitY<0 && m_vitX>0)
		m_angle += 360;
	if (m_vitX<0)
		m_angle += 180;

	m_vitesse = sqrt(m_vitX*m_vitX + m_vitY*m_vitY);
}

void Balle::bouger(float coeffVitesse)
{
	if (!m_active)
		return;

//coeffVitesse = 0.05;

	m_ancPosX = m_posX;
	m_ancPosY = m_posY;

	m_posX += m_vitX*coeffVitesse*g_coeffVitesse;
	m_posY += m_vitY*coeffVitesse*g_coeffVitesse;
	convertirEnPol();
	
}

float Balle::getAncPosX()
{
	return m_ancPosX;
}

float Balle::getAncPosY()
{
	return m_ancPosY;
}


void Balle::rebondir(int l, int h)
{
	if (m_posX < 0 || m_posX > l)
		m_vitX *= -1;
	if (m_posY < 0 || m_posY > h)
		m_vitY *= -1;

}

void Balle::incrementerPerdue()
{
	m_estPerdue++;
}

int Balle::estPerdue()
{
	return m_estPerdue;	
}

float Balle::getEtat()
{
	return m_etat;
}

void Balle::incrementerEtat()
{
	m_etat += g_coeffVitesse;
	if (m_etat > 50)
		m_etat = 50;
}

void Balle::activer()
{
	m_active=true;
}

void Balle::initialiserCompteurRebond(int noTrait)
{
	m_collisionAvecTrait[noTrait] = 30;
}

void Balle::razCompteurRebond(int noTrait)
{
	m_collisionAvecTrait[noTrait] = 0;
}

bool Balle::compteurAZero(int noTrait)
{
	if (m_collisionAvecTrait[noTrait] > 0)
		m_collisionAvecTrait[noTrait]--;

	if (m_collisionAvecTrait[noTrait] == 0)
		return true;
	else 
		return false;

}

void Balle::ajouterCompteurTrait(int nbTrait)
{
	for (int i=0;i<nbTrait; i++)
		m_collisionAvecTrait.push_back(0);
}

int Balle::getJoueur()
{
	return m_joueur;
}

void Balle::setJoueur(int i)
{
	m_joueur = i;
}
