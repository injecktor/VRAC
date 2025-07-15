# Architecture

Word (machine word) = 32 bits.

Half word = 16 bits.

1 byte = 8 bits.

Little endian.

## ISA

### Common instructions

|Instruction|Decode|Instruction type|Opcode|Alucode|Description|
|:-|:-|:-|:-:|:-:|:-:|
|j|jump|SR-type|10000|||
|j|jump|SL-type|10001|||
|je|jump if equal|SR-type|10010|||
|je|jump if equal|SL-type|10011|||
|jne|jump if not equal|SR-type|10100|||
|jne|jump if not equal|SL-type|10101|||
|jg|jump if greater|SR-type|10110|||
|jg|jump if greater|SL-type|10111|||
|jge|jump if greater or equal|SR-type|11000|||
|jge|jump if greater or equal|SL-type|11001|||
|jl|jump if less|SR-type|11010|||
|jl|jump if less|SL-type|11011|||
|jle|jump if less or equal|SR-type|11100|||
|jle|jump if less or equal|SL-type|11101|||
|sw|store word|SR-type|00001|||
|shw|store half word|SR-type|00010|||
|sb|store byte|SR-type|00011|||
|lw|load word|SR-type|00100|||
|lhw|load half word|SR-type|00101|||
|lb|load byte|SR-type|00110|||
|cpw|copy word|DR-type|00111|||
|cpw|copy word|RL-type|01000|||
|cphw|copy half word|DR-type|01001||Copies 16 LSB|
|cphw|copy half word|RL-type|01010||Copies 16 LSB|
|cpb|copy byte|DR-type|01011||Copies 8 LSB|
|cpb|copy byte|RL-type|01100||Copies 8 LSB|
|add|add|ADR-type|00000|000000||
|add|add|ARL-type|00000|000001||
|min|minus|ADR-type|00000|000110||
|min|minus|ARL-type|00000|000111||
|mul|multiplication|ADR-type|00000|001100||
|mul|multiplication|ARL-type|00000|001101||
|div|division|ADR-type|00000|010010||
|div|division|ARL-type|00000|010011||
|or|logical *or*|ADR-type|00000|011110||
|or|logical *or*|ARL-type|00000|011111||
|nor|logical *nor*|ADR-type|00000|100000||
|nor|logical *nor*|ARL-type|00000|100001||
|xor|logical *xor*|ADR-type|00000|100010||
|xor|logical *xor*|ARL-type|00000|100011||
|and|logical *and*|ADR-type|00000|100100||
|and|logical *and*|ARL-type|00000|100101||
|nand|logical *nand*|ADR-type|00000|100110||
|nand|logical *nand*|ARL-type|00000|100111||
|xnand|logical *xnand*|ADR-type|00000|100010||
|xnand|logical *xnand*|ARL-type|00000|100011||
|shl|shift left|ADR-type|00000|011000||
|shl|shift left|ARL-type|00000|011001||
|shr|shift right|ADR-type|00000|011010||
|shr|shift right|ARL-type|00000|011011||
|cmp|compare|ADR-type|00000|101100|Sets flags *eq* and *le*|
|cmp|compare|ARL-type|00000|101101|Sets flags *eq* and *le*|
|mod|remain of devision|ADR-type|00000|110010||
|mod|remain of devision|ARL-type|00000|110011||

### Signed specific instructions

|Instruction|Decode|Instruction type|Opcode|Alucode|Description|
|:-|:-|:-|:-:|:-:|:-:|
|adds|add as signed|ADR-type|00000|000010||
|adds|add as signed|ARL-type|00000|000011||
|mins|minus as signed|ADR-type|00000|001000||
|mins|minus as signed|ARL-type|00000|001001||
|muls|multiplication as signed|ADR-type|00000|001110||
|muls|multiplication as signed|ARL-type|00000|001111||
|divs|division as signed|ADR-type|00000|010100||
|divs|division as signed|ARL-type|00000|010101||
|abs|absolute value|ASR-type|00000|101000|Treats as 32 bit signed value|
|abs|absolute value|ADR-type|00000|101001|Treats as 32 bit signed value|
|shrs|shift right as signed|ADR-type|00000|011100||
|shrs|shift right as signed|ARL-type|00000|011101||
|cmps|compare as signed|ADR-type|00000|101110|Sets flags *eq* and *le*|
|cmps|compare as signed|ARL-type|00000|101111|Sets flags *eq* and *le*|
|mods|remain of devision as signed|ADR-type|00000|110100||
|mods|remain of devision as signed|ARL-type|00000|110111||

### Float point specific instructions

|Instruction|Decode|Instruction type|Opcode|Alucode|Description|
|:-|:-|:-|:-:|:-:|:-:|
|addf|add as float point|ADR-type|00000|000100|Treats as signed value|
|addf|add as float point|ARL-type|00000|000101|Treats as signed value|
|minf|minus as float point|ADR-type|00000|001010|Treats as signed value|
|minf|minus as float point|ARL-type|00000|001011|Treats as signed value|
|mulf|multiplication as float point|ADR-type|00000|010000|Treats as signed value|
|mulf|multiplication as float point|ARL-type|00000|010001|Treats as signed value|
|divf|division as float point|ADR-type|00000|010110|Treats as signed value|
|divf|division as float point|ARL-type|00000|010111|Treats as signed value|
|absf|absolute value as float point|ASR-type|00000|101010|Treats as 32 bit signed float point|
|absf|absolute value as float point|ADR-type|00000|101011|Treats as 32 bit signed float point|
|cmpf|compare as float point|ADR-type|00000|110000|Sets flags *eq* and *le*|
|cmpf|compare as float point|ARL-type|00000|110001|Sets flags *eq* and *le*|

## Flags

Flags serve to contain extra information.

*eq* (last compare was equal)

*le* (last compare was less)

*of* (last action had overflow)

## Registers

VRAC has 16 general purpose registers r0-r15

## Instruction types

In VRAC all instructions divided in 7 types ("**/**" defines size in bits):

- SL-type (Simple Literal) (4 bytes)

    |opcode/5|literal/27|
    |:-:|:-:|
    |**xxxxx**|**xxxxxxxxxxxxxxxxxxxxxxxxxxx**|

- SR-type (Simple Register) (2 bytes)

    |opcode/5|register/4|unused/7|
    |:-:|:-:|:-:|
    |**xxxxx**|**xxxx**|**xxxxxxx**|

- DR-type (Double Register) (2 bytes)

    |opcode/5|register/4|register/4|unused/3|
    |:-:|:-:|:-:|:-:|
    |**xxxxx**|**xxxx**|**xxxx**|**xxx**|

- RL-type (Register Literal) (4 bytes)

    |opcode/5|register/4|literal/23|
    |:-:|:-:|:-:|
    |**xxxxx**|**xxxx**|**xxxxxxxxxxxxxxxxxxxxxxx**|

- ASR-type (ALU Simple Register) (2 bytes)

    |opcode/5|alucode/6|register/4|unused/1|
    |:-:|:-:|:-:|:-:|
    |**00000**|**xxxxx0**|**xxxx**|**x**|

- ADR-type (ALU Double Register) (3 bytes)

    |opcode/5|alucode/6|register/4|register/4|unused/5|
    |:-:|:-:|:-:|:-:|:-:|
    |**00000**|**xxxxx0**|**xxxx**|**xxxx**|**xxxxx**|

- ARL-type (ALU Register Literal) (4 bytes)

    |opcode/5|alucode/6|register/4|literal/17|
    |:-:|:-:|:-:|:-:|
    |**00000**|**xxxxx1**|**xxxx**|**xxxxxxxxxxxxxxxxx**|
