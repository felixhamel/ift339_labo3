#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <stdint.h> // Compatibilité avec uint32_t

using namespace std;

class graphe
{
  private:

    struct noeud  // Description de toutes les composantes d'un noeud
    {
      // Partie statique
      uint32_t partieVariable;
      float latitude;
      float longitude;
      uint32_t futur[4];            // Autres trucs pour un usage futur

      // Partie variable
      uint16_t nbArcs;              // Nombre d'arcs
      map<uint32_t, float> liens;   // Liens entre ce noeud et d'autres noeuds
      string nom;                   // Nom du noeud
    };

    map<uint32_t, noeud> lesNoeuds;   // Les noeuds deja lus
    uint32_t nbNOEUDS;                // Le nombre de noeuds
    ifstream DATA;                    // Le flot d'entrée
    uint32_t DEBUT;                   // Debut de la partie fixe
    string nom;                       // Nom du graphe
    uint8_t architecture;             // Architecture du fichier (Little or Big endian)

    /**
     * Lire le noeud avec le numéro donné en paramètre.
     * @param noeud Numéro du noeud a lire dans le fichier et a charger en mémoire.
     */
    void lire_noeud(uint32_t noeud);

    /**
     * Lire un uint32_t à la position courante de DATA. (4 octets)
     * @param noeud variable a extraire du fichier.
     */
    void lire(uint32_t& noeud);

    /**
     * Lire un uint16_t à la position courante de DATA. (2 octets)
     * @param noeud variable à extraire du fichier.
     */
    void lire(uint16_t& noeud);

    /**
     * Lire un float à la position courante de DATA. (4 octets)
     * @param a variable à extraire du fichier.
     */
    void lire(float& a);

    /**
     * Constructeur par copie.
     */
    graphe(const graphe &graphe)=delete;

    /**
     * Désactiver l'opérateur =. Il vaut mieux utiliser le constructeur par copie.
     */
    graphe& operator=(const graphe &graphe)=delete;

    /**
     * Vérifie quel est l'architecture de la machine exécutant ce code.
     * Trouvé sur : http://stackoverflow.com/a/1001344
     * @return int 4321 = BigEndian, 1234 = LittleEndian.
     */
    const int architectureMachine() const;

    /**
     * Trouver le noeud avec le nom donné en paramètre.
     * @param  nom   Nom du noeud a trouver.
     * @param  debut Position de départ de la recherche
     * @param  fin   Position de fin de la recherche
     * @return int   Numero du noeud trouvé. Si introuvé, retourne le nombre de noeud dans ce graphe.
     */
    const uint32_t trouver_noeud_avec_nom(string& nom, unsigned long int debut, unsigned long int fin);

  public:

    /**
     * Constructeur.
     * @param: cheminVersFichier Chemin vers le fichier contenant le graphe a lire.
     */
    graphe(string cheminVersFichier);

    /**
     * Destructeur. Va fermer le fichier contenant le graphe.
     */
    ~graphe();

    /**
     * Retourne le nombre de noeud dans le fichier du graphe.
     * @return uint32_t Nombre de noeuds.
     */
    const uint32_t size() const;

    /**
     * Afficher le noeud avec le numéro donné en paramètre.
     * S'il n'est pas en mémoire, ce dernier va être lu.
     * @param noeud numéro du noeud a afficher.
     */
    void afficher_noeud(uint32_t noeud);

    /**
     * Obtenir le numéro du noeud avec ce nom.
     * @param  string Nom du noeud a retrouver.
     * @return        # du noeud avec le nom si trouvé, sinon retourne
     *                  le nombre du noeud dans le graphe.
     */
    const uint32_t obtenir_noeud(string& nom);
};
