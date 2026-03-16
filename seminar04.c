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
	m1.pret = atof(strtok(NULL, sep));

	aux = strtok(NULL, sep);
	m1.model = (char*)malloc(strlen(aux) + 1);
	strcpy(m1.model, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = (char*)malloc(strlen(aux) + 1);
	strcpy(m1.numeSofer, aux);

	m1.serie = *strtok(NULL, sep);

	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi: %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

void adaugaMasinaInLista(Nod** lista, Masina masinaNoua) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->next = NULL;

	Nod* aux = *lista;

	if (*lista != NULL) {
		while (aux->next != NULL) {
			aux = aux->next;
		}
		aux->next = nou;
	}
	else {
		*lista = nou;
	}
}

void afisareListaMasini(Nod* cap) {
	while (cap != NULL) {
		afisareMasina(cap->info);
		cap = cap->next;
	}
}

Nod* citireListaMasiniDinFisier(const char* numeFisier) {

	FILE* file = fopen(numeFisier, "r");

	Nod* lista = NULL;

	while (!feof(file)) {
		Masina masina = citireMasinaDinFisier(file);
		adaugaMasinaInLista(&lista, masina);
	}

	fclose(file);
	return lista;
}

void dezalocareListaMasini(Nod** lista) {

	Nod* head = *lista;
	Nod* next = NULL;

	while (head != NULL) {

		next = head->next;

		free(head->info.model);
		free(head->info.numeSofer);

		free(head);

		head = next;
	}

	*lista = NULL;
}

float calculeazaPretMediu(Nod* lista) {

	float suma = 0;
	int nrMasini = 0;

	while (lista != NULL) {
		suma += lista->info.pret;
		nrMasini++;
		lista = lista->next;
	}

	if (nrMasini != 0)
		return suma / nrMasini;

	return 0;
}


void stergeMasiniDinSeria(Nod** lista, char serieCautata) {

	while (*lista && (*lista)->info.serie == serieCautata) {

		Nod* aux = *lista;
		*lista = aux->next;

		free(aux->info.model);
		free(aux->info.numeSofer);
		free(aux);
	}

	Nod* p = *lista;

	while (p && p->next) {

		if (p->next->info.serie == serieCautata) {

			Nod* aux = p->next;
			p->next = aux->next;

			free(aux->info.model);
			free(aux->info.numeSofer);
			free(aux);
		}
		else {
			p = p->next;
		}
	}
}


float calculeazaPretulMasinilorUnuiSofer(Nod* lista, const char* numeSofer) {

	float suma = 0;

	while (lista) {

		if (strcmp(lista->info.numeSofer, numeSofer) == 0) {
			suma += lista->info.pret;
		}

		lista = lista->next;
	}

	return suma;
}


int main() {

	Nod* lista = citireListaMasiniDinFisier("masini.txt");
	afisareListaMasini(lista);
	printf("\nPret mediu = %f\n", calculeazaPretMediu(lista));
	printf("Pretul masinilor unui sofer este: %.2f\n",
		calculeazaPretulMasinilorUnuiSofer(lista, "Ionescu"));
	stergeMasiniDinSeria(&lista, 'A');
	printf("\nDupa stergere:\n");
	afisareListaMasini(lista);
	dezalocareListaMasini(&lista);

	return 0;
}
