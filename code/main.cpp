#include <iostream>
#include "graphe.h"

using namespace std;

/**
 * Main function.
 * @return code de sortie du programme.
 */
int main()
{
  string nomFichier;

  cout << "Veuillez entrer le nom du fichier a ouvrir : ";
  cin >> nomFichier;

  // Création du graphe
  graphe graphe(nomFichier);

  string nomDuNoeudATrouver;

  while(true) {
    cout << "Entrez le nom du noeud a trouver : ";
    cin >> nomDuNoeudATrouver;

    uint32_t noeud = graphe.obtenir_noeud(nomDuNoeudATrouver);
    if(noeud == graphe.size()) {
      cout << "Aucun noeud avec ce nom n'a été trouvé." << endl << endl;
    } else {
      graphe.afficher_noeud(noeud);
      cout << endl;
    }
  }
}
