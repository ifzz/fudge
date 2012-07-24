#include <elf.h>
#include <memory.h>
#include <string.h>

unsigned int elf_validate(struct elf_header *header)
{

    if (header->identify[0] != ELF_IDENTITY_MAGIC0)
        return 0;

    if (header->identify[1] != ELF_IDENTITY_MAGIC1)
        return 0;

    if (header->identify[2] != ELF_IDENTITY_MAGIC2)
        return 0;

    if (header->identify[3] != ELF_IDENTITY_MAGIC3)
        return 0;

    return 1;

}

struct elf_section_header *elf_get_section(struct elf_header *header, struct elf_section_header *sectionHeader, unsigned int type)
{

    unsigned int i;

    for (i = 0; i < header->shcount; i++)
    {

        if (sectionHeader[i].type == type)
            return &sectionHeader[i];

    }

    return 0;

}

unsigned int elf_find_symbol(struct elf_header *header, struct elf_section_header *sheader, struct elf_section_header *symHeader, struct elf_symbol *symTable, char *strTable, char *symbol)
{

    unsigned int i;

    for (i = 0; i < symHeader->size / symHeader->esize; i++)
    {

        struct elf_symbol *symEntry = &symTable[i];

        if (memory_match(symbol, strTable + symEntry->name, string_length(symbol)))
        {

            if (header->type == ELF_TYPE_RELOCATABLE)
                return symEntry->value + sheader[symEntry->shindex].address + sheader[symEntry->shindex].offset;
            else
                return symEntry->value;
            
        }

    }

    return 0;

}

