#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

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
};
typedef struct Nod Nod;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret= atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

void adaugaMasinaInLista(Nod** lista, Masina masinaNoua) {
	Nod*  nou=(Nod*)malloc(sizeof(Nod));
	nou->info=masinaNoua; //shallow copy
	nou->next=NULL;
	//Luam un pointer aux cu care ne deplasam
	Nod* aux=*lista;
	if ((*lista)!=NULL) {
	while (aux->next != NULL) { //am nod in fata -> ma mut pe el
	aux=aux->next;
	}
	aux->next=nou;
	}
	else {
	*lista=nou;
	}
}

void afisareListaMasini(Nod* cap) { //nu prin adresa pt ca nu mai modificam nimic 
	//afiseaza toate elemente de tip masina din lista simplu inlantuita
	//prin apelarea functiei afisareMasina()
	while (cap != NULL) {
		afisareMasina(cap->info);
		cap=cap->next;
	}
}



void adaugaLaInceputInLista(/*lista de masini*/ Masina masinaNoua) {
	//adauga la inceputul listei o noua masina pe care o primim ca parametru
}

Nod* citireListaMasiniDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul
	//void* e pointer la orice; echiv template; void* returneaza!!
	FILE *file=fopen(numeFisier, "r");
	Nod* lista=NULL;
	while (!feof(file)) 
	{
	Masina masina=citireMasinaDinFisier(file);
	adaugaMasinaInLista(&lista, masina);
	}
	fclose(file);
	return lista;
}


void dezalocareListaMasini(Nod** lista) {
	Nod* head=*lista;
	Nod* next=NULL;
	while (head != NULL) {
		next=head->next;
		free (head->info.model);
		free (head->info.numeSofer);
		free (head);
		head=next;
	}
	*lista=NULL;
	}

float calculeazaPretMediu(Nod* lista) {
	float suma=0;
	int nrMasini=0;
	while (lista != NULL) {
		suma+=lista->info.pret;
		nrMasini++;
		lista=lista->next;
	}
	if (nrMasini!=0) {
	return suma/nrMasini;
	return 0;
	}
}

void stergeMasiniDinSeria(/*lista masini*/ char serieCautata) {
	//sterge toate masinile din lista care au seria primita ca parametru.
	//tratati situatia ca masina se afla si pe prima pozitie, si pe ultima pozitie
}

float calculeazaPretulMasinilorUnuiSofer(/*lista masini*/ const char* numeSofer) {
	//calculeaza pretul tuturor masinilor unui sofer.
	return 0;
}

int main() {

Nod* lista=citireListaMasiniDinFisier("masini.txt");
afisareListaMasini(lista);
printf("\nDupa\n");
afisareListaMasini(lista);
float pretMediu=calculeazaPretMediu(lista);
printf("\nPret mediu=%f", pretMediu);
dezalocareListaMasini(&lista);
	return 0;
}