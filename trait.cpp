#include "stdafx.h"
#include "trait.h"
#include <complex>
#include <cstdlib>

using namespace std;

#define PI 3.14159265358979323846264338327950288419716939937510582

// traits toujours construits à partir de terrain.
Trait::Trait(float x, float y, float angle, int longueur, int largeur)
:m_x(x), m_y(y), m_angle(angle), m_longueur(longueur), m_largeur(largeur)
{
}

float Trait::getX()
{
	return m_x;
}
float Trait::getY()
{
	return m_y;
}
float Trait::getRotation()
{
	return m_angle;
}
int Trait::getLargeur()
{
	return m_largeur;
}
int Trait::getLongueur()
{
	return m_longueur;
}
float Trait::getMinX()
{
	return (m_x - abs(m_longueur/2*cos(m_angle*PI/180)));
}

float Trait::getMaxX()
{
	return (m_x + abs(m_longueur/2*cos(m_angle*PI/180)));
}
