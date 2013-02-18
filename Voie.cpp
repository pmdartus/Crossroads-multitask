/*************************************************************************
                           Voie  -  description
                             -------------------
    debut                : 06/02/2013
    copyright            : (C) pmdartus
    e-mail               : pierre-marie.dartus@insa-lyon.fr
*************************************************************************/

//---------- Réalisation de la tache <Voie> (fichier Voie.cpp) ---

/////////////////////////////////////////////////////////////////  INCLUDE
//-------------------------------------------------------- Include systeme
#include <vector>
#include <signal.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/msg.h>

#include <Voiture.h>
#include <Outils.h>

using namespace std;

//------------------------------------------------------ Include personnel
#include "Voie.h"
#include "Mere.h"

///////////////////////////////////////////////////////////////////  PRIVE
//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

//---------------------------------------------------- Variables statiques
static unsigned int nVoie;
static EtatFeux * feux;

int myBAL;

static std::vector<pid_t> vectDeplacement;

//------------------------------------------------------ Fonctions privées

static void  FinTache (int typeSignal)
{
  if (typeSignal == SIGUSR2)
  {
    // Détachement de la mémoire
    shmdt (feux);

    // Suppression des déplacements restants

    // Supression handlers

    // Fin de la tache
  }
}

static void FinDeplacement (int typeSignal)
{
  if (typeSignal == SIGCHLD)
  {
    // Supression dans la liste des deplacement de la voiture ayant finis
    // son déplacement
  }
}

//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques
void Voie( unsigned int numVoie, int idFeu, int idFile )
// Algorithme :
//
{

  //----------------------------
  // Initialisation 
  //----------------------------

  nVoie = numVoie;

  // Traitement  de la fin de la tache
  
  struct sigaction finTache;
  finTache.sa_handler = FinTache;
  finTache.sa_flags = 0;
  sigaction (SIGUSR2, &finTache, NULL);

  // Traitemet de la fin d'un déplacement

  struct sigaction finDeplacement;
  finTache.sa_handler = FinDeplacement;
  finTache.sa_flags = 0;
  sigaction (SIGCHLD, &finTache, NULL);



  // Attachement de la mémoire partagée
  feux = (EtatFeux *) shmat(idFeu, NULL, 0);

  // Réception d'un message
  struct MsgVoiture msg;

  //----------------------------
  // Moteur 
  //----------------------------

  for (;;)
  {
    // Attente de la prochaine voiture à traiter par la voie en question
    // Etat d'attente bloquant
    while ( msgrcv(myBAL, &msg, TAILLE_MSG_VOITURE, (long)(nVoie), 0) < 0  )
    {
      /*
      int imatriculation ;
      Voiture aVoiture = msg.uneVoiture;
      DessinerVoitureFeu(aVoiture.numero, aVoiture.entree, aVoiture.sortie);

      if (numVoie == 1 || numVoie == 3)
      {
        while (!feux->nS)
        {
        }
      }
      else
      {
        while (!feux->eO)
        {
        }
      }
      */

      //OperationVoie (MOINS, numVoie) ;

    }

  }
} //----- fin de Voie

