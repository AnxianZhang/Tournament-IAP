/****************************************
 Nom---------: Sprint4.c
 Role--------: Aplication qui organise 
               les tournois
 Auteur------: Anxian Zhang, Wang Florent
 Version-----: V1 du 13/11/2021
****************************************/

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#pragma warning(disable:4996)

// definition des constantes imposer: 
#define max_tournois 10                  // Nombre max de tounois
#define nb_matchs 127                    // Nombre de matchs par tournois
#define nb_joueuse 128                   // Nombre de joueuses par tournoi
#define lgmot 30                         // chaine de caractere max
    
// définitions des finales en fonction du nombre de matchs
#define finale_64 64
#define finale_32 96
#define finale_16 112
#define finale_8  120
#define finale_4  124
#define finale_2  126

// -----------------------------------------------definition des types-----------------------------------

typedef struct Joueuse {
    char nom_joueuse[lgmot + 1];
    unsigned int points;
}Joueuse;

typedef struct Match {
    unsigned int idx_gagnante;
    unsigned int idx_perdante;
}Match;

typedef struct Tournoi {
    char date_tournoi[5];
    char nom_tournoi[lgmot + 1];
    Match data_matchs[nb_matchs];
}Tournoi;

typedef struct TournoiWTA {
    unsigned int nb_tournois;
    unsigned int num_tournois;
    Tournoi dataTournois[max_tournois];
    Joueuse dataJoueuse[max_tournois * nb_joueuse];
}TournoiWTA;

/*------------------------------------------declaration des fonction-----------------------------------*/

/*
* recupere le numero du tournois actuelle, et verrifis si le tournois existe
* int_Tournoi_WTA [in-out] type TournoiWTA
* nom [in] type char
* anne [in] type annee
* t [return] le numéros du tournois actuele
* -1 [return] si un tournois est pas présent
*/
unsigned int recup_num_tournois(TournoiWTA *int_Tournoi_WTA, char nom[], char annee[])
{
    for(int t = 0; t< int_Tournoi_WTA->nb_tournois; t++)
    {
        if(strcmp(int_Tournoi_WTA->dataTournois[t].nom_tournoi, nom) == 0 && 
           strcmp(int_Tournoi_WTA->dataTournois[t].date_tournoi , annee) == 0)
        {
            return t;
        }
    }
    return -1;
}

/*
* fournit l'index du joueur en fonction de son nom 
* joueuse [in] type char
* int_Tournoi_WTA [in-out] type TournoiWtA
* j [return] type int, l'index de la joueuse
* -1 [return] si une joueuse n'existe pas
*/
unsigned int recup_idx_joueuse(TournoiWTA *int_Tournoi_WTA, char joueuse[], unsigned int startin_joueuse)
{
    for(int j = startin_joueuse; j<nb_joueuse*max_tournois; j++)
    {
        if(strcmp(int_Tournoi_WTA->dataJoueuse[j].nom_joueuse, joueuse) == 0)
        {
            return j;
        }
    }
    return -1;
}

/**
 * @brief attribution_points: permet d'attribuer des points en fonction du match 
 *        /!\ les points je sont qu'attribuer aux perdantes (sauf final)
 * @param int_Tournoi_WTA [in-out] type TournoiWTA
 * @param index_loser [in] type const unigned int
 * @param index_winner [in] type const unsigned int
 * @param startin_match [in] type cons unisigne int
 */
void attribution_points(TournoiWTA *int_Tournoi_WTA, const unsigned int index_loser,
                        const unsigned int index_winner, const unsigned int startin_match)
{
    // printf("match = %d\n", startin_match);
    if(startin_match <finale_64)
    {
        int_Tournoi_WTA->dataJoueuse[index_loser].points = 10;
        // printf("Index: %d point: %d\n", index_loser, int_Tournoi_WTA->dataJoueuse[index_loser].points);
    }
    else if (startin_match <finale_32 && startin_match >=finale_64 )
    {
        int_Tournoi_WTA->dataJoueuse[index_loser].points = 45;
        // printf("Index: %d point: %d\n", index_loser, int_Tournoi_WTA->dataJoueuse[index_loser].points);
    }
    else if (startin_match <finale_16 && startin_match >=finale_32)
    {
        int_Tournoi_WTA->dataJoueuse[index_loser].points = 90;
        // printf("Index: %d point: %d\n", index_loser, int_Tournoi_WTA->dataJoueuse[index_loser].points);
    }
    else if (startin_match <finale_8 && startin_match >=finale_16)
    {
        int_Tournoi_WTA->dataJoueuse[index_loser].points = 180;
        // printf("Index: %d point: %d\n", index_loser, int_Tournoi_WTA->dataJoueuse[index_loser].points);
    }
    else if (startin_match <finale_4 && startin_match >=finale_8)
    {
        int_Tournoi_WTA->dataJoueuse[index_loser].points = 360;
        // printf("Index: %d point: %d\n", index_loser, int_Tournoi_WTA->dataJoueuse[index_loser].points);
    }
    else if (startin_match <finale_2 && startin_match >=finale_4)
    {
        int_Tournoi_WTA->dataJoueuse[index_loser].points = 720;
        // printf("Index: %d point: %d\n", index_loser, int_Tournoi_WTA->dataJoueuse[index_loser].points);
    }
    else
    {
        int_Tournoi_WTA->dataJoueuse[index_loser].points = 1200;
        int_Tournoi_WTA->dataJoueuse[index_winner].points = 2000;
        // printf("Index: %d point: %d\n", index_loser, int_Tournoi_WTA->dataJoueuse[index_loser].points);
        // printf("Index: %d point: %d\n", index_winner, int_Tournoi_WTA->dataJoueuse[index_winner].points);
    }
    
    
}

void trie_joueuse(TournoiWTA *int_Tournoi_WTA, int start_rang_joueuse)
{
    char stockage_joueuse[lgmot+1];
    unsigned int stockage_points;
    for(int i= 0, r1 = start_rang_joueuse; i<nb_joueuse; i++, r1++)
        for(int j = 0, r2 = start_rang_joueuse; j<nb_joueuse; j++, r2++)
        {
            if (strcmp(int_Tournoi_WTA->dataJoueuse[r1].nom_joueuse, int_Tournoi_WTA->dataJoueuse[r2].nom_joueuse)<0)
            {
                strcpy(stockage_joueuse,int_Tournoi_WTA->dataJoueuse[r1].nom_joueuse);
                strcpy(int_Tournoi_WTA->dataJoueuse[r1].nom_joueuse, int_Tournoi_WTA->dataJoueuse[r2].nom_joueuse);
                strcpy(int_Tournoi_WTA->dataJoueuse[r2].nom_joueuse, stockage_joueuse);

                stockage_points = int_Tournoi_WTA->dataJoueuse[r1].points;
                int_Tournoi_WTA->dataJoueuse[r1].points = int_Tournoi_WTA->dataJoueuse[r2].points;
                int_Tournoi_WTA->dataJoueuse[r2].points = stockage_points;

            }   
        }
}

/*
* Recupere les 128 joueuses de chaque tournois, et attribut les index respecive en fonction de la joueuse d'un mtach
* num_tournois [in] type unseigned int
* startin_num_joueuse [in] type int
* startin_match [in] type int
* int_Tournoi_WTA [in-out] type TournoiWTA
* pas de valeur de retour
*/
void recup_joueuse(TournoiWTA *int_Tournoi_WTA, int startin_match, int startin_num_joueuse, unsigned int num_tournois,
                   unsigned int num_match)
{
    char winner[lgmot+1], loser[lgmot+1];
    
    if(startin_match == 0) // 0 équiveaux au premier match d'un tournois
    {
       for (int i = 0, j = startin_match, idx=startin_num_joueuse, num_match; i < nb_joueuse; i += 2, j++, idx+=2, num_match++)
        {
            scanf("%s %s", winner, loser);
            strcpy(int_Tournoi_WTA->dataJoueuse[idx].nom_joueuse, winner); 
            strcpy(int_Tournoi_WTA->dataJoueuse[idx+1].nom_joueuse, loser);
            int_Tournoi_WTA->dataTournois[num_tournois].data_matchs[j].idx_gagnante = idx; // mise en place des gagnates dans l'idx
            int_Tournoi_WTA->dataTournois[num_tournois].data_matchs[j].idx_perdante = idx + 1; //mise en place des perdantes dans l'idx

            // printf("%d %d\n", idx, idx+1);
            // idx+1 est l'index de la perdante et idx est celui de la gagnante // startin_m ne peut que être = à 0
            attribution_points(int_Tournoi_WTA, idx+1, idx, j); // reprendre startin_match avec nb joueuse ? av
        } 
    }
    else if(startin_match != 0)
    {
        for (int i = 0, j = startin_match, num_match; i <nb_joueuse-startin_match; i += 2, j++, num_match++) 
        {
            scanf("%s %s", winner, loser);
            unsigned int index_winner = recup_idx_joueuse(int_Tournoi_WTA, winner, startin_num_joueuse);
            unsigned int index_loser = recup_idx_joueuse(int_Tournoi_WTA, loser, startin_num_joueuse);

            int_Tournoi_WTA->dataTournois[num_tournois].data_matchs[j].idx_gagnante = index_winner;
            int_Tournoi_WTA->dataTournois[num_tournois].data_matchs[j].idx_perdante = index_loser;

            attribution_points(int_Tournoi_WTA, index_loser, index_winner, j);
            // printf("%d %d\n", index_winner, index_loser);
        } 
    }
}

/*
* Enregistre le nombre de tournois a creer
* int_Tournoi_WTA [in-out] type TournoiWTA
* pas de valeur de retour
*/
void definir_nombre_tournois(TournoiWTA* int_Tournoi_WTA) 
{
    scanf("%d", &(int_Tournoi_WTA->nb_tournois));
    assert(int_Tournoi_WTA->nb_tournois > 0 && int_Tournoi_WTA->nb_tournois <= max_tournois);
}

/*
* Enregistre le(s) tournoi(s)
* int_Tournois_WTA [in-out] type TournoiWTA
* pas de valeur de retour
*/
void enregistrement_tournoi(TournoiWTA *int_Tournoi_WTA)
{
    unsigned int num_tournois = int_Tournoi_WTA->num_tournois;
    unsigned int start_rang_joueuse = num_tournois*nb_joueuse; // le rang ou on commance l'enregistrement d'une joueuse

    // enregistrement du nom et de la date
    char nom[lgmot + 1], annee[5];
    scanf("%s %s", nom, annee);
    strcpy(int_Tournoi_WTA->dataTournois[num_tournois].nom_tournoi, nom);
    strcpy(int_Tournoi_WTA->dataTournois[num_tournois].date_tournoi, annee);

    recup_joueuse(int_Tournoi_WTA, 0, start_rang_joueuse,num_tournois, 0);
    recup_joueuse(int_Tournoi_WTA, finale_64, start_rang_joueuse, num_tournois, 64);
    recup_joueuse(int_Tournoi_WTA, finale_32, start_rang_joueuse, num_tournois, 96);
    recup_joueuse(int_Tournoi_WTA, finale_16, start_rang_joueuse, num_tournois, 112);
    recup_joueuse(int_Tournoi_WTA, finale_8, start_rang_joueuse, num_tournois, 120);
    recup_joueuse(int_Tournoi_WTA, finale_4, start_rang_joueuse, num_tournois, 124);
    recup_joueuse(int_Tournoi_WTA, finale_2, start_rang_joueuse, num_tournois, 126);

    int_Tournoi_WTA->num_tournois++;
}

/*
* affiche le ieme de final selon le numeros de match
* match [in] type int
* aucune valeur de retour
*/
void affiche_ieme_final(int match)
{
    if (match == 0)
        {
            printf("64emes de finale\n");
        }
        else if(match == finale_64)
        {
            printf("32emes de finale\n");
        }
        else if(match == finale_32)
        {
            printf("16emes de finale\n");
        }
        else if(match == finale_16)
        {
            printf("8emes de finale\n");
        }
        else if(match == finale_8)
        {
            printf("quarts de finale\n");
        }
        else if(match == finale_4)
        {
            printf("demi-finales\n");
        }
        else if(match == finale_2)
        {
            printf("finale\n");
        }
}

void somme_points_joueuse(TournoiWTA *int_Tournoi_WTA, TournoiWTA *single_player)
{
    unsigned int nombre_tournoi = int_Tournoi_WTA->nb_tournois;
    for(int i = 0; i<nb_joueuse*nombre_tournoi; i++)
    {
        for(int j = i+1; j<nb_joueuse*nombre_tournoi; j++)
        {
            if(strcmp(int_Tournoi_WTA->dataJoueuse[i].nom_joueuse, int_Tournoi_WTA->dataJoueuse[j].nom_joueuse) == 0)
            {
                int_Tournoi_WTA->dataJoueuse[i].points += int_Tournoi_WTA->dataJoueuse[j].points;
                single_player->dataJoueuse[i] = int_Tournoi_WTA->dataJoueuse[j];
            }
        }

        single_player->dataJoueuse[i] = int_Tournoi_WTA->dataJoueuse[i];
    }
}

// void assemblage_joueuses(TournoiWTA *int_Tournoi_WTA, TournoiWTA *single_player)
// {
//     unsigned int nombre_tournoi = int_Tournoi_WTA->nb_tournois;
//     for(int i = 0; i<nb_joueuse*nombre_tournoi; i++)
//     {
//         for(int j = i+1; j<nombre_tournoi*nb_joueuse; j++)
//         {
//             if()
//         }
//     }
// }

/*
* affiche les matchs d'un tournoi
* int_Tournoi_WTA [in-out] type TournoiWTA
* pas de valeur de retour
*/
void affichage_matchs_tournoi(TournoiWTA *int_Tournoi_WTA)
{   
    char nom[lgmot+1], annee[5];
    scanf("%s %s", nom, annee); // recuperation du nom et l'annee du tournois

    unsigned int num_tournois = recup_num_tournois(int_Tournoi_WTA, nom, annee);
    (num_tournois == -1) ? printf("Tournoi inconnue\n"): printf("%s %s\n", nom, annee);

    for(int m = 0; m<nb_matchs; m++)
    {
        affiche_ieme_final(m);

        Match num_match = int_Tournoi_WTA->dataTournois[num_tournois].data_matchs[m];
        printf("%s %s\n", int_Tournoi_WTA->dataJoueuse[num_match.idx_gagnante].nom_joueuse, 
               int_Tournoi_WTA->dataJoueuse[num_match.idx_perdante].nom_joueuse);
    }
}

/*
* affiche tout les match d'une joueuse
* int_Tournoi_WTA [in-out] type TournoiWTA
* aucune valeur de retour 
*/
void afficher_matchs_joueuse(TournoiWTA *int_Tournoi_WTA)
{
    char nom[lgmot+1], annee[5], affich_joueuse[lgmot+1];
    scanf("%s %s %s", nom, annee, affich_joueuse); // recuperation du nom, date et joueuse

    unsigned int num_tournois = recup_num_tournois(int_Tournoi_WTA, nom, annee);
    unsigned int start_rang_joueuse = num_tournois*nb_joueuse;
    unsigned int num_joueuse = recup_idx_joueuse(int_Tournoi_WTA, affich_joueuse, start_rang_joueuse); // recolte l'index de la joueuse
    if(num_tournois == -1) // verifis si le tournois existe
        printf("Tournoi inconnue\n");
    if(num_joueuse == -1) // verifis si la joueuse existe
        printf("Joueuse inconnue\n");

    for(int m = 0; m<nb_matchs; m++)
    {
        Match num_match = int_Tournoi_WTA->dataTournois[num_tournois].data_matchs[m];
        if(strcmp(int_Tournoi_WTA->dataJoueuse[num_match.idx_gagnante].nom_joueuse, affich_joueuse) == 0 || 
           strcmp(int_Tournoi_WTA->dataJoueuse[num_match.idx_perdante].nom_joueuse, affich_joueuse) == 0)
        {
            printf("%s %s\n", int_Tournoi_WTA->dataJoueuse[num_match.idx_gagnante].nom_joueuse, 
                   int_Tournoi_WTA->dataJoueuse[num_match.idx_perdante].nom_joueuse); 
        }
    }
}
/**
 * @brief Affiche les noms des joueuses avaec ses points
 * @param int_Tournoi_WTA [in] tyoe TournoisWTA
 */
void affichage_joueuses_tournoi(TournoiWTA *int_Tournoi_WTA)
{
    char nom[lgmot+1] = "New-York", annee[5] = "2021";
    scanf("%s %s", nom, annee); // recuperation du nom et l'annee du tournois

    unsigned int num_tournois = recup_num_tournois(int_Tournoi_WTA, nom, annee);
    (num_tournois == -1) ? printf("Tournoi inconnue\n"): printf("%s %s\n", nom, annee);
    unsigned int start_rang_joueuse = num_tournois*nb_joueuse;

    trie_joueuse(int_Tournoi_WTA, start_rang_joueuse);

    for (int i = 0, j=start_rang_joueuse; i < nb_joueuse; i++, j++)
    {
        printf("%s %d\n", int_Tournoi_WTA->dataJoueuse[j].nom_joueuse, int_Tournoi_WTA->dataJoueuse[j].points);
    }
}

void afficher_classement(TournoiWTA *int_Tournoi_WTA)
{ 
    unsigned int nombre_tournoi = int_Tournoi_WTA->nb_tournois;
    TournoiWTA *single_player;
    somme_points_joueuse(int_Tournoi_WTA, single_player);
    // assemblage_joueuses(int_Tournoi_WTA);

    for(int i = 0; i<nb_joueuse*nombre_tournoi; i++)
    {
        printf("%s %d\n", single_player->dataJoueuse[i].nom_joueuse, single_player->dataJoueuse->points);
    }
}
/*-------------------------------------------main---------------------------------------------*/

/*
* Il les commande du fichier et execute les commandes
* le but est d'afficher les tournois et leurs match
*/
int main(void)
{   
    char mot[lgmot + 1];
    
    //initialisation du tournois
    TournoiWTA int_Tournoi_WTA;
    int_Tournoi_WTA.nb_tournois = 0;
    int_Tournoi_WTA.num_tournois = 0;

    while (1)
    {
        scanf("%s", mot);

        if (strcmp(mot, "definir_nombre_tournois") == 0)
        {
            definir_nombre_tournois(&int_Tournoi_WTA);
        }
        else if (strcmp(mot, "enregistrement_tournoi") == 0)
        {
            enregistrement_tournoi(&int_Tournoi_WTA);
        }
        else if (strcmp(mot, "affichage_matchs_tournoi") == 0)
        {
            affichage_matchs_tournoi(&int_Tournoi_WTA);
        }
        else if (strcmp(mot, "afficher_matchs_joueuse") == 0)
        {
            afficher_matchs_joueuse(&int_Tournoi_WTA);
        }
        else if (strcmp(mot, "affichage_joueuses_tournoi") == 0)
        {
            affichage_joueuses_tournoi(&int_Tournoi_WTA);
        }
        else if (strcmp(mot, "afficher_classement") == 0)
        {
            afficher_classement(&int_Tournoi_WTA);
        }
        else if (strcmp(mot, "exit") == 0)
        {
            exit(0);
        }
        else
            printf("commande inconnue\n");
    }

    system("pause");
    return 0;
}