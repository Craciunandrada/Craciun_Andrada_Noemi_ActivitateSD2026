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

typedef struct NodArbore NodArbore;
struct NodArbore {
	Masina info;
	NodArbore* left;
	NodArbore* right;
};


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


void adaugaMasinaInArbore(NodArbore** root, Masina masinaNoua) {
	//adauga o noua masina pe care o primim ca parametru in arbore,
	//astfel incat sa respecte princiippile de arbore binar de cautare
	//dupa o anumita cheie pe care o decideti - poate fi ID

	if (*root) {
		if ((*root)->info.id > masinaNoua.id) {
			adaugaMasinaInArbore(&(*root)->left, masinaNoua);
		}
		else {
			adaugaMasinaInArbore(&(*root)->right, masinaNoua);
		}
	}
	else
	{
		NodArbore* nou = (NodArbore*)malloc(sizeof(NodArbore));
		nou->info = masinaNoua;
		nou->left = NULL;
		nou->right = NULL;
		*root = nou;
	}
}

NodArbore* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul

	FILE* file = fopen(numeFisier, "r");
	NodArbore* root = NULL;
	if (file) {
		while (!feof(file)) {
			Masina m = citireMasinaDinFisier(file);
			adaugaMasinaInArbore(&root, m);
		}
	}
	fclose(file);
	return root;
}

void afisareMasiniDinArbore(NodArbore* root) { //preordine
	//afiseaza toate elemente de tip masina din arborele creat
	//prin apelarea functiei afisareMasina()
	//parcurgerea arborelui poate fi realizata in TREI moduri
	//folositi toate cele TREI moduri de parcurgere

	if (root != NULL) {
		afisareMasina(root->info);
		afisareMasiniDinArbore(root->left);
		afisareMasiniDinArbore(root->right);
	}
}

void afisareInordine(NodArbore* root) {
	if (root != NULL) {
		afisareInordine(root->left);
		afisareMasina(root->info);
		afisareInordine(root->right);
	}
}

void dezalocareArboreDeMasini(NodArbore** radacina) {
	//sunt dezalocate toate masinile si arborele de elemente

	if (*radacina != NULL) {
		dezalocareArboreDeMasini(&(*radacina)->left);
		dezalocareArboreDeMasini(&(*radacina)->right);

		free((*radacina)->info.model);
		free((*radacina)->info.numeSofer);
		free(*radacina);
		*radacina = NULL;
	}
}

Masina getMasinaByID(NodArbore* radacina, int id) {
	if (radacina == NULL) {
		Masina m;
		m.id = -1;
		return m;
	}
	else if (id < radacina->info.id) {
		return getMasinaByID(radacina->left, id);
	}
	else if (id > radacina->info.id) {
		return getMasinaByID(radacina->right, id);
	}
	else {
		Masina m = radacina->info;
		m.model = malloc(strlen(radacina->info.model) + 1);
		strcpy(m.model, radacina->info.model);

		m.numeSofer = malloc(strlen(radacina->info.numeSofer) + 1);
		strcpy(m.numeSofer, radacina->info.numeSofer);

		return m;
	}
}


int determinaNumarNoduri(NodArbore* radacina) {
	//calculeaza numarul total de noduri din arborele binar de cautare
	if (radacina != NULL) {
		return 1 + determinaNumarNoduri(radacina->left) + determinaNumarNoduri(radacina->right);
	}
	else {

		return 0;
	}
}

int calculeazaInaltimeArbore(NodArbore* radacina) {
	//calculeaza inaltimea arborelui care este data de 
	//lungimea maxima de la radacina pana la cel mai indepartat nod frunza
	if (radacina) {
		return max(calculeazaInaltimeArbore(radacina->left), calculeazaInaltimeArbore(radacina->right)) + 1;
	}
	else {
		return 0;
	}
}

float calculeazaPretTotal(NodArbore* radacina) {
	if (radacina != NULL) {
		return radacina->info.pret
			+ calculeazaPretTotal(radacina->left)
			+ calculeazaPretTotal(radacina->right);
	}
	return 0;
}


float calculeazaPretulMasinilorUnuiSofer(NodArbore* radacina, const char* numeSofer) {
	float suma = 0;

	if (radacina != NULL) {
		if (strcmp(radacina->info.numeSofer, numeSofer) == 0) {
			suma += radacina->info.pret;
		}

		suma += calculeazaPretulMasinilorUnuiSofer(radacina->left, numeSofer);
		suma += calculeazaPretulMasinilorUnuiSofer(radacina->right, numeSofer);
	}

	return suma;
}

int main() {
	NodArbore* radacina = citireArboreDeMasiniDinFisier("masini.txt");

	afisareInordine(radacina);

	puts("Masina cu id 3 este:");
	Masina m = getMasinaByID(radacina, 3);
	afisareMasina(m);

	if (m.id != -1) {
		free(m.model);
		free(m.numeSofer);
	}

	printf("Numar de noduri: %d\n", determinaNumarNoduri(radacina));
	printf("Inaltime arbore: %d\n", calculeazaInaltimeArbore(radacina));
	printf("Pret total: %.2f\n", calculeazaPretTotal(radacina));
	printf("Pret masini Ionescu: %.2f\n", calculeazaPretulMasinilorUnuiSofer(radacina, "Ionescu"));

	dezalocareArboreDeMasini(&radacina);

	return 0;
}
