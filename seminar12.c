#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
} Masina;

typedef struct NodPrincipal NodPrincipal;
typedef struct NodSecundar NodSecundar;

struct NodSecundar {
	NodSecundar* next;
	NodPrincipal* info;
};

struct NodPrincipal {
	Masina info;
	NodPrincipal* next;
	NodSecundar* vecini;
};

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	Masina m1;

	fgets(buffer, 100, file);

	char* aux = strtok(buffer, sep);
	m1.id = atoi(aux);

	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));

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
	printf("Nr. usi: %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

void inserareListaP(NodPrincipal** graf, Masina masinaNoua) {
	NodPrincipal* nou = malloc(sizeof(NodPrincipal));
	nou->info = masinaNoua;
	nou->next = NULL;
	nou->vecini = NULL;

	if (*graf) {
		NodPrincipal* aux = *graf;
		while (aux->next) {
			aux = aux->next;
		}
		aux->next = nou;
	}
	else {
		*graf = nou;
	}
}

void inserareListaS(NodSecundar** listaSecundara, NodPrincipal* info) {
	NodSecundar* nod = malloc(sizeof(NodSecundar));
	nod->info = info;
	nod->next = NULL;

	if (*listaSecundara) {
		NodSecundar* aux = *listaSecundara;
		while (aux->next) {
			aux = aux->next;
		}
		aux->next = nod;
	}
	else {
		*listaSecundara = nod;
	}
}

NodPrincipal* cautaNodDupaID(NodPrincipal* listaPrincipala, int id) {
	while (listaPrincipala && listaPrincipala->info.id != id) {
		listaPrincipala = listaPrincipala->next;
	}
	return listaPrincipala;
}

void inserareMuchie(NodPrincipal* listaPrincipala, int idStart, int idStop) {
	NodPrincipal* nodStart = cautaNodDupaID(listaPrincipala, idStart);
	NodPrincipal* nodSfarsit = cautaNodDupaID(listaPrincipala, idStop);

	if (nodStart && nodSfarsit) {
		inserareListaS(&(nodStart->vecini), nodSfarsit);
		inserareListaS(&(nodSfarsit->vecini), nodStart);
	}
}

NodPrincipal* citireNoduriMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	NodPrincipal* listaPrincipala = NULL;

	if (f) {
		while (!feof(f)) {
			Masina m = citireMasinaDinFisier(f);
			inserareListaP(&listaPrincipala, m);
		}
		fclose(f);
	}

	return listaPrincipala;
}

void citireMuchiiDinFisier(NodPrincipal* listaPrincipala, const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");

	if (file) {
		int idStart;
		int idStop;

		while (fscanf(file, "%d %d", &idStart, &idStop) == 2) {
			inserareMuchie(listaPrincipala, idStart, idStop);
		}

		fclose(file);
	}
}

void afisareLista(NodPrincipal* graf, int id) {
	NodPrincipal* nod = cautaNodDupaID(graf, id);

	if (nod == NULL) {
		printf("Nu exista nodul cu id-ul %d.\n", id);
		return;
	}

	NodSecundar* cap = nod->vecini;

	printf("Vecinii nodului cu id-ul %d sunt:\n\n", id);

	while (cap) {
		afisareMasina(cap->info->info);
		cap = cap->next;
	}
}

void dezalocareNoduriGraf(NodPrincipal** listaPrincipala) {
	if (listaPrincipala == NULL || *listaPrincipala == NULL) {
		return;
	}

	NodPrincipal* curentP = *listaPrincipala;

	while (curentP) {
		NodSecundar* curentS = curentP->vecini;

		while (curentS) {
			NodSecundar* auxS = curentS;
			curentS = curentS->next;
			free(auxS);
		}

		free(curentP->info.model);
		free(curentP->info.numeSofer);

		NodPrincipal* auxP = curentP;
		curentP = curentP->next;
		free(auxP);
	}

	*listaPrincipala = NULL;
}

int main() {
	NodPrincipal* graf = NULL;

	graf = citireNoduriMasiniDinFisier("masini.txt");

	citireMuchiiDinFisier(graf, "muchii.txt");

	afisareLista(graf, 1);

	dezalocareNoduriGraf(&graf);

	return 0;
}
