#pragma once
#include "pch.h"
#include "Tripcar.h"

#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

using namespace std;

class Individual
{
public:
	using Comparator = bool(Individual::*)(Individual, Individual);
	vector<Chromosome> genes;       //车辆排序按点输出
	vector<float> objectiveSet;     //4个目标
	vector<Individual*> dominatedSet;
	int dominatedCount = 0;
	int rank = 0;
	float distance = 0;

	Individual();
	Individual(Chromosome, int);
	void evaluation();
	void maxevaluation();
	void minevaluation();
	bool dominate(Individual);
};

#endif // !INDIVIDUAL_H
