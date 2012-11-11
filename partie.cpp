#include "stdafx.h"
#include <SFML/Graphics.hpp> 
#include <SFML/Audio.hpp>
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

Partie::Partie(RenderWindow &app, int nbJoueurs, int nPoints, bool iaPresente)
:m_nbJoueurs(nbJoueurs), m_iaPresente(iaPresente)
{

	m_retourMenu = false;

	// Initialisation
	app.ShowMouseCursor(false);  // Tu vas marcher, connard ?
	m_hauteur=app.GetHeight();
	m_largeur=app.GetWidth();

	Vector2f centre(m_largeur/2, m_hauteur/2);
	Vector2f demiTaille(m_largeur/2, m_hauteur/2);
	View vue(centre, demiTaille);
	app.SetView(vue);

	Terrain terrain (m_nbJoueurs, m_joueur, m_largeur, m_hauteur, nPoints, m_trait);

	m_compteurEclair = 0;

	Image imgAP;
	if (!imgAP.LoadFromFile("res/imgs/Nebula.jpg"))
	{
		cout << "res/imgs/Nebula introuvable" << endl;
		return; // On ferme le programme
	}

	m_sprAP.SetImage(imgAP);
	m_sprAP.SetPosition(0,0);

	m_sprAP.SetScale( m_hauteur/m_sprAP.GetSize().y, m_hauteur/m_sprAP.GetSize().y);
	if (m_largeur > m_sprAP.GetSize().x)
		m_sprAP.SetScale( m_largeur/m_sprAP.GetSize().x, m_largeur/m_sprAP.GetSize().x);


	// Chargement image balle
	Image imgBalle;
	if (!imgBalle.LoadFromFile("res/imgs/Balle.bmp"))
	{
		cout << "res/imgs/Balle.bmp introuvable" << endl;
		return; // On ferme le programme
	}
	imgBalle.CreateMaskFromColor(Color(0, 0, 255));

	Sprite sprBalle;
	sprBalle.SetImage(imgBalle);
	sprBalle.SetCenter(7,7); // Centre de la balle

	Image imgTrou;
	if (!imgTrou.LoadFromFile("res/imgs/Trou.bmp"))
	{
		cout << "res/imgs/Trou.bmp introuvable" << endl;
		return; // On ferme le programme
	}
	imgTrou.CreateMaskFromColor(Color(0, 0, 255));
	
	m_sprTrou.SetImage(imgTrou);
	m_sprTrou.SetCenter(10,10); // Centre du trou
	m_sprTrou.SetPosition(m_largeur/2, m_hauteur/2);


	// Dessin raquettes
	vector<Shape> imgRaquettes;	
	for (int i=0; i<m_nbJoueurs; i++)
	{
		imgRaquettes.push_back(Shape::Rectangle(0.0f, 0.0f, m_joueur[i].getLongueur(), m_joueur[i].getLargeur(), definirCouleur(i)));
		imgRaquettes[i].SetCenter(m_joueur[i].getLongueur()/2,m_joueur[i].getLargeur()/2); // Centre au milieu
	}

	// Dessin trait pour perdant


	if (!m_imgEclair.LoadFromFile("res/imgs/imageEclair.png"))
	{
		cout << "res/imgs/imageEclair.png introuvable" << endl;
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



	if(!m_font.LoadFromFile("res/ttf/LinLibertine_R.ttf",80))
{
    cout << "police introuvable" << endl;
	return;
}
	for (int i=0; i<m_nbJoueurs;i++)
	{
		m_texteScore.push_back(String("0",m_font,60));
		m_texteScore[i].SetColor(Color(210,210,150));
		m_texteScore[i].SetPosition(m_joueur[i].getScorePosX(), m_joueur[i].getScorePosY());
		m_texteScore[i].SetCenter(10,10);
	}

	for (int i=0; i<m_nbJoueurs;i++)
	{
		m_texteTouches.push_back(String("",m_font,30));
		m_texteTouches[i].SetColor(Color(210,210,150));
		m_texteTouches[i].SetPosition(m_joueur[i].getIndicationTouchesPosX(), m_joueur[i].getIndicationTouchesPosY());
		m_texteTouches[i].SetCenter(0,20);
	}

	if (m_nbJoueurs == 2)
	{
		if (!m_iaPresente)
		{
			m_texteTouches[0].SetText("Z \nS");
		}
		m_texteTouches[1].SetText("Haut \nBas");
	}

	if (m_nbJoueurs == 3)
	{
		m_texteTouches[0].SetText("D \nQ");
		m_texteTouches[1].SetText("Gauche \nDroite");
		m_texteTouches[2].SetText("J \nK");
	}

	if (m_nbJoueurs == 4)
	{

		m_texteTouches[0].SetText("Z \nS");
		m_texteTouches[1].SetText("J \nK");
		m_texteTouches[2].SetText("2(NumPad) \n5(NumPad)");
		m_texteTouches[3].SetText("Gauche \nDroite");

	}

	
	m_compteurTransparenceTouches =0;


//---- SON -------

	Listener::SetPosition(m_largeur/2, m_hauteur/2, 0.0f); // Position du joueur.
	Listener::SetTarget(0,0,-1);

	SoundBuffer rebond;
	if (!rebond.LoadFromFile("res/sounds/rebond.ogg"))
	{
		cout << "res/sounds/rebond.ogg introuvable" << endl;
	}
	m_sonRebond.SetBuffer(rebond);
	m_sonRebond.SetAttenuation(0);
	m_sonRebond.SetVolume(70);

	SoundBuffer rebondRaquette;
	if (!rebondRaquette.LoadFromFile("res/sounds/rebondRaquette.ogg"))
	{
		cout << "res/sounds/rebondRaquette.ogg introuvable" << endl;
	}
	m_sonRebondRaquette.SetBuffer(rebondRaquette);
	m_sonRebondRaquette.SetAttenuation(0);
	m_sonRebondRaquette.SetVolume(50);

	SoundBuffer eclair;
	if (!eclair.LoadFromFile("res/sounds/eclair.ogg"))
	{
		cout << "res/sounds/eclair.ogg introuvable" << endl;
	}
	m_sonEclair.SetBuffer(eclair);
	m_sonEclair.SetAttenuation(0);
	m_sonEclair.SetVolume(50);


// -------------- NECESSAIRE POUR BONUS ! ----------------------------------
		if (!m_imgBonus.LoadFromFile("res/imgs/bonus.png"))
		{
			cout << "res/imgs/bonus.png introuvable" << endl;
			exit(2); // On ferme le programme
		}
		m_imgBonus.CreateMaskFromColor(Color(0, 0, 0));
	
	if (!m_imgVortex.LoadFromFile("res/imgs/vortex.png"))
		{
			cout << "res/imgs/vortex.png introuvable" << endl;
			exit(2); // On ferme le programme
		}
	if (!m_imgVortex2.LoadFromFile("res/imgs/vortex2.png"))
		{
			cout << "res/imgs/vortex2.png introuvable" << endl;
			exit(2); // On ferme le programme
		}
	if (!m_imgVortex2J.LoadFromFile("res/imgs/vortex2J.png"))
		{
			cout << "res/imgs/vortex2J.png introuvable" << endl;
			exit(2); // On ferme le programme
		}

	if (!m_bufferBonus.LoadFromFile("res/sounds/bonus.ogg"))
	{
		cout << "res/sounds/bonus.ogg introuvable" << endl;
	}

	if (!m_bufferCasse.LoadFromFile("res/sounds/casse.ogg"))
	{
		cout << "res/sounds/casse.ogg introuvable" << endl;
	}

	if (!m_bufferVortex.LoadFromFile("res/sounds/vortex.ogg"))
	{
		cout << "res/sounds/vortex.ogg introuvable" << endl;
	}
	if (!m_bufferVortex2.LoadFromFile("res/sounds/vortex2.ogg"))
	{
		cout << "res/sounds/vortex2.ogg introuvable" << endl;
	}
//---------------------------------------------------------

	m_clock.Reset();
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

		evenement(app);// S'occupe des événements claviers
		collision(); // Redéfinit le mouvement de la balle. Vérifie aussi si la balle est perdue.
		bougerAjouterBalles(sprBalle); // Bouge les balles
		gererBallePerdue();
		if (m_iaPresente)
			deplacerIA();

		appliquerAjouterBonus(app, terrain);

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





void Partie::collision()
{

	for(int j=0; j<m_balle.size(); j++)
	{
		// Objectif : obtenir les coordonnées du point de la raquette perpendiculaire à la balle
		bool collision;
		float aRaquette, aBalle, bRaquette, bBalle, aTrait, bTrait;
		int x,y;
		bool sensHoraire;

		// RAQUETTE
		for (int i=0; i<m_nbJoueurs;i++)
		{
			if (!m_joueur[i].estVivant())
				continue;

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

void Partie::bougerAjouterBalles(sf::Sprite &sprBalle, float coeffVitesse)
{
	bool ajouterBalle = false;
	if (m_balle.size() == 0 || rand()%((int)(120/g_coeffVitesse)) == 0) // Pas de balle, ou alors une fois sur 120 !!!  
		ajouterBalle = true;

	if (ajouterBalle)
	{
		m_vitesseInitialeBalle = 6 * m_largeur/1600.f;

		m_sprBalle.push_back(sprBalle);
		m_balle.push_back(Balle(m_largeur/2, m_hauteur/2, m_vitesseInitialeBalle, rand()%360));
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


void Partie::evenement(RenderWindow &app)
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

	if (m_nbJoueurs == 2)
	{
		if (!m_iaPresente)
		{
			if(input.IsKeyDown(Key::Z))
				m_joueur[0].bougerDir1();
			if(input.IsKeyDown(Key::S))
				m_joueur[0].bougerDir2();
		}
		if(input.IsKeyDown(Key::Down))
			m_joueur[1].bougerDir1();
		if(input.IsKeyDown(Key::Up))
			m_joueur[1].bougerDir2();
	}

	if (m_nbJoueurs == 3)
	{
		if(input.IsKeyDown(Key::D))
			m_joueur[0].bougerDir1();
		if(input.IsKeyDown(Key::Q))
			m_joueur[0].bougerDir2();
		if(input.IsKeyDown(Key::Right))
			m_joueur[1].bougerDir1();
		if(input.IsKeyDown(Key::Left))
			m_joueur[1].bougerDir2();
		if(input.IsKeyDown(Key::J))
			m_joueur[2].bougerDir1();
		if(input.IsKeyDown(Key::K))
			m_joueur[2].bougerDir2();
	}

	if (m_nbJoueurs == 4)
	{
		if(input.IsKeyDown(Key::Z))
			m_joueur[0].bougerDir1();
		if(input.IsKeyDown(Key::S))
			m_joueur[0].bougerDir2();
		if(input.IsKeyDown(Key::K))
			m_joueur[1].bougerDir1();
		if(input.IsKeyDown(Key::J))
			m_joueur[1].bougerDir2();
		if(input.IsKeyDown(Key::Numpad2))
			m_joueur[2].bougerDir1();
		if(input.IsKeyDown(Key::Numpad5))
			m_joueur[2].bougerDir2();
		if(input.IsKeyDown(Key::Left))
			m_joueur[3].bougerDir1();
		if(input.IsKeyDown(Key::Right))
			m_joueur[3].bougerDir2();
	}

}



//---------------------------------
void Partie::preparerSprites(RenderWindow &app, vector<Shape> &imgRaquettes, vector<Shape> &imgTrait)
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

		for (int i=0;i<m_nbJoueurs;i++)
		{		
			if (!m_joueur[i].estVivant())
				continue;
			imgRaquettes[i].SetPosition(m_joueur[i].getX(),m_joueur[i].getY());
			imgRaquettes[i].SetRotation(-(m_joueur[i].getRotation())); // Les raquettes tournent dans le sens des aiguilles d'une montre
			m_texteScore[i].SetText(nbToString(m_joueur[i].getScore()));
		}

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

void Partie::effacerEcran(sf::RenderWindow &app)
{
       // Remplissage de l'écran (couleur noire par défaut)
        app.Clear(Color(0,0,0));
}

void Partie::dessinerSprites(RenderWindow &app, vector<Shape> &imgRaquettes, vector<Shape> &imgTrait, int niveauFondu)
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
	for (int i=0;i<m_balle.size();i++)
	{
		float opacite = 255;
		if (m_balle[i].estPerdue() > -1)
			opacite = 255 - m_balle[i].estPerdue()*8.3;


		m_sprBalle[i].SetColor(definirCouleur(m_balle[i].getJoueur(), opacite));
		app.Draw(m_sprBalle[i]);
	}

	for (int i=0; i<m_nbJoueurs;i++)
	{
		if (!m_joueur[i].estVivant())
			continue;

		if (transparenceTouche <= 255)
			m_texteTouches[i].SetColor(Color(210,210,150,transparenceTouche));
		else
			m_texteTouches[i].SetColor(Color(210,210,150));


		app.Draw(m_texteScore[i]);
		app.Draw(m_texteTouches[i]);
		app.Draw(imgRaquettes[i]);
	}



	for (int i=0; i<imgTrait.size();i++)
		app.Draw(imgTrait[i]);

	for (int i=0; i<m_sprEclair.size();i++)
		app.Draw(m_sprEclair[i]);
}


void Partie::afficherSprites(sf::RenderWindow &app)
{
        // Affichage de la fenêtre à l'écran
        app.Display();
}



void Partie::gererBallePerdue()
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


string Partie::nbToString(int x)
{
   std::ostringstream o;

   if (!(o << x))
     cout<<"Conversion impossible" <<endl;

    
   return (o.str());
}

// Retourne true si partie continue, false si c'est terminé.
bool Partie::joueurElimine(Terrain &terrain)
{
	for (int i=0; i<m_nbJoueurs; i++)
	{
		if (m_joueur[i].getScore() <= 0 && m_joueur[i].estVivant())
		{		

			m_joueur[i].tuerJoueur();

	//		if (m_nbJoueurs == 2) // Pas de traits à rajouter en mode 2 joueurs (?)
	//			continue;

//--------------------------SON------------------------------------
			m_sonEclair.SetPosition(m_joueur[i].getX(),m_joueur[i].getY(),30);
			m_sonEclair.Play();
//-----------------------------------------------------------------


			terrain.remplacerJoueur(i, m_trait, m_balle); // Ajoute un trait

			int it = m_trait.size()-1;
			m_sprEclair.push_back(Sprite(m_imgEclair, Vector2f(m_trait[it].getX(), m_trait[it].getY()), Vector2f(1,1), -m_trait[it].getRotation()));  
			m_sprEclair[m_sprEclair.size()-1].SetSubRect(IntRect(21,21,655,58)); // Nécessaire pour le repositionnement... dsl :(	
			m_sprEclair[m_sprEclair.size()-1].SetCenter(m_sprEclair[m_sprEclair.size()-1].GetSize().x / 2, m_sprEclair[m_sprEclair.size()-1].GetSize().y / 2); // A l'élément ajouté	
			m_sprEclair[m_sprEclair.size()-1].SetScale(Vector2f(m_trait[it].getLongueur()/634.f, .5f));

		}
	}
	int joueursRestants = m_nbJoueurs;
	for(int i=0; i<m_nbJoueurs; i++)
	{
		if(!m_joueur[i].estVivant())
			joueursRestants--;
	}
	if (joueursRestants == 1)
		return false;
	else 
		return true;
}

void Partie::finPartie(sf::RenderWindow &app, std::vector<sf::Shape> &imgRaquettes, std::vector<sf::Shape> &imgTrait, View &vue, Sprite &sprBalle)
{

	// Qui a gagné ?
	int nGagnant;
	for (int i=0;i<m_nbJoueurs;i++)
	{
		if (m_joueur[i].estVivant())
			nGagnant = i+1;
	}

	// Effet de balles au ralenti et zoom sur le vainqueur.
	float tempsEcoule = 0;
	while (tempsEcoule < 2) // Tant qu'il s'est écoulé moins de 2 secondes
	{

		vue.Zoom(pow(1.008f,g_coeffVitesse));
		vue.Move(Vector2f((m_joueur[nGagnant-1].getX() - vue.GetCenter().x)/100 * g_coeffVitesse, (m_joueur[nGagnant-1].getY()- vue.GetCenter().y)/100 * g_coeffVitesse));
		collision();
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
	// Affichage texte
	string num = nbToString(nGagnant); 

	String texteFin("", m_font, 50);

	// TEXTE DIFFERENT SELON MODE J/J OU J/O
	if (m_iaPresente)
	{
		if (m_joueur[0].getScore() > 0) // Si l'ordi gagne
			texteFin.SetText("VAINQUEUR : \n MACHINE");
		else
			texteFin.SetText("VAINQUEUR : \n HUMAIN");

	}
	else
	{
		string texte("VAINQUEUR : \n JOUEUR ");
		texte += num;
		texteFin.SetText(texte);
	}


	texteFin.SetColor(definirCouleur(nGagnant - 1));
	texteFin.SetCenter(125,65);
	texteFin.SetPosition(vue.GetCenter().x, (vue.GetCenter().y));

	float i=0;
	while (i < 50)
	{
		float framerate = 1.f /m_clock.GetElapsedTime();
		g_coeffVitesse = 60.f/framerate;
		tempsEcoule += 1.f/framerate;
		m_clock.Reset();

		i += g_coeffVitesse;
		if (i>50)
		i = 50;

		//texteFin.SetPosition(vue.GetCenter().x, (vue.GetCenter().y)*i/50);
		texteFin.SetScale(51-i, 51-i);
		texteFin.SetRotation(150 - i*3);

		effacerEcran(app);
		dessinerSprites(app, imgRaquettes, imgTrait);
		app.Draw(texteFin);
		afficherSprites(app);
	}
	
	m_clock.Reset();
	while (m_clock.GetElapsedTime() < 3);
		
}

void Partie::deplacerIA()
{
	// determination de la balle la plus proche de la raquette
	int noBalleProche = -1;
	int distance = 10000;
	for (int i=0; i<m_balle.size();i++)
	{

		if ((m_balle[i].getPosX() < distance && m_balle[i].getPosX() > m_joueur[0].getX() - m_joueur[0].getLargeur()/2) && m_balle[i].getVitX() <= 0)
		{
			distance = m_balle[i].getPosX();
			noBalleProche = i;
		}
	}
	
// Puis on bouge la raquette...
	if (noBalleProche != -1)
	{
		if (m_balle[noBalleProche].getPosY() - 10 < m_joueur[0].getY()) // Balle trop basse ?
			m_joueur[0].bougerDir1();
		if (m_balle[noBalleProche].getPosY() + 10 > m_joueur[0].getY()) // Balle trop haute ?
			m_joueur[0].bougerDir2();
	}
}


void Partie::appliquerAjouterBonus(sf::RenderWindow &app, Terrain &terrain)
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

Color Partie::definirCouleur(int noJoueur, float opacite)
{

	if (noJoueur == 0) // Si balle appartenant à personne.
		return Color(255,40,20, opacite);
	if (noJoueur == 1) // Si balle appartenant à personne.
		return Color(200,200,20, opacite);
	if (noJoueur == 2) // Si balle appartenant à personne.
		return Color(20,255,20, opacite);
	if (noJoueur == 3) // Si balle appartenant à personne.
		return Color(20,100,255, opacite);
	else	
		return Color(255,255,255, opacite);
}
