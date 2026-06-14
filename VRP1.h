#pragma once
#include<vector>
#include<string>
#include"Tripcar.h"
#include "ExperimentConfig.h"
using namespace std;

#ifndef VRP1_H
#define VRP1_H

const int MAX2 = 601;
double peer_time[MAX2][MAX2];         //两个客户点上车点之间的行驶时间
double peer_time_2[MAX2][MAX2];       //两个客户点下车点之间的行驶时间
double peer_time_3[MAX2][MAX2];		  //行程之间前一个客户点下车点到后一个客户点上车点的行驶时间
double peer_time_4[MAX2][MAX2];       //行程内前一个客户点出发点到下一个客户点目的地的行驶时间
double peer_srcdistance[MAX2][MAX2];  //两个客户点之间的出发点行驶距离
double peer_desdistance[MAX2][MAX2];  //两个客户点之间的目的地行驶距离
double peer_sd_distance[MAX2][MAX2];  //行程之间前一个客户点下车点到后一个客户点上车点的行驶距离
double peer_sd2_distance[MAX2][MAX2]; //行程内前一个客户点出发点到下一个客户点目的地的行驶距离
Order order[MAX2];
Trip trip[MAX2];
Car car[MAX2];
vector<int>inds;
//vector<double>arcValues;
double arcValues[5];
const double EPS = 0.000001;

// External-archive support: extreme[k] preserves the best known solution for
// objective k so boundary solutions are not lost during epsilon-box pruning.
Chromosome extreme[FUNC_NUM1];

// AOS reward/quality matrices. Rows are M1-M8 local-search mutation operators;
// columns are the four minimization objectives.
double LS_Matrix[8][4];             // Smoothed operator quality used by AOS.
double SF_Matrix[8][4];             // Operator selection frequency.
double UF_Matrix[8][4];             // Accumulated operator reward/improvement.

// Per-generation AOS statistics. These are folded into LS_Matrix at generation
// boundaries through update_Last_End1/update_Last_End2 and update_Quality.
double LSC_Matrix[8][4];            // Previous/current generation quality.
double SFC_Matrix[8][4];            // Current generation selection frequency.
double UFC_Matrix[8][4];            // Current generation reward/improvement.

double PM_Matrix[8][4];             // Probability Matching probabilities.
double AP_Matrix[8][4];             // Adaptive Pursuit probabilities.
double MAB_Matrix[8][4];            // Multi-Armed Bandit scores.

double init_data[4];                // Objective vector before one mutation move.
double global_f[4];                 // Legacy global-best tracker for IoI logic.
double curr_f[4];                   // Legacy current-generation best tracker.
double w[4];                        // Legacy objective-improvement weights.
double IoI[4];                      // Legacy IoI probabilities.

#endif // VRP1_H
