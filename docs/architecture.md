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
|sw|store word||||
|shw|store half word||||
|sb|store byte||||
|lw|load word||||
|lhw|load half word||||
|lb|load byte||||
|cpw|copy word||||
|cphw|copy half word|||Copies 16 LSB|
|cpb|copy byte|||Copies 8 LSB|
|and|logical *and*||||
|nand|logical *nand*||||
|xnand|logical *xnand*||||
|or|logical *or*||||
|nor|logical *nor*||||
|xor|logical *xor*||||
|shl|shift left||||
|shr|shift right||||

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