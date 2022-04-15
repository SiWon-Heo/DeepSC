static int write_file_descriptors ( struct archive_write * a ) {
 struct iso9660 * iso9660 = a -> format_data ;
 struct isofile * file ;
 int64_t blocks , offset ;
 int r ;
 blocks = 0 ;
 offset = 0 ;
 if ( iso9660 -> el_torito . catalog != NULL ) {
 r = make_boot_catalog ( a ) ;
 if ( r < 0 ) return ( r ) ;
 }
 if ( iso9660 -> el_torito . boot != NULL ) {
 file = iso9660 -> el_torito . boot -> file ;
 blocks = file -> content . blocks ;
 offset = file -> content . offset_of_temp ;
 if ( offset != 0 ) {
 r = write_file_contents ( a , offset , blocks << LOGICAL_BLOCK_BITS ) ;
 if ( r < 0 ) return ( r ) ;
 blocks = 0 ;
 offset = 0 ;
 }
 }
 for ( file = iso9660 -> data_file_list . first ;
 file != NULL ;
 file = file -> datanext ) {
 if ( ! file -> write_content ) continue ;
 if ( ( offset + ( blocks << LOGICAL_BLOCK_BITS ) ) < file -> content . offset_of_temp ) {
 if ( blocks > 0 ) {
 r = write_file_contents ( a , offset , blocks << LOGICAL_BLOCK_BITS ) ;
 if ( r < 0 ) return ( r ) ;
 }
 blocks = 0 ;
 offset = file -> content . offset_of_temp ;
 }
 file -> cur_content = & ( file -> content ) ;
 do {
 blocks += file -> cur_content -> blocks ;
 file -> cur_content = file -> cur_content -> next ;
 }
 while ( file -> cur_content != NULL ) ;
 }
 if ( blocks > 0 ) {
 r = write_file_contents ( a , offset , blocks << LOGICAL_BLOCK_BITS ) ;
 if ( r < 0 ) return ( r ) ;
 }
 return ( ARCHIVE_OK ) ;
 }