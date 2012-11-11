#pragma once
#include "terrain.h"
class Bonus
{

private:
	unsigned int m_noBonus; 
	unsigned int m_nbBonus;

	unsigned int m_largeur, m_hauteur;

	sf::SoundBuffer m_buffercasse, m_bufferVortex, m_bufferVortex2, m_bufferBonus;
	sf::Image m_imgBonus, m_imgVortex, m_imgVortex2, m_imgVortex2J;

	sf::Sound m_casse, m_sonBonus;
	sf::Sprite m_sprBonus, m_sprVortex, m_sprVortex2;

	sf::Clock m_clock; // Compteur qui compte le temps écoulé depuis la création du bonus.

	float m_x, m_y, m_angle, m_vitX, m_vitY;
	float m_coeffPuissance;

	sf::PostFX m_effet;

	std::vector<float> m_coordTerrainX;
	std::vector<float> m_coordTerrainY;

	int m_nbJoueurs;
	int m_noJoueur;

public:
	Bonus(sf::RenderWindow &app,
		std::vector<Balle> &balle,
		  Terrain terrain,
		  sf::SoundBuffer &bufferBonus,
		  sf::SoundBuffer &bufferCasse,
		  sf::SoundBuffer &bufferVortex,
		  sf::SoundBuffer &bufferVortex2,
		  sf::Image &imgBonus,
		  sf::Image &imgVortex,
		  sf::Image &imgVortex2,
		  sf::Image &imgVortex2J);
	bool appliquer(std::vector<Balle> &balle, std::vector<Joueur> &joueur);
	void dessinerSprites(sf::RenderWindow &app);
	void initialiserBonus(int noJoueur, std::vector<Joueur> &joueur);
	float distanceBonusBalleProche(std::vector<Balle> &balle);
};
