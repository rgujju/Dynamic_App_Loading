# ELF2TINF

## Brief

Convert ELF files into Tiny binary format file (TINF)
Create the binary and header file of the app in tiny binary format

## TINF

 Tiny Bin Format (TINF)
 Field name            | Field size bits (bytes)    | Meaning
 ----------------------|----------------------------|-------------------------------------------------------------
  magic                | 32  (4)                    | Always 'TINF' to identify App
  app_name             | 128 (16)                   | Name of App (Actual length allowed is 15 characters)
  major_version        | 8   (1)                    | Major version of App 
  minor_version        | 8   (1)                    | Minor version of App 
  text_size            | 16  (2)                    | Size of text section in words (loaded onto flash)
  data_size            | 16  (2)                    | Size of data section in words (loaded onto RAM) 
  bss_size             | 16  (2)                    | Size of the bss section in words (loaded onto RAM) 
  num_got_entries      | 32  (4)                    | Number of GOT entries
  .text section        |                            | The .text section from the elf 
  .data section        |                            | The .data section from the elf
  .got section         |                            | The .got section from the elf

## Requirements
Requires https://github.com/eliben/pyelftools

## Usage:
usage: elf2tinf.py [-h] [--major MAJOR] [--minor MINOR] elf_file app_name
positional arguments:
  elf_file       Elf file to convert
  app_name       Name of the app, should be less than 15 characters

optional arguments:
  -h, --help     show this help message and exit
  --major MAJOR  Major version of app. default:1
  --minor MINOR  Minor version of app. default:0

