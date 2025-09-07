# VRAC
VRAC (Virtual RISC Architecture CPU) is 32 bit RISC architecture with non-fixed instruction size.

## How to use

- Build VAssembler. C++17 or higher is needed.
- [Assemble]() your vassembler language project. As a result you'll get a **.vasmexec** file.
- Open *logisim-evolution-x-all.jar*, click *File*->*Open...*, select VRAC.circ.
- Find *ROM* block, right click, Edit Contents.
- In a new box click *Open...* and select previously assembled **.vasmexec** file.
- Start simulation.

You can change the clock frequency in simulation settings.

## Implementation details

- [Architecture](https://github.com/injecktor/VRAC/tree/master/docs/architecture.md)

- [Microarchitecture]()

- [Vassemble]()
