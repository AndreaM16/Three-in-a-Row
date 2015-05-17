/**  Three in a row
Andrea Medda - 47989 - 27/08/2013
**/

#include "funzioni.h"

int main (int argc, char *argv[])
{
    /// Variabili temporanee
    int r,c;
    char temp[30];

    /// Inizializzazione della funzione random e della libreria gtk
        srand(time(NULL));
        gtk_init (&argc, &argv);

    /// Struttura che contiene i dati di gioco (punti, tempo e coordinate)
        clickcord cooord[RIGHE][COLONNE];
        dati_di_gioco datas;
        datas.punti = 0;
        clickscambio clicks;
        clicks.games = &datas;
        clicks.n_click = 0;
        datas.colpidipistola = PISTOLATE;
        datas.tempo = TEMPO;

    /// Generazione della main window e stampa
        GtkWidget *finestra = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title(GTK_WINDOW(finestra),"TIAR");
        gtk_window_position(GTK_WINDOW(finestra),GTK_WIN_POS_CENTER);
        gtk_window_set_default_size(GTK_WINDOW(finestra),150,150);
        gtk_window_set_resizable(GTK_WINDOW(finestra),0);
        g_signal_connect(finestra, "destroy", gtk_main_quit, NULL);
        datas.interfaccia = finestra;

        generamatrice(&datas);
        //stampamatrice(&datas);

    /// Riempimento verticale
        GtkWidget *v_box = gtk_vbox_new(FALSE, 0);
        gtk_container_add (GTK_CONTAINER (finestra) , v_box);


    /// Generazione menu' bar e suo inserimento nella vbox principale
        GtkWidget* menu_bar = gtk_menu_bar_new();
        gtk_box_pack_start(GTK_BOX(v_box),menu_bar, FALSE, FALSE,0);

    /// Generazione menu' file
        GtkWidget* menu_file = gtk_menu_item_new_with_label("Menu'");
        gtk_menu_bar_append(GTK_MENU_BAR(menu_bar), menu_file);

    /// Generazione sottomenu' con link al menu' file
        GtkWidget* menu_sub = gtk_menu_new();
        gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu_file), menu_sub);

        GtkWidget* menu_salva = gtk_menu_item_new_with_label ("Salva Partita");
        GtkWidget* menu_carica = gtk_menu_item_new_with_label ("Carica Partita");
        GtkWidget* menu_esci = gtk_menu_item_new_with_label ("Esci");

    /// Collegamento funzioni agli item del menu
        g_signal_connect(G_OBJECT(menu_esci), "activate", G_CALLBACK(gtk_main_quit),NULL);
        g_signal_connect(G_OBJECT(menu_salva), "activate", G_CALLBACK(salva),&datas);
        g_signal_connect(G_OBJECT(menu_carica), "activate", G_CALLBACK(carica),&datas);

    /// Opzioni del menu'
        gtk_menu_append(GTK_MENU(menu_sub), menu_salva);
        gtk_menu_append(GTK_MENU(menu_sub), menu_carica);
        gtk_menu_append(GTK_MENU(menu_sub), menu_esci);

    /// Hbox per ogni row di gemme

        GtkWidget **hboxgriglia;
        hboxgriglia = (GtkWidget**) malloc (RIGHE*sizeof(GtkWidget*));

    /// Un oggetto per ogni image

        GtkWidget*** matriceimmagini;
        matriceimmagini = (GtkWidget**) malloc(RIGHE*sizeof(GtkWidget**));
        for (r=0; r<RIGHE; r++)
        {
            matriceimmagini[r] = (int**) malloc (COLONNE*sizeof(GtkWidget*));
        }

        datas.matriceimmagini = matriceimmagini;

    /// Ogni box è collegato ad un immagine, la funzione clickato prende in ingresso un widget evento che identifica l'immagine tramite le coordinate presenti nella struttura coord
        GtkWidget*** eventi;
        eventi = (GtkWidget**) malloc(RIGHE*sizeof(GtkWidget**));
        for (r=0; r<RIGHE; r++)
        {
            eventi[r] = (GtkWidget**) malloc (COLONNE*sizeof(GtkWidget*));
        }
        datas.matriceeventi = eventi;

    //È il box verticale in cui vengono inseriti i label per i punti e per il tempo, è stato fatto in questo modo per avere centrate le due scritte: tempo e punti tramite due vbox inserite in una hbox
        GtkWidget* hboxhead;
        hboxhead = gtk_hbox_new(FALSE, 0);
        gtk_container_add (GTK_CONTAINER (v_box) , hboxhead);
        GtkWidget* vboxpunti;
        vboxpunti = gtk_vbox_new(FALSE, 0);
        gtk_container_add (GTK_CONTAINER (hboxhead) , vboxpunti);
        GtkWidget* vboxtempo;
        vboxtempo = gtk_vbox_new(FALSE, 0);
        gtk_container_add (GTK_CONTAINER (hboxhead) , vboxtempo);
        GtkWidget *testo_punti = gtk_label_new("Punteggio:\n 000");
        gtk_box_pack_start(GTK_BOX(vboxpunti), testo_punti, TRUE, TRUE, 1);
        datas.label_punti = testo_punti;
        GtkWidget *testo_tempo = gtk_label_new("Tempo:\n 000");
        gtk_box_pack_start(GTK_BOX(vboxtempo), testo_tempo, TRUE, TRUE, 1);
        datas.label_tempo = testo_tempo;

    /// Timer di gioco
    g_timeout_add(1000,(GSourceFunc)timer,&datas);

    /// Generazione griglia di partenza, sprintf 'stampa' un percorso su una variabile, quindi collega l'immagine al widget
        for (r=0; r<RIGHE; r++)
        {
            hboxgriglia[r] = gtk_hbox_new(FALSE, 0);
            gtk_container_add (GTK_CONTAINER (v_box) , hboxgriglia[r]);

            for(c=0; c<COLONNE; c++)
            {
                    sprintf(temp, "sim/%d.png", datas.statogemme[r][c]);
                    eventi[r][c] = gtk_event_box_new();
                    matriceimmagini[r][c] = gtk_image_new_from_file(temp);
                    gtk_container_add (GTK_CONTAINER (eventi[r][c]),matriceimmagini[r][c]);
                    cooord[r][c].c = c;
                    cooord[r][c].r = r;
                    cooord[r][c].games = &datas;
                    cooord[r][c].scambio = &clicks;
                    g_signal_connect(G_OBJECT(eventi[r][c]),"button_press_event",G_CALLBACK(cliccato),&cooord[r][c]);
                    gtk_box_pack_start(GTK_BOX(hboxgriglia[r]),eventi[r][c],FALSE,FALSE,0);
            }

        }

    /// Visualizzazione interfaccia
        gtk_widget_show_all (finestra);
        gtk_main ();
  return 0;
}

