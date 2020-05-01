import sys
import argparse
import logging
import struct

from elftools.elf.elffile import ELFFile
from elftools import elf

# Tiny Bin Format (TINF)
# +----------------------+----------------------------+-------------------------------------------------------------+
# | Field name           | Field size bits (bytes)    | Meaning                                                     |
# +----------------------+----------------------------+-------------------------------------------------------------+
# | magic         	 | 32  (4)                    | Always 'TINF' to identify App                               |
# | app_name      	 | 128 (16)                   | Name of App (Actual length allowed is 27)                   |
# | major_version 	 | 8   (1)                    | Major version of App                                        |
# | minor_version 	 | 8   (1)                    | Minor version of App                                        |
# | text_size     	 | 16  (2)                    | Size of text section in words (loaded onto flash)           |
# | data_size     	 | 16  (2)                    | Size of data section in words (loaded onto RAM)             |
# | bss_size      	 | 16  (2)                    | Size of the bss section in words (loaded onto RAM)          |
# | num_got_entries      | 32  (4)                    | Number of GOT entries                                       |
# | .text section        |                            | The .text section from the elf                              |
# | .data section        |                            | The .data section from the elf                              |
# | .got section         |                            | The .got section from the elf                               |
# +---------------+-----------------------------------+-------------------------------------------------------------+

text_section = '.text'
data_section = '.data'
bss_section = '.bss'
got_section = '.got'
symtab_section = '.symtab'

def ConvertFile(elf_file, app_name, major_version=1, minor_version=0):
    logging.info("Coverting "+elf_file+" to TINF")

    if(len(app_name)<=15):
        logging.debug("Length of app_name is "+str(len(app_name)))
        tin = bytearray("TINF",encoding='utf8')
        tin.extend(app_name.encode('utf8'))
        # Pad 0s to make app_name length = 15(app_name)+1(0)
        # Last byte should be 0, for things like printf
        for i in range (16-len(app_name)):
            tin.append(0) # RGCHECK: check if 0 actually corresponds to NULL in C
    else:
        logging.error("App name is greater than 15 characters.")
        return -1

    # Since version fields are 8 bits log, value of it cannot be greater than 255
    if(major_version > 255 or minor_version > 255):
        tin.error("Version cannot be greater than 255")
        return -1
    # Add the major version of the app
    logging.debug("Setting major version to "+str(major_version))
    tin+=struct.pack('<B',major_version)
    # Add the minor version of the app
    logging.debug("Setting minor version to "+str(minor_version))
    tin+=struct.pack('<B',minor_version)

    # Open the input elf file
    with open(elf_file, 'rb') as f:
        # Read elf file
        elffile = ELFFile(f)

        #TODO: Check if elf class is ELF32 and type is EXEC and Machine is ARM


        # Fetch the .text section, this is the section which has all the code
        # It will be kept on the flash on the mcu
        # In our linker script we have put .rodata into text section itself
        # so dont need to handle that separately.
        text = elffile.get_section_by_name(text_section)
        if not isinstance(text, elf.sections.Section):
            logging.error("ELF file has no "+text_section+" section")
            return -1

        # Get the size of text section
        text_section_size = text['sh_size']
        logging.info(text_section+" section is of size "+str(text_section_size)+" bytes or "+str(text_section_size/4)+" 32 bit words")
        # Add size of text section to the header in words (since it is 32 bit system we divide by 4 to get number of words)
        if(text_section_size%4 != 0):
            text_section_size = (text_section_size +3)&-4
        tin+=struct.pack('<H',int(text_section_size/4))

        # Fetch the .data section, this section has content which will go into RAM
        data = elffile.get_section_by_name(data_section)
        if not isinstance(data, elf.sections.Section):
            logging.error("ELF file has no "+data_section+" section")
            return -1

        # Get the size of data section
        data_section_size = data['sh_size']
        logging.info(data_section+" section is of size "+str(data_section_size)+" bytes or "+str(data_section_size/4)+" 32 bit words")
        # Add size of data section to the header in words (since it is 32 bit system we divide by 4 to get number of words)
        tin+=struct.pack('<H',int(data_section_size/4))

        # Fetch the .bss section, this section has content which is 0 and goes into RAM
        # like uninitialzed global variables
        bss = elffile.get_section_by_name(bss_section)
        if not isinstance(bss, elf.sections.Section):
            logging.error("ELF file has no "+bss_section+" section")
            return -1

        # Get the size of bss section
        bss_section_size = bss['sh_size']
        logging.info(bss_section+" section is of size "+str(bss_section_size)+" bytes or "+str(bss_section_size/4)+" 32 bit words")
        # Add size of bss section to the header in words (since it is 32 bit system we divide by 4 to get number of words)
        tin+=struct.pack('<H',int(bss_section_size/4))

#        # Fetch the symbol table to get the GOT address.
#        symtab = elffile.get_section_by_name(symtab_section)
#        if not isinstance(symtab, elf.sections.SymbolTableSection):
#            logging.error("ELF file has no "+symtab+" section")
#            return -1
#        # Fetch symbol app_sgot which is defined in the linker script
#        # This address is the location of the base of Global offset table of the app
#        # This value will be loaded in to R9 register before launching of the app
#        # All global data access will be through this table.
#        app_sgot = symtab.get_symbol_by_name('app_sgot')
#        if(app_sgot == None):
#            logging.error("ELF file has no 'app_sgot' symbol. Probably wrong linker script used.")
#            return -1
#        app_sgot_value = (app_sgot[0].entry)['st_value']
#        logging.debug("Found symbol app_sgot with value "+ str(app_sgot_value))
#
#        # Add value of GOT to the header
#        tin+=struct.pack('<I',app_sgot_value)

        # Fetch the .got section, this section has the GOT table
        got = elffile.get_section_by_name(got_section)
        if not isinstance(got, elf.sections.Section):
            logging.error("ELF file has no "+got_section+" section")
            return -1

        # Get the size of got section
        got_section_size = got['sh_size']
        logging.info(got_section+" section is of size "+str(got_section_size)+" bytes or "+str(got_section_size/4)+" 32 bit words")
        # Add size of got section to the header in words (since it is 32 bit system we divide by 4 to get number of words)
        tin+=struct.pack('<I',int(got_section_size/4))
	# This is nothing but the number of got entries


        # Add the text section to the img
        #print(text_data())
        tin+=text.data()

        # Add the data section to the img
        tin+=data.data()

	# Add the GOT section to the img
        tin+=got.data()

        # TODO Perform sanity checks regarding size of text, data, etc

    tin_file = elf_file+".tinf"
    with open(tin_file, "wb") as f:
        f.write(tin)

    header_file = elf_file+"_tinf.h"
    with open(header_file,"w") as f:
        f.write("const uint8_t app[] = {")
        #for i in range (0,(text_section_size+data_section_size+got_section_size)*4):
        for i,hex_val in enumerate(tin):
            f.write(hex(hex_val))
            f.write(",")
            if(i%10==0):
                f.write("\n")
        f.write("};")

    return 0;

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("elf_file", help="Elf file to convert")
    parser.add_argument("app_name", help="Name of the app, should be less than 15 characters")
    parser.add_argument("--major", help="Major version of app. default:1",type=int)
    parser.add_argument("--minor", help="Minor version of app. default:0",type=int)
    args = parser.parse_args()
    logging.basicConfig(level=logging.DEBUG)

    if(ConvertFile(args.elf_file,args.app_name,args.major,args.minor)<0):
        logging.error("TINF creation failed")
