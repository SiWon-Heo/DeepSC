static int isoent_collect_dirs ( struct vdd * vdd , struct isoent * rootent , int depth ) {
 struct isoent * np ;
 if ( rootent == NULL ) rootent = vdd -> rootent ;
 np = rootent ;
 do {
 path_table_add_entry ( & ( vdd -> pathtbl [ depth ] ) , np ) ;
 if ( np -> subdirs . first != NULL && depth + 1 < vdd -> max_depth ) {
 np = np -> subdirs . first ;
 depth ++ ;
 continue ;
 }
 while ( np != rootent ) {
 if ( np -> drnext == NULL ) {
 np = np -> parent ;
 depth -- ;
 }
 else {
 np = np -> drnext ;
 break ;
 }
 }
 }
 while ( np != rootent ) ;
 return ( ARCHIVE_OK ) ;
 }