#include "stdafx.h"
#include <iostream>
#include <SFML/Graphics.hpp> 
#include <SFML/Audio.hpp> 
#include <complex>
#include "balle.h"
#include "bonus.h"

extern float g_coeffVitesse;

using namespace sf;
using namespace std;

#define PI 3.14159265358979323846264338327950288419716939937510582

#pragma warning (disable:4018) // Comparaison signé/non
#pragma warning (disable:4244) // conversion int en float

Bonus::Bonus(RenderWindow &app,
			 vector<Balle> &balle,
			 Terrain terrain,
			 SoundBuffer &bufferBonus,
			 SoundBuffer &buffercasse,
			 SoundBuffer &bufferVortex,
			 SoundBuffer &bufferVortex2,
			 Image &imgBonus,
			 Image &imgVortex,
			 Image &imgVortex2,
			 Image &imgVortex2J)

			 :m_bufferBonus(bufferBonus),
			  m_buffercasse(buffercasse),
			  m_bufferVortex(bufferVortex),
			  m_bufferVortex2(bufferVortex2),
			  m_imgBonus(imgBonus),
			  m_imgVortex(imgVortex),
			  m_imgVortex2(imgVortex2),
			  m_imgVortex2J(imgVortex2J)
{
	m_coordTerrainX = terrain.getCoordX();
	m_coordTerrainY = terrain.getCoordY();

	m_nbBonus = 2;
	m_largeur = app.GetWidth();
	m_hauteur = app.GetHeight();
	m_noBonus = 0;
	m_clock.Reset(); // Compteur pour savoir le temps écoulé depuis le début.
	m_nbJoueurs = m_coordTerrainX.size()/2;

	do{
		m_x = m_largeur/4 + rand()%m_largeur/2;
		m_y = m_hauteur/4 + rand()%m_hauteur/2;
	} while (distanceBonusBalleProche(balle) < 200);


	m_vitX = 0;
	m_vitY = 0;

		
}

// retourne faux si terminé.
bool Bonus::appliquer(vector<Balle> &balle, vector<Joueur> &joueur) // Faire bouger les balles, faire apparaître un truc, déplacer bonus...
{
	m_coeffPuissance = 1; 
	m_sonBonus.SetPosition(m_x,m_y,200);
	if (m_noBonus == 0)
	{

		if (m_clock.GetElapsedTime() < 2) // si il s'est écoulé - d'une seconde.
			m_coeffPuissance = m_clock.GetElapsedTime() / 2;
		
		if (m_coeffPuissance > 1)
			m_coeffPuissance = 1;
	
		// Son
		//

		// Bouger bonus.
		float dist = 10000;
		int numBalle = 0;
		for (int i=0;i<balle.size();i++)
		{
			float tmp = Terrain::calculeDistance(m_x, m_y, balle[i].getPosX(), balle[i].getPosY());
			if (tmp < dist)
			{
				dist = tmp;
				numBalle = i;
			}
		}

		if (dist < 150)
		{
			if (m_x < balle[numBalle].getPosX())
				m_vitX -= ((balle[numBalle].getPosX()-m_x) / dist) * ((balle[numBalle].getPosX()-m_x) / dist) * 0.2;
			else
				m_vitX += ((balle[numBalle].getPosX()-m_x) / dist) * ((balle[numBalle].getPosX()-m_x) / dist) * 0.2;

			if (m_y < balle[numBalle].getPosY())
				m_vitY -= ((balle[numBalle].getPosY()-m_y) / dist) * ((balle[numBalle].getPosY()-m_y) / dist) * 0.2;
			else
				m_vitY += ((balle[numBalle].getPosY()-m_y) / dist) * ((balle[numBalle].getPosY()-m_y) / dist) * 0.2;

		}
		if (m_vitX > 2 * (m_largeur / 1600.f))
			m_vitX = 2 * (m_largeur / 1600.f);
		if (m_vitY > 2 * (m_largeur / 1600.f))
			m_vitY = 2 * (m_largeur / 1600.f);
		if (m_vitX < -2 * (m_largeur / 1600.f))
			m_vitX = -2 * (m_largeur / 1600.f);
		if (m_vitY < -2 * (m_largeur / 1600.f))
			m_vitY = -2 * (m_largeur / 1600.f);
		
		// le bonus passe d'une extrimité de l'écran à l'autre
		if (m_x > m_largeur)
			m_x = 0;
		if (m_y > m_hauteur)
			m_y = 0;
		if (m_x < 0)
			m_x = m_largeur;
		if (m_y < 0)
			m_y = m_hauteur;


		m_x += m_vitX * g_coeffVitesse;
		m_y += m_vitY * g_coeffVitesse;

		if (m_sonBonus.GetStatus() == Sound::Stopped)
		{
			m_sonBonus.SetLoop(true);
			m_sonBonus.SetAttenuation(0);
			m_sonBonus.SetBuffer(m_bufferBonus);
			m_sonBonus.Play();
		}

		m_sonBonus.SetVolume(80*m_coeffPuissance);
		m_sonBonus.SetPosition(m_x, m_y,200);

		for (int i=0; i<balle.size(); i++)
		{
			float distance = sqrt((balle[i].getPosX() - m_x) * (balle[i].getPosX() - m_x) + (balle[i].getPosY() - m_y) * (balle[i].getPosY() - m_y));
			if (distance <= balle[i].getRayon() + 10)
			{
				//-------------- Son
				m_casse.SetAttenuation(0);
				m_casse.SetVolume(60);
				m_casse.SetPosition(m_x,m_y,200);
				m_casse.SetBuffer(m_buffercasse);
				m_casse.Play();
				
				// ------------------
				initialiserBonus(balle[i].getJoueur(), joueur);

			}

		}
	}

	if (m_noBonus == 1)
	{

		int forceVortex = 24; 

		if (m_clock.GetElapsedTime() < 2) // si il s'est écoulé - d'une seconde.
			m_coeffPuissance = m_clock.GetElapsedTime() / 2;
		
		if (m_coeffPuissance > 1)
			m_coeffPuissance = 1;

		if (m_clock.GetElapsedTime() > 12)
			m_coeffPuissance = (14 - m_clock.GetElapsedTime()) / 2.0;

			//SON
			m_sonBonus.SetVolume(80.0f*m_coeffPuissance);

		for (unsigned int i=0; i<balle.size(); i++)
		{
			float distance = sqrt((balle[i].getPosX() - m_x) * (balle[i].getPosX() - m_x) + (balle[i].getPosY() - m_y) * (balle[i].getPosY() - m_y));

		if (distance < 150)
			distance = 150;
		
			float dx = (m_x - balle[i].getPosX())/distance;
			float dy = (m_y - balle[i].getPosY())/distance;
			float vitesseHorizontal = (forceVortex*dx)/ (distance/**distance*/) * m_coeffPuissance * g_coeffVitesse;
			float vitesseVertical = (forceVortex*dy) / (distance/**distance*/) * m_coeffPuissance * g_coeffVitesse;

			balle[i].ajouterVitesseX(vitesseHorizontal);
			balle[i].ajouterVitesseY(vitesseVertical);	

		}
	}

	if (m_noBonus == 2)
	{
		if (m_noJoueur != -1)
		{
			m_x = joueur[m_noJoueur].getX();
			m_y = joueur[m_noJoueur].getY();
			m_angle = joueur[m_noJoueur].getRotation();
			m_x += 20* sin(m_angle * PI / 180);
			m_y -= 20* cos(m_angle * PI / 180);
		}

		int forceVortex = 5000; 

		if (m_clock.GetElapsedTime() < 2) // si il s'est écoulé - de 2 secondes.
			m_coeffPuissance = m_clock.GetElapsedTime() / 2;
		
		if (m_coeffPuissance > 1)
			m_coeffPuissance = 1;

		if (m_clock.GetElapsedTime() > 14)
			m_coeffPuissance = (16 - m_clock.GetElapsedTime()) / 2.0;

			//SON
			m_sonBonus.SetVolume(80.0f*m_coeffPuissance);

		for (unsigned int i=0; i<balle.size(); i++)
		{
			float distance = sqrt((balle[i].getPosX() - m_x) * (balle[i].getPosX() - m_x) + (balle[i].getPosY() - m_y) * (balle[i].getPosY() - m_y));

		if (distance < 80)
			distance = 80;
		
			float dx = (m_x - balle[i].getPosX())/distance;
			float dy = (m_y - balle[i].getPosY())/distance;
			float vitesseHorizontal = (forceVortex*dx)/ (distance*distance) * m_coeffPuissance * g_coeffVitesse;
			float vitesseVertical = (forceVortex*dy) / (distance*distance) * m_coeffPuissance * g_coeffVitesse;

			balle[i].ajouterVitesseX(-vitesseHorizontal);
			balle[i].ajouterVitesseY(-vitesseVertical);	

		}
	}

		if (m_sonBonus.GetStatus() == Sound::Stopped)
		 cout << " ??? " << endl;

	if (m_coeffPuissance < 0 || m_noBonus == -1)
		return false;
	else
		return true;
}

void Bonus::dessinerSprites(RenderWindow &app)
{

	//m_effet.SetTexture("framebuffer", NULL);
	//m_effet.SetParameter("color", 1.f, 1.f, 1.f);

	if (m_noBonus == 0)
	{

		m_sprBonus.SetImage(m_imgBonus); // Obligé de le préciser à chaque fois.

		m_sprBonus.SetScale(m_coeffPuissance, m_coeffPuissance);
		m_sprBonus.SetCenter(m_sprBonus.GetSize().x / 2.0 / m_coeffPuissance, m_sprBonus.GetSize().y / 2.0 / m_coeffPuissance);

		m_sprBonus.SetPosition(m_x,m_y);
		m_sprBonus.Rotate(-15 * g_coeffVitesse);
		app.Draw(m_sprBonus);
	}

	if (m_noBonus == 1)
	{

		m_sprVortex.SetImage(m_imgVortex); // Obligé de le préciser à chaque fois.

		m_sprVortex.SetScale(m_coeffPuissance, m_coeffPuissance);
		m_sprVortex.SetCenter(m_sprVortex.GetSize().x / 2.0 / m_coeffPuissance, m_sprVortex.GetSize().y / 2.0 / m_coeffPuissance);

		m_sprVortex.SetPosition(m_x,m_y);
		m_sprVortex.Rotate(-5 * g_coeffVitesse);
		app.Draw(m_sprVortex);
	}

	if (m_noBonus == 2)
	{
		if (m_noJoueur == -1)
			m_sprVortex2.SetImage(m_imgVortex2); // Obligé de le préciser à chaque fois.
		else
			m_sprVortex2.SetImage(m_imgVortex2J); // Obligé de le préciser à chaque fois.


		m_sprVortex2.SetScale(m_coeffPuissance, m_coeffPuissance);
		m_sprVortex2.SetCenter(m_sprVortex2.GetSize().x / 2.0 / m_coeffPuissance, m_sprVortex2.GetSize().y / 2.0 / m_coeffPuissance);

		m_sprVortex2.SetPosition(m_x,m_y);
		if (m_noJoueur == -1)
			m_sprVortex2.Rotate(-5 * g_coeffVitesse);
		else
			m_sprVortex2.SetRotation(-m_angle);


		app.Draw(m_sprVortex2);
	}
	//app.Draw(m_effet);

}

void Bonus::initialiserBonus(int noJoueur, vector<Joueur> &joueur)
{
	m_noJoueur = noJoueur;

	m_noBonus = rand()%m_nbBonus + 1;

	//DEBUG
	//m_noBonus = 2;
	m_clock.Reset();

	if (m_noBonus == 1) // Vortex qui attire ?
	{

		if ( m_noJoueur != -1) // Si il s'agit bien d'un joueur qui a touché le bonus.
		{
			// Faut déterminer qui se prendra le vortex, niak.
			int joueurPenalise;
			int scoreMax = -1;
			for (int i=0; i<joueur.size();i++)
			{
				if (joueur[i].getScore() > scoreMax && i != noJoueur)
				{
					scoreMax = joueur[i].getScore();
					joueurPenalise = i;
				}
			}

			m_x = (m_coordTerrainX[joueurPenalise*2] + m_coordTerrainX[joueurPenalise*2 + 1]) / 2;
			m_y = (m_coordTerrainY[joueurPenalise*2] + m_coordTerrainY[joueurPenalise*2 + 1]) / 2;

			float angle = joueur[joueurPenalise].getRotation();
			m_x -= 50* sin(angle * PI / 180);
			m_y += 50* cos(angle * PI / 180);

		}

		// Jouer son
		m_sonBonus.SetBuffer(m_bufferVortex);
		m_sonBonus.SetAttenuation(0);
		m_sonBonus.SetPosition(m_x,m_y,200);
		m_sonBonus.Play();

	}


if (m_noBonus == 2) // Vortex qui repousse ?
	{

		// Jouer son
		m_sonBonus.SetBuffer(m_bufferVortex2);
		m_sonBonus.SetAttenuation(0);
		m_sonBonus.SetPosition(m_x,m_y,200);
		m_sonBonus.Play();

	}
}

float Bonus::distanceBonusBalleProche(vector<Balle> &balle)
{
	float dist = 10000;
	for (int i=0;i<balle.size();i++)
	{
		float tmp = Terrain::calculeDistance(m_x, m_y, balle[i].getPosX(), balle[i].getPosY());
		if (tmp < dist)
			dist = tmp;
	}

	return dist;
}
