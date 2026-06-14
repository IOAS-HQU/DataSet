#pragma once
#include "pch.h"
#include "Population.h"
#include "Individual.h"

#ifndef GENETIC_ALGORITHM_H
#define GENETIC_ALGORITHM_H

typedef vector<Individual*> Front;

class GeneticAlgorithm
{
public:
	void wheelSelection(Population&);
	vector<Front> fastNonDominatedSort(Population*);
	void crowdingDistanceAssignment(Front, float, float);
	void crossover(Population&);
	void mutation(Population&);
	Chromosome crossover_greedy(Population&, int);
	Chromosome mutation_target(Population&, int);
	Chromosome Crossover_Greedy(Chromosome, Chromosome, int);
	Chromosome Mutation_Greedy(Chromosome&, int, vector<Chromosome>&);
	Chromosome Binary_tournament_selection(Population&);
	bool Unique_population(Population&, Chromosome);
};

#endif // !GENETIC_ALGORITHM_H
