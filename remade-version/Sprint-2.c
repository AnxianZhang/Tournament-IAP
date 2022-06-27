/**
 * @file Sprint-2.c
 * @author Anxian ZHANG (gadanxianzhang@gmail.com)
 * @brief application providing assistance in 
          the organization of tournaments
 * @version 1.3
 * @date 2022-06-10
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#define NB_MAX_TOURNAMENTS 10
#define NB_MAX_MATCHS 127 //nb max per tournament
#define NB_MAX_PLAYERS 128 //nb max per tournament
#define CHAR_SIZE 30

typedef struct Player{
    char name[CHAR_SIZE + 1];
    unsigned int pts; 
} Player;

typedef struct Match{
    unsigned int idxWinner;
    unsigned int idxLoser;
} Match;

typedef struct Tournament{
    char name[CHAR_SIZE + 1];
    unsigned int year;
    Match matchs[NB_MAX_MATCHS];
} Tournament;

typedef struct WTATournament{
    unsigned int nbTournaments;
    unsigned int nbSavedTournaments;
    unsigned int nbSavedPlayers;
    Tournament tournaments[NB_MAX_TOURNAMENTS];
    Player players[NB_MAX_PLAYERS * NB_MAX_TOURNAMENTS];
} WTATournament;

//Function to do
//s1
void defineNbTournaments(WTATournament *tournament){
    unsigned int nbTournaments;
    scanf("%u", &nbTournaments);
    assert(nbTournaments <= NB_MAX_TOURNAMENTS && nbTournaments > 0);
    tournament->nbTournaments = nbTournaments;
    tournament->nbSavedTournaments = 0;
    tournament->nbSavedPlayers = 0;
}

int getIdxPlayer(WTATournament *tournament, char name[]){
    for (int i = 0; i < tournament->nbSavedTournaments * NB_MAX_PLAYERS + NB_MAX_PLAYERS; ++i){
        if (strcmp(tournament->players[i].name, name) == 0)
            return i;
    }
    return -1;
}

bool isSaved(WTATournament tournament, char playerName[]){
    for (int i = 0; i < tournament->nbSavedTournaments * NB_MAX_PLAYERS + NB_MAX_PLAYERS; ++i){
        if (strcmp(tournament->players[i].name, playerName) == 0 )
            return true;
    }
    return false;
}

void savePlayers(WTATournament *tournament,char playerName[]){
    strcpy(tournament->players[tournament->nbSavedPlayers].name, playerName);
    tournament->players[tournament->nbSavedPlayers].pts = 0;
}

void saveMatch(WTATournament *tournament, unsigned int idxMatch, char winner[], char loser[]){
    unsigned int idxLoser = getIdxPlayer(tournament, loser);
    unsigned int idxWinner = getIdxPlayer(tournament, winner);

    assert(idxLoser >= 0 && idxWinner >= 0);

    unsigned int numT = tournament->nbSavedTournaments;
    tournament->tournaments[numT].matchs[idxMatch].idxLoser = idxLoser;
    tournament->tournaments[numT].matchs[idxMatch].idxWinner = idxWinner;
}


void saveTournament(WTATournament *tournament){
    //save the identity of the tournament
    scanf("%s", &(tournament->tournaments[tournament->nbSavedTournaments].name));
    scanf("%u", &(tournament->tournaments[tournament->nbSavedTournaments].year));

    for (unsigned int i = 0; i < NB_MAX_MATCHS; ++i){
        char winnerName[CHAR_SIZE + 1], loserName[CHAR_SIZE + 1];
        scanf("%s %s", winnerName, loserName);
        if (!isSaved(tournament, winnerName)){
            savePlayers(tournament ,winnerName);
            ++tournament->nbSavedPlayers;
        }
        if (!isSaved(tournament, loserName)){
            savePlayers(tournament ,loserName);
            ++tournament->nbSavedPlayers;
        }
        saveMatch(tournament, i, winnerName, loserName);
    }
    ++tournament->nbSavedTournaments;
}

void showFinal(unsigned int numMatch){
    switch(numMatch){
        case 0:
            printf("64emes de finale\n"); break;
        case 64:
            printf("32emes de finale\n"); break;
        case 96:
            printf("16emes de finale\n"); break;
        case 112:
            printf("8emes de finale\n"); break;
        case 120:
            printf("quarts de finale\n"); break;
        case 124:
            printf("demi-finales\n"); break;
        case 126:
            printf("finale\n"); break;
    }
}

int getIdxTournament(WTATournament *tournament, char name[], unsigned int year){
    for(int i = 0; i < NB_MAX_TOURNAMENTS; ++i){
        if (strcmp(tournament->tournaments[i].name, name) == 0 && 
            tournament->tournaments[i].year == year)
                return i;
    }
    return -1;
}

void showMatch(WTATournament *tournament, unsigned int idxT, unsigned int idxM){
    showFinal(idxM);
    Match m = tournament->tournaments[idxT].matchs[idxM];
    unsigned int i = m.idxWinner, j = m.idxLoser;
    printf("%s %s\n", tournament->players[i].name, tournament->players[j].name);
}

void showTournamentsMatchs(WTATournament *tournament){
    char tournamentName[CHAR_SIZE + 1];
    unsigned int tournamentYear;
    scanf("%s %u", tournamentName, &tournamentYear);
    int idxTournament = getIdxTournament(tournament, tournamentName, tournamentYear);

    assert(idxTournament >= 0);

    printf("%s ", tournament->tournaments[idxTournament].name);
    printf("%u\n", tournament->tournaments[idxTournament].year);
    for(int m = 0; m < NB_MAX_MATCHS; ++m)
        showMatch(tournament, idxTournament, m);
}

void showPlayersMatchs(WTATournament *tournament){
    char tournamentName[CHAR_SIZE + 1], playerName[CHAR_SIZE + 1];
    unsigned int tournamentYear = 0;
    scanf("%s %u %s", tournamentName, &tournamentYear, playerName);
    int idxT = getIdxTournament(tournament, tournamentName, tournamentYear);
    int idxP = getIdxPlayer(tournament, playerName);

    assert(idxT >= 0 && idxP >= 0);

    for(int m = 0; m < NB_MAX_MATCHS; ++m){
        if (tournament->tournaments[idxT].matchs[m].idxLoser == idxP ||
            tournament->tournaments[idxT].matchs[m].idxWinner == idxP)
            showMatch(tournament, idxT, m);
    }
}

int main (void){
    WTATournament tournament;
    char word[CHAR_SIZE + 1];

    while(1){
        scanf("%s", word);
        if (strcmp(word, "definir_nombre_tournois") == 0)
            defineNbTournaments(&tournament);
        else if (strcmp(word, "enregistrement_tournoi") == 0)
            saveTournament(&tournament);
        else if (strcmp(word, "affichage_matchs_tournoi") == 0)
            showTournamentsMatchs(&tournament);
        else if (strcmp(word, "afficher_matchs_joueuse") == 0)
            showPlayersMatchs(&tournament);
        else if (strcmp(word, "exit") == 0)
            exit(0);
    }
}