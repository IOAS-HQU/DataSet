# Dataset Notes

This directory is the unified local dataset root used by the executable.
Actual benchmark data files are ignored by Git; only this README is tracked.

Recommended local layout:

```text
data/
|-- realworld_45/
|   |-- data/
|   |   |-- Mytxt.txt
|   |   `-- processed/
|   `-- time-distance-matrix/
`-- solomon_36/
    |-- data/
    |   |-- Mytxt.txt
    |   |-- c/
    |   |-- r/
    |   `-- rc/
    `-- time-distance-matrix/
```

The program auto-detects `Mytxt.txt` from this unified dataset directory. It first checks `data/Mytxt.txt`, then `data/<benchmark>/data/Mytxt.txt`, and finally searches recursively under the working directory.

Supported `Mytxt.txt` formats:

```text
# Platform / real-world format
stoptime max_delay data_file time1 time2 time3 time4 dist1 dist2 dist3 dist4

# Solomon format
stoptime data_file time1 time2 time3 time4
```

Bare filenames are allowed. The loader resolves them by searching the selected benchmark's `data/` and `time-distance-matrix/` folders, so CSV files do not need to be copied into the repository root.
