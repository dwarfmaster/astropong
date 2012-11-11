#pragma once


#include <vector>
#include <SFML/Graphics.hpp> 
#include <SFML/Audio.hpp>
#include "balle.h"
#include "joueur.h"
#include "trait.h"
#include "terrain.h"
#include "musique.h"
#include "bonus.h"

class Partie
{
private:
	unsigned int m_hauteur, m_largeur, m_nbJoueurs;  
	std::vector<Joueur> m_joueur;
	std::vector<Balle> m_balle;
	std::vector<Trait> m_trait;
	std::vector <sf::Sprite> m_sprBalle;
	std::vector <sf::Sprite> m_sprEclair;
	int m_compteurEclair;
	std::vector<sf::String> m_texteScore;
	std::vector<sf::String> m_texteTouches;


	sf::SoundBuffer m_bufferCasse, m_bufferVortex, m_bufferVortex2, m_bufferBonus;
	sf::Image m_imgVortex, m_imgVortex2, m_imgVortex2J, m_imgBonus;

	sf::Font m_font;
	sf::Clock m_clock;

	sf::Image m_imgEclair;

	sf::Sprite m_sprTrou;
	sf::Sprite m_sprAP;

	sf::Sound m_sonRebond, m_sonRebondRaquette, m_sonEclair;
	Musique m_musique;
	float m_compteurTransparenceTouches;
	bool m_iaPresente; // IA activée ?

	std::vector<Bonus> m_bonus;

	float m_vitesseInitialeBalle;

	bool m_retourMenu;

public:
	Partie(sf::RenderWindow &app, int nbJoueurs, int nPoints = 30, bool iaPresente = false);
	void collision();
	void bougerAjouterBalles(sf::Sprite &sprBalle, float coeffVitesse=1);
	void appliquerAjouterBonus(sf::RenderWindow &app, Terrain &terrain);
	void evenement(sf::RenderWindow &app);
	void preparerSprites(sf::RenderWindow &app, std::vector<sf::Shape> &imgRaquettes, std::vector<sf::Shape> &imgTrait);
	void dessinerSprites(sf::RenderWindow &app, std::vector<sf::Shape> &imgRaquettes, std::vector<sf::Shape> &imgTrait, int niveauFondu=255);
	void gererBallePerdue();
	std::string nbToString(int x);
	bool joueurElimine(Terrain &terrain);
	void finPartie(sf::RenderWindow &app, std::vector<sf::Shape> &imgRaquettes, std::vector<sf::Shape> &imgTrait, sf::View &vue, sf::Sprite &sprBalle);
	void effacerEcran(sf::RenderWindow &app);
	void afficherSprites(sf::RenderWindow &app);
	void deplacerIA();
	sf::Color definirCouleur(int nojoueur, float opacite=255);



};

// PARTIE SOLO -------------------------------------------


class PartieSolo
{
private:
	float m_score;
	unsigned int m_hauteur, m_largeur;  
	std::vector<Joueur> m_joueur;
	std::vector<Balle> m_balle;
	std::vector<Trait> m_trait;
	std::vector <sf::Sprite> m_sprBalle;
	std::vector <sf::Sprite> m_sprEclair;
	int m_compteurEclair;
	std::vector<sf::String> m_texteScore;
	std::vector<sf::String> m_texteTouches;

	sf::String m_texteVraiScore;

	sf::String m_resGlobal, m_resEcran;

	sf::SoundBuffer m_bufferCasse, m_bufferVortex, m_bufferVortex2, m_bufferBonus;
	sf::Image m_imgVortex, m_imgVortex2, m_imgVortex2J, m_imgBonus;

	sf::Font m_font;
	sf::Clock m_clock, m_clockCouleur;

	sf::Image m_imgEclair;

	sf::Sprite m_sprTrou;
	sf::Sprite m_sprAP;


	sf::Sound m_sonRebond, m_sonRebondRaquette, m_sonEclair, m_sonBam;
	Musique m_musique;
	float m_compteurTransparenceTouches;
	std::vector<Bonus> m_bonus;
	float m_vitesseInitialeBalle;
	bool m_retourMenu;
	bool m_estConnecte;

public:
	PartieSolo(sf::RenderWindow &app, int nPoints = 30);
	void collision();
	void bougerAjouterBalles(sf::Sprite &sprBalle, float coeffVitesse=1);
	void appliquerAjouterBonus(sf::RenderWindow &app, Terrain &terrain);
	void evenement(sf::RenderWindow &app);
	void preparerSprites(sf::RenderWindow &app, std::vector<sf::Shape> &imgRaquettes, std::vector<sf::Shape> &imgTrait);
	void dessinerSprites(sf::RenderWindow &app, std::vector<sf::Shape> &imgRaquettes, std::vector<sf::Shape> &imgTrait, int niveauFondu=255);
	void gererBallePerdue();
	std::string nbToString(int x);
	bool joueurElimine(Terrain &terrain);
	void finPartie(sf::RenderWindow &app, std::vector<sf::Shape> &imgRaquettes, std::vector<sf::Shape> &imgTrait, sf::View &vue, sf::Sprite &sprBalle);
	void effacerEcran(sf::RenderWindow &app);
	void afficherSprites(sf::RenderWindow &app);
	void deplacerIA();
	sf::Color definirCouleur(int nojoueur, float opacite=255);



};
