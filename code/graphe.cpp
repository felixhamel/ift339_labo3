#include "graphe.h"

#include <set>
#include <chrono>
#include <ctime>
#include <vector>
#include <cmath>
#include <string.h>

#define __LITTLE_ENDIAN 1234
#define __BIG_ENDIAN    4321

graphe::graphe(string cheminVersFichier)
{
	DATA.open(cheminVersFichier.c_str(), ios::in|ios::binary);
	if(!DATA.is_open()) {
		cout << "Erreur d'ouverture du fichier, celui-ci n'existe pas." << endl;
		exit(1);
	} else {
		DATA >> nom;
		DATA.ignore(1);
		DATA >> nbNOEUDS;
		DATA.ignore(1);
		DATA >> architecture;
		DATA.ignore(1);

		DEBUT = DATA.tellg();
	}
}

graphe::~graphe()
{
	// Fermer le fichier a la sortie du programme.
	if (DATA.is_open()) {
		DATA.close();
	}
}

void graphe::lire_noeud(uint32_t noeud)
{
	if(noeud < nbNOEUDS) {
		// Si le noeud n'a jamais été lu, alors il va l'être !
		if(lesNoeuds[noeud].partieVariable == 0) {

			// Lecture des données statiques du noeud
			DATA.seekg(DEBUT + (28 * noeud), ios::beg);
			this->lire(lesNoeuds[noeud].partieVariable);
			this->lire(lesNoeuds[noeud].latitude);
			this->lire(lesNoeuds[noeud].longitude);
			for(int i = 0; i < 4; ++i) {
				this->lire(lesNoeuds[noeud].futur[i]);
			}

			// Lecture des données variable du noeud
			DATA.seekg(lesNoeuds[noeud].partieVariable);
			this->lire(lesNoeuds[noeud].nbArcs);
			for(int i = 0; i < lesNoeuds[noeud].nbArcs; ++i) {
				uint32_t numero;
				this->lire(numero);
				this->lire(lesNoeuds[noeud].liens[numero]);
		  }

			uint16_t nombreDeCaracteres;
			this->lire(nombreDeCaracteres);

			char* nom = new char[nombreDeCaracteres];
			DATA.read(nom, nombreDeCaracteres);
			lesNoeuds[noeud].nom = nom;
			lesNoeuds[noeud].nom = lesNoeuds[noeud].nom.substr(0, nombreDeCaracteres-1);

			delete nom;
	  }
	}
}

void graphe::lire(uint16_t& noeud)
{
	DATA.read(reinterpret_cast<char*>(&noeud), 2);

	// Si l'architecture diffère du fichier, on swap les bits.
	int architectureMachine = this->architectureMachine();
	if((architecture == 1 && architectureMachine != __LITTLE_ENDIAN) ||
		(architecture == 0 && architectureMachine != __BIG_ENDIAN)) {
			// http://stackoverflow.com/a/2182184
			noeud = (noeud >> 8) | (noeud << 8);
	}
}

void graphe::lire(uint32_t& noeud)
{
	DATA.read(reinterpret_cast<char*>(&noeud), 4);

	// Si l'architecture diffère du fichier, on swap les bits.
	int architectureMachine = this->architectureMachine();
	if((architecture == 1 && architectureMachine != __LITTLE_ENDIAN) ||
		(architecture == 0 && architectureMachine != __BIG_ENDIAN)) {
			// http://stackoverflow.com/a/13001420
			noeud = (noeud >> 24) | ((noeud << 8) & 0x00FF0000) | ((noeud >> 8) & 0x0000FF00) | (noeud << 24);
	}
}

void graphe::lire(float& a)
{
	DATA.read(reinterpret_cast<char*>(&a), 4);

	// Si l'architecture diffère du fichier, on swap les bits.
	int architectureMachine = this->architectureMachine();
	if((architecture == 1 && architectureMachine != __LITTLE_ENDIAN) ||
		(architecture == 0 && architectureMachine != __BIG_ENDIAN)) {
			char *floatToConvert = ( char* ) & a; // http://stackoverflow.com/a/2782742
			swap(floatToConvert[0], floatToConvert[3]);
			swap(floatToConvert[1], floatToConvert[2]);
	}
}

const uint32_t graphe::size() const
{
	return this->nbNOEUDS;
}

void graphe::afficher_noeud(uint32_t noeud)
{
	this->lire_noeud(noeud);

	auto leNoeud = lesNoeuds[noeud];

	cout << "+--------------------------------------------------------------------+" << endl;
	cout << " Noeud #" << noeud << endl;
	//cout << " - PartieVariable: " << leNoeud.partieVariable << endl;
	cout << " - Latitude: " << leNoeud.latitude << endl;
	cout << " - Longitude: " << leNoeud.longitude << endl;
	cout << " - Nom: " << leNoeud.nom << endl;
	for(int i = 0; i < 4; ++i) {
		cout << " -> Futur[" << i << "]: " << leNoeud.futur[i] << endl;
	}
	cout << " - Nombre d'arcs: " << leNoeud.nbArcs << endl;
	for(map<uint32_t, float>::iterator it = leNoeud.liens.begin(); it != leNoeud.liens.end(); ++it) {
		cout << " -> Arc vers le noeud " << it->first << " avec un poids de " << it->second << endl;
	}
	cout << "+--------------------------------------------------------------------+" << endl;
}

const int graphe::architectureMachine() const
{
	short int word = 0x0001;
	char *byte = (char *) &word;
	return(byte[0] ? __LITTLE_ENDIAN : __BIG_ENDIAN);
}

const uint32_t graphe::obtenir_noeud(string& nom)
{
	return trouver_noeud_avec_nom(nom, 0, size()-1);
}

const uint32_t graphe::trouver_noeud_avec_nom(string& nom, uint32_t debut, uint32_t fin)
{
	// Vérifier si nous avons atteint les limites de notre recherche.
	if(debut > fin) {
		return debut;
	}

	// Établir l'index courant
	uint32_t index = floor((debut + fin) / 2);

	// Lire le noeud en mémoire
	lire_noeud(index);
	noeud noeud = lesNoeuds[index];

	// Comparer
	int comparaison = strcmp(nom.c_str(), noeud.nom.c_str());
	if(comparaison == 0) {
		return index;
	} else if(comparaison < 0) {
		if(index == debut) {
			return debut;
		}
		fin = index-1;
	} else {
		debut = index+1;
	}
	return trouver_noeud_avec_nom(nom, debut, fin);
}
