#ifndef FUNZIONI_H_INCLUDED
#define FUNZIONI_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include <time.h>
#include <math.h>

#define RIGHE 8
#define COLONNE 8
#define GEMME 7
#define PISTOLATE 3
#define TEMPO 60

typedef struct
{
    int punti;
    int statogemme[RIGHE][COLONNE];
    int colpidipistola;
    int tempo;
    GtkWidget*** matriceimmagini;
    GtkWidget*** matriceeventi;
    GtkWidget** label_tempo;
    GtkWidget** label_punti;
    GtkWidget** interfaccia;
} dati_di_gioco;

typedef struct
{
    int r;
    int c;
    dati_di_gioco* games;
} clickcord;

/*/// Stampa matrice in consolle
void stampamatrice(dati_di_gioco* data)
{
    int r,c;
    for (r=0; r<RIGHE; r++)
    {
        for(c=0; c<COLONNE; c++)
        {
            printf(" %d ",data->statogemme[r][c]);
        }
        printf("\n");
    }

    printf("-------------\n");
} */

/// Generazione matrice
void generamatrice(dati_di_gioco* data)
{
    int r,c;
    for (r=0; r<RIGHE; r++)
    {
        for(c=0; c<COLONNE; c++)
        {
            data->statogemme[r][c] = rand()%GEMME;
            while((data->statogemme[r][c] == data->statogemme[r-1][c] && data->statogemme[r][c] == data->statogemme[r-2][c]) || (data->statogemme[r][c] == data->statogemme[r][c-1] && data->statogemme[r][c] == data->statogemme[r][c-2])){
                data->statogemme[r][c] = rand()%GEMME;
            }
        }
    }

   // printf("--- MATRICE GENERATA\n");
}

/// Click event
void cliccato(GtkWidget * widget,GdkEventButton *event,clickcord *coord){
  //  printf("CLICK: %d %d (%d)\n",coord->r,coord->c,event->button);

    coord->games->statogemme[coord->r][coord->c]++;
    if(coord->games->statogemme[coord->r][coord->c] == GEMME) coord->games->statogemme[coord->r][coord->c] = 0;

aggiorna_griglia(coord->games);
}

///Aggiornamento matrice
void aggiorna_griglia(dati_di_gioco *dati){
int r,c;
char temp[20];
    for (r=0; r<RIGHE; r++)
    {

        for(c=0; c<COLONNE; c++)
        {
            sprintf(temp, "sim/%d.png", dati->statogemme[r][c]);
            gtk_image_set_from_file(dati->matriceimmagini[r][c], temp);
        }
    }
}

 /// Salva partita
void salva(GtkWidget * widget,dati_di_gioco *dati)
{
        int r;
        int temp[3];
            temp[0] = dati->punti;
            temp[1] = dati->tempo;
            temp[2] = dati->colpidipistola;
        FILE* fp = NULL;
        fp = fopen("partita.sav","w+b");
        fwrite(temp,sizeof(int),3,fp);
        for (r=0; r<RIGHE; r++)
        {
            fwrite(dati->statogemme[r],sizeof(int),COLONNE,fp);
        }
        fclose(fp);
}

/// Carica partita, si può modificare anche una partita già giocata in precedenza, salvata e messa sulla cartella dell'editor
void carica(GtkWidget * widget,dati_di_gioco *dati)
{
        int r;
        int temp[3];
        char tempt[10];
        FILE* fp = NULL;
        fp = fopen("partita.sav","r+b");
        fread(temp,sizeof(int),3,fp);
        for (r=0; r<RIGHE; r++)
        {
            fread(dati->statogemme[r],sizeof(int),COLONNE,fp);
        }
        fclose(fp);

        dati->punti = temp[0];
        dati->tempo = temp[1];
        dati->colpidipistola = temp[2];

        sprintf(tempt, "Punteggio:\n %d", dati->punti);
        gtk_label_set_text(GTK_LABEL(dati->label_punti),tempt);

        sprintf(tempt, "Tempo: \n %d", dati->tempo);
        gtk_label_set_text(GTK_LABEL(dati->label_tempo),tempt);
        aggiorna_griglia(dati);
}
 /// Esci
void esci(GtkWidget * widget,dati_di_gioco *dati)
{
    salva(NULL,dati);
    gtk_main_quit();
}
#endif // FUNZIONI_H_INCLUDED
