#pragma once
#include <vector>
class Musique
{

private:
	std::vector<sf::Music*> m_listeMusique;
//	sf::Music m_musique0, m_musique1;
	int m_musiqueJouee;
	int m_nbMusiques;

public:
	Musique();
	void jouer();
	void arreter();
	void setPitch(float p);
};
