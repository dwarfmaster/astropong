// PongSFML.cpp : définit le point d'entrée pour l'application console.
//



#include "stdafx.h"
#include <cstdlib>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "menu.h"
#include "partie.h"


using namespace sf;
using namespace std;

// VARIABLE GLOBALE !!! ROOOHHH !
float g_coeffVitesse;


int main(int argc, char *argv[])
{
	RenderWindow app(VideoMode::GetMode(0), "Pong ! ",Style::Fullscreen);
	// Synchro verticale

	string chemin = string(argv[0]);
	chemin.erase(chemin.rfind("\\")+1);
	//cout << chemin << endl;

	app.UseVerticalSync(true);
	//app.SetFramerateLimit(80);
	while (1)
	{	
			bool iaPresente = false;
			int nbJoueurs = 4;
			Menu menu(app);
		
			short vie = 12;
			vie += 18;

			if (menu.getSurvival())
				PartieSolo part(app,vie);
			else
			{
	
				nbJoueurs = menu.getNbJoueurs();
				iaPresente = menu.getIA();
				Partie part(app,nbJoueurs,30,iaPresente); // Lancement de la partie, avec n joueurs et npoints, avec IA ?
			}
	}
	return (1);
}
