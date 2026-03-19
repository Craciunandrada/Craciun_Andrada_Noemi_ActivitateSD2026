#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraMasina {
    int id;
    int nrUsi;
    float pret;
    char* model;
    char* numeSofer;
    unsigned char serie;
};
typedef struct StructuraMasina Masina;


struct Nod {

    Masina info;
    struct Nod* next;
    struct Nod* prev;

};
typedef struct Nod Nod;


struct Lista {

    Nod* first;
    Nod* last;

};
typedef struct Lista Lista;



Masina citireMasinaDinFisier(FILE* file) {

    char buffer[100];
    char sep[3] = ",\n";

    fgets(buffer, 100, file);

    char* aux;

    Masina m;

    aux = strtok(buffer, sep);
    m.id = atoi(aux);

    m.nrUsi = atoi(strtok(NULL, sep));
    m.pret = atof(strtok(NULL, sep));

    aux = strtok(NULL, sep);
    m.model = malloc(strlen(aux) + 1);
    strcpy(m.model, aux);

    aux = strtok(NULL, sep);
    m.numeSofer = malloc(strlen(aux) + 1);
    strcpy(m.numeSofer, aux);

    m.serie = *strtok(NULL, sep);

    return m;
}



void afisareMasina(Masina m) {

    printf("Id: %d\n", m.id);
    printf("Nr usi: %d\n", m.nrUsi);
    printf("Pret: %.2f\n", m.pret);
    printf("Model: %s\n", m.model);
    printf("Sofer: %s\n", m.numeSofer);
    printf("Serie: %c\n\n", m.serie);

}



void afisareListaMasini(Lista lista) {

    Nod* temp = lista.first;

    while (temp) {

        afisareMasina(temp->info);
        temp = temp->next;

    }

}



void adaugaMasinaLaSfarsit(Lista* lista, Masina m) {

    Nod* nou = (Nod*)malloc(sizeof(Nod));

    nou->info = m;
    nou->next = NULL;
    nou->prev = lista->last;

    if (lista->last) {

        lista->last->next = nou;

    }
    else {

        lista->first = nou;

    }

    lista->last = nou;
}



void adaugaMasinaLaInceput(Lista* lista, Masina m) {

    Nod* nou = (Nod*)malloc(sizeof(Nod));

    nou->info = m;
    nou->prev = NULL;
    nou->next = lista->first;

    if (lista->first) {

        lista->first->prev = nou;

    }
    else {

        lista->last = nou;

    }

    lista->first = nou;
}



Lista citireListaDinFisier(const char* numeFisier) {

    FILE* f = fopen(numeFisier, "r");

    Lista lista;
    lista.first = NULL;
    lista.last = NULL;

    while (!feof(f)) {

        Masina m = citireMasinaDinFisier(f);
        adaugaMasinaLaSfarsit(&lista, m);

    }

    fclose(f);

    return lista;
}



void dezalocareLista(Lista* lista) {

    while (lista->first) {

        Nod* temp = lista->first;

        free(temp->info.model);
        free(temp->info.numeSofer);

        lista->first = lista->first->next;

        free(temp);
    }

    lista->last = NULL;
}



float calculeazaPretMediu(Lista lista) {

    float suma = 0;
    int contor = 0;

    Nod* temp = lista.first;

    while (temp) {

        suma += temp->info.pret;
        contor++;

        temp = temp->next;
    }

    if (contor == 0)
        return 0;

    return suma / contor;
}



void stergeMasinaDupaID(Lista* lista, int id) {

    Nod* temp = lista->first;

    while (temp) {

        if (temp->info.id == id) {

            if (temp->prev)
                temp->prev->next = temp->next;
            else
                lista->first = temp->next;

            if (temp->next)
                temp->next->prev = temp->prev;
            else
                lista->last = temp->prev;

            free(temp->info.model);
            free(temp->info.numeSofer);

            Nod* sters = temp;
            temp = temp->next;

            free(sters);
        }
        else {

            temp = temp->next;
        }
    }
}



char* getNumeSoferMasinaScumpa(Lista lista) {

    Nod* max = lista.first;

    Nod* temp = lista.first->next;

    while (temp) {

        if (temp->info.pret > max->info.pret)
            max = temp;

        temp = temp->next;
    }

    char* nume = malloc(strlen(max->info.numeSofer) + 1);

    strcpy(nume, max->info.numeSofer);

    return nume;
}



int main() {

    Lista lista = citireListaDinFisier("masini.txt");

    afisareListaMasini(lista);

    printf("Pret mediu = %f\n", calculeazaPretMediu(lista));

    char* nume = getNumeSoferMasinaScumpa(lista);
    printf("Sofer masina scumpa: %s\n", nume);

    free(nume);

    stergeMasinaDupaID(&lista, 2);

    printf("\nDupa stergere:\n");

    afisareListaMasini(lista);

    dezalocareLista(&lista);

    return 0;
}