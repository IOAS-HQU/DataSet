# Xiamen-Zhangzhou 45-Instance Benchmark

This benchmark group contains **45 real-world operational instances** for VRP-IRS. The instances are generated from intercity ride-sharing order data and road-network information.

Each instance is named in the following format:

```text
alpha1-num1-num2-num3
```

where:

- `alpha1` indicates the road-network and instance scale, with three possible values: `S`, `M`, and `L`.
- `num1` is the number of orders from city A to city B.
- `num2` is the number of orders from city B to city A.
- The pair `num1-num2` represents the directional travel-demand ratio between the two cities.
- `num3` indicates the time-window type, with three possible values: `1`, `2`, and `3`.

Directory contents:

```text
data/                  # Order instance files
time-distance-matrix/  # Time and distance matrices between order locations
```

Notes on the order data:

- The raw order data uses Beijing time and is kept for traceability.
- The processed order data has already been converted into the runtime format and can be used directly by the algorithm.

The `time-distance-matrix/` directory contains eight matrix groups. They describe source/destination to source/destination relations for both distance and travel time:

```text
src-src-dis
src-dest-dis
dest-src-dis
dest-dest-dis
src-src-time
src-dest-time
dest-src-time
dest-dest-time
```

For example, `dest-dest-dis` represents the distance matrix from all order destination points to all order destination points.
