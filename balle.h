#pragma once
#include <vector>
class Balle
{
private:

	float m_posX, m_posY, m_vitX, m_vitY, m_ancPosX, m_ancPosY; // Coordonnées cartesienne
	float m_vitesse, m_angle; // Coordonnées polaires
	void convertirEnCart(); // définit vitX et vitY selon la vitesse et l'angle
	void convertirEnPol(); // inversement
	int m_rayon;
	int m_estPerdue;
	float m_etat;
	bool m_active;
	std::vector<int> m_collisionAvecTrait; // Pour empêcher ce bug à la con...
	int m_joueur;

public:
	Balle(float posX, float posY, float vitesse, float angle);
	float getPosX();
	float getVitX();
	float getPosY();
	float getVitY();
	float getAncPosX();
	float getAncPosY();
	float getVitesse();
	float getAngle();
	int estPerdue();
	int getRayon();
	int getJoueur();
	
	float getEtat();
	void incrementerEtat();
	void activer();

	void incrementerPerdue();

	// modifie les coordonnées polaires
	void setPosX(float x);
	void setVitX(float x);
	void setPosY(float y);
	void setVitY(float y);


	// Modifie aussi les coordonnées cartésiennes
	void setVitesse(float vit);
	void ajouterVitesseX(float vit);
	void ajouterVitesseY(float vit);
	void setAngle(float angle);
	void ajouterVitesse(float vit);
	void ajouterAngle(float angle);
	void bouger(float coeffVitesse=1);

	void setJoueur(int i);
	
	void rebondir(int l, int h);

	void initialiserCompteurRebond(int noTrait);
	bool compteurAZero(int noTrait); // vrai si fini
	void razCompteurRebond(int noTrait);
	void ajouterCompteurTrait(int nbTrait);

};
