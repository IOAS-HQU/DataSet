#pragma once

#ifndef EXPERIMENT_CONFIG_H
#define EXPERIMENT_CONFIG_H

// Central switches for reproducible D-NSGA2+ experiments.
// Change these values for ablation studies instead of editing algorithm code.
//
// Paper mapping:
// - OBJECTIVE_IOP_SPO implements the IoP/SPO dynamic objective-selection strategy.
// - AOS_PM, AOS_AP, and AOS_MAB implement the adaptive operator-selection variants.
// - CROSSOVER_ENCO keeps the enhanced crossover stage used before local mutation.
// - REWARD_IMPROVEMENT uses normalized objective improvement as the operator reward.

enum ObjectiveSelectionMode
{
    OBJECTIVE_ROUND_ROBIN = 0,
    OBJECTIVE_RANDOM = 1,
    OBJECTIVE_IOP_SPO = 2
};

enum AOSMode
{
    AOS_PM = 0,
    AOS_AP = 1,
    AOS_MAB = 2
};

enum CrossoverMode
{
    CROSSOVER_ENCO = 0,
    CROSSOVER_DISABLED = 1
};

enum RewardMode
{
    REWARD_IMPROVEMENT = 0,
    REWARD_FREQUENCY = 1
};

// Basic experimental scale. NOI_LIMIT is the inner offspring-generation budget
// per generation in Algorithm 1 of the D-NSGA2+ workflow.
const int EXPERIMENT_POPULATION_SIZE = 10;
const int EXPERIMENT_GENE_SIZE = 1;
const int EXPERIMENT_GENERATION_LIMIT = 10;
const int EXPERIMENT_RUN_COUNT = 30;
const int EXPERIMENT_NOI_LIMIT = 50;

// The original real-world input list contains 45 tasks. For Solomon-only
// experiments, set EXPERIMENT_TASK_END to 36 before running.
const int EXPERIMENT_TASK_START = 1;
const int EXPERIMENT_TASK_END = 45;

// Dataset discovery. Leave EXPERIMENT_TASK_LIST_PATH empty to auto-detect a
// benchmark task list from a unified dataset directory. The detector first
// checks data/Mytxt.txt, then recursively searches for Mytxt.txt and chooses a
// file whose task count matches EXPERIMENT_TASK_END when possible.
const char* const EXPERIMENT_TASK_LIST_PATH = "";
const double EXPERIMENT_DEFAULT_MAX_DELAY = 30.0;

// Main paper configuration: dynamic objective selection + PM-based AOS.
const ObjectiveSelectionMode EXPERIMENT_OBJECTIVE_MODE = OBJECTIVE_IOP_SPO;
const AOSMode EXPERIMENT_AOS_MODE = AOS_PM;
const CrossoverMode EXPERIMENT_CROSSOVER_MODE = CROSSOVER_ENCO;
const RewardMode EXPERIMENT_REWARD_MODE = REWARD_IMPROVEMENT;
const bool EXPERIMENT_INIT_ROTATION_DIVERSITY = true;

// M1-M8 ablation switch. Set one value to false for the corresponding
// mutation-operator ablation without changing the algorithm flow.
// These switches are used both in the preprocessing stage and in the online
// AOS mutation stage, so an operator is disabled consistently everywhere.
const bool EXPERIMENT_MUTATION_OPERATOR_ENABLED[8] = {
    true, true, true, true, true, true, true, true
};

// Dynamic objective-selection parameters. The current implementation uses the
// previous best value as the comparison window (h=1) and EXPERIMENT_IOP_MU to
// avoid division by zero when calculating improvement potential.
const int EXPERIMENT_IOP_WINDOW_H = 1;
const double EXPERIMENT_IOP_MU = 1.0e-9;

// Paper-level AOS/archive parameters. Keep these names aligned with
// run_config.txt and experiment documentation.
const double EXPERIMENT_ALPHA = 0.7;
const double EXPERIMENT_PM_PMIN = 0.05;
const double EXPERIMENT_AP_PMAX = 0.65;
const double EXPERIMENT_AP_BETA = 0.7;
const double EXPERIMENT_MAB_C = 1.0;
const double EXPERIMENT_EPSILON_BOX = 0.05;

// Learning-based preprocessing. When enabled, the code samples local-search
// moves before the evolutionary loop to initialize operator-quality estimates.
const bool EXPERIMENT_ENABLE_PREPROCESSING = true;
const int EXPERIMENT_PREPROCESS_SECONDS = 20;
const bool EXPERIMENT_USE_FIXED_SEED = true;
const unsigned int EXPERIMENT_BASE_SEED = 20260613;
const bool EXPERIMENT_ENABLE_LOG = false;

#endif // EXPERIMENT_CONFIG_H
