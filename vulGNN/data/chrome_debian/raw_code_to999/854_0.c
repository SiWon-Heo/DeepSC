static int isoent_make_path_table ( struct archive_write * a ) {
 struct iso9660 * iso9660 = a -> format_data ;
 int depth , r ;
 int dir_number ;
 if ( iso9660 -> dircnt_max >= MAX_DEPTH && ( ! iso9660 -> opt . limit_depth || iso9660 -> opt . iso_level == 4 ) ) r = isoent_alloc_path_table ( a , & ( iso9660 -> primary ) , iso9660 -> dircnt_max + 1 ) ;
 else r = isoent_alloc_path_table ( a , & ( iso9660 -> primary ) , MAX_DEPTH ) ;
 if ( r < 0 ) return ( r ) ;
 if ( iso9660 -> opt . joliet ) {
 r = isoent_alloc_path_table ( a , & ( iso9660 -> joliet ) , iso9660 -> dircnt_max + 1 ) ;
 if ( r < 0 ) return ( r ) ;
 }
 isoent_collect_dirs ( & ( iso9660 -> primary ) , NULL , 0 ) ;
 if ( iso9660 -> opt . joliet ) isoent_collect_dirs ( & ( iso9660 -> joliet ) , NULL , 0 ) ;
 if ( iso9660 -> opt . rr ) {
 r = isoent_rr_move ( a ) ;
 if ( r < 0 ) return ( r ) ;
 }
 isofile_connect_hardlink_files ( iso9660 ) ;
 r = isoent_traverse_tree ( a , & ( iso9660 -> primary ) ) ;
 if ( r < 0 ) return ( r ) ;
 if ( iso9660 -> opt . joliet ) {
 r = isoent_traverse_tree ( a , & ( iso9660 -> joliet ) ) ;
 if ( r < 0 ) return ( r ) ;
 }
 dir_number = 1 ;
 for ( depth = 0 ;
 depth < iso9660 -> primary . max_depth ;
 depth ++ ) {
 r = isoent_make_path_table_2 ( a , & ( iso9660 -> primary ) , depth , & dir_number ) ;
 if ( r < 0 ) return ( r ) ;
 }
 if ( iso9660 -> opt . joliet ) {
 dir_number = 1 ;
 for ( depth = 0 ;
 depth < iso9660 -> joliet . max_depth ;
 depth ++ ) {
 r = isoent_make_path_table_2 ( a , & ( iso9660 -> joliet ) , depth , & dir_number ) ;
 if ( r < 0 ) return ( r ) ;
 }
 }
 if ( iso9660 -> opt . limit_dirs && dir_number > 0xffff ) {
 archive_set_error ( & a -> archive , ARCHIVE_ERRNO_MISC , "Too many directories(%d) over 65535." , dir_number ) ;
 return ( ARCHIVE_FATAL ) ;
 }
 calculate_path_table_size ( & ( iso9660 -> primary ) ) ;
 if ( iso9660 -> opt . joliet ) calculate_path_table_size ( & ( iso9660 -> joliet ) ) ;
 return ( ARCHIVE_OK ) ;
 }