#include "pch.h"
#include "Population.h"

const double EPSS = 0.000001;
void set_initialization_offset(int offset);

bool Population::check_population(Individual X)
{
    if (individualSet.size() == 0)
        return true;
    vector<Individual>::iterator I_iter = individualSet.begin();
    for (; I_iter != individualSet.end(); I_iter++) {
        if ((fabs((*I_iter).genes[0].f[0] - X.genes[0].f[0]) < EPSS) &&
            (fabs((*I_iter).genes[0].f[1] - X.genes[0].f[1]) < EPSS) &&
            (fabs((*I_iter).genes[0].f[2] - X.genes[0].f[2]) < EPSS) &&
            (fabs((*I_iter).genes[0].f[3] - X.genes[0].f[3]) < EPSS))
            return false;
    }
    return true;
}

Population::Population(int _popSize, int _geneSize)
{
    populationSize = _popSize;
    geneSize = _geneSize;
}

void Population::initialize()
{
    int i = 0;
    int attempts = 0;
    int max_attempts = populationSize * 100;
    while (i < populationSize && attempts < max_attempts)
    {
        set_initialization_offset(attempts);
        Individual in(chrome, geneSize);
        if (check_population(in)) {
            individualSet.push_back(in);
            i++;
        }
        attempts++;
    }
    while (i < populationSize)
    {
        set_initialization_offset(attempts++);
        Individual in(chrome, geneSize);
        individualSet.push_back(in);
        i++;
    }
}

void Population::clear()
{
    individualSet.clear();
}

Population Population::copy()
{
    return Population(populationSize, geneSize);
}

Population Population::copy_all()
{
    Population _tmpPop(populationSize, geneSize);
    _tmpPop.individualSet = individualSet;
    return _tmpPop;
}

Population Population::combination(Population q)
{
    Population _tmp(populationSize * 2, geneSize * 2);
    _tmp.individualSet = individualSet;

    for (auto ind : q.individualSet)
    {
        _tmp.individualSet.push_back(ind);
    }
    return _tmp;
}

void Population::evaluation()
{
    for (auto& ind : individualSet)
    {
        ind.evaluation();
    }
}
