/*************************************************************************
                           Interface  -  description
                             -------------------
    debut                : 06/02/2013
    copyright            : (C) pmdartus
    e-mail               : pierre-marie.dartus@insa-lyon.fr
*************************************************************************/

//---------- Réalisation de la tache <Interface> (fichier Interface.cpp) ---

/////////////////////////////////////////////////////////////////  INCLUDE
//-------------------------------------------------------- Include systeme
#include <Generateur.h>
#include <Voiture.h>
#include <Menu.h>
#include <Outils.h>

#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/msg.h>

using namespace std;

//------------------------------------------------------ Include personnel
#include "Interface.h"

///////////////////////////////////////////////////////////////////  PRIVE
//------------------------------------------------------------- Constantes
NUM_VOITURE_MAX = 299;
//------------------------------------------------------------------ Types

//---------------------------------------------------- Variables statiques
static bool bGeneLaunched;
static unsigned int numVoiture;

static Duree * dureeFeux;
static int mySem;

static int myBAL;

//------------------------------------------------------ Fonctions privées
void FermerInterface ()
// Algorithme : Trivial
{
  // Detache la mémorie partagée avec le prcessus
  shmdt(dureeFeux);

  // On tue la tache dans l'oeuf
  exit (0);
}

//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques

void Interface ( pid_t gene, int idSem, int memDuree, int idFile )
// Algorithme : Trivial
{
  //----------------------------
  // Initialisation 
  //----------------------------

  // Init variables
  bGeneLaunched = false;
  numVoiture = 1;
  myBal = idFile;

  // Attachement de la mémoire partagé
  dureeFeux = (Duree *) shmat(memDuree, NULL, 0);

  // Gestion des semaphores


  //----------------------------
  // Moteur 
  //----------------------------

  for (;;)
  {
    Menu();
  }
} //------ Fin d'Interface

void Commande (char code)
// Algorithme : Trivial, une commande implique une action
{
  switch ( code )
  {
    case 'F':
      // Destruction de la tache en cour et fin de l'application
      FermerInterface();
    break;

    case 'G':
    // Arret du generateur si actif actuellement, sinon mise en route du gene 
      if (bGeneLaunched==true)
      {
        kill (gene, SIGSTOP);
      }
      else 
      {
        kill (gene, SIGCONT);
      }
    break;

    default:
    // Commande non reconnue par le parsseur
      Effacer(MESSAGE);
      Afficher(MESSAGE, "ERREUR | Commande non conforme");
    break;
} //------ Fin de Commande (char code)

void Commande (TypeVoie entree, TypeVoie sortie)
// Algorithme : Creation de la voiture, du message puis ajout dans la BAL
{
  Voiture aVoiture;
  MsgVoiture msg;
  size_t msgSize;

  // Modification du numero de la voiture a creer
  numVoiture<NUM_VOITURE_MAX : numVoiture += 1 , numVoiture = 1 ;

  // Mise en place de la voiture
  aVoiture.entree = entree;
  aVoiture.sortie = sortie;
  aVoiture.numero  = numVoiture;

  // Mise en place du message
  msg.uneVoiture = aVoiture;
  msg.type = msg.uneVoiture.entree;

  msgSize = size_of(MsgVoiture);

  msgsnd(myBAL, &msg, msgSize, 0);
} //------ Fin de Commande (TypeVoie entree, TypeVoie sortie)

void Commande (TypeVoie voie, unsigned int duree)
// Algorithme : Prise du jeton, modification de la duree puis le jeton
// est reposé
{
  //Creation des SemBuf
  struct sembuf duree_V = {SEM_DUREE, 1, 0};
  struct sembuf duree_P = {SEM_DUREE, -1, 0};

  if (voie == "NORD" || voie == "SUD")
  {
    semop(mySem, &duree_P, 1);
    dureeFeux->nS = duree;
    semop(mySem, &duree_V, 1);
  }
  else if (voie == "OUEST" || voie == "EST")
  {
    semop(mySem, &duree_P, 1);
    dureeFeux->eO = duree;
    semop(mySem, &duree_V, 1);
  }
  else 
  {
    // Nous avons un grâve problème !!
    Effacer(MESSAGE);
    Afficher(MESSAGE, "ERREUR | La voie demandée n'est pas valide");
    sleep (10);
    exit (1)
  }
} //------ Fin de Commande ( TypeVoie voie, unsigned int duree)