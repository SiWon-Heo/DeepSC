static int archive_read_format_cpio_read_header ( struct archive_read * a , struct archive_entry * entry ) {
 struct cpio * cpio ;
 const void * h ;
 struct archive_string_conv * sconv ;
 size_t namelength ;
 size_t name_pad ;
 int r ;
 cpio = ( struct cpio * ) ( a -> format -> data ) ;
 sconv = cpio -> opt_sconv ;
 if ( sconv == NULL ) {
 if ( ! cpio -> init_default_conversion ) {
 cpio -> sconv_default = archive_string_default_conversion_for_read ( & ( a -> archive ) ) ;
 cpio -> init_default_conversion = 1 ;
 }
 sconv = cpio -> sconv_default ;
 }
 r = ( cpio -> read_header ( a , cpio , entry , & namelength , & name_pad ) ) ;
 if ( r < ARCHIVE_WARN ) return ( r ) ;
 h = __archive_read_ahead ( a , namelength + name_pad , NULL ) ;
 if ( h == NULL ) return ( ARCHIVE_FATAL ) ;
 if ( archive_entry_copy_pathname_l ( entry , ( const char * ) h , namelength , sconv ) != 0 ) {
 if ( errno == ENOMEM ) {
 archive_set_error ( & a -> archive , ENOMEM , "Can't allocate memory for Pathname" ) ;
 return ( ARCHIVE_FATAL ) ;
 }
 archive_set_error ( & a -> archive , ARCHIVE_ERRNO_FILE_FORMAT , "Pathname can't be converted from %s to current locale." , archive_string_conversion_charset_name ( sconv ) ) ;
 r = ARCHIVE_WARN ;
 }
 cpio -> entry_offset = 0 ;
 __archive_read_consume ( a , namelength + name_pad ) ;
 if ( archive_entry_filetype ( entry ) == AE_IFLNK ) {
 h = __archive_read_ahead ( a , ( size_t ) cpio -> entry_bytes_remaining , NULL ) ;
 if ( h == NULL ) return ( ARCHIVE_FATAL ) ;
 if ( archive_entry_copy_symlink_l ( entry , ( const char * ) h , ( size_t ) cpio -> entry_bytes_remaining , sconv ) != 0 ) {
 if ( errno == ENOMEM ) {
 archive_set_error ( & a -> archive , ENOMEM , "Can't allocate memory for Linkname" ) ;
 return ( ARCHIVE_FATAL ) ;
 }
 archive_set_error ( & a -> archive , ARCHIVE_ERRNO_FILE_FORMAT , "Linkname can't be converted from %s to " "current locale." , archive_string_conversion_charset_name ( sconv ) ) ;
 r = ARCHIVE_WARN ;
 }
 __archive_read_consume ( a , cpio -> entry_bytes_remaining ) ;
 cpio -> entry_bytes_remaining = 0 ;
 }
 if ( namelength == 11 && strcmp ( ( const char * ) h , "TRAILER!!!" ) == 0 ) {
 archive_clear_error ( & a -> archive ) ;
 return ( ARCHIVE_EOF ) ;
 }
 if ( record_hardlink ( a , cpio , entry ) != ARCHIVE_OK ) {
 return ( ARCHIVE_FATAL ) ;
 }
 return ( r ) ;
 }