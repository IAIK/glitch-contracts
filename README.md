# Closing the Gap: Leakage Contracts for Processors with Transitions and Glitches

This repository contains the end-to-end verification tool for transient leakage contracts described in the [Closing the Gap](https://tches.iacr.org/index.php/TCHES/article/view/11785) paper.

The hardware verifier can be used to prove compliance of hardware with a leakage contract, while the software verifier ensures compliance of RISC-V programs with the contract. End-to-end security is proven if both checks are run successfully.

## Repository Content

| Project  | Description |
| -------- | ------------- |
| [sw_verifier](sw_verifier) | IBEX-Contract and software verification utility |
| [hw_verifier](hw_verifier) | hardware verification utility |
