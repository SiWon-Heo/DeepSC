static void get_rela_elf64 ( Dwarf_Small * data , unsigned int i , int endianness , int machine , struct Dwarf_Elf_Rela * relap ) {
 # ifdef HAVE_ELF64_RELA Elf64_Rela * relp = ( Elf64_Rela * ) ( data + ( i * sizeof ( Elf64_Rela ) ) ) ;
 relap -> r_offset = relp -> r_offset ;
 # define ELF64MIPS_REL_SYM ( i ) ( ( i ) & 0xffffffff ) # define ELF64MIPS_REL_TYPE ( i ) ( ( i >> 56 ) & 0xff ) if ( machine == EM_MIPS && endianness == DW_OBJECT_LSB ) {
 relap -> r_type = ELF64MIPS_REL_TYPE ( relp -> r_info ) ;
 relap -> r_symidx = ELF64MIPS_REL_SYM ( relp -> r_info ) ;
 # undef MIPS64SYM # undef MIPS64TYPE }
 else {
 relap -> r_type = ELF64_R_TYPE ( relp -> r_info ) ;
 relap -> r_symidx = ELF64_R_SYM ( relp -> r_info ) ;
 }
 relap -> r_addend = relp -> r_addend ;
 # endif }