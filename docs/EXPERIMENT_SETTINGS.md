# Experiment Settings

This document records the public experiment switches and hyperparameters used by the complete D-NSGA2+ implementation. The goal is to keep paper terminology, code-level names, and `Result/run_config.txt` names consistent.

## Mechanism Switches

| Public name | Current value | Source | Description |
| --- | ---: | --- | --- |
| `OBJECTIVE_MODE` | `iop_spo` | Complete D-NSGA2+ mechanism | Compute IoP/SPO at the beginning of each generation and sample the active objective from SPO. |
| `AOS_MODE` | `PM` | Paper-style AOS setting | PM is the default adaptive operator selection mode. |
| `CROSSOVER_MODE` | `ENCO` | Complete D-NSGA2+ mechanism | Enables the enhanced crossover mechanism; use `DISABLED` for ablation. |
| `REWARD_MODE` | `IMPROVEMENT` | AOS reward design | Rewards operators by objective improvement; `FREQUENCY` is available for ablation. |
| `MUTATION_OPERATOR_ENABLED` | `M1-M8 enabled` | Complete implementation | Controls M1-M8 for mutation ablation; preprocessing and main mutation follow the same switches. |
| `ENABLE_PREPROCESSING` | `true` | Complete mechanism | Enables the learning-based preprocessing stage. |
| `INIT_ROTATION_DIVERSITY` | `true` | Release default | Adds deterministic rotation in time-greedy initialization to improve initial diversity. |
| `USE_FIXED_SEED` | `true` | Release default | Makes independent runs reproducible. |
| `BASE_SEED` | `20260613` | Release default | Each run uses `BASE_SEED + task * 1000 + run`. |
| `ENABLE_LOG` | `false` | Release default | Keeps large experiment output concise. Set to `true` when checking IoP/SPO, AOS selection, and archive-size traces. |

## Run Scale

| Public name | Current value | Source | Description |
| --- | ---: | --- | --- |
| `RUN_COUNT` | `30` | Paper-style experiment setting | Independent runs per task. |
| `NOI_LIMIT` | `50` | Algorithm setting | Inner iterations per generation. |
| `GENERATION_LIMIT` | `10` | Original code path | The outer loop is still affected by the task-level `stoptime`. |
| `POPULATION_SIZE` | `10` | Original implementation | Population size retained from the original code. |
| `GENE_SIZE` | `1` | Original implementation | Chromosome structure retained from the original code. |
| `TASK_START` | `1` | Main program setting | First task index in `Mytxt.txt`. |
| `TASK_END` | `45` | Default input-list setting | Change this value for benchmark suites with a different number of tasks. |
| `TASK_LIST_PATH` | auto-detect | Release default | Leave empty to auto-detect a unified benchmark `Mytxt.txt`; set explicitly to force a specific dataset. |
| `DEFAULT_MAX_DELAY` | `30` | Compatibility setting | Used when reading the concise 6-column Solomon task-list format. |

## Relative Paths

The program uses the current working directory as the run directory.

| Item | Default relative path | Description |
| --- | --- | --- |
| Input list | Auto-detected `Mytxt.txt` | Checks `EXPERIMENT_TASK_LIST_PATH`, `data/Mytxt.txt`, `data/<benchmark>/data/Mytxt.txt`, then recursively searches the working directory. |
| Data files | Defined by `Mytxt.txt` | Bare filenames and relative paths are resolved inside the selected benchmark's `data/` and `time-distance-matrix/` folders. |
| Run result | `./Result/task X result Y .txt` | Output for task `X`, independent run `Y`. |
| Task final result | `./Result/task X final result.txt` | Merged run-level EP outputs for the same task, followed by nondominated filtering. |
| Run config | `./Result/run_config.txt` | Actual runtime configuration. Keep this file with generated results. |
| Stage output | `./Stage/` | Intermediate archive/debug output; ignored by Git. |

## IoP/SPO Dynamic Objective Selection

| Public name | Current value | Source | Description |
| --- | ---: | --- | --- |
| `IOP_MU` | `1e-9` | Release default | Small positive value used to avoid division by zero. |
| `IOP_WINDOW_H` | `1` | Compatibility record | The implementation compares against historical global best values; this value is retained in the config record. |

High-level flow:

1. At the beginning of each generation, collect the current parent population's best value on each of the four minimization objectives.
2. Compare current best values with the historical global best values to compute `IoP_m`.
3. Apply softmax to the four IoP values to obtain `SPO_m`.
4. Sample the active evolutionary objective from `SPO` when generating offspring.
5. After the generation-level IoP/SPO calculation, update historical global best values.

## AOS Parameters

| Public name | Current value | Source | Description |
| --- | ---: | --- | --- |
| `ALPHA` | `0.7` | Original `Aerfa`, renamed | Exponential smoothing factor for AOS quality estimation. |
| `PM_PMIN` | `0.05` | Original `pmin`, renamed | Minimum probability used by PM/AP to prevent operator probability collapse. |
| `AP_PMAX` | `0.65` | Original `pmax`, renamed | Target probability of the winning operator in AP. |
| `AP_BETA` | `0.7` | Original `Beta`, renamed | AP learning rate. |
| `MAB_C` | `1.0` | Release default | Exploration scaling factor for MAB. |
| `EPSILON_BOX` | `0.05` | Original `epsilon`, renamed | Epsilon-box parameter for the external archive. |

The public parameter names are centralized in `ExperimentConfig.h`. Runtime records written to `run_config.txt` omit the `EXPERIMENT_` prefix for readability.

## Initialization and Archive

| Mechanism | Current implementation | Description |
| --- | --- | --- |
| Initialization | Time-series greedy initialization | Orders trips by start time and performs feasible insertion scans with deterministic rotation for diversity. |
| External archive EP | Pareto + epsilon-box update | EP is updated after accepted offspring and environmental selection. |
| Task final result | Merged run-level EP outputs | All run-level EP outputs for the same task are merged and nondominated-filtered. |

## Release Notes

- The main run scale is controlled by `ExperimentConfig.h` rather than hardcoded values.
- IoP/SPO replaces fixed round-robin objective selection in the complete implementation.
- AOS supports PM/AP/MAB through one configuration switch.
- M1-M8 operator switches are shared by preprocessing and the main evolutionary stage.
- `Result/run_config.txt` is generated for each run directory and should be archived with experimental outputs.
