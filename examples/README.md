# Examples

This directory is reserved for public toy examples or small sanity-check instructions.

For a quick sanity check, keep the executable and a small dataset under the same working tree:

```text
run_case/
|-- DNSGA2.exe
|-- data/
|   `-- demo/
|       |-- data/
|       |   `-- Mytxt.txt
|       `-- time-distance-matrix/
|-- Result/
`-- Stage/
```

Then run the executable from that working directory:

```powershell
.\DNSGA2.exe
```

The program discovers `data/demo/data/Mytxt.txt` automatically. Keep example data small and license-compatible if you decide to publish it.
