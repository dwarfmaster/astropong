#pragma once

class Trait
{

private:

	float m_y, m_x, m_angle; // Position et rotation de la raquette
	int m_largeur, m_longueur;

public:
	Trait(float x, float y, float angle, int longueur, int largeur);
	float getX();
	float getY();
	float getRotation();
	int getLargeur();
	int getLongueur();
	float getMinX();
	float getMaxX();
};
