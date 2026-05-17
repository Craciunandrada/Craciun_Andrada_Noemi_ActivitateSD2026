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

struct Heap {
	int lungime;
	Masina* vector;
	int nrElementeVizibile;
};
typedef struct Heap Heap;

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

Heap initializareHeap(int lungime) {
	Heap h;
	h.lungime = lungime;
	h.nrElementeVizibile = 0;
	h.vector = malloc(sizeof(Masina) * lungime);
	return h;
}

void filtreazaHeap(Heap heap, int pozitieNod) {
	int ps = (2 * pozitieNod) + 1;
	int pd = (2 * pozitieNod) + 2;
	int pmax = pozitieNod;
	if (ps < heap.nrElementeVizibile && heap.vector[pmax].pret < heap.vector[ps].pret) {
		pmax = ps;
	}
	if (pd < heap.nrElementeVizibile && heap.vector[pmax].pret < heap.vector[pd].pret) {
		pmax = pd;
	}
	if (pmax != pozitieNod) {
		Masina aux = heap.vector[pmax];
		heap.vector[pmax] = heap.vector[pozitieNod];
		heap.vector[pozitieNod] = aux;
		if (pmax < (heap.nrElementeVizibile - 2) / 2) {
			filtreazaHeap(heap, pmax);
		}
	}
}

Heap citireHeapDeMasiniDinFisier(const char* numeFisier) {
	Heap heap = initializareHeap(10);
	FILE* f = fopen(numeFisier, "r");
	int i = 0;
	while (!feof(f)) {
		Masina m = citireMasinaDinFisier(f);
		heap.vector[i] = m;
		i++;
	}
	fclose(f);
	heap.nrElementeVizibile = i;
	for (int j = (i - 2) / 2; j >= 0; j--) {
		filtreazaHeap(heap, j);
	}
	return heap;
}

void afisareHeap(Heap heap) {
	for (int i = 0; i < heap.nrElementeVizibile; i++) {
		afisareMasina(heap.vector[i]);
	}
}

void afiseazaHeapAscuns(Heap heap) {
	for (int i = heap.nrElementeVizibile; i < heap.lungime; i++) {
		afisareMasina(heap.vector[i]);
	}
}

Masina extrageMasina(Heap* heap) {
	Masina m;
	m.id = -1;
	if (heap->nrElementeVizibile != 0) {
		m = heap->vector[0];
		heap->vector[0] = heap->vector[heap->nrElementeVizibile - 1];
		heap->vector[heap->nrElementeVizibile - 1] = m;
		heap->nrElementeVizibile--;
		for (int j = (heap->nrElementeVizibile - 2) / 2; j >= 0; j--) {
			filtreazaHeap(*heap, j);
		}
	}
	return m;
}


void dezalocareHeap(Heap* heap) {
	for (int i = 0; i < heap->lungime; i++) {
		free(heap->vector[i].model);
		free(heap->vector[i].numeSofer);
	}
	free(heap->vector);
	heap->vector = NULL;
	heap->lungime = 0;
	heap->nrElementeVizibile = 0;
}

int main() {
	Heap heap = citireHeapDeMasiniDinFisier("masini.txt");
	afisareHeap(heap);
	printf("Extrageri: \n");
	afisareMasina(extrageMasina(&heap));
	printf("\n");
	afisareMasina(extrageMasina(&heap));
	printf("\n");
	afisareMasina(extrageMasina(&heap));
	printf("\n");
	printf("Heap ascuns: \n");
	afiseazaHeapAscuns(heap);
	dezalocareHeap(&heap);
	afisareHeap(heap);
	return 0;
}
