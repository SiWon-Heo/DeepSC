static int dwarf_elf_object_access_load_section ( void * obj_in , Dwarf_Half section_index , Dwarf_Small * * section_data , int * error ) {
 dwarf_elf_object_access_internals_t * obj = ( dwarf_elf_object_access_internals_t * ) obj_in ;
 if ( section_index == 0 ) {
 return DW_DLV_NO_ENTRY ;
 }
 {
 Elf_Scn * scn = 0 ;
 Elf_Data * data = 0 ;
 scn = elf_getscn ( obj -> elf , section_index ) ;
 if ( scn == NULL ) {
 * error = DW_DLE_MDE ;
 return DW_DLV_ERROR ;
 }
 data = elf_getdata ( scn , NULL ) ;
 if ( data == NULL ) {
 * error = DW_DLE_MDE ;
 return DW_DLV_ERROR ;
 }
 * section_data = data -> d_buf ;
 }
 return DW_DLV_OK ;
 }