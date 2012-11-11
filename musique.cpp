#include "stdafx.h"
#include <SFML/Audio.hpp> 
#include <iostream>
#include "musique.h"

using namespace std;
using namespace sf;


Musique::Musique()
{ 
	// Ici on ajoute les morceaux 
	m_listeMusique.push_back(new Music());
	if (!m_listeMusique[0]->OpenFromFile("transit.ogg"))
	{
		cout << " transit.ogg introuvable" << endl;
	}

	m_listeMusique.push_back(new Music());
	if (!m_listeMusique[1]->OpenFromFile("galaxy.ogg"))
	{
		cout << " galaxy.ogg introuvable" << endl;
	}

	m_listeMusique.push_back(new Music());
	if (!m_listeMusique[2]->OpenFromFile("sparkling.ogg"))
	{
		cout << " sparkling.ogg introuvable" << endl;
	}

	m_nbMusiques = m_listeMusique.size();
	m_musiqueJouee = rand()%m_nbMusiques;
}

void Musique::jouer()
{
	if ( m_listeMusique[m_musiqueJouee]->GetStatus() == 0)
	{
		m_musiqueJouee++;
		if (m_musiqueJouee == m_nbMusiques)
			m_musiqueJouee = 0;
		m_listeMusique[m_musiqueJouee]->Play(); 
	}
 }

void Musique::arreter()
{
	m_listeMusique[m_musiqueJouee]->Stop();
}

void Musique::setPitch(float p)
{
	m_listeMusique[m_musiqueJouee]->SetPitch(p);

}
