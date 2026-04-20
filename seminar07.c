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
typedef struct Nod Nod;

//creare structura pentru un nod dintr-o lista simplu inlantuita
struct Nod {
	Masina masina;
	struct Nod* next;
};

//hashtable = vector de liste
struct HashTable {
	int dim;
	Nod** head;
};
typedef struct HashTable HashTable;

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

void afisareListaMasini(Nod* nod) {
	while (nod) {
		afisareMasina(nod->masina);
		nod = nod->next;
	}
}

void adaugaMasinaInLista(Nod** nod, Masina masinaNoua) {
	Nod* nou = malloc(sizeof(Nod));
	nou->masina = masinaNoua;
	nou->next = NULL;

	if (!*nod) {
		*nod = nou;
	}
	else {
		Nod* aux = *nod;
		while (aux->next) {
			aux = aux->next;
		}
		aux->next = nou;
	}
}


HashTable initializareHashTable(int dimensiune) {
	HashTable ht;
	ht.dim = dimensiune;
	ht.head = malloc(sizeof(Nod*) * dimensiune);

	for (int i = 0; i < dimensiune; i++) {
		ht.head[i] = NULL;
	}

	return ht;
}

int calculeazaHash(int id, int dimensiune) {
	return (id * 31) % dimensiune;
}

void inserareMasinaInTabela(HashTable ht, Masina masina) {
	int hash = calculeazaHash(masina.id, ht.dim);

	if (ht.head[hash] != NULL) {
		adaugaMasinaInLista(&ht.head[hash], masina);
		return;
	}
	Nod* nod = malloc(sizeof(Nod));
	nod->masina = masina;
	nod->next = NULL;
	ht.head[hash] = nod;

}

HashTable citireMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	HashTable ht = initializareHashTable(5);
	while (!feof(f)) {
		Masina masina = citireMasinaDinFisier(f);
		inserareMasinaInTabela(ht, masina);
	}
	fclose(f);

	return ht;
}

void afisareTabelaDeMasini(HashTable ht) {
	for (int i = 0; i < ht.dim; i++) {
		if (ht.head[i] != NULL) {
			printf("Cluster %d:\n", i + 1);

			Nod* aux = ht.head[i];
			afisareListaMasini(aux);

			printf("=====\n");
		}
	}
}

void dezalocareTabelaDeMasini(HashTable* ht) {
	for (int i = 0; i < ht->dim; i++) {
		if (ht->head[i] != NULL) {
			Nod* p = ht->head[i];
			Nod* next = NULL;

			while (p != NULL) {
				free(p->masina.model);
				free(p->masina.numeSofer);

				next = p->next;
				free(p);
				p = next;
			}
		}
	}
	free(ht->head);
	ht->head = NULL;
	ht->dim = 0;
}

float* calculeazaPreturiMediiPerClustere(HashTable ht, int* nrClustere) {
	int nr = 0;
	for (int i = 0; i < ht.dim; i++) {
		if (ht.head[i] != NULL) {
			nr++;
		}
	}

	float* mediiPerCluster = malloc(nr * sizeof(float));
	int j = 0;
	for (int i = 0; i < ht.dim; i++) {
		if (ht.head[i] != NULL) {
			int nrNoduri = 0;
			float sum = 0;

			Nod* aux = ht.head[i];
			while (aux != NULL) {
				nrNoduri++;
				sum += aux->masina.pret;

				aux = aux->next;
			}

			mediiPerCluster[j++] = sum / nrNoduri;
		}
	}
	*nrClustere = nr;
	return mediiPerCluster;
}

Masina getMasinaById(HashTable ht, int id) {
	Masina m;
	m.id = -1;
	int hash = calculeazaHash(id, ht.dim);

	Nod* aux = ht.head[hash];
	while (aux != NULL) {
		if (aux->masina.id == id) {
			m = aux->masina;
			m.model = malloc(sizeof(char) * (strlen(aux->masina.model) + 1));
			strcpy(m.model, aux->masina.model);

			m.numeSofer = malloc(sizeof(char) * (strlen(aux->masina.numeSofer) + 1));
			strcpy(m.numeSofer, aux->masina.numeSofer);
			return m;
		}
		aux = aux->next;
	}
}

int main() {
	HashTable ht = citireMasiniDinFisier("masini.txt");
	//afisareTabelaDeMasini(ht);
	//afisareMasina(getMasinaById(ht, 9));

	int nr;
	float* medii = calculeazaPreturiMediiPerClustere(ht, &nr);

	for (int i = 0; i < nr; i++) {
		printf("Pret mediu cluster %d: %.2f\n", i + 1, medii[i]);
	}

	dezalocareTabelaDeMasini(&ht);

	return 0;
}
