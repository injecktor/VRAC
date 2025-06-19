# Architecture

Word (machine word) = 32 bits.

Half word = 16 bits.

1 byte = 8 bits.

Little endian.

## ISA

### Common instructions

|Instruction|Decode|Opcode with literals|Opcode with registers|Description|
|:-|:-|:-:|:-:|:-:|
|cmp|compare|||Sets flags *eq* and *le*|
|j|jump|||Sets flags *eq* and *le*|
|je|jump if equal|||Sets flags *eq* and *le*|
|jne|jump if not equal|||Sets flags *eq* and *le*|
|jg|jump if greater|||Sets flags *eq* and *le*|
|jge|jump if greater or equal|||Sets flags *eq* and *le*|
|jl|jump if less|||Sets flags *eq* and *le*|
|jle|jump if less or equal|||Sets flags *eq* and *le*|
|add|add||||
|min|minus||||
|mul|multiplication||||
|div|division||||
|or|logical *or*||||
|nor|logical *nor*||||
|xor|logical *xor*||||
|and|logical *and*||||
|nand|logical *nand*||||
|xnand|logical *xnand*||||
|shl|shift left||||
|shr|shift right||||
|sw|store word||||
|shw|store half word||||
|sb|store byte||||
|lw|load word||||
|lhw|load half word||||
|lb|load byte||||
|cpw|copy word||||
|cphw|copy half word|||Copies 16 LSB|
|cpb|copy byte|||Copies 8 LSB|

### Signed specific instructions

|Instruction|Decode|Opcode with literals|Opcode with registers|Description|
|:-|:-|:-:|:-:|:-:|
|adds|add as signed||||
|mins|minus as signed||||
|muls|multiplication as signed||||
|divs|division as signed||||
|abs|absolute value||| Treats as 32 bit signed value|
|shrs|shift right as signed||||


### Float point specific instructions

|Instruction|Decode|Opcode with literals|Opcode with registers|Description|
|:-|:-|:-:|:-:|:-:|
|addf|add as float point|||Treats as signed value|
|minf|minus as float point|||Treats as signed value|
|mulf|multiplication as float point|||Treats as signed value|
|divf|division as float point|||Treats as signed value|
|absf|absolute value as float point|||Treats as 32 bit signed float point|

## Flags

Flags serve to contain extra information.

eq (last compare was equal)
le (last compare was less)
of (last action had overflow)

## Registers

VRAC has 16 general purpose registers r0-r15

7 джампов + 7 для регистров

## Machine code

- ST-type (storeload) (2 bytes)

    |opcode/5|register/4|
    |:-:|:-:|
    |xxxxx|xxxx|

- ALU-type (2-4 bytes)

    - Type 1 (2 bytes)

        |opcode/5|logic/6|register/4|
        |:-:|:-:|:-:|
        |xxxxx|xxxxx0|xxxx|

    - Type 2 (3 bytes)

        |opcode/5|logic/6|register1/4|register2/4|
        |:-:|:-:|:-:|:-:|
        |xxxxx|xxxxx0|xxxx|xxxx|

    - Type 3 (4 bytes)

        |opcode/5|logic/6|register/4|literal/17|
        |:-:|:-:|:-:|:-:|
        |xxxxx|xxxxx1|xxxx|xxxxxxxxxxxxxxxxx|

- J-type (jump) (4 bytes)

    |opcode/5|literal/27|
    |:-:|:-:|
    |xxxxx|xxxxxxxxxxxxxxxxxxxxxxxxxxx|

opcode
alu 00000
sw 00001
shw 00010
sb 00011
lw 00100
lhw 00101
lb 00110
cpw 00111
cphw 01000
cpb 01001
j 10000-11101

logic 
add 00000
adds 00001
addf 00010

min 00011
mins 00100
minf 00101

mul 00110
muls 00111
mulf 01000

div 01001
divs 01010
divf 01011

shl 01100
shr 01101
shrs 01110

or 01111
nor 10000
xor 10001

and 10010
nand 10011
xnand 10001 same as xor

abs 10100
absf 10101

cmp 10110