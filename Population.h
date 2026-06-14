#include "pch.h"
#include "Individual.h"

#pragma once

#ifndef POPULATION_H
#define POPULATION_H

using namespace std;

class Population
{
public:
	vector<Individual> individualSet;
	int populationSize;
	int geneSize;
	Chromosome chrome;
	Population(int, int);
	void initialize();
	void clear();
	Population copy();
	Population copy_all();
	Population combination(Population);
	void evaluation();
	bool check_population(Individual);
};

#endif // !POPULATION_H
