#include "stdafx.h"
#include <SFML/Graphics.hpp> 
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <vector>
#include <complex>
#include "joueur.h"
#include "bonus.h"
#include "balle.h"
#include "terrain.h"
#include "musique.h"
#include "partie.h"

#define PI 3.14159265358979323846264338327950288419716939937510582

using namespace sf;
using namespace std;

extern float g_coeffVitesse;

#pragma warning (disable:4018) // Comparaison signé/non
#pragma warning (disable:4244) // conversion int en float

PartieSolo::PartieSolo(RenderWindow &app, int nPoints)
{

// Récupération du meilleur score !

	m_hauteur=app.GetHeight();
	m_largeur=app.GetWidth();

	int mdp = (m_largeur + 456) * (m_hauteur - 236) - 1 ;

	std::ostringstream oss1, oss2, oss3;
	oss1 << m_largeur; 
	oss2 << m_hauteur; 
	oss3 << mdp; 


	Http http;
	http.SetHost("http://happywebsite.free.fr");	

    Http::Request Request;
    Request.SetMethod(sf::Http::Request::Get);
    Request.SetURI("/happyteam/parties/jeux/pong/getScore.php?largeur=" + oss1.str() + "&hauteur=" + oss2.str() + "&mdp=" + oss3.str());

	Http::Response rep = http.SendRequest(Request);

	if(rep.GetStatus() == Http::Response::Ok) // L'envoi au serveur a fonctionné :)
	{
		vector<string> tabRes;
		string res = rep.GetBody();

		for (int i=0;i<4;i++)
		{
			int pos = res.find("-");
			tabRes.push_back(res);
			if (i<3)
			{
				tabRes[i].erase(pos);
				res = res.substr(pos+1);
			}
		}

		m_resGlobal.SetFont(m_font);
		m_resGlobal.SetText("Meilleur score global :\n\n" + tabRes[1] + "\n Par " + tabRes[0]);
		m_resGlobal.SetSize(28 * m_largeur / 1600);

		m_resEcran.SetFont(m_font);
		m_resEcran.SetText("Meilleur score pour\nvotre écran :\n\n" + tabRes[3] + "\n Par " + tabRes[2]);
		m_resEcran.SetSize(28 * m_largeur / 1600);

		m_resGlobal.SetPosition(10,40);
		m_resEcran.SetPosition(m_largeur*5/6 + 5,40);

		m_estConnecte = true;

	}
	else
		m_estConnecte = false;

// Init du reste


	m_retourMenu = false;
	m_score = 0;

	// Initialisation
	app.ShowMouseCursor(false);  // Tu vas marcher, connard ?


	Vector2f centre(m_largeur/2, m_hauteur/2);
	Vector2f demiTaille(m_largeur/2, m_hauteur/2);
	View vue(centre, demiTaille);
	app.SetView(vue);

	Terrain terrain (1, m_joueur, m_largeur, m_hauteur, nPoints, m_trait);

	m_compteurEclair = 0;

	Image imgAP;
	if (!imgAP.LoadFromFile("Nebula.jpg"))
	{
		cout << "Nebula introuvable" << endl;
		return; // On ferme le programme
	}

	m_sprAP.SetImage(imgAP);
	m_sprAP.SetPosition(0,0);

	m_sprAP.SetScale( m_hauteur/m_sprAP.GetSize().y, m_hauteur/m_sprAP.GetSize().y);
	if (m_largeur > m_sprAP.GetSize().x)
		m_sprAP.SetScale( m_largeur/m_sprAP.GetSize().x, m_largeur/m_sprAP.GetSize().x);


	// Chargement image balle
	Image imgBalle;
	if (!imgBalle.LoadFromFile("Balle.bmp"))
	{
		cout << "Balle.bmp introuvable" << endl;
		return; // On ferme le programme
	}
	imgBalle.CreateMaskFromColor(Color(0, 0, 255));

	Sprite sprBalle;
	sprBalle.SetImage(imgBalle);
	sprBalle.SetCenter(7,7); // Centre de la balle

	Image imgTrou;
	if (!imgTrou.LoadFromFile("Trou.bmp"))
	{
		cout << "Trou.bmp introuvable" << endl;
		return; // On ferme le programme
	}
	imgTrou.CreateMaskFromColor(Color(0, 0, 255));
	
	m_sprTrou.SetImage(imgTrou);
	m_sprTrou.SetCenter(10,10); // Centre du trou
	m_sprTrou.SetPosition(m_largeur/2, m_hauteur/4);


	// Dessin raquettes
	vector<Shape> imgRaquettes;	
	imgRaquettes.push_back(Shape::Rectangle(0.0f, 0.0f, m_joueur[0].getLongueur(), m_joueur[0].getLargeur(), definirCouleur(0)));
	imgRaquettes[0].SetCenter(m_joueur[0].getLongueur()/2,m_joueur[0].getLargeur()/2); // Centre au milieu

	// Dessin trait pour perdant


	if (!m_imgEclair.LoadFromFile("imageEclair.png"))
	{
		cout << "imageEclair.png introuvable" << endl;
		return; // On ferme le programme
	}
	m_imgEclair.CreateMaskFromColor(Color(255, 255, 255));


	// Dessin trait
	vector<Shape> imgTrait;
	
	for (int i=0; i<m_trait.size(); i++)
		{
			imgTrait.push_back(Shape::Rectangle(0.0, 0.0, m_trait[i].getLongueur(), m_trait[i].getLargeur(),Color(255,255,255)));
			imgTrait[i].SetCenter(m_trait[i].getLongueur()/2 - 1, m_trait[i].getLargeur()/2);
		}



	if(!m_font.LoadFromFile("LinLibertine_R.ttf",80))
{
    cout << "police introuvable" << endl;
	return;
}
	m_texteVraiScore.SetFont(m_font);
	m_texteVraiScore.SetPosition(m_largeur/5, 10);
	m_texteVraiScore.SetColor(Color(200,200,200));
	m_texteVraiScore.SetSize(70);

	m_texteScore.push_back(String("0",m_font,40));
	m_texteScore[0].SetPosition(m_largeur*4/5, 10);
	m_texteScore[0].SetColor(Color(210,210,150));
	m_texteScore[0].SetCenter(60,10);

	m_texteTouches.push_back(String("",m_font,30));
	m_texteTouches[0].SetColor(Color(210,210,150));
	m_texteTouches[0].SetPosition(m_joueur[0].getIndicationTouchesPosX(), m_joueur[0].getIndicationTouchesPosY());
	m_texteTouches[0].SetCenter(0,20);
	
	m_texteTouches[0].SetText("Gauche \nDroite");
	
	m_compteurTransparenceTouches = 0;


//---- SON -------

	Listener::SetPosition(m_largeur/2, m_hauteur/2, 0.0f); // Position du joueur.
	Listener::SetTarget(0,0,-1);

	SoundBuffer rebond;
	if (!rebond.LoadFromFile("rebond.ogg"))
	{
		cout << "rebond.ogg introuvable" << endl;
	}
	m_sonRebond.SetBuffer(rebond);
	m_sonRebond.SetAttenuation(0);
	m_sonRebond.SetVolume(70);

	SoundBuffer rebondRaquette;
	if (!rebondRaquette.LoadFromFile("rebondRaquette.ogg"))
	{
		cout << "rebondRaquette.ogg introuvable" << endl;
	}
	m_sonRebondRaquette.SetBuffer(rebondRaquette);
	m_sonRebondRaquette.SetAttenuation(0);
	m_sonRebondRaquette.SetVolume(50);

	SoundBuffer eclair;
	if (!eclair.LoadFromFile("eclair.ogg"))
	{
		cout << "eclair.ogg introuvable" << endl;
	}
	m_sonEclair.SetBuffer(eclair);
	m_sonEclair.SetAttenuation(0);
	m_sonEclair.SetVolume(50);

	SoundBuffer bam;
	if (!bam.LoadFromFile("bam.ogg"))
	{
		cout << "bam.ogg introuvable" << endl;
	}
	m_sonBam.SetBuffer(bam);
	m_sonBam.SetAttenuation(0);
	m_sonBam.SetVolume(80);


// -------------- NECESSAIRE POUR BONUS ! ----------------------------------
		if (!m_imgBonus.LoadFromFile("bonus.png"))
		{
			cout << "bonus.png introuvable" << endl;
			exit(2); // On ferme le programme
		}
		m_imgBonus.CreateMaskFromColor(Color(0, 0, 0));
	
	if (!m_imgVortex.LoadFromFile("vortex.png"))
		{
			cout << "vortex.png introuvable" << endl;
			exit(2); // On ferme le programme
		}
	if (!m_imgVortex2.LoadFromFile("vortex2.png"))
		{
			cout << "vortex2.png introuvable" << endl;
			exit(2); // On ferme le programme
		}
	if (!m_imgVortex2J.LoadFromFile("vortex2J.png"))
		{
			cout << "vortex2J.png introuvable" << endl;
			exit(2); // On ferme le programme
		}

	if (!m_bufferBonus.LoadFromFile("bonus.ogg"))
	{
		cout << "bonus.ogg introuvable" << endl;
	}

	if (!m_bufferCasse.LoadFromFile("casse.ogg"))
	{
		cout << "casse.ogg introuvable" << endl;
	}

	if (!m_bufferVortex.LoadFromFile("vortex.ogg"))
	{
		cout << "vortex.ogg introuvable" << endl;
	}
	if (!m_bufferVortex2.LoadFromFile("vortex2.ogg"))
	{
		cout << "vortex2.ogg introuvable" << endl;
	}
//---------------------------------------------------------

	m_clock.Reset();
	m_clockCouleur.Reset();
	while (m_clock.GetElapsedTime() < 0.1f);
	bool continuer = true;

	int i=0;
	while (i<255)
	{
		float framerate = 1.f / m_clock.GetElapsedTime();
		m_clock.Reset();
		g_coeffVitesse = 60.f/framerate;

		if (i<255)
			i+= 2*g_coeffVitesse;
		if (i>255)
			i = 255;

		evenement(app);// S'occupe des événements claviers
		preparerSprites(app, imgRaquettes, imgTrait);
		effacerEcran(app);
		dessinerSprites(app, imgRaquettes, imgTrait, i);
		afficherSprites(app);
	}


//--------------------------------BOUCLE PRINCIPALE-----------------------------------
	while (app.IsOpened() && continuer)
	{
		//Image i;
		//i.CopyScreen(app);
		//i.GetPixel


		if (m_retourMenu)
		{
			m_musique.arreter();
			return;
		}
		float framerate = 1.f / m_clock.GetElapsedTime();
		m_clock.Reset();
		g_coeffVitesse = 60.f/framerate;

		int nbBalleColories = 0;
		for (int i=0; i< m_balle.size();i++)
		{
			if (m_balle[i].getJoueur() == 0)
				nbBalleColories++;
		}
		
		m_score += g_coeffVitesse/12 * nbBalleColories;

		evenement(app);// S'occupe des événements claviers
		collision(); // Redéfinit le mouvement de la balle. Vérifie aussi si la balle est perdue.
		bougerAjouterBalles(sprBalle); // Bouge les balles
		gererBallePerdue();
		//appliquerAjouterBonus(app, terrain);
		preparerSprites(app, imgRaquettes, imgTrait);
		effacerEcran(app);
		dessinerSprites(app, imgRaquettes, imgTrait, 255);
		afficherSprites(app);
		m_musique.jouer();
		continuer = joueurElimine(terrain);

    } 
	finPartie(app, imgRaquettes, imgTrait, vue, sprBalle);
//------------------------------------------------------------------------------------
}



void PartieSolo::collision()
{

	for(int j=0; j<m_balle.size(); j++)
	{
		// Objectif : obtenir les coordonnées du point de la raquette perpendiculaire à la balle
		bool collision;
		float aRaquette, aBalle, bRaquette, bBalle, aTrait, bTrait;
		int x,y;
		bool sensHoraire;

		// RAQUETTE
		int i=0;

		if (m_joueur[0].estVivant())
		{

			collision = true;
			// Méthode simple pour raquette verticale pour determiner (x,y)
			if ((int)m_joueur[i].getRotation() % 180 == 90)
			{
				x = m_joueur[i].getX();
				y = m_balle[j].getPosY();  

				if (m_balle[j].getPosY()+m_balle[j].getRayon() < m_joueur[i].getY() - m_joueur[i].getLongueur()/2) // Balle trop haute ?
					collision = false;
				if (m_balle[j].getPosY() - m_balle[j].getRayon() > m_joueur[i].getY() + m_joueur[i].getLongueur()/2) // Balle trop basse ?
					collision = false;	

			}
			else
			{
				// Méthode complexe dont je suis vachement fier :D 
				// +- 10° min pour être sûr !
				

				// Déterminer l'equation y=ax+b pour les 2 droites qui nous intéressent
				aRaquette = tan(m_joueur[i].getRotation() *PI/180); // coeff "a" de la droite parallèle à la raquette
				aBalle = tan((m_joueur[i].getRotation() + 90) *PI/180); // coeff "a" de la droite perpendiculaire à la raquette passant par la balle

				// b = yi -a*xi
				bRaquette = m_joueur[i].getY() - aRaquette*m_joueur[i].getX() ;
				bBalle = m_balle[j].getPosY() - aBalle*m_balle[j].getPosX() ;

				// x = (b'-b)/(a-a')
				x = int((bBalle - bRaquette)/(aRaquette - aBalle));

				// y = ax+b
				y = int(aRaquette*x + bRaquette);

				if (x < m_joueur[i].getMinX() 
				|| x > m_joueur[i].getMaxX())
					collision = false; // x sur la raquette
			}

			// Test pour savoir si il y a bien collision (distance balle-raquette mesurée)
			//RACINE CARREE !
			if (sqrt((m_balle[j].getPosX() - x)*(m_balle[j].getPosX() - x) + (m_balle[j].getPosY() - y)*(m_balle[j].getPosY() - y))  // distance balle raquette
				<= m_joueur[i].getLargeur()/2 + m_balle[j].getRayon()
				&& collision == true) // Distance tailleBalle + demi-largeur Raquette

			{

			//---------------SON-----------------
				m_sonRebondRaquette.SetPosition(m_balle[j].getPosX(),m_balle[j].getPosY(),200);
				m_sonRebondRaquette.Play();
			//-----------------------------------

				m_balle[j].setJoueur(i);

				for (int k=0; k<m_trait.size();k++)
					m_balle[j].razCompteurRebond(k);
				

				m_balle[j].setAngle(m_joueur[i].getRotation() - 90);

				// On remet la balle à sa vitesse initiale
				m_balle[j].setVitesse(m_vitesseInitialeBalle);

				// Calcul distance entre le centre de la raquette et le point de contact (x,y)
				float distance = sqrt((x-m_joueur[i].getX())*(x-m_joueur[i].getX()) + (y-m_joueur[i].getY())*(y-m_joueur[i].getY()));

				// Determination sens à appliquer à l'angle
				sensHoraire = false;
				if(m_joueur[i].getRotation() >= 315 || m_joueur[i].getRotation() < 45) // vers le haut
				{
					if (m_balle[j].getPosX() > m_joueur[i].getX())
						sensHoraire = true;
				}	
				if(m_joueur[i].getRotation() >= 45 && m_joueur[i].getRotation() < 135) // vers la droite
				{
					if (m_balle[j].getPosY() > m_joueur[i].getY())
						sensHoraire = true;
				}
				if(m_joueur[i].getRotation() >= 135 && m_joueur[i].getRotation() < 225) // vers le bas
				{
					if (m_balle[j].getPosX() < m_joueur[i].getX())
						sensHoraire = true;
				}
				if(m_joueur[i].getRotation() >= 225 && m_joueur[i].getRotation() < 315) // vers la gauche
				{
					if (m_balle[j].getPosY() < m_joueur[i].getY())
						sensHoraire = true;
				}
	//----------------------------------------------
				int deviation = 130;
	//---------------------------------------------
				if (sensHoraire)
					m_balle[j].ajouterAngle(deviation*distance/m_joueur[i].getLongueur());
				else
					m_balle[j].ajouterAngle(-deviation*distance/m_joueur[i].getLongueur());


			}
			else // Si il n'y a pas collision, on vérifie si la balle n'est pas perdue
			{
				if(sqrt((m_balle[j].getPosX() - x)*(m_balle[j].getPosX() - x) + (m_balle[j].getPosY() - y)*(m_balle[j].getPosY() - y))  
				> m_joueur[i].getLargeur()/2 + m_balle[j].getRayon()) // Si la balle dépasse la raquette d'un sens ou dans l'autre
				{

			

					// Il faut vérifier dans quel sens il dépasse
					if(m_joueur[i].getRotation() >= 315 || m_joueur[i].getRotation() < 45) // vers le haut
					{
						if (m_balle[j].getPosY() > y && m_balle[j].estPerdue() == -1)
						{
							m_balle[j].incrementerPerdue();
							m_joueur[i].decrementerScore();
						}
					}	
					if(m_joueur[i].getRotation() >= 45 && m_joueur[i].getRotation() < 135) // vers la droite
					{
						if (m_balle[j].getPosX() < x && m_balle[j].estPerdue() == -1)
						{
							m_balle[j].incrementerPerdue();
							m_joueur[i].decrementerScore();
						}
					}
					if(m_joueur[i].getRotation() >= 135 && m_joueur[i].getRotation() < 225) // vers le bas
					{
						if (m_balle[j].getPosY() < y && m_balle[j].estPerdue() == -1)
						{
							m_balle[j].incrementerPerdue();
							m_joueur[i].decrementerScore();
						}
					}
					if(m_joueur[i].getRotation() >= 225 && m_joueur[i].getRotation() < 315) // vers la gauche
					{
						if (m_balle[j].getPosX() > x && m_balle[j].estPerdue() == -1)
						{
							m_balle[j].incrementerPerdue();
							m_joueur[i].decrementerScore();
						}
					}
				}
			}
		}
		// TRAIT
		for (int i=0; i<m_trait.size();i++)
		{
			collision = true;
			// Méthode simple pour trait vertical pour determiner (x,y)
			if ((int)m_trait[i].getRotation() % 180 == 90)
			{
				x = m_trait[i].getX();
				y = m_balle[j].getPosY();  

				if (m_balle[j].getPosY()+m_balle[j].getRayon() < m_trait[i].getY() - m_trait[i].getLongueur()/2) // Balle trop haute ?
					collision = false;
				if (m_balle[j].getPosY() - m_balle[j].getRayon() > m_trait[i].getY() + m_trait[i].getLongueur()/2) // Balle trop basse ?
					collision = false;	

			}
			else
			{
			
				// +- 10° min pour être sûr !
				

				// Déterminer l'equation y=ax+b pour les 2 droites qui nous intéressent
				aTrait = tan(m_trait[i].getRotation() *PI/180); // coeff "a" de la droite parallèle à la raquette
				aBalle = tan((m_trait[i].getRotation() + 90) *PI/180); // coeff "a" de la droite perpendiculaire à la raquette passant par la balle

				// b = yi -a*xi
				bTrait = m_trait[i].getY() - aTrait*m_trait[i].getX() ;
				bBalle = m_balle[j].getPosY() - aBalle*m_balle[j].getPosX() ;

				// x = (b'-b)/(a-a')
				x = int((bBalle - bTrait)/(aTrait - aBalle));

				// y = ax+b
				y = int(aTrait*x + bTrait);

				if (x < m_trait[i].getMinX() 
				|| x > m_trait[i].getMaxX())
					collision = false; // x sur le trait
			}

			// Test pour savoir si il y a bien collision (distance balle-raquette mesurée)
			//RACINE CARREE !
			if (m_balle[j].compteurAZero(i) && sqrt((m_balle[j].getPosX() - x)*(m_balle[j].getPosX() - x) + (m_balle[j].getPosY() - y)*(m_balle[j].getPosY() - y))  // distance balle raquette
				<= m_trait[i].getLargeur()/2 + m_balle[j].getRayon()
				&& collision == true) // JE VERIFIE QU'IL YA AIT PAS EU DE COLLISION JUSTE AVANT.

			{
			//----------SON------------------------------
				m_sonRebond.SetPosition(m_balle[j].getPosX(),m_balle[j].getPosY(),200);
				m_sonRebond.Play(); // son. Décalé :(
			//-------------------------------------------
		
				m_balle[j].initialiserCompteurRebond(i);

				int normale;
				if ((int)(m_trait[i].getRotation()) %180 >= 45 &&(int)( m_trait[i].getRotation()) %180 <135 ) // Raquette horizontale, verifier x
				{
					if (m_balle[j].getPosX() < x) // Balle à gauche
						normale =  ((int)(m_trait[i].getRotation()) %180) + 90;
					else
						normale =  ((int)(m_trait[i].getRotation()) %180) - 90;

				}
				else
				{
					if (m_balle[j].getPosY() < y) // Balle en haut
						normale =  ((int)(m_trait[i].getRotation()) %180) - 90;
					else
						normale =  ((int)(m_trait[i].getRotation()) %180) + 90;
				}
				if (normale<0)
					normale+=360;

		
				 m_balle[j].ajouterAngle(180); // Sens inverse
				 m_balle[j].ajouterAngle(2*(normale - m_balle[j].getAngle()));

							
			}
		}
	}
}

void PartieSolo::bougerAjouterBalles(sf::Sprite &sprBalle, float coeffVitesse)
{
	bool ajouterBalle = false;
	if (m_balle.size() == 0 || rand()%((int)(120/g_coeffVitesse)) == 0) // Pas de balle, ou alors une fois sur 100
		ajouterBalle = true;

	if (ajouterBalle)
	{
		m_vitesseInitialeBalle = 6 * m_largeur/1600.f;

		m_sprBalle.push_back(sprBalle);
		m_balle.push_back(Balle(m_largeur/2, m_hauteur/4, m_vitesseInitialeBalle, rand()%360));
		m_balle[m_balle.size() - 1].ajouterCompteurTrait(m_trait.size());

	}

	for(int i=0; i<m_balle.size(); i++)
	{
		if (m_balle[i].getEtat() < 50) // Etat 50 : balle en marche !
		{
				
				m_balle[i].incrementerEtat();
				m_sprBalle[i].SetScale((m_balle[i].getEtat())/50.0, (m_balle[i].getEtat())/50.0);
				
				if (m_balle[i].getEtat() >= 50)
					m_balle[i].activer();

		}
		m_balle[i].bouger(coeffVitesse);
	} 
}


void PartieSolo::evenement(RenderWindow &app)
{


	Event event;
    while (app.GetEvent(event)) 
    {
		if (event.Type == Event::KeyPressed) // Appui sur une touche du clavier
        {
            switch (event.Key.Code) // La touche qui a été appuyée
            {
                case Key::Escape : // Echap
                    m_retourMenu = true;
					break;
				case Key::F1 :
					m_compteurTransparenceTouches = 0;
					break;
			}
		}
    }
	const Input & input = app.GetInput();

// Définition des touches
		if(input.IsKeyDown(Key::Left))
			m_joueur[0].bougerDir1();
		if(input.IsKeyDown(Key::Right))
			m_joueur[0].bougerDir2();
	
}



//---------------------------------
void PartieSolo::preparerSprites(RenderWindow &app, vector<Shape> &imgRaquettes, vector<Shape> &imgTrait)
{
		//----------Placement des sprites sur les positions des objets---------------
		
		for (int i=0;i<m_balle.size();i++)
		{
			m_sprBalle[i].SetPosition(m_balle[i].getPosX(), m_balle[i].getPosY());
		}

		for (int i=0; i<imgTrait.size();i++)
		{
			imgTrait[i].SetPosition(m_trait[i].getX(), m_trait[i].getY());
			imgTrait[i].SetRotation(-m_trait[i].getRotation());
			imgTrait[i].SetScaleY(m_trait[i].getLargeur());

		}
	
		if (m_joueur[0].estVivant())
		{	
			imgRaquettes[0].SetPosition(m_joueur[0].getX(),m_joueur[0].getY());
			imgRaquettes[0].SetRotation(-(m_joueur[0].getRotation())); // Les raquettes tournent dans le sens des aiguilles d'une montre
			m_texteScore[0].SetText("Vies :\n  " + nbToString(m_joueur[0].getScore()));
		}


		m_texteVraiScore.SetText(nbToString(m_score));

		// Les eclairs.
		m_compteurEclair += g_coeffVitesse;
		if (m_compteurEclair > 8)
		m_compteurEclair = 0;

		for (int i=0; i<m_sprEclair.size();i++)
		{
			if (m_compteurEclair <= 2)
				m_sprEclair[i].SetSubRect(IntRect(21,21,655,58));
			else if (m_compteurEclair <= 5)
				m_sprEclair[i].SetSubRect(IntRect(20,133,666,168));
			else
				m_sprEclair[i].SetSubRect(IntRect(20,245,666,278));

		}
		
}

void PartieSolo::effacerEcran(sf::RenderWindow &app)
{
       // Remplissage de l'écran (couleur noire par défaut)
        app.Clear(Color(0,0,0));
}

void PartieSolo::dessinerSprites(RenderWindow &app, vector<Shape> &imgRaquettes, vector<Shape> &imgTrait, int niveauFondu)
{

//----------Affichage des sprites---------------
	
	m_sprAP.SetColor(Color(255,255,255,niveauFondu));

	if (m_compteurTransparenceTouches < 800)
		m_compteurTransparenceTouches += g_coeffVitesse;
	if (m_compteurTransparenceTouches > 800)
		m_compteurTransparenceTouches = 800;
	int transparenceTouche = 1600 - m_compteurTransparenceTouches*2;


	app.Draw(m_sprAP);
	app.Draw(m_sprTrou);

	for (int i=0; i<m_bonus.size(); i++)
		m_bonus[i].dessinerSprites(app);

	// Dessin des balles. Couleur selon le joueur.
	

		if (transparenceTouche <= 255)
			m_texteTouches[0].SetColor(Color(210,210,150,transparenceTouche));
		else
			m_texteTouches[0].SetColor(Color(210,210,150));

		app.Draw(m_texteVraiScore);

		app.Draw(m_texteScore[0]);
		app.Draw(m_texteTouches[0]);
		if (m_joueur[0].estVivant())
			app.Draw(imgRaquettes[0]);

	for (int i=0;i<m_balle.size();i++)
	{
		float opacite = 255;
		if (m_balle[i].estPerdue() > -1)
			opacite = 255 - m_balle[i].estPerdue()*8.3;


		m_sprBalle[i].SetColor(definirCouleur(m_balle[i].getJoueur(), opacite));
		app.Draw(m_sprBalle[i]);
	}

	for (int i=0; i<imgTrait.size();i++)
		app.Draw(imgTrait[i]);

	for (int i=0; i<m_sprEclair.size();i++)
		app.Draw(m_sprEclair[i]);

	app.Draw(m_resGlobal);
	app.Draw(m_resEcran);
}


void PartieSolo::afficherSprites(sf::RenderWindow &app)
{
        // Affichage de la fenêtre à l'écran
        app.Display();
}



void PartieSolo::gererBallePerdue()
{
	for (int i=0; i<m_balle.size();i++)
	{
		if (m_balle[i].estPerdue() > -1 && m_balle[i].estPerdue() < 40)
		{	
			m_balle[i].incrementerPerdue();		
		}
		if (m_balle[i].estPerdue() == 30)
		{
			m_balle.erase(m_balle.begin() + i);
			m_sprBalle.erase(m_sprBalle.begin() + i);
		}

	}
}


string PartieSolo::nbToString(int x)
{
   std::ostringstream o;

   if (!(o << x))
     cout<<"Conversion impossible" <<endl;

    
   return (o.str());
}

// Retourne true si partie continue, false si c'est terminé.
bool PartieSolo::joueurElimine(Terrain &terrain)
{
	if (m_joueur[0].getScore() <= 0 && m_joueur[0].estVivant())
	{		
		m_joueur[0].tuerJoueur();

//--------------------------SON------------------------------------
		m_sonEclair.SetPosition(m_joueur[0].getX(),m_joueur[0].getY(),30);
		m_sonEclair.Play();
//-----------------------------------------------------------------


		terrain.remplacerJoueur(0, m_trait, m_balle); // Ajoute un trait

		int it = m_trait.size()-1;
		m_sprEclair.push_back(Sprite(m_imgEclair, Vector2f(m_trait[it].getX(), m_trait[it].getY()), Vector2f(1,1), -m_trait[it].getRotation()));  
		m_sprEclair[m_sprEclair.size()-1].SetSubRect(IntRect(21,21,655,58)); // Nécessaire pour le repositionnement... dsl :(	
		m_sprEclair[m_sprEclair.size()-1].SetCenter(m_sprEclair[m_sprEclair.size()-1].GetSize().x / 2, m_sprEclair[m_sprEclair.size()-1].GetSize().y / 2); // A l'élément ajouté	
		m_sprEclair[m_sprEclair.size()-1].SetScale(Vector2f(m_trait[it].getLongueur()/634.f, .5f));
		
		return false;
	}	
	else 
		return true;
}

void PartieSolo::finPartie(sf::RenderWindow &app, std::vector<sf::Shape> &imgRaquettes, std::vector<sf::Shape> &imgTrait, View &vue, Sprite &sprBalle)
{
	m_score = (int)(m_score);

	String texteFin("", m_font, 25);
	String texteAnnonce("Enregistrez votre score en ligne !", m_font, 25);
	// Effet de balles au ralenti et zoom sur le vainqueur.
	float tempsEcoule = 0;
	while (tempsEcoule < 2) // Tant qu'il s'est écoulé moins de 2 secondes
	{

		evenement(app);// S'occupe des événements claviers
		collision();
		//gererBallePerdue();
		bougerAjouterBalles(sprBalle, 1-tempsEcoule/2);

		m_musique.setPitch(1-0.5*tempsEcoule);

		preparerSprites(app, imgRaquettes, imgTrait);
		effacerEcran(app);
		dessinerSprites(app, imgRaquettes, imgTrait);
		afficherSprites(app);

		float framerate = 1.f /m_clock.GetElapsedTime();
		g_coeffVitesse = 60.f/framerate;
		tempsEcoule += 1.f/framerate;
		m_clock.Reset();

	}
	m_musique.arreter();

	if (m_estConnecte)
	{

		// ICI ENTRER LE NOM

		std::string nom;
		bool continuer = true;

		Shape champ = Shape::Rectangle(0,0,500,120,Color(0,0,0), 20, Color(150,50,50));
		champ.SetCenter(300,60);
		champ.SetPosition(m_largeur/2, m_hauteur/2);
		String imgNom("Entrez votre nom", m_font, 50);
		imgNom.SetCenter(0, 35);
		imgNom.SetPosition(m_largeur/2 - 220, m_hauteur/2);

		bool retour = false;

		Event event;
		while (continuer && !retour)
		{
			while (app.GetEvent(event))
			{
				if (event.Type == Event::TextEntered && nom.size() < 15)
				{
					if (event.Text.Unicode > 30 && event.Text.Unicode < 127 && event.Text.Unicode != 32)
					{
						nom += static_cast<char>(event.Text.Unicode);
						imgNom.SetText(nom);
					}
				}

				if (event.Type == Event::KeyPressed && event.Key.Code == Key::Back && nom.size() > 0)
				{	   
					nom = nom.substr(0, nom.size()-1);
					imgNom.SetText(nom);
				}

				if (event.Type == Event::KeyPressed && event.Key.Code == Key::Escape)
				{	   
					retour = true;
				}

				if (event.Key.Code == Key::Return)
				{
					continuer = false;
				}
			}

			texteFin.SetText("Appuyez sur Entrée pour enregister votre score en ligne\nAppuyez sur Echap pour annuler l'enregistrement");
			texteFin.SetCenter(200,65);
			texteFin.SetPosition(m_largeur/2, 4*m_hauteur/5);

			texteAnnonce.SetCenter(200,65);
			texteAnnonce.SetPosition(m_largeur/2, 2*m_hauteur/5);

			preparerSprites(app, imgRaquettes, imgTrait);
			effacerEcran(app);
			dessinerSprites(app, imgRaquettes, imgTrait);
			app.Draw(champ);

			app.Draw(imgNom);
			app.Draw(texteFin);
			app.Draw(texteAnnonce);
			afficherSprites(app);
		
		}
		if (!retour)
		{
			texteFin.SetText("Envoi du score sur\nle serveur en cours");
			//preparerSprites(app, imgRaquettes, imgTrait);
			effacerEcran(app);
			dessinerSprites(app, imgRaquettes, imgTrait);


			// Calcul de la variable mdp, pour éviter toute tricherie.
			int mdp = (int)(m_score * 18985) % 4953;
			mdp += nom.size() * 16;
			mdp += m_largeur + 3;

			m_clock.Reset();
			
			texteFin.SetColor(Color(200,200,240));
			texteFin.SetCenter(140,65);
			texteFin.SetPosition(vue.GetCenter().x, (vue.GetCenter().y));
			app.Draw(texteFin);
			afficherSprites(app);

			std::ostringstream oss1, oss2, oss3, oss4;
			oss1 << m_score; 
			oss2 << m_largeur; 
			oss3 << m_hauteur; 
			oss4 << mdp; 

			Http http;
			http.SetHost("http://happywebsite.free.fr");	

			Http::Request Request;
			Request.SetMethod(sf::Http::Request::Get);
			Request.SetURI("/happyteam/parties/jeux/pong/enrScore.php?nom=" +nom + "&score=" + oss1.str() + "&largeur=" + oss2.str() + "&hauteur=" + oss3.str() + "&mdp=" + oss4.str());

			Http::Response rep = http.SendRequest(Request);

			if(rep.GetStatus() == Http::Response::Ok) // L'envoi au serveur a fonctionné :)
			{
				vector<string> tabRes;
				string res = rep.GetBody();

				for (int i=0;i<4;i++)
				{
					int pos = res.find("-");
					tabRes.push_back(res);
					if (i<3)
					{
						tabRes[i].erase(pos);
						res = res.substr(pos+1);
					}
				}
				
				String resultatGlobal("", m_font, 32 * m_largeur/1600);
				String resultatParEcran("", m_font, 32 * m_largeur/1600);

				if (tabRes[0] == "1")
					resultatGlobal.SetText("Position dans le\nclassement général :\n\n\n" + tabRes[0] + "er / " + tabRes[1]);
				else
					resultatGlobal.SetText("Position dans le\nclassement général :\n\n\n" + tabRes[0] + "ème / " + tabRes[1]);

				if (tabRes[2] == "1")
					resultatParEcran.SetText("Position dans le\nclassement pour\nvotre taille d'écran :\n\n" + tabRes[2] + "er / " + tabRes[3]);
				else
					resultatParEcran.SetText("Position dans le\nclassement pour\nvotre taille d'écran :\n\n" + tabRes[2] + "ème / " + tabRes[3]);

				resultatGlobal.SetColor(Color(250,250,250));
				resultatGlobal.SetCenter(150,65);
				resultatGlobal.SetPosition(m_largeur / 3, m_hauteur/2);
				resultatParEcran.SetColor(Color(250,250,250));
				resultatParEcran.SetCenter(150,65);
				resultatParEcran.SetPosition(m_largeur * 2/3, m_hauteur/2);

				texteFin.SetText("Appuyez sur Entrée pour continuer...");
				texteFin.SetCenter(200,65);
				texteFin.SetPosition(m_largeur/2, 4*m_hauteur/5);

				m_clock.Reset();
				float coeff;

				continuer = true;
				bool bamPasJoue = true;
				while (continuer)
				{
					if (m_clock.GetElapsedTime()>0.5)
						coeff = 1;
					else
						coeff = 1 + 100 * (0.5-m_clock.GetElapsedTime());

					resultatGlobal.SetScale(coeff, coeff);
					resultatParEcran.SetScale(coeff, coeff);
					effacerEcran(app);
					dessinerSprites(app, imgRaquettes, imgTrait);
					app.Draw(resultatGlobal);
					app.Draw(resultatParEcran);
					if (coeff <1.1)
					{
						app.Draw(texteFin);
						if (bamPasJoue)
						{
							m_sonBam.Play();
							bamPasJoue = false;
						}
					}
					
					afficherSprites(app);


					while (app.GetEvent(event))
					{
						if (event.Type == Event::KeyPressed && event.Key.Code == Key::Return)
							continuer = false;
					}
				}	
			}
			else
			{
				texteFin.SetText("La connexion avec le serveur a échoué !");
				effacerEcran(app);
				dessinerSprites(app, imgRaquettes, imgTrait);
				app.Draw(texteFin);
				afficherSprites(app);
				while (m_clock.GetElapsedTime() < 2);
			}
		}	
	}
	else
	{
			texteFin.SetColor(Color(200,200,240));
			texteFin.SetCenter(140,65);
			texteFin.SetPosition(vue.GetCenter().x, (vue.GetCenter().y));
			texteFin.SetText("Vous n'êtes pas connecté");
			effacerEcran(app);
			dessinerSprites(app, imgRaquettes, imgTrait);
			app.Draw(texteFin);
			afficherSprites(app);
			while (m_clock.GetElapsedTime() < 2);

	}
}

void PartieSolo::deplacerIA()
{
	// determination de la balle la plus proche de la raquette
	int noBalleProche = 0;
	int distance = 10000;
	for (int i=0; i<m_balle.size();i++)
	{

		if ((m_balle[i].getPosX() < distance && m_balle[i].getPosX() > m_joueur[0].getX() - m_joueur[0].getLargeur()/2))
		{
			distance = m_balle[i].getPosX();
			noBalleProche = i;
		}
	}

	// Puis on bouge la raquette...
	if (m_balle[noBalleProche].getPosY() - 10 < m_joueur[0].getY()) // Balle trop basse ?
		m_joueur[0].bougerDir1();
	if (m_balle[noBalleProche].getPosY() + 10 > m_joueur[0].getY()) // Balle trop haute ?
		m_joueur[0].bougerDir2();
}


void PartieSolo::appliquerAjouterBonus(sf::RenderWindow &app, Terrain &terrain)
{
	// Ajout de bonus ?
	if (rand()%((int)(500.0f/g_coeffVitesse)) == 0 && m_bonus.size() < 1)
	{
		m_bonus.push_back(Bonus(app, m_balle, terrain, m_bufferBonus, m_bufferCasse, m_bufferVortex, m_bufferVortex2, m_imgBonus, m_imgVortex, m_imgVortex2, m_imgVortex2J));
	}
	for (unsigned int i=0; i< m_bonus.size(); i++)
	{
		if(!m_bonus[i].appliquer(m_balle, m_joueur))
			m_bonus.erase(m_bonus.begin() + i);
			
	}
}

Color PartieSolo::definirCouleur(int noJoueur, float opacite)
{

	if (noJoueur == 0) // Si balle appartenant à joueur 0
	{
		float tmp = m_clockCouleur.GetElapsedTime();
		while (tmp > 10)
			tmp -= 10;

		// tmp est toujours compris entre 0 et 10.

		if (m_clockCouleur.GetElapsedTime() < 10)	
			return Color(200 - tmp*20,tmp*20,20, opacite);
        else if (m_clockCouleur.GetElapsedTime() < 20)	
			return Color(0,200 - tmp*10, tmp*20, opacite);
		else if (m_clockCouleur.GetElapsedTime() < 30)
			return Color(tmp*20,100 - tmp*10, 200 - tmp*18, opacite);
		else 
		{
			m_clockCouleur.Reset();
			return Color(200,0,20, opacite);
		}
	}
	else	
		return Color(255,255,255, opacite);
}
