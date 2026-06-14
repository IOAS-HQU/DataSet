# Reproducibility Notes

This document records the output locations and the recommended HV/IGD reference settings for reproducing the experiments.

## Output Files

The program auto-detects `Mytxt.txt` from the unified dataset directory and writes outputs to `Result/` and `Stage/`. It first checks `data/Mytxt.txt`, then `data/<benchmark>/data/Mytxt.txt`, and finally searches recursively under the working directory.

| Path | Meaning |
| --- | --- |
| `Result/task X result Y .txt` | Output of task `X` in independent run `Y`; each line contains four objective values for one solution. |
| `Result/task X final result.txt` | Final nondominated set for task `X`, produced by merging all run-level EP outputs for that task and filtering nondominated points. |
| `Result/run_config.txt` | Actual parameters, switches, and random-seed settings used in the run. |
| `Stage/` | Intermediate archive/debug output; not tracked by Git. |

## Objectives

All four objectives are treated as minimization objectives:

1. Number of vehicles or routes.
2. Total delay.
3. Total travel distance or cost.
4. Occupancy-related objective, already exported in minimization form by the program.

## HV Reference Setting

Recommended task-level HV setting:

1. Deduplicate each run-level result and retain nondominated points.
2. For the same task, use a common `ideal` and `nadir` across all methods included in the comparison.
3. Normalize the four objectives using the common `ideal/nadir`.
4. Use `[1.1, 1.1, 1.1, 1.1]` as the normalized HV reference point.
5. Larger HV indicates better objective-space coverage.

## IGD Reference Setting

Recommended task-level IGD setting:

1. Deduplicate each run-level result and retain nondominated points.
2. For the same task, use a common `ideal` and `nadir` across all methods included in the comparison.
3. Normalize the four objectives using the common `ideal/nadir`.
4. Merge normalized nondominated points from all methods and all runs included in the same comparison.
5. Apply nondominated filtering to the merged set to obtain the task-level reference front.
6. Compute IGD from each run-level nondominated set to that common reference front.
7. Smaller IGD indicates better convergence to the reference front.

## Reporting Reminder

- Compute metrics task by task because different tasks can have different objective scales.
- When reporting algorithm comparisons, explicitly state the common `ideal/nadir`, the HV reference point, and the IGD reference-front construction rule.
- `Result/`, `Stage/`, generated outputs, and large datasets are not tracked by Git.
