# Omnetpp for network reconfiguration v1.2
See [paper](https://ieeexplore.ieee.org/document/8571306). 

Simulation  test v1.2

Using OMNeT++ 5.1.1

It's about a reconfigurable optical network using ocs switches.  This version contains 16 racks, 2 16*16 ocs switches. Each rack has 10 computing nodes and 1 ToR(EPS switch). All the reconfiguration operations are dominated by a centralized controller.

Here we adopted 4 classical hpc network topologies(mesh, torus, dragonfly, optimalN16K3) to adapt different traffics.