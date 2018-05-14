#include <iostream>
#include <cmath>
#include <ctime>
#include <fstream>

using namespace std;

struct Dane {
    unsigned short miasto;
    unsigned int polaczenie;
    unsigned char zapytanie;

    struct G_Lista_polaczenie *n_polaczenie;
    struct T_zapytanie *n_zapytanie;
}; //dane zadania
struct T_zapytanie {
    unsigned short poczatek, koniec, max_minut;
};//Tablica zapytań

struct G_Lista_krawedzi_przyleglych {
    unsigned short czas, cel;
    bool czy_dostepny;

    struct G_Lista_polaczenie *budowa_polaczenia, *ostatnia_przebudowa;
    struct G_Lista_krawedzi_przyleglych *next;
}; //Lista krawędzie przyległych grafu
struct G_Lista_polaczenie {
    char data[11], rodzaj;
    unsigned short poczatek, koniec, dystans, czas;

    struct G_Lista_krawedzi_przyleglych *krawedz_do_konca, *krawedz_do_poczatku;
    struct G_Lista_polaczenie *prev;
};//Lista budów i przebudów trasy

class Graf {
private:
    struct K_Wezel {
        unsigned short Numer_Wierzcholka, Odleglosc;
    };// struktura wezla w kopcu
    struct Kopiec {
        unsigned short rozmiar, *pos;
        struct K_Wezel **Stacje_Docelowe;
    }; // struktura kopca
    void Kopiec_bin(struct Kopiec *Kopiec, unsigned short indeks) {
        unsigned short najmniejszy = indeks, lewy = 2 * indeks + 1, prawy = lewy + 1;

        if (lewy < Kopiec->rozmiar &&
            Kopiec->Stacje_Docelowe[lewy]->Odleglosc < Kopiec->Stacje_Docelowe[najmniejszy]->Odleglosc)
            najmniejszy = lewy;

        if (prawy < Kopiec->rozmiar &&
            Kopiec->Stacje_Docelowe[prawy]->Odleglosc < Kopiec->Stacje_Docelowe[najmniejszy]->Odleglosc)
            najmniejszy = prawy;

        if (najmniejszy != indeks) {
            struct K_Wezel *smallestNode = Kopiec->Stacje_Docelowe[najmniejszy];
            struct K_Wezel *idxNode = Kopiec->Stacje_Docelowe[indeks];

            Kopiec->pos[smallestNode->Numer_Wierzcholka] = indeks;
            Kopiec->pos[idxNode->Numer_Wierzcholka] = najmniejszy;

            Zamien_Wezel(&Kopiec->Stacje_Docelowe[najmniejszy], &Kopiec->Stacje_Docelowe[indeks]);

            Kopiec_bin(Kopiec, najmniejszy);
        }
    }//struktura kopca binarnego

    struct K_Wezel *Nowy_Wezel(unsigned short Nr_W, unsigned short Odl) {
        struct K_Wezel *tmp = new K_Wezel;
        tmp->Numer_Wierzcholka = Nr_W;
        tmp->Odleglosc = Odl;

        return tmp;
    } //Tworzenie wezla

    void Zamien_Wezel(struct K_Wezel **a, struct K_Wezel **b) {
        struct K_Wezel *tmp = *a;
        *a = *b;
        *b = tmp;
    } // zmaiana miejscami wezlow
    void Zmniejsz_Klucz(struct Kopiec *Kopiec, unsigned short Nr_W, unsigned short Odl) {
        unsigned short i = Kopiec->pos[Nr_W];

        Kopiec->Stacje_Docelowe[i]->Odleglosc = Odl;

        while (i && Kopiec->Stacje_Docelowe[i]->Odleglosc < Kopiec->Stacje_Docelowe[(i - 1) / 2]->Odleglosc) {
            Kopiec->pos[Kopiec->Stacje_Docelowe[i]->Numer_Wierzcholka] = (i - 1) / 2;
            Kopiec->pos[Kopiec->Stacje_Docelowe[(i - 1) / 2]->Numer_Wierzcholka] = i;
            Zamien_Wezel(&Kopiec->Stacje_Docelowe[i], &Kopiec->Stacje_Docelowe[(i - 1) / 2]);

            i = (i - 1) / 2;
        }
    }

    struct G_Lista_sasiedztwa {
        struct G_Lista_krawedzi_przyleglych *head;
    }; //lista sadziadow w grafie
    struct G_Graf {
        unsigned short V;
        struct G_Lista_sasiedztwa *array;
    };// struktura grafu
    static struct G_Lista_krawedzi_przyleglych *Nowa_Lista_Wezlow(unsigned short Cel, unsigned short Czas) {
        struct G_Lista_krawedzi_przyleglych *Nowy_Wezel = new G_Lista_krawedzi_przyleglych;

        Nowy_Wezel->cel = Cel;
        Nowy_Wezel->czas = Czas;
        Nowy_Wezel->next = nullptr;
        Nowy_Wezel->czy_dostepny = false;

        return Nowy_Wezel;
    } //tworzenie nowej listy wezlow

public:
    struct G_Graf *graf;

    Graf(unsigned short Rozmiar) {
        graf = new G_Graf;

        graf->V = Rozmiar;
        graf->array = (G_Lista_sasiedztwa *) calloc(sizeof(*graf->array), Rozmiar);

    } // tworzenie grafu

    struct G_Lista_krawedzi_przyleglych **Graf_Dodaj_Karawedz(unsigned short Stacja, unsigned short Cel, unsigned short Czas) {
        static struct G_Lista_krawedzi_przyleglych *Stacja_Cel[2];

        Stacja_Cel[0] = Nowa_Lista_Wezlow(Cel, Czas);
        Stacja_Cel[0]->next = graf->array[Stacja].head;
        graf->array[Stacja].head = Stacja_Cel[0];

        Stacja_Cel[1] = Nowa_Lista_Wezlow(Stacja, Czas);
        Stacja_Cel[1]->next = graf->array[Cel].head;
        graf->array[Cel].head = Stacja_Cel[1];

        return Stacja_Cel;
    } //dodanie krawedzi

    unsigned short dijkstra(unsigned short Stacja, unsigned short Cel) {
        unsigned short *Odleglosc = new unsigned short[graf->V];

        struct Kopiec *Min_Kopiec = new Kopiec;
        Min_Kopiec->pos = new unsigned short[graf->V];
        Min_Kopiec->rozmiar = 0;
        Min_Kopiec->Stacje_Docelowe = new K_Wezel *[graf->V];

        for (unsigned short Nr_wierzcholka = 0; Nr_wierzcholka < graf->V; ++Nr_wierzcholka) {
            Odleglosc[Nr_wierzcholka] = USHRT_MAX;
            Min_Kopiec->Stacje_Docelowe[Nr_wierzcholka] = Nowy_Wezel(Nr_wierzcholka, Odleglosc[Nr_wierzcholka]);
            Min_Kopiec->pos[Nr_wierzcholka] = Nr_wierzcholka;
        }

        Min_Kopiec->rozmiar = graf->V;
        Min_Kopiec->Stacje_Docelowe[Stacja] = Nowy_Wezel(Stacja, Odleglosc[Stacja]);
        Min_Kopiec->pos[Stacja] = Stacja;
        Odleglosc[Stacja] = 0;
        Zmniejsz_Klucz(Min_Kopiec, Stacja, Odleglosc[Stacja]);

        struct K_Wezel *Min_L_P;
        struct G_Lista_krawedzi_przyleglych *P_H;
        unsigned short u, v;
        while (Min_Kopiec->rozmiar) // while not empty
        {
            //extract minimum
            if (!Min_Kopiec->rozmiar)
                Min_L_P = nullptr;
            else {
                struct K_Wezel *root = Min_Kopiec->Stacje_Docelowe[0], *lastNode = Min_Kopiec->Stacje_Docelowe[Min_Kopiec->rozmiar - 1];
                Min_Kopiec->Stacje_Docelowe[0] = lastNode;
                Min_Kopiec->pos[root->Numer_Wierzcholka] = Min_Kopiec->rozmiar - 1;
                Min_Kopiec->pos[lastNode->Numer_Wierzcholka] = 0;
                --Min_Kopiec->rozmiar;
                Kopiec_bin(Min_Kopiec, 0);
                Min_L_P = root;
            }

            u = Min_L_P->Numer_Wierzcholka;

            P_H = graf->array[u].head;
            while (P_H != nullptr) {
                v = P_H->cel;

                if ((Min_Kopiec->pos[v] < Min_Kopiec->rozmiar) && Odleglosc[u] != USHRT_MAX && P_H->czy_dostepny &&
                    P_H->czas + Odleglosc[u] + 5 < Odleglosc[v]) {
                    Odleglosc[v] = P_H->czas + Odleglosc[u] + 5;

                    Zmniejsz_Klucz(Min_Kopiec, v, Odleglosc[v]);
                }
                P_H = P_H->next;
            }
        }

        return Odleglosc[Cel] - 5;
    } // szukanie najszybszej sciezki
};

int main() {
    FILE *in = fopen("in3.txt", "rt");
    if (!in) {
        cout << "Plik nie istnieje!";
        return 0;
    }

    struct Dane dane;
    fscanf(in, " %hu %u %hhu ", &dane.miasto, &dane.polaczenie, &dane.zapytanie);

    struct Graf *graf = new Graf(dane.miasto + 1);
    dane.n_polaczenie = new G_Lista_polaczenie[dane.polaczenie];
    dane.n_zapytanie = new T_zapytanie[dane.zapytanie];

    struct G_Lista_krawedzi_przyleglych **koniec_start;
    struct G_Lista_polaczenie *obecny = dane.n_polaczenie, *budowa_polaczenia;
    unsigned short predkosc;
    for (unsigned int i = 0; i < dane.polaczenie; ++i, ++obecny) {
        while (!fgets(obecny->data, 11, in));
        fscanf(in, " %c %hd %hd %hu", &obecny->rodzaj, &obecny->poczatek, &obecny->koniec, &predkosc);
        if (obecny->rodzaj == 'b') {
            fscanf(in, " %hu ", &obecny->dystans);

            obecny->prev = nullptr;

            obecny->czas = (obecny->dystans * 60) / predkosc;
            koniec_start = graf->Graf_Dodaj_Karawedz(obecny->poczatek, obecny->koniec, obecny->czas);

            obecny->krawedz_do_konca = koniec_start[0];
            obecny->krawedz_do_poczatku = koniec_start[1];

            koniec_start[0]->budowa_polaczenia = koniec_start[1]->budowa_polaczenia = obecny;
            koniec_start[0]->ostatnia_przebudowa = koniec_start[1]->ostatnia_przebudowa = obecny;

        } else {
            struct G_Lista_krawedzi_przyleglych *krawedz_do_konca = graf->graf->array[obecny->poczatek].head;
            while (krawedz_do_konca && krawedz_do_konca->cel != obecny->koniec) {
                krawedz_do_konca = krawedz_do_konca->next;
            }

            budowa_polaczenia = krawedz_do_konca->budowa_polaczenia;

            obecny->krawedz_do_konca = budowa_polaczenia->krawedz_do_konca;
            obecny->krawedz_do_poczatku = budowa_polaczenia->krawedz_do_poczatku;

            obecny->dystans = budowa_polaczenia->dystans;

            obecny->czas = (obecny->dystans * 60) / predkosc;

            obecny->prev = obecny->krawedz_do_konca->ostatnia_przebudowa;

            krawedz_do_konca->ostatnia_przebudowa = obecny;
            obecny->krawedz_do_poczatku->ostatnia_przebudowa = obecny;

            fgetc(in);
        }
    }

    for (unsigned char i = 0; i < dane.zapytanie; ++i)
        fscanf(in, " %hu %hu %hu", &(dane.n_zapytanie[i].poczatek), &(dane.n_zapytanie[i].koniec), &(dane.n_zapytanie[i].max_minut));

    fclose(in);
    clock_t czas_poczatek = clock();
    fstream plik_wyjscie( "out.txt", ios::out );
    if( plik_wyjscie.is_open() ) {
        unsigned int koniec;
        unsigned short odpowiedz, max_minut;
        for (unsigned char i = 0; i < dane.zapytanie; ++i) {
            for (int i = dane.polaczenie - 1; i > 0; --i) {
                if (dane.n_polaczenie[i].rodzaj == 'b')
                    dane.n_polaczenie[i].krawedz_do_poczatku->czy_dostepny = dane.n_polaczenie[i].krawedz_do_konca->czy_dostepny = false;
                else
                    dane.n_polaczenie[i].krawedz_do_poczatku->czas = dane.n_polaczenie[i].krawedz_do_konca->czas = dane.n_polaczenie[i].prev->czas;
            }

            max_minut = dane.n_zapytanie[i].max_minut;
            for (unsigned int j = 0; j < dane.polaczenie; ++j) {
                dane.n_polaczenie[j].krawedz_do_poczatku->czy_dostepny = dane.n_polaczenie[j].krawedz_do_konca->czy_dostepny = true;
                dane.n_polaczenie[j].krawedz_do_poczatku->czas = dane.n_polaczenie[j].krawedz_do_konca->czas = dane.n_polaczenie[j].czas;

                odpowiedz = graf->dijkstra(dane.n_zapytanie[i].poczatek, dane.n_zapytanie[i].koniec);
                koniec = j;

                if (odpowiedz <= max_minut)
                    break;
            }

            plik_wyjscie << (odpowiedz < dane.n_zapytanie[i].max_minut ? dane.n_polaczenie[koniec].data : "NIE") << endl;
            cout << (odpowiedz < dane.n_zapytanie[i].max_minut ? dane.n_polaczenie[koniec].data : "NIE") << endl;

        }
        clock_t czas_koniec = clock();
        plik_wyjscie.close();

        cout << "Czas: " << ((double) (czas_koniec - czas_poczatek)) / CLOCKS_PER_SEC << "s";
    }
    else
        cout << "Blad otwarcia pliku out.txt!";

    return 0;
}