# VRAC
VRAC (Virtual RISC Architecture CPU) is a project that demonstrates how microarchitectures look like.

## How to use

## Implementation details

### Architecture

#### ISA

cmp (compare)

j (jump)
je (jump if equal)
jne (jump if not equal)
jg (jump if greater)
jl (jump if less)
jge (jump if greater or equal)
jle (jump if less or equal)

add
adds (add as signed)
min (minus)
mins (minus as signed)
mul (multiplication)
muls (multiplication as signed)
div (division)
divs (division as signed)
abs (absolute value)

sw (store word)
lw (load word)

cp (copy value)

and
nand
xnand
or
nor
xor
xnor

shl (shift left)
shr (shift right)
shrs (shift right as signed)

#### Flags

Flags serve to contain extra information.

eq (last compare is equal)
le (last compare is less)
of (last action has overflow)

#### Registers

VRAC has 16 general purpose registers r0-r15

### Microarchitecture

### Vassemble
