/**
 * @file App.c
 * @author Anxian ZHANG (gadanxianzhang@gmail.com)
 * @brief application providing assistance in 
          the organization of tournaments
 * @version 5.5
 * @date 2022-06-20
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#pragma warning(disable:4996) // for scanf

#define NB_MAX_TOURNAMENTS 10
#define NB_MAX_MATCHS 127 //nb max per tournament
#define NB_MAX_PLAYERS 128 //nb max per tournament
#define CHAR_SIZE 30
#define MAX_SHOW_TOURNAMENT 4

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
    unsigned int nbNewPlayers; // new players that wasn't saved before
    Match matchs[NB_MAX_MATCHS];
} Tournament;

typedef struct WTATournament{
    unsigned int nbTournaments;
    unsigned int nbSavedTournaments;
    unsigned int nbSavedPlayers;
    Tournament tournaments[NB_MAX_TOURNAMENTS];
    Player players[NB_MAX_PLAYERS * NB_MAX_TOURNAMENTS];
} WTATournament;

/**
 * @brief Sprint 1, part 1;
 *        Initialse and difine the number of tournament(s)
 * 
 * @param tournament [out] : the tournament
 */
void defineNbTournaments(WTATournament *tournament){
    unsigned int nbTournaments;
    scanf("%u", &nbTournaments);
    assert(nbTournaments <= NB_MAX_TOURNAMENTS && nbTournaments > 0);
    tournament->nbTournaments = nbTournaments;
    tournament->nbSavedTournaments = 0;
    tournament->nbSavedPlayers = 0;
}

/**
 * @brief Get the index of Player with his name 
 * 
 * @param tournament [in] : the tournament
 * @param name [in] : player name
 * @return the index 
 */
int getIdxPlayer(const WTATournament *tournament, const char name[]){
    for (int i = 0; i < tournament->nbSavedTournaments * NB_MAX_PLAYERS + NB_MAX_PLAYERS; ++i){
        if (strcmp(tournament->players[i].name, name) == 0)
            return i;
    }
    return -1;
}

/**
 * @brief Search if the player has been registered
 * 
 * @param tournament [in] : the tournament
 * @param playerName [in] : player name
 * @return true 
 * @return false 
 */
bool isSaved(const WTATournament *tournament, const char playerName[]){
    for (int i = 0; i < tournament->nbSavedTournaments * NB_MAX_PLAYERS + NB_MAX_PLAYERS; ++i){
        if (strcmp(tournament->players[i].name, playerName) == 0 )
            return true;
    }
    return false;
}

/**
 * @brief Save a player in a specific tournament and initialize it
 * 
 * @param tournament [in-out] : the tournament
 * @param playerName [in] : player name
 */
void savePlayers(WTATournament *tournament, const char playerName[]){
    strcpy(tournament->players[tournament->nbSavedPlayers].name, playerName);
    tournament->players[tournament->nbSavedPlayers].scores = 0;

    ++tournament->tournaments[tournament->nbSavedTournaments].nbNewPlayers;
    ++tournament->nbSavedPlayers;
}

/**
 * @brief Save a match with the index of 2 players
 * 
 * @param tournament [in-out] : the tournament
 * @param idxMatch [in] : index of a match
 * @param winner [in]: winner name
 * @param loser [in] : loser name
 *
 * @see getIdxPlayer(const WTATournament *tournament, const char name[])
 */
void saveMatch(WTATournament *tournament, unsigned int idxMatch, const char winner[], const char loser[]){
    unsigned int idxLoser = getIdxPlayer(tournament, loser);
    unsigned int idxWinner = getIdxPlayer(tournament, winner);

    assert(idxLoser >= 0 && idxWinner >= 0);

    unsigned int numT = tournament->nbSavedTournaments;
    tournament->tournaments[numT].matchs[idxMatch].idxLoser = idxLoser;
    tournament->tournaments[numT].matchs[idxMatch].idxWinner = idxWinner;
}

/**
 * @brief Sprint 1, part 2;
 *        Save a tournament
 * 
 * @param tournament [in-out] : the tournament
 *
 * @see isSaved(const WTATournament *tournament, const char playerName[])
 * @see savePlayers(WTATournament *tournament, const char playerName[])
 * @see saveMatch(WTATournament *tournament, unsigned int idxMatch, const char winner[], const char loser[])
 */
void saveTournament(WTATournament *tournament){
    //save the identity of the tournament
    scanf("%s", &(tournament->tournaments[tournament->nbSavedTournaments].name));
    scanf("%u", &(tournament->tournaments[tournament->nbSavedTournaments].year));

    tournament->tournaments[tournament->nbSavedTournaments].nbNewPlayers = 0;

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

/**
 * @brief displays the appropriate final according to the match number
 * 
 * @param numMatch [in] : march number
 */
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

/**
 * @brief Get the index of a tournament, with the name of a tournament and it year
 * 
 * @param tournament [in] : the tournament
 * @param name [in] : name of the tournament
 * @param year [in] : year od the tournament
 * @return int : the index of tournament
 */
int getIdxTournament(const WTATournament *tournament, const char name[], unsigned int year){
    for(int i = 0; i < NB_MAX_TOURNAMENTS; ++i){
        if (strcmp(tournament->tournaments[i].name, name) == 0 && 
            tournament->tournaments[i].year == year)
                return i;
    }
    return -1;
}

/**
 * @brief show a match of a tournament, with the index of a tournament
 *        and the index of a match
 * 
 * @param tournament [in] : the tournament
 * @param idxT [in] : the index of a tournament
 * @param idxM [in] : the index of a match
 *
 * @see showFinal(unsigned int numMatch)
 */
void showMatch(const WTATournament *tournament, unsigned int idxT, unsigned int idxM){
    showFinal(idxM);
    Match m = tournament->tournaments[idxT].matchs[idxM];
    unsigned int i = m.idxWinner, j = m.idxLoser;
    printf("%s %s\n", tournament->players[i].name, tournament->players[j].name);
}

/**
 * @brief Sprint 1, part 3 (last part);
 *        Show a tournament that has been saved
 * 
 * @param tournament [in] : the tournament
 *
 * @see showMatch(const WTATournament *tournament, unsigned int idxT, unsigned int idxM)
 * @see getIdxTournament(const WTATournament *tournament, const char name[], unsigned int year)
 */
void showTournamentsMatchs(const WTATournament *tournament){
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

/**
 * @brief Sprint 2;
 *        Show all matchs of a players
 * 
 * @param tournament [in] : the tournament
 *
 * @see getIdxTournament(const WTATournament *tournament, const char name[], unsigned int year)
 * @see showMatch(const WTATournament *tournament, const unsigned int idxT, unsigned int idxM)
 */
void showPlayersMatchs(const WTATournament *tournament){
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

/**
 * @brief copy the players of a tournament into a table
 * 
 * @param tournament [in] : the tournament
 * @param players [out] : table of players
 * @param idxT [in] : index of a tournament
 */
void playersOfTournament(const WTATournament *tournament , Player players[], unsigned int idxT){
    unsigned int counterPlayer = 0;
    for(int m = 0; m < ROUND_OF_32; ++m){ //m = match
        unsigned int idxWinner = tournament->tournaments[idxT].matchs[m].idxWinner;
        unsigned int idxLoser = tournament->tournaments[idxT].matchs[m].idxLoser;

        players[counterPlayer] = tournament->players[idxWinner];
        players[counterPlayer + 1] = tournament->players[idxLoser];
        counterPlayer += 2;
    }
}

/**
 * @brief Get the index of a player in a table of players
 * 
 * @param players [in] : the table of players
 * @param pName [in] : the player
 * @param maxP [in] : lenght of the table
 * @return int the index of a players
 */
int getSecondIdxPlayer(const Player players[] , const char pName[], unsigned int maxP){
    for (int i = 0; i < maxP; ++i){
        if (strcmp(players[i].name, pName) == 0)
            return i;
    }
    return -1;
}

/**
 * @brief sort the table of players by their name 
 * 
 * @param players [out] : players table
 */
void sortPlayersByName(Player players[]){
    Player pTmp;
    for (int i = 0; i < NB_MAX_PLAYERS; ++i)
        for(int j = 0; j < NB_MAX_MATCHS; ++j)
            if (strcmp(players[i].name, players[j].name) < 0){
                pTmp = players[i];
                players[i] =  players[j];
                players[j] = pTmp;
            }
}

/**
 * @brief Give scores to a / two player(s) according to the index of a match
 * 
 * @param players [out] : the copy of players
 * @param idxM [in] : index of a match
 * @param idxLoser [in] : index of loser
 * @param idxWinner [in] : index of winner
 */
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

/**
 * @brief gives scores to all players of a tournamenr
 * 
 * @param tournament [in] : the tournament
 * @param players [out] : the copy of players
 * @param idxT [in] : index of a tournament
 * @param maxP [in] : lenght of the table of players
 * 
 * @see getSecondIdxPlayer(const Player players[] , const char pName[], unsigned int maxP)
 * @see giveScores(Player players[], unsigned int idxM, unsigned int idxLoser, unsigned int idxWinner)
 */
void giveScoresToPlayers(const WTATournament *tournament, Player players[], unsigned int idxT, unsigned int maxP){
    for (int idxM = 0; idxM < NB_MAX_MATCHS; ++idxM){
        unsigned int idxWinner = 0;
        unsigned int idxLoser = tournament->tournaments[idxT].matchs[idxM].idxLoser;
        idxLoser = getSecondIdxPlayer(players, tournament->players[idxLoser].name, maxP);
        if (idxM == NB_MAX_MATCHS - 1){ // only for the last match
            idxWinner = tournament->tournaments[idxT].matchs[idxM].idxWinner;
            idxWinner = getSecondIdxPlayer(players, tournament->players[idxWinner].name, maxP);
        }

        giveScores(players, idxM, idxLoser, idxWinner);
    }
}

/**
 * @brief Sprint 3;
 *        Show all players of a tourment with their scores,
 *        and sorted by their name
 * 
 * @param tournament [in] : the tournament
 *
 * @see getIdxTournament(const WTATournament *tournament, const char name[], unsigned int year)
 * @see playersOfTournament(const WTATournament *tournament , Player players[], unsigned int idxT)
 * @see giveScoresToPlayers(const WTATournament *tournament, Player players[], unsigned int idxT, unsigned int maxP)
 * @see sortPlayersByName(Player players[])
 */
void showTournamentsPlayers(const WTATournament *tournament){
    char tournamentName[CHAR_SIZE + 1];
    unsigned int tournamentYear;
    scanf("%s %u", tournamentName, &tournamentYear);
    printf("%s %u\n", tournamentName, tournamentYear);
    int idxTournament = getIdxTournament(tournament, tournamentName, tournamentYear);

    if (idxTournament < 0) printf("This tournament does not exist\n");

    Player players[NB_MAX_PLAYERS];

    playersOfTournament(tournament, players, idxTournament);
    giveScoresToPlayers(tournament, players, idxTournament, NB_MAX_PLAYERS);
    sortPlayersByName(players);

    for (int i = 0; i < NB_MAX_PLAYERS; ++i)
        printf("%s %u\n", players[i].name, players[i].scores); 
}

/**
 * @brief Sort a table of players by their scores, if there is the
 *        sames amount of scores, then sort by their name
 * 
 * @param players [out] : table of players
 * @param nbSavedPlayers [in] : number of players in the table
 */
void sortPlayersByScores(Player players[], unsigned int nbSavedPlayers){
    Player pTmp;
    for (int i = 0; i < nbSavedPlayers; ++i)
        for (int j = 0; j < nbSavedPlayers; ++j){
            if (players[i].scores == players[j].scores){
                if (strcmp(players[i].name, players[j].name) < 0){
                    pTmp = players[i];
                    players[i] =  players[j];
                    players[j] = pTmp;
                }
            }
            if (players[i].scores > players[j].scores){
                pTmp = players[i];
                players[i] =  players[j];
                players[j] = pTmp;
            }
        }
}

/**
 * @brief Indicate if the players has been copied in the table players
 * 
 * @param players [in] : table of players
 * @param pName [in] : player name
 * @param maxP [in] : number of players in the table
 * @return true 
 * @return false 
 */
bool isCopied(const Player players[], const char pName[], unsigned int maxP){
    for (int i = 0; i < maxP; ++i)
        if (strcmp(players[i].name, pName) == 0)
            return true;
    return false; 
}

/**
 * @brief Sprint 4 and 5;
 *        displays the 4 previous tournaments with the players 
 *        and their scores that have been recorded starting from the last one recorded
 * 
 * @param tournament [in] : the tournament
 */
void showRanks(const WTATournament *tournament){
    Player players[NB_MAX_PLAYERS * MAX_SHOW_TOURNAMENT];
    unsigned int totalPlayers = 0; // total players of 4 tournaments
    
    for (int i = 0, numT = tournament->nbSavedTournaments - 1; i < MAX_SHOW_TOURNAMENT; ++i, --numT){
        if (numT >= 0){
            for(int m = 0; m < ROUND_OF_32; ++m){ //m = match
                unsigned int idxWinner = tournament->tournaments[numT].matchs[m].idxWinner;
                unsigned int idxLoser = tournament->tournaments[numT].matchs[m].idxLoser;

                if (!isCopied(players, tournament->players[idxWinner].name, totalPlayers)){
                    players[totalPlayers] = tournament->players[idxWinner];
                    ++totalPlayers;
                }
                if (!isCopied(players, tournament->players[idxLoser].name, totalPlayers)){
                    players[totalPlayers] = tournament->players[idxLoser];
                    ++totalPlayers;
                }
            }
            giveScoresToPlayers(tournament, players, numT, totalPlayers);
        }
    }
    sortPlayersByScores(players, totalPlayers);

    for (int i = 0; i < totalPlayers; ++i)
        printf("%s %u\n", players[i].name, players[i].scores);
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
        else if (strcmp(word, "affichage_joueuses_tournoi") == 0)
            showTournamentsPlayers(&tournament);
        else if (strcmp(word, "afficher_classement") == 0)
            showRanks(&tournament);
        else if (strcmp(word, "exit") == 0)
            exit(0);
    }
}