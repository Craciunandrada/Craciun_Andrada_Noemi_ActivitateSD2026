#include<stdio.h>
#include<stdlib.h>
#include <string.h>

struct Film {
	int id;
	int durata;
	char* denumire;
	float buget;
	char varstaMinima;
};
struct Film initializare(int _id, int _durata, char* _denumire, float _buget, char _varstaMinima) {
	struct Film s;
	s.id = _id;
	s.durata = _durata;
	s.denumire = malloc((strlen(_denumire) + 1) * sizeof(char));
	strcpy(s.denumire, _denumire);
	s.buget = _buget;
	s.varstaMinima = _varstaMinima;
	return s;
}

struct Film copiereFilm(struct Film film) {
	return initializare(film.id, film.durata, film.denumire, film.buget, film.varstaMinima);
}

void afisare(struct Film s) {
	printf("\nDetalii film:");
	printf("\nId: %d", s.id);
	printf("\nDurata in minute: %d", s.durata);
	printf("\nDenumire: %s", s.denumire);
	printf("\nBuget: %5.2f", s.buget);
	printf("\nVarsta minima: %d", s.varstaMinima);
	printf("\n");
}

void afisareVector(struct Film* vector, int nrElemente) {
	for (int i = 0; i < nrElemente; i++) {
		afisare(vector[i]);
	}
}

struct Film* copiazaPrimeleNElemente(struct Film* vector, int nrElemente, int nrElementeCopiate) {
	struct Film* vectorNou = malloc(nrElementeCopiate * sizeof(struct Film));

	for (int i = 0; i < nrElementeCopiate; i++) {
		vectorNou[i] = copiereFilm(vector[i]);
	}

	return vectorNou;
}

void dezalocare(struct Film** vector, int* nrElemente) {
	for (int i = 0; i < *nrElemente; i++)
		free((*vector)[i].denumire);
	free(*vector);
	*vector = NULL;
	*nrElemente = 0;
}

void copiazaAnumiteElemente(struct Film* vector, char nrElemente, float bugetMaxim, struct Film** vectorNou, int* dimensiune) {
	*dimensiune = 0;

	for (int i = 0; i < nrElemente; i++)
		if (vector[i].buget < bugetMaxim)
			(*dimensiune)++;

	(*vectorNou) = malloc((*dimensiune) * sizeof(struct Film));

	for (int i = 0, j = 0; i < nrElemente; i++) {
		if (vector[i].buget < bugetMaxim)
			(*vectorNou)[j++] = copiereFilm(vector[i]);
	}
}

struct Film getPrimulElementConditionat(struct Film* vector, int nrElemente, const char* numeCautat) {
	struct Film s;
	s.id = -1;
	s.denumire = NULL;
	for (int i = 0; i < nrElemente;++i) {
		if (!strcmp(vector[i].denumire, numeCautat));
		return vector[i];
	}
	return s;
}



int main() {
	struct Film f;
	f = initializare(1, 120, "Harry Potter", 23456.7, 10);
	int nrFilme = 3;
	struct Film* filme = malloc(nrFilme * sizeof(struct Film));
	filme[0] = initializare(3, 120, "James Bond", 23456.7, 10);
	*(filme + 1) = f;
	filme[2] = initializare(2, 90, "About time", 4562.8, 15);
	int nrFilmeCopiate = 1;
	struct Film* vectorNou = copiazaPrimeleNElemente(filme, nrFilme, nrFilmeCopiate);
	dezalocare(&vectorNou, &nrFilmeCopiate);
	struct Film* filmeIeftine;
	int nrFilmeIeftine;
	copiazaAnumiteElemente(filme, nrFilme, 23450, &filmeIeftine, &nrFilmeIeftine);
	afisareVector(filmeIeftine, nrFilmeIeftine);
	afisare(getPrimulElementConditionat(filme, nrFilme, "Dune"));
	dezalocare(&filme, &nrFilme);

	return 0;
}