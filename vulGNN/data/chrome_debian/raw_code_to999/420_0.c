static int dwarf_elf_object_relocate_a_section ( void * obj_in , Dwarf_Half section_index , Dwarf_Debug dbg , int * error ) {
 int res = DW_DLV_ERROR ;
 dwarf_elf_object_access_internals_t * obj = 0 ;
 struct Dwarf_Section_s * relocatablesec = 0 ;
 if ( section_index == 0 ) {
 return DW_DLV_NO_ENTRY ;
 }
 obj = ( dwarf_elf_object_access_internals_t * ) obj_in ;
 res = find_section_to_relocate ( dbg , section_index , & relocatablesec , error ) ;
 if ( res != DW_DLV_OK ) {
 return res ;
 }
 if ( relocatablesec -> dss_reloc_index == 0 ) {
 * error = DW_DLE_RELOC_SECTION_MISSING_INDEX ;
 return DW_DLV_ERROR ;
 }
 res = dwarf_elf_object_access_load_section ( obj_in , relocatablesec -> dss_reloc_index , & relocatablesec -> dss_reloc_data , error ) ;
 if ( res != DW_DLV_OK ) {
 return res ;
 }
 if ( ! obj -> symtab ) {
 obj -> symtab = & dbg -> de_elf_symtab ;
 obj -> strtab = & dbg -> de_elf_strtab ;
 }
 if ( obj -> symtab -> dss_index != relocatablesec -> dss_reloc_link ) {
 * error = DW_DLE_RELOC_MISMATCH_RELOC_INDEX ;
 return DW_DLV_ERROR ;
 }
 if ( obj -> strtab -> dss_index != obj -> symtab -> dss_link ) {
 * error = DW_DLE_RELOC_MISMATCH_STRTAB_INDEX ;
 return DW_DLV_ERROR ;
 }
 if ( ! obj -> symtab -> dss_data ) {
 res = dwarf_elf_object_access_load_section ( obj_in , obj -> symtab -> dss_index , & obj -> symtab -> dss_data , error ) ;
 if ( res != DW_DLV_OK ) {
 return res ;
 }
 }
 if ( ! obj -> strtab -> dss_data ) {
 res = dwarf_elf_object_access_load_section ( obj_in , obj -> strtab -> dss_index , & obj -> strtab -> dss_data , error ) ;
 if ( res != DW_DLV_OK ) {
 return res ;
 }
 }
 res = loop_through_relocations ( dbg , obj , relocatablesec , error ) ;
 return res ;
 }