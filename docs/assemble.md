# Assemble

You can assemble your vassembler language project with console.

``vasm.exe tests/1.vasm``

Will generate *1.vasmexec* file. Also, you can build several files.

``vasm.exe tests/1.vasm tests/2.vasm tests/3.vasm``

Will generate *1.vasmexec* file which contains code from all 3 files.
If you want to specify a name to the vasmexec file you can use **-o** flag.

``vasm.exe tests/1.vasm -o project``
``vasm.exe -o project tests/1.vasm``

Both cases generate *project.vasmexec* file.

Other flags:

- **-v** Select verbosity level