#include "stdafx.h"
#include <SFML/Graphics.hpp> 
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <complex>
#include "menu.h"
#include "partie.h"

using namespace std;
using namespace sf;

extern float g_coeffVitesse;

#pragma warning (disable:4244) // conversion int en float

Menu::Menu(RenderWindow &app)
{
	app.ShowMouseCursor(true);
	m_hauteur=app.GetHeight();
	m_largeur=app.GetWidth();

	//DEBUG

	m_iaPresente = false;
	m_survival = false;

	Vector2f centre(m_largeur/2, m_hauteur/2);
	Vector2f demiTaille(m_largeur/2, m_hauteur/2);
	View vue(centre, demiTaille);
	app.SetView(vue);

	m_numMenu = 0;
	Image imgAP;
	if (!imgAP.LoadFromFile("etoiles.jpg") && !imgAP.LoadFromFile("etoiles"))
	{
		cout << "etoiles.jpg introuvable" << endl;
		return; // On ferme le programme
	}

	Image imgTitre;
	if (!imgTitre.LoadFromFile("titre.png"))
	{
		cout << "titre.png introuvable" << endl;
		return; // On ferme le programme
	}
	imgTitre.CreateMaskFromColor(Color(0, 0, 0));
	m_sprTitre.SetImage(imgTitre);
	m_sprTitre.SetCenter(m_sprTitre.GetSize().x / 2, m_sprTitre.GetSize().y / 2);

	Image imgSprites;
	if (!imgSprites.LoadFromFile("imageMenu.png"))
	{
		cout << "imageMenu.png introuvable" << endl;
		return; // On ferme le programme
	}

	Image imgSprites2;
	if (!imgSprites2.LoadFromFile("imageMenu2.png"))
	{
		cout << "imageMenu2.png introuvable" << endl;
		return; // On ferme le programme
	}

	imgSprites.CreateMaskFromColor(Color(0, 0, 254));
	imgSprites2.CreateMaskFromColor(Color(0, 0, 254));


	m_sprJouer.SetImage(imgSprites);
	m_sprJouer.SetSubRect(IntRect(491,66,825,205)); // Recadrage ici pour fixer le milieu
	m_sprJouer.SetCenter(m_sprJouer.GetSize().x / 2, m_sprJouer.GetSize().y / 2);
	m_sprJouer.SetScale(m_largeur/1600.f, m_largeur/1600.f);

	m_sprCredit.SetImage(imgSprites);
	m_sprCredit.SetSubRect(IntRect(491,242,825,381));
	m_sprCredit.SetCenter(m_sprCredit.GetSize().x / 2, m_sprCredit.GetSize().y / 2);
	m_sprCredit.SetScale(m_largeur/1600.f, m_largeur/1600.f);

	m_sprQuitter.SetImage(imgSprites);
	m_sprQuitter.SetSubRect(IntRect(491,411,825,550));
	m_sprQuitter.SetCenter(m_sprQuitter.GetSize().x / 2, m_sprQuitter.GetSize().y / 2);
	m_sprQuitter.SetScale(m_largeur/1600.f, m_largeur/1600.f);

	m_sprRetour.SetImage(imgSprites);
	m_sprRetour.SetSubRect(IntRect(90,577,242,729));
	m_sprRetour.SetCenter(m_sprRetour.GetSize().x / 2, m_sprRetour.GetSize().y / 2);	
	m_sprRetour.SetScale(m_largeur/1600.f, m_largeur/1600.f);

	m_spr1J.SetImage(imgSprites);
	m_spr1J.SetSubRect(IntRect(490,762,642,914));
	m_spr1J.SetCenter(m_spr1J.GetSize().x / 2, m_spr1J.GetSize().y / 2);	
	m_spr1J.SetScale(m_largeur/1600.f, m_largeur/1600.f);

	m_spr2J.SetImage(imgSprites);
	m_spr2J.SetSubRect(IntRect(490,577,642,729));
	m_spr2J.SetCenter(m_spr2J.GetSize().x / 2, m_spr2J.GetSize().y / 2);	
	m_spr2J.SetScale(m_largeur/1600.f, m_largeur/1600.f);

	m_spr3J.SetImage(imgSprites);
	m_spr3J.SetSubRect(IntRect(90,762,242,914));
	m_spr3J.SetCenter(m_spr3J.GetSize().x / 2, m_spr3J.GetSize().y / 2);
	m_spr3J.SetScale(m_largeur/1600.f, m_largeur/1600.f);

	m_spr4J.SetImage(imgSprites);
	m_spr4J.SetSubRect(IntRect(90,946,242,1098));
	m_spr4J.SetCenter(m_spr4J.GetSize().x / 2, m_spr4J.GetSize().y / 2);	
	m_spr4J.SetScale(m_largeur/1600.f, m_largeur/1600.f);

	m_sprSurvie.SetImage(imgSprites2);
	m_sprSurvie.SetSubRect(IntRect(91,20,425,159));
	m_sprSurvie.SetCenter(m_sprSurvie.GetSize().x / 2, m_sprSurvie.GetSize().y / 2);	
	m_sprSurvie.SetScale(m_largeur/1600.f, m_largeur/1600.f);

	m_sprMulti.SetImage(imgSprites2);
	m_sprMulti.SetSubRect(IntRect(91,189,425,328));
	m_sprMulti.SetCenter(m_sprMulti.GetSize().x / 2, m_sprMulti.GetSize().y / 2);	
	m_sprMulti.SetScale(m_largeur/1600.f, m_largeur/1600.f);

	m_sprAP.SetImage(imgAP);
	m_sprAP.SetPosition(0,0);

	m_sprAP.SetScale( m_largeur/m_sprAP.GetSize().x, m_largeur/m_sprAP.GetSize().x);
	if (m_hauteur > m_sprAP.GetSize().y)
		m_sprAP.SetScale( m_hauteur/m_sprAP.GetSize().y, m_hauteur/m_sprAP.GetSize().y);

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


	if(!m_font.LoadFromFile("LinLibertine_R.ttf",80))
	{
		cout << "police introuvable" << endl;
		return;
	}


	if (!m_musiqueMenu.OpenFromFile("zen.ogg") && !m_musiqueMenu.OpenFromFile("zen"))
	{
		cout << "zen.ogg introuvable" << endl;
	}
	m_musiqueMenu.SetLoop(true);
	m_musiqueMenu.Play();

	m_clock.Reset();
	// Ajouter les balles
	for (int i=0; i<50; i++)
	{
		m_balle.push_back(Balle(rand()%m_largeur, rand()%m_hauteur, rand()%8 + 3, rand()%360));
		m_balle[i].activer();
		m_sprBalle.push_back(Sprite(sprBalle));
	}


	bool continuer = true;
	while (m_clock.GetElapsedTime() < 0.1f);

	// Fondu pour faire apparaître le menu. J'ai trop la classe. On est en C++, haha.
	int i=0;
	while (app.IsOpened())
	{
	
		float framerate = 1.f /m_clock.GetElapsedTime();

		g_coeffVitesse = 60.f/framerate;
		m_clock.Reset();
	
		if (i<255)
			i+= 3*g_coeffVitesse;
		if (i>255)
			i = 255;

		bougerBalles();
		continuer = evenement(app);
		if (!continuer)
			break;
		preparerSprites(app);

		effacerEcran(app);
		dessinerSprites(app,i);
		afficherSprites(app);
    } 
}

// Retourne vrai si menu terminé.
bool Menu::evenement(RenderWindow &app)
{

	Event event;
    while (app.GetEvent(event)) 
    {
		if (event.Type == Event::KeyPressed) // Appui sur une touche du clavier
        {
            switch (event.Key.Code) // La touche qui a été appuyée
            {
                case Key::Escape : // Echap
                    exit(0);
			}
		}
		if (event.Type == Event::MouseMoved) // Mouvement de la souris
		{
			m_sourisX = event.MouseMove.X;
			m_sourisY = event.MouseMove.Y;
		}
		if (event.Type == Event::MouseButtonPressed) // Clic souris
		{
			Mouse::Button button = event.MouseButton.Button;
			if (button == Mouse::Left) // Bouton gauche
			{
				if (m_numMenu == 0)
				{
					if (sourisSurSprites(m_sprJouer))
						m_numMenu = 3;
					if (sourisSurSprites(m_sprCredit))
						m_numMenu = 2;
					if (sourisSurSprites(m_sprQuitter))
						exit(0);
				}

				else if (m_numMenu == 3)
				{
					if (sourisSurSprites(m_sprMulti))					
						m_numMenu = 1;

					if (sourisSurSprites(m_sprSurvie))
					{
						m_survival = true;
						debuterPartie(app);
						return false;
					}
					if (sourisSurSprites(m_sprRetour))
						m_numMenu = 0;

				}

				else if (m_numMenu == 1)
				{
					if (sourisSurSprites(m_spr1J))
					{
						m_nbJoueurs = 2;
						m_iaPresente = true;
						debuterPartie(app);
						return false;
					}

					if (sourisSurSprites(m_spr2J))
					{
						m_nbJoueurs = 2;
						debuterPartie(app);
						return false;
					}
					if (sourisSurSprites(m_spr3J))
					{
						m_nbJoueurs = 3;
						debuterPartie(app);
						return false;
					}
					if (sourisSurSprites(m_spr4J))
					{
						m_nbJoueurs = 4;
						debuterPartie(app);
						return false;
					}
					if (sourisSurSprites(m_sprRetour))
						m_numMenu = 3;
				}
				else if (m_numMenu == 2)
				{
					if (sourisSurSprites(m_sprRetour))
						m_numMenu = 0;
				}
			}
		}
    }
	return true;

}

void Menu::preparerSprites(RenderWindow &app)
{
	m_sprTitre.SetPosition (m_largeur/2, 100);

	m_sprJouer.SetPosition(m_largeur/2, m_hauteur/4);
	if (sourisSurSprites(m_sprJouer))
		m_sprJouer.SetSubRect(IntRect(491,66,825,205));
	else
		m_sprJouer.SetSubRect(IntRect(91,66,425,205));

	m_sprCredit.SetPosition(m_largeur/2, m_hauteur/2);
	if (sourisSurSprites(m_sprCredit))
		m_sprCredit.SetSubRect(IntRect(491,242,825,381));
	else
		m_sprCredit.SetSubRect(IntRect(91,242,425,381));

	m_sprQuitter.SetPosition(m_largeur/2, 3*m_hauteur/4);
	if (sourisSurSprites(m_sprQuitter))
		m_sprQuitter.SetSubRect(IntRect(491,411,825,550));
	else
		m_sprQuitter.SetSubRect(IntRect(91,411,425,550));
//--
	m_spr1J.SetPosition(m_largeur/5, m_hauteur/2);
	if (sourisSurSprites(m_spr1J))
		m_spr1J.SetSubRect(IntRect(698,762,850,914));
	else
		m_spr1J.SetSubRect(IntRect(490,762,642,914));

	m_spr2J.SetPosition(2*m_largeur/5, m_hauteur/2);
	if (sourisSurSprites(m_spr2J))
		m_spr2J.SetSubRect(IntRect(698,577,850,729));
	else
		m_spr2J.SetSubRect(IntRect(490,577,642,729));

	m_spr3J.SetPosition(3*m_largeur/5, m_hauteur/2);
	if (sourisSurSprites(m_spr3J))
		m_spr3J.SetSubRect(IntRect(298,762,450,914));
	else
		m_spr3J.SetSubRect(IntRect(90,762,242,914));

	m_spr4J.SetPosition(4*m_largeur/5, m_hauteur/2);
	if (sourisSurSprites(m_spr4J))
		m_spr4J.SetSubRect(IntRect(298,946,450,1098));
	else
		m_spr4J.SetSubRect(IntRect(90,946,242,1098));

	m_sprSurvie.SetPosition(m_largeur/2, 2*m_hauteur/3);
	if (sourisSurSprites(m_sprSurvie))
		m_sprSurvie.SetSubRect(IntRect(491,20,825,159));
	else
		m_sprSurvie.SetSubRect(IntRect(91,20,425,159));

	m_sprMulti.SetPosition(m_largeur/2, m_hauteur/3);
	if (sourisSurSprites(m_sprMulti))
		m_sprMulti.SetSubRect(IntRect(491,189,825,328));
	else
		m_sprMulti.SetSubRect(IntRect(91,189,425,328));


	m_sprRetour.SetPosition(200, m_hauteur-200);
	if (sourisSurSprites(m_sprRetour))
		m_sprRetour.SetSubRect(IntRect(298,577,450,729));
	else
		m_sprRetour.SetSubRect(IntRect(90,577,242,729));


	for (int i=0; i<50; i++)
	{
		m_sprBalle[i].SetPosition(m_balle[i].getPosX(), m_balle[i].getPosY());
	}
	m_txtNbJoueurs.SetText("Nombre de joueurs : ");
	m_txtNbJoueurs.SetFont(m_font);
	m_txtNbJoueurs.SetSize(70 * m_largeur/1600);
	m_txtNbJoueurs.SetColor(Color(240,10,10));
	m_txtNbJoueurs.SetCenter(250,65);
	m_txtNbJoueurs.SetPosition(m_largeur/2, 250);

	m_txtCredit1.SetText("Développeur : Augustin Plichon (aug125)");
	m_txtCredit1.SetFont(m_font);
	m_txtCredit1.SetSize(55 * m_largeur/1600);
	m_txtCredit1.SetColor(Color(210,210,150));
	m_txtCredit1.SetPosition(m_largeur/4, 180);

	m_txtCredit2.SetText("Musiques de Just'in Space : \n- Transit\n- Galaxy\n- Sparkling\n- Zen\n  \n Testeur en chef : lymk \n Testeurs : polkatric, Skyline73, Finkelzohn \n\n N'oubliez pas de visiter le site ! http://happywebsite.free.fr/");
	m_txtCredit2.SetFont(m_font);
	m_txtCredit2.SetSize(35 * m_largeur/1600);
	m_txtCredit2.SetColor(Color(210,210,150));
	m_txtCredit2.SetPosition(m_largeur/4, 300);
}

void Menu::effacerEcran(RenderWindow &app)
{
       // Remplissage de l'écran (couleur noire par défaut)
        app.Clear(Color(0,0,0));
}

void Menu::dessinerSprites(RenderWindow &app, int niveauFondu)
{
 
	// FONDU
	m_sprTitre.SetColor(Color(255,255,255, niveauFondu));
	m_txtNbJoueurs.SetColor(Color(240,10,10, niveauFondu));
	m_sprAP.SetColor(Color(255,255,255, niveauFondu));
	m_sprJouer.SetColor(Color(255,255,255, niveauFondu));
	m_sprCredit.SetColor(Color(255,255,255, niveauFondu));
	m_sprQuitter.SetColor(Color(255,255,255, niveauFondu));
	m_spr1J.SetColor(Color(255,255,255, niveauFondu));
	m_spr2J.SetColor(Color(255,255,255, niveauFondu));
	m_spr3J.SetColor(Color(255,255,255, niveauFondu));
	m_spr4J.SetColor(Color(255,255,255, niveauFondu));
	m_sprSurvie.SetColor(Color(255,255,255, niveauFondu));
	m_sprMulti.SetColor(Color(255,255,255, niveauFondu));
	m_sprRetour.SetColor(Color(255,255,255, niveauFondu));

	for (int i=0; i<50;i++)
		m_sprBalle[i].SetColor(Color(255,255,255, niveauFondu));

//----------Affichage des sprites---------------


	app.Draw(m_sprAP);

	for (int i=0; i<50; i++)
		app.Draw(m_sprBalle[i]);
	app.Draw(m_sprTitre);

	if (m_numMenu == 0) // Menu principal
	{
	app.Draw(m_sprJouer);
	app.Draw(m_sprCredit);
	app.Draw(m_sprQuitter);
	}
	if (m_numMenu == 1) // Jouer
	{
		app.Draw(m_spr1J);
		app.Draw(m_spr2J);
		app.Draw(m_spr3J);
		app.Draw(m_spr4J);
		app.Draw(m_txtNbJoueurs);
		app.Draw(m_sprRetour);
	}
	if (m_numMenu == 2) // Jouer
	{
		app.Draw(m_sprRetour);
		app.Draw(m_txtCredit1);
		app.Draw(m_txtCredit2);
	}
	if (m_numMenu == 3)
	{
		app.Draw(m_sprRetour);
		app.Draw(m_sprMulti);
		app.Draw(m_sprSurvie);
	}

}


void Menu::afficherSprites(sf::RenderWindow &app)
{
        // Affichage de la fenêtre à l'écran
        app.Display();
}

bool Menu::sourisSurSprites(const Sprite &spr)
{
	if (   m_sourisX >= spr.GetPosition().x - spr.GetSize().x/2
		&& m_sourisX <= spr.GetPosition().x + spr.GetSize().x/2
		&& m_sourisY >= spr.GetPosition().y - spr.GetSize().y/2
		&& m_sourisY <= spr.GetPosition().y + spr.GetSize().y/2)	
		return true;
	else
		return false;

}

int Menu::getNbJoueurs()
{
	return m_nbJoueurs;
}

int Menu::getNbVies()
{
	return m_nbVies;
}

void Menu::debuterPartie(RenderWindow &app)
{
	int i = 255;
	while (i>0)
	{
		i -= 3*g_coeffVitesse;
		if (i<0)
			i = 0;

		float framerate = 1.f /m_clock.GetElapsedTime();
		g_coeffVitesse = 60.f/framerate;
		m_clock.Reset();
		preparerSprites(app);
		effacerEcran(app);
		dessinerSprites(app, i);
		afficherSprites(app);
		m_musiqueMenu.SetVolume(i*100/255);

	}

}

void Menu::bougerBalles()
{
	for (int i=0; i<50; i++)
	{
		m_balle[i].bouger(1);
		m_balle[i].rebondir(m_largeur, m_hauteur);
	}
}

bool Menu::getIA()
{
	return m_iaPresente;
}

bool Menu::getSurvival()
{
	return m_survival;
}
