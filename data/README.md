# Dataset Layout

This directory defines the unified dataset layout used by the released C++ code. The repository tracks only this layout, `.gitkeep` placeholders, and the two task-list files. Large CSV benchmark files should be copied from `benchmark_instances/` after cloning or after merging the dataset branch.

## Target Layout

```text
data/
|-- realworld_45/
|   |-- data/
|   |   |-- Mytxt.txt
|   |   `-- processed/
|   `-- time-distance-matrix/
|       |-- src-src-time/
|       |-- dest-dest-time/
|       |-- dest-src-time/
|       |-- src-dest-time/
|       |-- src-src-dis/
|       |-- dest-dest-dis/
|       |-- dest-src-dis/
|       `-- src-dest-dis/
`-- solomon_36/
    |-- data/
    |   |-- Mytxt.txt
    |   |-- c/
    |   |-- r/
    |   `-- rc/
    `-- time-distance-matrix/
```

## Migration From `benchmark_instances/`

### Platform 45 Instances

Source directory:

```text
benchmark_instances/xiamen_zhangzhou_45/
```

Copy files as follows:

```text
benchmark_instances/xiamen_zhangzhou_45/data/processed/*.csv
  -> data/realworld_45/data/processed/

benchmark_instances/xiamen_zhangzhou_45/time-distance-matrix/src-src-time/*.csv
  -> data/realworld_45/time-distance-matrix/src-src-time/

benchmark_instances/xiamen_zhangzhou_45/time-distance-matrix/dest-dest-time/*.csv
  -> data/realworld_45/time-distance-matrix/dest-dest-time/

benchmark_instances/xiamen_zhangzhou_45/time-distance-matrix/dest-src-time/*.csv
  -> data/realworld_45/time-distance-matrix/dest-src-time/

benchmark_instances/xiamen_zhangzhou_45/time-distance-matrix/src-dest-time/*.csv
  -> data/realworld_45/time-distance-matrix/src-dest-time/

benchmark_instances/xiamen_zhangzhou_45/time-distance-matrix/src-src-dis/*.csv
  -> data/realworld_45/time-distance-matrix/src-src-dis/

benchmark_instances/xiamen_zhangzhou_45/time-distance-matrix/dest-dest-dis/*.csv
  -> data/realworld_45/time-distance-matrix/dest-dest-dis/

benchmark_instances/xiamen_zhangzhou_45/time-distance-matrix/dest-src-dis/*.csv
  -> data/realworld_45/time-distance-matrix/dest-src-dis/

benchmark_instances/xiamen_zhangzhou_45/time-distance-matrix/src-dest-dis/*.csv
  -> data/realworld_45/time-distance-matrix/src-dest-dis/
```

Use the provided task list:

```text
data/realworld_45/data/Mytxt.txt
```

### Solomon 36 Instances

Source directory:

```text
benchmark_instances/modified_solomon_36/
```

Copy files as follows:

```text
benchmark_instances/modified_solomon_36/data/c/*.csv
  -> data/solomon_36/data/c/

benchmark_instances/modified_solomon_36/data/r/*.csv
  -> data/solomon_36/data/r/

benchmark_instances/modified_solomon_36/data/rc/*.csv
  -> data/solomon_36/data/rc/

benchmark_instances/modified_solomon_36/time-distance-matrix/*.csv
  -> data/solomon_36/time-distance-matrix/
```

Use the provided task list:

```text
data/solomon_36/data/Mytxt.txt
```

## Task-List Formats

`data/realworld_45/data/Mytxt.txt` uses the full 11-column format:

```text
stoptime max_delay data_file time1 time2 time3 time4 dist1 dist2 dist3 dist4
```

`data/solomon_36/data/Mytxt.txt` uses the concise 6-column format:

```text
stoptime data_file time1 time2 time3 time4
```

For the Solomon format, the code uses `EXPERIMENT_DEFAULT_MAX_DELAY` and reuses the four time matrices as distance matrices, matching the historical Solomon experimental setup.

## Running One Dataset

Default configuration uses `EXPERIMENT_TASK_END = 45`, so the loader prefers `data/realworld_45/data/Mytxt.txt`.

To run Solomon, set the task count to 36 in `ExperimentConfig.h`:

```cpp
const int EXPERIMENT_TASK_END = 36;
```

or explicitly set:

```cpp
const char* const EXPERIMENT_TASK_LIST_PATH = "data/solomon_36/data/Mytxt.txt";
```

Generated outputs are written to `Result/` and `Stage/`, which are ignored by Git.

