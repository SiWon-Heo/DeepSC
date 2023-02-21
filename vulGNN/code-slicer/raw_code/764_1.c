static # else auto # endif inline void __attribute__ ( ( unused , always_inline ) ) elf_get_dynamic_info ( struct link_map * l , ElfW ( Dyn ) * temp ) {
 ElfW ( Dyn ) * dyn = l -> l_ld ;
 ElfW ( Dyn ) * * info ;
 # ifndef RTLD_BOOTSTRAP if ( dyn == NULL ) return ;
 # endif info = l -> l_info ;
 while ( dyn -> d_tag != DT_NULL ) {
 if ( ( Elf32_Word ) dyn -> d_tag < DT_NUM ) info [ dyn -> d_tag ] = dyn ;
 else if ( dyn -> d_tag >= DT_LOPROC && dyn -> d_tag < DT_LOPROC + DT_THISPROCNUM ) info [ dyn -> d_tag - DT_LOPROC + DT_NUM ] = dyn ;
 else if ( ( Elf32_Word ) DT_VERSIONTAGIDX ( dyn -> d_tag ) < DT_VERSIONTAGNUM ) info [ VERSYMIDX ( dyn -> d_tag ) ] = dyn ;
 else if ( ( Elf32_Word ) DT_EXTRATAGIDX ( dyn -> d_tag ) < DT_EXTRANUM ) info [ DT_EXTRATAGIDX ( dyn -> d_tag ) + DT_NUM + DT_THISPROCNUM + DT_VERSIONTAGNUM ] = dyn ;
 else if ( ( Elf32_Word ) DT_VALTAGIDX ( dyn -> d_tag ) < DT_VALNUM ) info [ DT_VALTAGIDX ( dyn -> d_tag ) + DT_NUM + DT_THISPROCNUM + DT_VERSIONTAGNUM + DT_EXTRANUM ] = dyn ;
 else if ( ( Elf32_Word ) DT_ADDRTAGIDX ( dyn -> d_tag ) < DT_ADDRNUM ) info [ DT_ADDRTAGIDX ( dyn -> d_tag ) + DT_NUM + DT_THISPROCNUM + DT_VERSIONTAGNUM + DT_EXTRANUM + DT_VALNUM ] = dyn ;
 ++ dyn ;
 }
 # define DL_RO_DYN_TEMP_CNT 8 # ifndef DL_RO_DYN_SECTION if ( l -> l_addr != 0 ) {
 ElfW ( Addr ) l_addr = l -> l_addr ;
 int cnt = 0 ;
 # define ADJUST_DYN_INFO ( tag ) do if ( info [ tag ] != NULL ) {
 if ( temp ) {
 temp [ cnt ] . d_tag = info [ tag ] -> d_tag ;
 temp [ cnt ] . d_un . d_ptr = info [ tag ] -> d_un . d_ptr + l_addr ;
 info [ tag ] = temp + cnt ++ ;
 }
 else info [ tag ] -> d_un . d_ptr += l_addr ;
 }
 while ( 0 ) ADJUST_DYN_INFO ( DT_HASH ) ;
 ADJUST_DYN_INFO ( DT_PLTGOT ) ;
 ADJUST_DYN_INFO ( DT_STRTAB ) ;
 ADJUST_DYN_INFO ( DT_SYMTAB ) ;
 # if ! ELF_MACHINE_NO_RELA ADJUST_DYN_INFO ( DT_RELA ) ;
 # endif # if ! ELF_MACHINE_NO_REL ADJUST_DYN_INFO ( DT_REL ) ;
 # endif ADJUST_DYN_INFO ( DT_JMPREL ) ;
 ADJUST_DYN_INFO ( VERSYMIDX ( DT_VERSYM ) ) ;
 ADJUST_DYN_INFO ( DT_ADDRTAGIDX ( DT_GNU_HASH ) + DT_NUM + DT_THISPROCNUM + DT_VERSIONTAGNUM + DT_EXTRANUM + DT_VALNUM ) ;
 # undef ADJUST_DYN_INFO assert ( cnt <= DL_RO_DYN_TEMP_CNT ) ;
 }
 # endif if ( info [ DT_PLTREL ] != NULL ) {
 # if ELF_MACHINE_NO_RELA assert ( info [ DT_PLTREL ] -> d_un . d_val == DT_REL ) ;
 # elif ELF_MACHINE_NO_REL assert ( info [ DT_PLTREL ] -> d_un . d_val == DT_RELA ) ;
 # else assert ( info [ DT_PLTREL ] -> d_un . d_val == DT_REL || info [ DT_PLTREL ] -> d_un . d_val == DT_RELA ) ;
 # endif }
 # if ! ELF_MACHINE_NO_RELA if ( info [ DT_RELA ] != NULL ) assert ( info [ DT_RELAENT ] -> d_un . d_val == sizeof ( ElfW ( Rela ) ) ) ;
 # endif # if ! ELF_MACHINE_NO_REL if ( info [ DT_REL ] != NULL ) assert ( info [ DT_RELENT ] -> d_un . d_val == sizeof ( ElfW ( Rel ) ) ) ;
 # endif # ifdef RTLD_BOOTSTRAP assert ( info [ VERSYMIDX ( DT_FLAGS_1 ) ] == NULL || info [ VERSYMIDX ( DT_FLAGS_1 ) ] -> d_un . d_val == DF_1_NOW ) ;
 assert ( info [ DT_FLAGS ] == NULL || info [ DT_FLAGS ] -> d_un . d_val == DF_BIND_NOW ) ;
 assert ( info [ DT_RUNPATH ] == NULL ) ;
 assert ( info [ DT_RPATH ] == NULL ) ;
 # else if ( info [ DT_FLAGS ] != NULL ) {
 l -> l_flags = info [ DT_FLAGS ] -> d_un . d_val ;
 if ( l -> l_flags & DF_SYMBOLIC ) info [ DT_SYMBOLIC ] = info [ DT_FLAGS ] ;
 if ( l -> l_flags & DF_TEXTREL ) info [ DT_TEXTREL ] = info [ DT_FLAGS ] ;
 if ( l -> l_flags & DF_BIND_NOW ) info [ DT_BIND_NOW ] = info [ DT_FLAGS ] ;
 }
 if ( info [ VERSYMIDX ( DT_FLAGS_1 ) ] != NULL ) {
 l -> l_flags_1 = info [ VERSYMIDX ( DT_FLAGS_1 ) ] -> d_un . d_val ;
 if ( l -> l_flags_1 & DF_1_NOW ) info [ DT_BIND_NOW ] = info [ VERSYMIDX ( DT_FLAGS_1 ) ] ;
 }
 if ( info [ DT_RUNPATH ] != NULL ) info [ DT_RPATH ] = NULL ;
 # endif }