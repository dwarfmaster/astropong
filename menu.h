#pragma once
#include <vector>
#include <SFML/Graphics.hpp> 
#include <SFML/Audio.hpp>
#include "balle.h"

class Menu
{

private:
	sf::Sprite m_sprAP, m_sprTitre, m_sprJouer, m_sprRetour, m_sprCredit, m_sprQuitter,
	m_spr1J, m_spr2J, m_spr3J, m_spr4J, m_sprSurvie, m_sprMulti;

	sf::Font m_font;

	sf::String m_txtNbJoueurs, m_txtCredit1, m_txtCredit2 ;
	std::vector<Balle> m_balle;
	std::vector <sf::Sprite> m_sprBalle;
	int m_hauteur, m_largeur;
	int m_sourisX, m_sourisY;
	int m_nbJoueurs, m_nbVies;

	bool m_survival;
	
	sf::Clock m_clock;


	int m_numMenu; 
/*	0 : menu principal
	1 : menu jouer
	2 : menu credit
	3 : menu type de jeu

*/

	sf::Music m_musiqueMenu;
	bool m_iaPresente;


public:
	Menu(sf::RenderWindow &app);
	bool evenement(sf::RenderWindow &app);
	void preparerSprites(sf::RenderWindow &app);
	void effacerEcran(sf::RenderWindow &app);
	void dessinerSprites(sf::RenderWindow &app, int niveauFondu=255);
	void afficherSprites(sf::RenderWindow &app);

	bool sourisSurSprites(const sf::Sprite &spr);
	void debuterPartie(sf::RenderWindow &app);
	int getNbJoueurs();
	int getNbVies();
	bool getIA();
	void bougerBalles();
	bool getSurvival();
};
