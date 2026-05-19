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
	NodArbore* left;
	NodArbore* right;
	Masina info;
};

Masina citireMasinaDinFisier(FILE* f) {
	Masina m1;
	char buffer[100];
	char sep[3] = ",\n"; //un octet e pt sfarsit de sir 
	fgets(buffer, 100, f); //bufferul in care va citi, cate chr va citi, streamul din care va citi 
	//strtok returneaza char* = sirul de caractere pe care il gaseste pana la primul separator
	m1.id = atoi(strtok(buffer, sep));
	m1.nrUsi = atoi(strtok(NULL, sep)); //daca i l dam iar pe buffer, pleaca iar de la 
	// adresa lui buffer, adica de la 0; deci ii dam NULL si verifica unde a ramas dupa apelul precedent
	m1.pret = atof(strtok(NULL, sep));
	char* aux;
	aux = strtok(NULL, sep);
	m1.model = (char*)malloc(sizeof(char) * strlen(aux) + 1); //alocam spatiu pt un sir de chr
	strcpy(m1.model, aux);
	aux=strtok(NULL, sep);
	m1.numeSofer=(char*)malloc(sizeof(char)*strlen(aux)+1);
	strcpy(m1.numeSofer, aux);
	m1.serie=strtok(NULL, sep)[0]; //ne trb doar elementul de pe prima pozitie, un sg caracter
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
		if (masinaNoua.id > (*root)->info.id) {
			adaugaMasinaInArbore(&(*root)->right, masinaNoua); //cauta recursiv 
			//in continuare in arborele dr
		}
		else {
			adaugaMasinaInArbore(&(*root)->left, masinaNoua);
		}
	}
	else { // id == radacina->info.id, am gasit nodul
	//alocam memorie in heap pentru masina gasita
	NodArbore* nou = (NodArbore*)malloc(sizeof(NodArbore));
	// strlen=lungimea textului; sizeof=memoria ocupata
	nou->info = masinaNoua;
	nou->left = NULL;
	nou->right = NULL;
	*root = nou;
	}
}

NodArbore* citireArboreDeMasiniDinFisier(const char* numeFisier) {

	FILE* f = fopen(numeFisier, "r");
	NodArbore* root = NULL;

	if (f == NULL) {
		printf("Fisierul nu poate fi deschis");
		return NULL;
	}

	while (!feof(f)) {
		Masina m = citireMasinaDinFisier(f);
		adaugaMasinaInArbore(&root, m);
	}

	fclose(f);

	return root;
}

void afisareMasiniDinArbore(NodArbore* root) {
	//afiseaza toate elemente de tip masina din arborele creat
	//prin apelarea functiei afisareMasina()
	//parcurgerea arborelui poate fi realizata in TREI moduri
	//folositi toate cele TREI moduri de parcurgere
	if (root != NULL) { //preordine
		afisareMasina(root->info);
		afisareMasiniDinArbore(root->left);
		afisareMasiniDinArbore(root->right);
	}
}

void afisareInOrdine(NodArbore* root) {
	if (root != NULL) {
		afisareInOrdine(root->left);
		afisareMasina(root->info);
		afisareInOrdine(root->right);
	}
}

void afisarePostOrdine(NodArbore* root) {
	if (root != NULL) {
		afisarePostOrdine(root->left);
		afisarePostOrdine(root->right);
		afisareMasina(root->info);
	}
}

void dezalocareArboreDeMasini(NodArbore** root) {
	//sunt dezalocate toate masinile si arborele de elemente
	if (*root) {
		dezalocareArboreDeMasini(&(*root)->left);
		dezalocareArboreDeMasini(&(*root)->right);
		free((*root)->info.numeSofer);
		free((*root)->info.model);
		free(*root);
		*root = NULL;
	}
}

Masina getMasinaByID(NodArbore* root, int id) {
	if (root == NULL) {
		Masina m;
		m.id = -1;
	}
	if (id > root->info.id) {
		return getMasinaByID(root->right, id);
	}
	else if (id < root->info.id) {
		return getMasinaByID(root->left, id);
	}
	//daca iese din if-uri, inseamna ca am gasit masina si trb sa returnam COPIA ei
	Masina m = root->info;
	m.model = malloc(strlen(root->info.model) + 1);
	strcpy(m.model, root->info.model);

	m.numeSofer = malloc(strlen(root->info.numeSofer) + 1);
	strcpy(m.numeSofer, root->info.numeSofer);

	return m;
}

int determinaNumarNoduri(NodArbore* root) {
	//calculeaza numarul total de noduri din arborele binar de cautare
	if (root) {
		return 1 + determinaNumarNoduri(root->left) + determinaNumarNoduri(root->right);
	}
	else {
		return 0;
	}
}

int calculeazaInaltimeArbore(NodArbore* root) {
	//calculeaza inaltimea arborelui care este data de 
	//lungimea maxima de la radacina pana la cel mai indepartat nod frunza
	if (root) {
		return 1 + max(calculeazaInaltimeArbore(root->left), calculeazaInaltimeArbore(root->right));
	}
	else {
		return 0;
	}
}

float calculeazaPretTotal(NodArbore* root) {
	//calculeaza pretul tuturor masinilor din arbore.
	if (root) {
		return root->info.pret + calculeazaPretTotal(root->left) + calculeazaPretTotal(root->right);
	}
	else {
		return 0;
	}
}

float calculeazaPretulMasinilorUnuiSofer(NodArbore* root, const char* numeSofer) {
	//calculeaza pretul tuturor masinilor unui sofer.
	float suma = 0;
	if (root) {
		if (strcmp(root->info.numeSofer, numeSofer) == 0) {
			suma += root->info.pret;
		}
		calculeazaPretulMasinilorUnuiSofer(root->left, numeSofer);
		calculeazaPretulMasinilorUnuiSofer(root->right, numeSofer);
	}
	return suma;
}

int main() {
	NodArbore* root = citireArboreDeMasiniDinFisier("masini.txt");
	afisareMasiniDinArbore(root);

	printf("%f", calculeazaPretulMasinilorUnuiSofer(root, " Ionescu"));

	dezalocareArboreDeMasini(&root);
	afisareMasiniDinArbore(root);

	return 0;
}
