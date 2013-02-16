/*************************************************************************
                           Mere  -  description
                             -------------------
    debut                : 06/02/2013
    copyright            : (C) nbuisson
    e-mail               : nicolas.buisson@insa-lyon.fr
*************************************************************************/

//---------- Réalisation de la tache <Mere> (fichier Mere.cpp) ---

/////////////////////////////////////////////////////////////////  INCLUDE
//-------------------------------------------------------- Include systeme
#include <unistd.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <signal.h>
#include <stdlib.h>
#include <Outils.h>
#include <Heure.h>

using namespace std;

//------------------------------------------------------ Include personnel
#include "Mere.h"

///////////////////////////////////////////////////////////////////  PRIVE
//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

struct msgVoiture
{
    long type;
    unsigned int numVoiture;
};

//---------------------------------------------------- Variables statiques

//------------------------------------------------------ Fonctions privées
//static type nom ( liste de parametres )
// Mode d'emploi :
//
// Contrat :
//
// Algorithme :
//
//{
//} //----- fin de nom

static void creerMemoires(int& etatFeux, int& duree)
// Mode d'emploi :
// Crée les zones de mémoires partagées
// Alogrithme :
// Trivial
{
    // Création de la zone de mémoire EtatFeux
	etatFeux = shmget(IPC_PRIVATE, sizeof(EtatFeux), 0666 | IPC_CREAT);

	// Création de la zone de mémoire Duree
	duree = shmget(IPC_PRIVATE, sizeof(Duree), 0666 | IPC_CREAT);
} //----- fin de creerMemoires

static void creerBAL(int (&fileVoitures)[4])
// Mode d'emploi :
// Crée les boites aux lettres contenant les files de voitures
// Algorithme :
// Trivial
{
    for(int i = 0 ; i < 4 ; i++)
    {
        fileVoitures[i] = msgget(IPC_PRIVATE, 0666 | IPC_CREAT);
    }
} //----- fin de creerBAL

static void detruireMemoires(int duree, int etatFeux)
// Mode d'emploi :
// Détruit les zones de mémoires partagées
// Algorithme :
// Trivial
{
    // Destruction de EtatFeux
    shmctl(etatFeux, IPC_RMID, 0);

    // Destruction de Duree
    shmctl(duree, IPC_RMID, 0);
} //----- fin de detruireMemoires

static void detruireBAL(int fileVoitures[4])
// Mode d'emploi :
// Détruit les boites aux lettres
// Algorithme :
// Trivial
{
    for(int i = 0 ; i < 4 ; i++)
    {
        msgctl(fileVoitures[i], IPC_RMID, 0);
    }
} //----- fin de detruireBAL

//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques
//type Nom ( liste de parametres )
// Algorithme :
//
//{
//} //----- fin de Nom

int main()
{
    pid_t pidHeure;
    pid_t pidInterface;
    pid_t pidVoie[4];
    pid_t pidFeux;
    pid_t pidDeplacement;
    pid_t pidGenerateur;

    int idEtatFeux;
    int idDuree;
    int idFileVoiture[4];

    // Mise en place de la structure de donnée EtatFeux
    EtatFeux memEtatFeux;
    memEtatFeux.eO = false;
    memEtatFeux.nS = false;

    // Mise en place de la structure de donnée Durée
    Duree memDuree;
    memDuree.eO = 12;
    memDuree.nS = 18;

    // Création des zones mémoires
    creerMemoires(idEtatFeux, idDuree);

    // Création des boites aux lettres
    creerBAL(idFileVoiture);

    // Initialisation de l'application
    InitialiserApplication(XTERM);

    // Création de l'heure
    pidHeure = CreerEtActiverHeure();

    sleep(10);

    // Destruction de l'heure
    kill(pidHeure, SIGUSR2);

    // Destruction de l'interface
    TerminerApplication(true);

    // Destruction des zones de mémoires partagées
    detruireMemoires(idEtatFeux, idDuree);

    // Destruction des boites aux lettres
    detruireBAL(idFileVoiture);

	return 0;
} //----- fin de main
