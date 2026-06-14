# Modified Solomon 36-Instance Benchmark

This benchmark group contains **36 modified Solomon instances** for VRP-IRS.

Each instance is named in the following format:

```text
Solomon-alpha1-alpha2-num1-num2
```

where:

- `alpha1` indicates the customer-location distribution type.
- `alpha2` indicates the time-window distribution type.
- `alpha1` and `alpha2` each take one of the three values: `c`, `r`, or `rc`.
- `num1` is the number of orders from city A to city B.
- `num2` is the number of orders from city B to city A.
- The pair `num1-num2` represents the directional travel-demand ratio between the two cities.

Directory contents:

```text
data/                  # Order instance files
time-distance-matrix/  # Time and distance matrices between order locations
```

The benchmark combines three customer-location distributions, three time-window types, and four directional demand ratios, producing 36 instances in total.
