/**
 * @file App.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-06-06
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

#define ROUND_OF_64 0
#define ROUND_OF_32 64
#define ROUND_OF_16 96
#define ROUND_OF_8 112
#define ROUND_OF_4 120
#define ROUND_OF_2 124
#define ROUND_OF_FINAL 126

typedef struct Player{
    char name[CHAR_SIZE + 1];
    unsigned int scores; 
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

bool isSaved(WTATournament *tournament, char playerName[]){
    for (int i = 0; i < tournament->nbSavedTournaments * NB_MAX_PLAYERS + NB_MAX_PLAYERS; ++i){
        if (strcmp(tournament->players[i].name, playerName) == 0 )
            return true;
    }
    return false;
}

void savePlayers(WTATournament *tournament,char playerName[]){
    strcpy(tournament->players[tournament->nbSavedPlayers].name, playerName);
    tournament->players[tournament->nbSavedPlayers].scores = 0;

    ++tournament->nbSavedPlayers;
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

        if (!isSaved(tournament, winnerName))
            savePlayers(tournament ,winnerName);
        if (!isSaved(tournament, loserName))
            savePlayers(tournament ,loserName);

        saveMatch(tournament, i, winnerName, loserName);
    }
    ++tournament->nbSavedTournaments;
}

void showFinal(unsigned int numMatch){
    switch(numMatch){
        case ROUND_OF_64:
            printf("64emes de finale\n"); break;
        case ROUND_OF_32:
            printf("32emes de finale\n"); break;
        case ROUND_OF_16:
            printf("16emes de finale\n"); break;
        case ROUND_OF_8:
            printf("8emes de finale\n"); break;
        case ROUND_OF_4:
            printf("quarts de finale\n"); break;
        case ROUND_OF_2:
            printf("demi-finales\n"); break;
        case ROUND_OF_FINAL:
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

//s1
void showTournamentsMatchs(WTATournament *tournament){
    char tournamentName[CHAR_SIZE + 1];
    unsigned int tournamentYear;
    scanf("%s %u", tournamentName, &tournamentYear);
    int idxTournament = getIdxTournament(tournament, tournamentName, tournamentYear);

    if (idxTournament < 0) printf("This tournament does not exist\n");

    printf("%s ", tournament->tournaments[idxTournament].name);
    printf("%u\n", tournament->tournaments[idxTournament].year);
    for(int m = 0; m < NB_MAX_MATCHS; ++m)
        showMatch(tournament, idxTournament, m);
}

//s2
void showPlayersMatchs(WTATournament *tournament){
    char tournamentName[CHAR_SIZE + 1], playerName[CHAR_SIZE + 1];
    unsigned int tournamentYear = 0;
    scanf("%s %u %s", tournamentName, &tournamentYear, playerName);
    int idxT = getIdxTournament(tournament, tournamentName, tournamentYear);
    int idxP = getIdxPlayer(tournament, playerName);

    if (idxT < 0) printf("This tournament does not exist\n");
    if (idxP < 0) printf("this player does not exist\n");

    for(int m = 0; m < NB_MAX_MATCHS; ++m)
        if (tournament->tournaments[idxT].matchs[m].idxLoser == idxP ||
            tournament->tournaments[idxT].matchs[m].idxWinner == idxP)
            showMatch(tournament, idxT, m);
}

void playersOfTournament(WTATournament *tournament ,Player players[], unsigned int idxT){
    unsigned int counterPlayer = 0;
    for(int m = 0; m < ROUND_OF_32; ++m){ //m = match
        unsigned int idxWinner = tournament->tournaments[idxT].matchs[m].idxWinner;
        unsigned int idxLoser = tournament->tournaments[idxT].matchs[m].idxLoser;

        players[counterPlayer] = tournament->players[idxWinner];
        players[counterPlayer + 1] = tournament->players[idxLoser];
        counterPlayer += 2;
    }
}

int getSecondIdxPlayer(Player players[] ,char pName[]){
    for (int i = 0; i < NB_MAX_PLAYERS; ++i){
        if (strcmp(players[i].name, pName) == 0)
            return i;
    }
    return -1;
}

void sortPlayers(Player players[]){
    Player pTmp;
    for (int i = 0; i < NB_MAX_PLAYERS; ++i)
        for(int j = 0; j < NB_MAX_MATCHS; ++j)
            if (strcmp(players[i].name, players[j].name) < 0){
                pTmp = players[i];
                players[i] =  players[j];
                players[j] = pTmp;
            }
}

void giveScores(Player players[], unsigned int idxM, unsigned int idxLoser, unsigned int idxWinner){
    if (idxM >= ROUND_OF_64 && idxM < ROUND_OF_32)
        players[idxLoser].scores += 10;
    else if (idxM >= ROUND_OF_32 && idxM < ROUND_OF_16)
        players[idxLoser].scores += 45;
    else if (idxM >= ROUND_OF_16 && idxM < ROUND_OF_8)
        players[idxLoser].scores += 90;
    else if (idxM >= ROUND_OF_8 && idxM < ROUND_OF_4)
        players[idxLoser].scores += 180;
    else if (idxM >= ROUND_OF_4 && idxM < ROUND_OF_2)
        players[idxLoser].scores += 360;
    else if (idxM >= ROUND_OF_2 && idxM < ROUND_OF_FINAL)
        players[idxLoser].scores += 720;
    else if (idxM >= ROUND_OF_FINAL){
        players[idxLoser].scores += 1200;
        players[idxWinner].scores += 2000;
    }  
}

void giveScoresToPlayers(WTATournament *tournament, Player players[], unsigned int idxT){
    playersOfTournament(tournament, players, idxT);

    for (int idxM = 0; idxM < NB_MAX_MATCHS; ++idxM){
        unsigned int idxWinner = tournament->tournaments[idxT].matchs[idxM].idxWinner;
        unsigned int idxLoser = tournament->tournaments[idxT].matchs[idxM].idxLoser;
        idxLoser = getSecondIdxPlayer(players, tournament->players[idxLoser].name);
        idxWinner = getSecondIdxPlayer(players, tournament->players[idxWinner].name);

        giveScores(players, idxM, idxLoser, idxWinner);
    }
}

//s3
void showTournamentsPlayers(WTATournament *tournament){
    char tournamentName[CHAR_SIZE + 1];
    unsigned int tournamentYear;
    scanf("%s %u", tournamentName, &tournamentYear);
    printf("%s %u\n", tournamentName, tournamentYear);
    int idxTournament = getIdxTournament(tournament, tournamentName, tournamentYear);

    if (idxTournament < 0) printf("This tournament does not exist\n");

    Player players[NB_MAX_PLAYERS];

    giveScoresToPlayers(tournament, players, idxTournament);
    sortPlayers(players);

    for (int i = 0; i < NB_MAX_PLAYERS; ++i)
        printf("%s %u\n", players[i].name, players[i].scores); 
}

//s4
void showRanks(){
    //TODO
}
//End of function to do

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
        else if (strcmp(word, "affichage_joueuses_tournoi") == 0)
            showTournamentsPlayers(&tournament);
        else if (strcmp(word, "exit") == 0)
            exit(0);
    }
}