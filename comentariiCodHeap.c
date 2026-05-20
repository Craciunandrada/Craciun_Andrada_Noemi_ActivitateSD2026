qf#define _CRT_SECURE_NO_WARNINGS
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

//creare structura pentru heap
//un vector de elemente, lungimea vectorului si numarul de elemente din vector

struct Heap {
	int lungime; //cat e vectorul in total
	Masina* vector;
	int nrMasini; //cate pozitii din vector sunt ocupate
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

Heap initializareHeap(int lungime) { //initializeaza heap ul cu 0 elemente
	Heap heap;
	heap.lungime=lungime;
	heap.nrMasini=0;
	heap.vector=(Masina*)malloc(sizeof(Masina)*lungime); //aloca un vector de lungime elem de tip Masina
	return heap;
}

void filtreazaHeap(Heap heap, int pozitieNod) {
	//filtreaza heap-ul pentru nodul a carei pozitie o primeste ca parametru
	//compara nodul cu descendentii

		int pozFiuSt = 2 * pozitieNod + 1;
		int pozFiuDr = 2 * pozitieNod + 2;
		int pozMax = pozitieNod; //presupunem ca nodul curent are valoarea maxima

		if (pozFiuSt < heap.nrMasini &&
			heap.vector[pozMax].pret < heap.vector[pozFiuSt].pret) {
			pozMax = pozFiuSt;
		} //l am gasit pe pozFiuSt ca fiind mai mare, deci pe el il punem peste val pozMax actuala

		if (pozFiuDr < heap.nrMasini &&
			heap.vector[pozMax].pret < heap.vector[pozFiuDr].pret) {
			pozMax = pozFiuDr;
		}

		if (pozMax != pozitieNod) { //daca pozitia maximului nu este nodul curent, facem interschimbare
			Masina aux = heap.vector[pozMax];
			heap.vector[pozMax] = heap.vector[pozitieNod];
			heap.vector[pozitieNod] = aux;

			if (pozMax<=(heap.nrMasini-2)/2) { //indexul ultimului nod care are cel putin un copil
			//practic verifica daca nodul coresp pozMax mai are copii; 
			//daca da =>continua filtrarea recursiv
			filtreazaHeap(heap, pozMax);
			}
		}
	}

Heap citireHeapDeMasiniDinFisier(const char* numeFisier) {
	//citim toate masinile din fisier si le stocam intr-un heap 
	// pe care trebuie sa il filtram astfel incat sa respecte 
	// principiul de max-heap sau min-heap dupa un anumit criteriu
	// sunt citite toate elementele si abia apoi este filtrat vectorul
	FILE* file=fopen(numeFisier, "r");
	Heap heap= initializareHeap(10);
	
	while (!feof(file)) {
		heap.vector[heap.nrMasini++]=citireMasinaDinFisier(file);
	}
	fclose(file);
	for (int i = (heap.nrMasini - 2) / 2;i >= 0;i--) {
		filtreazaHeap(heap, i);
	}
	return heap;
}
/* 
heap.nrMasini = 0;
heap.vector[0] = prima masina;
heap.nrMasini devine 1; 
*/

void afisareHeap(Heap heap) {
	for (int i = 0;i < heap.nrMasini;i++) {
		afisareMasina(heap.vector[i]);
	}
}

void afiseazaHeapAscuns(Heap heap) {
	for (int i = heap.nrMasini; i < heap.lungime; i++) {
		afisareMasina(heap.vector[i]);
	}
}
 
Masina extrageMasina(Heap* heap) {  //primeste pointer pt ca modifica heapul original
//daca primea prin valoare, modifica doar o copie 

	//extrage si returneaza masina de pe prima pozitie
	//elementul extras nu il stergem...doar il ascundem

	if (heap->nrMasini > 0) {  //daca exista masini de extras
		Masina aux=heap->vector[0];  //salveaza masina din varful max heap-ului, care are pretul cel >
		heap->vector[0]=heap->vector[heap->nrMasini-1]; //pune ultimul element activ in varful heapului
		heap->vector[heap->nrMasini-1]=aux; //muta masina extrasa la finalul zonei active
		heap->nrMasini--; //scade nr de elemente active
		for (int i = (heap->nrMasini - 2) / 2; i >= 0; i--) {
			filtreazaHeap(*heap, i); //sau filtreazaHeap(*heap, 0) pentru ca doar radacina a fost stricata
		}
		return aux; //shallow copy
	}
}


void dezalocareHeap(Heap* heap) {
	//sterge toate elementele din heap
	for (int i = 0; i < heap->lungime; i++) {
		free(heap->vector[i].model);
		free(heap->vector[i].numeSofer);
	}
	free(heap->vector);
	heap->vector = NULL;
	heap->lungime = 0;
	heap->nrMasini = 0;
}

int main() {
	Heap heap=citireHeapDeMasiniDinFisier("masini.txt");
	afisareHeap(heap);

	printf("Masini extrase:");
	afisareMasina(extrageMasina(&heap));
	afisareMasina(extrageMasina(&heap));
	afisareMasina(extrageMasina(&heap));
	afisareMasina(extrageMasina(&heap));
	afisareMasina(extrageMasina(&heap));
	afisareMasina(extrageMasina(&heap));
	afisareMasina(extrageMasina(&heap));
	afisareMasina(extrageMasina(&heap));
	afisareMasina(extrageMasina(&heap));
	afisareMasina(extrageMasina(&heap));

	printf("Heap-ul ascuns: \n");
	afiseazaHeapAscuns(heap);

	dezalocareHeap(&heap);

	return 0;
}
