# AX-08
An 8-bit computer written in C++ for windows computers.
Can be used as a learning tool for children, or to allow
them to understand the intricacies of computer processors
or architecture.

### Warning: Currently In DEVELOPMENT, project not complete.

## How to use:
To compile run `ax08comp <file> <rom_output>` [IN DEVELOPMENT]

To emulate run `ax08emu <rom>` [NOT IMPLEMENTED]

## Added:
### version 0.2
- Finished the parser instruction listing (all instructions work now)
- Corrected the ROM Image size bug
- Removed the "adc" instruction as it was not needed
- Remapped a few instructions
- "sub" instruction now assigned to 0x01 and not 0xF0
- Edited spec.txt slightly to better reflect the current state of the compiler
### version 0.1
- Added `@org` preprocessor directive
- Added the ability to output rom files
- Revamp'd the parser system - now successfully ouputs the correct binary