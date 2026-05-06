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

int calculeazaInaltimeArbore(NodArbore* root) {
//nr de niveluri!!
	return 0;
}

//ALTE FUNCTII NECESARE:

void rotireDreapta(NodArbore** root) {
	NodArbore* aux = (*root)->left;
	(*root)->left = aux->right;
	aux->right = (*root);
	(*root) = aux;
}

void rotireStanga(NodArbore** root) {
	NodArbore* aux = (*root)->right;
	(*root)->right = aux->left;
	aux->left = (*root);
	(*root) = aux;
}

char calculGE(NodArbore* root) {
	if (root) {
		return calculeazaInaltimeArbore(root->left) - calculeazaInaltimeArbore(root->right);
	}
	else {
		return 0;
	}
}

void adaugaMasinaInArboreEchilibrat(NodArbore** root, Masina masinaNoua) {
	if (*root) {
		if ((*root)->info.id > masinaNoua.id) {

			adaugaMasinaInArboreEchilibrat(&(*root)->left, masinaNoua);
		}
		else {
			adaugaMasinaInArboreEchilibrat(&(*root)->right, masinaNoua);
		}
		int grad = calculGE((*root));
		if (grad == 2) {
			if (calculGE((*root)->left) == -1) {
				rotireDreapta(&(*root)->left);
				rotireDreapta(&(*root));
			}
			else {
				rotireDreapta(&(*root)); //sau scriem (root)
			}
		}
		else if (grad == -2) {
			if (calculGE((*root)->right) == 1) {
				rotireDreapta(&(*root)->right);
			}
			rotireStanga(root);
		}

	}
	else {
		NodArbore* nou = (NodArbore*)malloc(sizeof(NodArbore));
		nou->info = masinaNoua;
		nou->left = NULL;
		nou->right = NULL;
		(*root) = nou;
	}
}

void* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	NodArbore* root = NULL;
	while (!feof(f)) {
		Masina masina = citireMasinaDinFisier(f);
		adaugaMasinaInArboreEchilibrat(&root, masina);
	}
}

void afisareMasiniDinArbore(NodArbore* root) { //preordine rsd
	if (root) {
		afisareMasina(root->info);
		afisareMasiniDinArbore(root->left);
		afisareMasiniDinArbore(root->right);
	}
}

void dezalocareArboreDeMasini(NodArbore** root) { //postordine
	if (*root) {
		dezalocareArboreDeMasini(&(*root)->left);
		dezalocareArboreDeMasini(&(*root)->right);
		free((*root)->info.numeSofer);
		free((*root)->info.model);
		*root = NULL;
	}
}

//Preluati urmatoarele functii din laboratorul precedent.
//Acestea ar trebuie sa functioneze pe noul arbore echilibrat.

Masina getMasinaByID(/*arborele de masini*/int id);

int determinaNumarNoduri(/*arborele de masini*/);

float calculeazaPretTotal(/*arbore de masini*/);

float calculeazaPretulMasinilorUnuiSofer(NodArbore* root, const char* numeSofer) {
float sum=0;
	if (root) {
	sum+=calculeazaPretulMasinilorUnuiSofer(root->left, numeSofer);
	sum += calculeazaPretulMasinilorUnuiSofer(root->left, numeSofer);
	if (strcmp(root->info.numeSofer, numeSofer)==0) { //ia prima litera din primul cuvant
	// si din ea scade codul ascii din celalalt cuvant
		sum+=root->info.pret;
	}
	}
	return sum;
}

int main() {
NodArbore* root=citireArboreDeMasiniDinFisier("masini_arbore.txt");
afisareMasiniDinArbore(root);

printf("%f", calculeazaPretulMasinilorUnuiSofer(root, " Ionescu"));

dezalocareArboreDeMasini(&root);
afisareMasiniDinArbore(root);

	return 0;
}
