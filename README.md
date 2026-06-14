# VRP-IRS Benchmark Datasets

This repository provides public benchmark datasets for the **Vehicle Routing Problem for Intercity Ride-Sharing (VRP-IRS)**.

The datasets are organized into two benchmark groups:

1. **Modified Solomon Benchmark Instances**, denoted as `Sm`.
2. **Xiamen-Zhangzhou Real-world Operational Instances**.

The repository is intended to provide benchmark instances and dataset documentation only. It does not contain algorithm implementation code or generated experimental results.

## Repository Structure

```text
Dataset/
|-- README.md
|-- modified_solomon_36/
|   |-- README.md
|   |-- data/
|   `-- time-distance-matrix/
`-- xiamen_zhangzhou_45/
    |-- README.md
    |-- data/
    `-- time-distance-matrix/
```

## Dataset Groups

### Modified Solomon 36-Instance Benchmark

The modified Solomon benchmark is adapted from the classical Solomon VRPTW benchmark to better reflect two-city intercity ride-sharing operations. It contains 36 instances covering different spatial distributions, time-window distributions, and directional demand ratios.

See [Dataset/modified_solomon_36/README.md](Dataset/modified_solomon_36/README.md) for details.

### Xiamen-Zhangzhou 45-Instance Benchmark

The Xiamen-Zhangzhou benchmark is derived from real-world intercity ride-sharing order data and road-network information. It contains 45 instances with different scales, directional demand ratios, and time-window settings.

See [Dataset/xiamen_zhangzhou_45/README.md](Dataset/xiamen_zhangzhou_45/README.md) for details.

## Dataset Description

A high-level description of both benchmark groups is available in:

[Dataset/README.md](Dataset/README.md)

## Notes

- The repository stores benchmark data and dataset documentation only.
- Generated experimental outputs should not be committed to this repository.
- Please keep the directory structure unchanged when using the datasets in experiments.

