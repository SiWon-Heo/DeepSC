static int dwarf_elf_object_access_internals_init ( void * obj_in , dwarf_elf_handle elf , int * error ) {
 dwarf_elf_object_access_internals_t * obj = ( dwarf_elf_object_access_internals_t * ) obj_in ;
 char * ehdr_ident = 0 ;
 Dwarf_Half machine = 0 ;
 obj -> elf = elf ;
 if ( ( ehdr_ident = elf_getident ( elf , NULL ) ) == NULL ) {
 * error = DW_DLE_ELF_GETIDENT_ERROR ;
 return DW_DLV_ERROR ;
 }
 obj -> is_64bit = ( ehdr_ident [ EI_CLASS ] == ELFCLASS64 ) ;
 if ( ehdr_ident [ EI_DATA ] == ELFDATA2LSB ) {
 obj -> endianness = DW_OBJECT_LSB ;
 }
 else if ( ehdr_ident [ EI_DATA ] == ELFDATA2MSB ) {
 obj -> endianness = DW_OBJECT_MSB ;
 }
 if ( obj -> is_64bit ) {
 # ifdef HAVE_ELF64_GETEHDR obj -> ehdr64 = elf64_getehdr ( elf ) ;
 if ( obj -> ehdr64 == NULL ) {
 * error = DW_DLE_ELF_GETEHDR_ERROR ;
 return DW_DLV_ERROR ;
 }
 obj -> section_count = obj -> ehdr64 -> e_shnum ;
 machine = obj -> ehdr64 -> e_machine ;
 obj -> machine = machine ;
 # else * error = DW_DLE_NO_ELF64_SUPPORT ;
 return DW_DLV_ERROR ;
 # endif }
 else {
 obj -> ehdr32 = elf32_getehdr ( elf ) ;
 if ( obj -> ehdr32 == NULL ) {
 * error = DW_DLE_ELF_GETEHDR_ERROR ;
 return DW_DLV_ERROR ;
 }
 obj -> section_count = obj -> ehdr32 -> e_shnum ;
 machine = obj -> ehdr32 -> e_machine ;
 obj -> machine = machine ;
 }
 obj -> length_size = obj -> is_64bit ? 8 : 4 ;
 obj -> pointer_size = obj -> is_64bit ? 8 : 4 ;
 # ifdef WIN32 if ( obj -> is_64bit && machine == EM_PPC64 ) {
 obj -> pointer_size = 4 ;
 }
 # endif if ( obj -> is_64bit && machine != EM_MIPS ) {
 obj -> length_size = 4 ;
 }
 return DW_DLV_OK ;
 }