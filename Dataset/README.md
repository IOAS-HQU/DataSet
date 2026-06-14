# Benchmark Instance Description

This repository contains two benchmark groups for the **Vehicle Routing Problem for Intercity Ride-Sharing (VRP-IRS)**:

1. **Modified Solomon Benchmark Instances**, denoted as `Sm`.
2. **Xiamen-Zhangzhou Real-world Operational Instances**.

The two benchmark groups are designed to evaluate algorithm performance under both controlled benchmark conditions and realistic intercity ride-sharing operating conditions.

## 1. Modified Solomon Benchmark Instances (`Sm`)

The first benchmark group is adapted from the classical **Solomon VRPTW benchmark**. The original Solomon instances mainly describe single-depot urban logistics distribution, which does not fully match the two-city intercity ride-sharing scenario studied in this work. Therefore, the Solomon instances are modified to reflect the main operating characteristics of VRP-IRS.

The generation process is summarized as follows:

- **Original source**: classical Solomon VRPTW instances, including `C`, `R`, and `RC` instance types.
- **Spatial structure adaptation**: customer locations are mapped into two separated urban regions, representing the origin city and the destination city in intercity travel.
- **Intercity corridor setting**: an intercity travel corridor is introduced between the two urban regions. The intercity distance range is set to 60-100 distance units to simulate long-distance cross-city travel.
- **Demand conversion**: logistics demand is converted into passenger demand. The passenger count of each order is randomly generated from `{1, 2, 3, 4}`, matching the capacity range of common private cars or small ride-sharing vehicles.
- **Time-window preservation**: the time-window characteristics of the original Solomon instances are retained. `C` represents clustered demand, `R` represents randomly distributed demand, and `RC` represents mixed demand.

This benchmark group contains **36 modified Solomon instances**. These instances are mainly used to evaluate algorithm robustness, convergence, and multi-objective search ability under standardized benchmark conditions.

## 2. Xiamen-Zhangzhou Real-world Operational Instances

The second benchmark group is derived from historical ride-sharing order data in the **Xiamen-Zhangzhou** intercity operating scenario in China. Compared with the modified Solomon instances, this benchmark group is closer to real-world operation and reflects practical characteristics such as spatial distribution, road-network distance, travel time, and directional demand imbalance.

The generation process is summarized as follows:

- **Original source**: historical order data from an intercity ride-sharing platform operating between Xiamen and Zhangzhou.
- **Road-network information**: distance matrices and time matrices are constructed from real road-network distance and travel-time information.
- **Instance scale**: instances are divided into three groups according to order quantity:
  - `S`: small-scale instances with 20 orders.
  - `M`: medium-scale instances with 100-300 orders.
  - `L`: large-scale instances with 200-600 orders.
- **Directional imbalance**: order-direction ratios are set from `1:1` to `1:5` to simulate uneven travel demand between the two cities.
- **Time-window setting**: passenger time-window widths are set to 30, 45, or 60 minutes to represent different levels of scheduling tightness.

This benchmark group contains **45 real-world operational instances**. These instances are mainly used to evaluate algorithm scalability and practical applicability in realistic intercity ride-sharing scenarios.
