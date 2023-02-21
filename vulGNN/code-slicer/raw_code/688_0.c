SPL_METHOD ( RecursiveDirectoryIterator , hasChildren ) {
 zend_bool allow_links = 0 ;
 spl_filesystem_object * intern = ( spl_filesystem_object * ) zend_object_store_get_object ( getThis ( ) TSRMLS_CC ) ;
 if ( zend_parse_parameters ( ZEND_NUM_ARGS ( ) TSRMLS_CC , "|b" , & allow_links ) == FAILURE ) {
 return ;
 }
 if ( spl_filesystem_is_invalid_or_dot ( intern -> u . dir . entry . d_name ) ) {
 RETURN_FALSE ;
 }
 else {
 spl_filesystem_object_get_file_name ( intern TSRMLS_CC ) ;
 if ( ! allow_links && ! ( intern -> flags & SPL_FILE_DIR_FOLLOW_SYMLINKS ) ) {
 php_stat ( intern -> file_name , intern -> file_name_len , FS_IS_LINK , return_value TSRMLS_CC ) ;
 if ( zend_is_true ( return_value ) ) {
 RETURN_FALSE ;
 }
 }
 php_stat ( intern -> file_name , intern -> file_name_len , FS_IS_DIR , return_value TSRMLS_CC ) ;
 }
 }