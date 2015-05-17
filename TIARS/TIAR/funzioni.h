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

/// Definisco una struttura che contenga tutti i dati di gioco utili
typedef struct
{
    int punti;
    int statogemme[RIGHE][COLONNE];
    int colpidipistola;
    int tempo;

// Creo dei puntatori a puntatori di puntatori per non utilizzare variabili globali
    GtkWidget*** matriceimmagini;
    GtkWidget*** matriceeventi;
    GtkWidget** label_tempo;
    GtkWidget** label_punti;
    GtkWidget** interfaccia;
} dati_di_gioco;

/// Definisco una struttura che contenga le coordinate dei click effettuati
typedef struct
{
    int n_click;
    int r[2];
    int c[2];
    dati_di_gioco* games;
} clickscambio;
typedef struct

{
    int r;
    int c;
    dati_di_gioco* games;
    clickscambio* scambio;
} clickcord;

 /// Se il tempo trascorso è superiore a 0 secondi allora stampo di volta in volta il tempo rimanente
int timer(dati_di_gioco *datas)
{
    char temp[50];
    if(datas->tempo > 0)
    {
        sprintf(temp, "Tempo: \n %d", datas->tempo);
        gtk_label_set_text(GTK_LABEL(datas->label_tempo),temp);
        datas->tempo--;
        return 1;
    }
    else // Altrimenti do il messaggio di Game Over
    {
        sprintf(temp, "Tempo: 0", datas->tempo);
        gtk_label_set_text(GTK_LABEL(datas->label_tempo),temp);

// 'Stampiamo' il risultato della partita a fine gioco
            if(datas->punti > 0) sprintf(temp, "%d punti, Really?... Go home!", datas->punti);
            if(datas->punti > 3000) sprintf(temp, "%d punti, Bronze league", datas->punti);
            if(datas->punti > 7000) sprintf(temp, "%d punti, Silver league!", datas->punti);
            if(datas->punti > 16000) sprintf(temp, "%d punti, Gold league!", datas->punti);
            if(datas->punti > 36000) sprintf(temp, "%d punti, Platinum league!", datas->punti);
            if(datas->punti > 77000) sprintf(temp, "%d punti, Diamond league!", datas->punti);
            if(datas->punti > 150000) sprintf(temp, "%d punti, Master league!", datas->punti);
            if(datas->punti > 365000) sprintf(temp, "%d punti, Grandmaster league!", datas->punti);
            GtkWidget* dialog_gameover = gtk_message_dialog_new(GTK_WINDOW(datas->interfaccia),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,"Game over!");
            gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(dialog_gameover), temp);
            gtk_dialog_run(GTK_DIALOG(dialog_gameover));
            gtk_widget_destroy(dialog_gameover);

 printf("\nGame over!");
        return 0; // Ritornando 0 alla funzione timer invocata del main questa non si riavvierà più
    }

}
/*
/// Stampa matrice in consolle
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

    printf(" -----------------------\n");
}*/

/// Genero la matrice di gemme e verifico che non ci sia alcuna tripletta, se per caso trovo una tripletta cambiamo una gemma della tripletta con una diversa.
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

//printf("---------Matrice--------\n");
}

/// Eliminiamo un valore. Usiamo questa funzione per distruggere le gemme che hanno formato una tripletta e così via
void elimina(dati_di_gioco* data,int riga_e,int colonna_e)
{
    char temp[10];
    for(;riga_e>=0;riga_e--){
    data->statogemme[riga_e][colonna_e] = data->statogemme[riga_e-1][colonna_e];
    }

        sprintf(temp, "Punteggio:\n %d", data->punti); /* 'Stampiamo' il punteggio nella box tempo */
        gtk_label_set_text(GTK_LABEL(data->label_punti),temp);
        //printf("Elimino gemma \n");
    data->statogemme[0][colonna_e] = rand()%GEMME;
    data->punti = data->punti + 5; /* Ad ogni esplosione aggiungo 5 punti al punteggio */
}

/// Click event, tramite l'utilizzo del richiamo della funzione tramite un eventbox si può sapere con qualche tasto un eventbox (gemma) è stata cliccata, quindi, decidere se utilizzare il click standard o i colpi di pistola.
void cliccato(GtkWidget * widget,GdkEventButton *event,clickcord *coord){
    //printf("Click: %d %d (%d)\n",coord->r,coord->c,event->button);
    if(coord->games->tempo > 0){
    switch(event->button) //controlla con quale tasto è stato cliccato
    {
    case 1: //tasto sx (modalità gioco)
        switch(coord->scambio->n_click) //controlla quante gemme sono state sequenzialmente selezionate
        {
        case 0:
            coord->scambio->n_click++;
            coord->scambio->r[0] = coord->r;
            coord->scambio->c[0] = coord->c;
            break;
        case 1:
            coord->scambio->n_click = 0;
            coord->scambio->r[1] = coord->r;
            coord->scambio->c[1] = coord->c;

            scambia(coord->scambio); // Eseguo lo scambio

            if(verifica(coord,0) == 0) // Verifico se è avvenuta un esplosione
            {
               // printf("Nessuna esplosione, ri-eseguo lo scambio \n");
                scambia(coord->scambio); // se non è avvenuta ri-eseguo lo scambio
            }
            else // altrimenti ricontrollo tutta la matrice
            {
                while(verifica(coord,1)){
                coord->games->punti = coord->games->punti *3 / 2; // punti bonus
               //  printf("Esplosione bonus \n");
               }
            }
            }
            break;

        break;

    case 3: //Colpo di pistola, elimino le 8 gemme che stanno intorno ad una certa gemma selezionata, controllando che queste non escano dal range dell matrice
        if(coord->games->colpidipistola >= 0){

        if(coord->r-1 >= 0 && coord->r-1 < RIGHE && coord->c-1 >=0 && coord->c-1 < COLONNE){ elimina(coord->games,coord->r-1,coord->c-1); }
        if(coord->r-1 >= 0 && coord->r-1 < RIGHE && coord->c >=0 && coord->c < COLONNE){ elimina(coord->games,coord->r-1,coord->c); }
        if(coord->r-1 >= 0 && coord->r-1 < RIGHE && coord->c+1 >=0 && coord->c+1 < COLONNE){ elimina(coord->games,coord->r-1,coord->c+1); }
        if(coord->r >= 0 && coord->r < RIGHE && coord->c-1 >=0 && coord->c-1 < COLONNE){ elimina(coord->games,coord->r,coord->c-1); }
        if(coord->r >= 0 && coord->r < RIGHE && coord->c >=0 && coord->c < COLONNE){ elimina(coord->games,coord->r,coord->c); }
        if(coord->r >= 0 && coord->r < RIGHE && coord->c+1 >=0 && coord->c+1 < COLONNE){ elimina(coord->games,coord->r,coord->c+1); }
        if(coord->r+1 >= 0 && coord->r+1 < RIGHE && coord->c-1 >=0 && coord->c-1 < COLONNE){ elimina(coord->games,coord->r+1,coord->c-1); }
        if(coord->r+1 >= 0 && coord->r+1 < RIGHE && coord->c >=0 && coord->c < COLONNE){ elimina(coord->games,coord->r+1,coord->c); }
        if(coord->r+1 >= 0 && coord->r+1 < RIGHE && coord->c+1 >=0 && coord->c+1 < COLONNE){ elimina(coord->games,coord->r+1,coord->c+1); }

        coord->games->colpidipistola--; // Scaliamo i colpi di pistola
        }
        else {
            /* Se i colpi di pistola sono finiti creiamo una window per stampare: Colpi di pistola esauriti */
            GtkWidget* dialog_shotsover = gtk_message_dialog_new(GTK_WINDOW(coord->games->interfaccia),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,"Colpi di pistola esauriti");
            gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(dialog_shotsover), "Colpi di pistola esauriti");
            gtk_dialog_run(GTK_DIALOG(dialog_shotsover));
            gtk_widget_destroy(dialog_shotsover);
            // printf("Colpi di pistola esauriti \n");
        }
        break;
    }}
aggiorna_griglia(coord->games);
}

/// Funzione che verifica se uno scambio è fattibile attraverso la formula della distanza tra due punti, è possibile solo se la distanza è 1
void scambia(clickscambio* click)
{
            //printf("Verifico se lo scambio è possibile \n");
            int dist = sqrt(pow(click->r[0]-click->r[1],2)+pow(click->c[0]-click->c[1],2));

            if((click->r[0] == click->r[1] || click->c[0] == click->c[1]) && dist == 1)
            {

            int temp = click->games->statogemme[click->r[0]][click->c[0]];
            click->games->statogemme[click->r[0]][click->c[0]] = click->games->statogemme[click->r[1]][click->c[1]];
            click->games->statogemme[click->r[1]][click->c[1]] = temp;
            aggiorna_griglia(click->games);
}
}

/// Aggiornamento matrice, funzione che serve per aggiornare ogni volta le immagini delle gemme sulla griglia di gioco
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
 /// Salvataggio dei dati. Funzione che serve per salvare i dati utili per caricare una partita: punti, tempo, colpi di pistola. Ho usato un vettore intero temp[3] per contenere i 3 dati di tipi intero.
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

/// Caricamento partita. Funzione che carica i dati salvati in precedenza.
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
        aggiorna_griglia(dati); // Logicamente aggiorniamo la griglia con i dati caricati
}

int verifica(clickcord *contenuto,int tutta)
{
    int punti = 0,ind,su_or = 0,giu_or = 0,su_vr = 0,giu_vr = 0,r,c;

    //printf("-----> VERIFICA \n");


 /* Decido se controllare tutta la matrice per triplette o solamente i punti in cui son stati effettuati gli scambi.
    Le due funzioni svolgono pressochè lo stesso lavoro, a partire dalla stessa cella controlliamo quante in
    orizzontale e verticale se il numero di celle uguali e consecutive è >= 3 allora invochiamo la funzione elimina per ognuna di queste */
if(tutta == 0){
    for(ind = 1; ind>=0; ind--)
    {
        c = contenuto->scambio->c[ind];
        r = contenuto->scambio->r[ind];

        su_or = c;
        while((su_or+1) < COLONNE && contenuto->games->statogemme[r][su_or] == contenuto->games->statogemme[r][su_or+1])
        {
            su_or++;
        }
        giu_or = c;
        while((giu_or-1) >= 0 && contenuto->games->statogemme[r][giu_or] == contenuto->games->statogemme[r][giu_or-1])
        {
            giu_or--;
        }

        su_vr = r;
        while((su_vr+1) < RIGHE && contenuto->games->statogemme[su_vr][c] == contenuto->games->statogemme[su_vr+1][c])
        {
            su_vr++;
        }


        giu_vr = r;
        while((giu_vr-1) >= 0 && contenuto->games->statogemme[giu_vr][c] == contenuto->games->statogemme[giu_vr-1][c])
        {
            giu_vr--;
        }

        if(su_or-giu_or >= 2 && su_vr-giu_vr >= 2)
        {

            for(; su_or-giu_or>=0; giu_or++)
            {
                elimina(contenuto->games,r,giu_or);
                punti++;
            }
            for(; su_vr-giu_vr>0; giu_vr++)
            {
                elimina(contenuto->games,su_vr,c);
                punti++;
            }

        }
        else if (su_or-giu_or >= 2)
        {
            for(; su_or-giu_or>=0; giu_or++)
            {
                elimina(contenuto->games,r,giu_or);
                punti++;
            }
        }
        else if (su_vr-giu_vr >= 2)
        {
            for(; su_vr-giu_vr>=0; giu_vr++)
            {
                elimina(contenuto->games,su_vr,c);
                punti++;
            }
        }
    }
}else
{
    for(r=0;r<RIGHE;r++){
            for(c=0;c<COLONNE;c++){
        su_or = c;
        while((su_or+1) < COLONNE && contenuto->games->statogemme[r][su_or] == contenuto->games->statogemme[r][su_or+1])
        {
            su_or++;
        }
        giu_or = c;
        while((giu_or-1) >= 0 && contenuto->games->statogemme[r][giu_or] == contenuto->games->statogemme[r][giu_or-1])
        {
            giu_or--;
        }

        su_vr = r;
        while((su_vr+1) < RIGHE && contenuto->games->statogemme[su_vr][c] == contenuto->games->statogemme[su_vr+1][c])
        {
            su_vr++;
        }


        giu_vr = r;
        while((giu_vr-1) >= 0 && contenuto->games->statogemme[giu_vr][c] == contenuto->games->statogemme[giu_vr-1][c])
        {
            giu_vr--;
        }

        if(su_or-giu_or >= 2 && su_vr-giu_vr >= 2)
        {
            contenuto->games->tempo+=2;

            for(; su_or-giu_or>=0; giu_or++)
            {
                elimina(contenuto->games,r,giu_or);
                punti++;
            }
            for(; su_vr-giu_vr>0; giu_vr++)
            {
                elimina(contenuto->games,su_vr,c);
                punti++;
            }

        }
        else if (su_or-giu_or >= 2)
        {
            contenuto->games->tempo+=2;
            for(; su_or-giu_or>=0; giu_or++)
            {
                elimina(contenuto->games,r,giu_or);
                punti++;
            }
        }
        else if (su_vr-giu_vr >= 2)
        {
            contenuto->games->tempo+=2;
            for(; su_vr-giu_vr>=0; giu_vr++)
            {
                elimina(contenuto->games,su_vr,c);
                punti++;
            }
        }
    }}
}

    if (punti > 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
#endif
