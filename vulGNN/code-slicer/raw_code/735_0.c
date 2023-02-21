static int spl_filesystem_file_read_line_ex ( zval * this_ptr , spl_filesystem_object * intern , int silent TSRMLS_DC ) {
 zval * retval = NULL ;
 if ( SPL_HAS_FLAG ( intern -> flags , SPL_FILE_OBJECT_READ_CSV ) || intern -> u . file . func_getCurr -> common . scope != spl_ce_SplFileObject ) {
 if ( php_stream_eof ( intern -> u . file . stream ) ) {
 if ( ! silent ) {
 zend_throw_exception_ex ( spl_ce_RuntimeException , 0 TSRMLS_CC , "Cannot read from file %s" , intern -> file_name ) ;
 }
 return FAILURE ;
 }
 if ( SPL_HAS_FLAG ( intern -> flags , SPL_FILE_OBJECT_READ_CSV ) ) {
 return spl_filesystem_file_read_csv ( intern , intern -> u . file . delimiter , intern -> u . file . enclosure , intern -> u . file . escape , NULL TSRMLS_CC ) ;
 }
 else {
 zend_call_method_with_0_params ( & this_ptr , Z_OBJCE_P ( getThis ( ) ) , & intern -> u . file . func_getCurr , "getCurrentLine" , & retval ) ;
 }
 if ( retval ) {
 if ( intern -> u . file . current_line || intern -> u . file . current_zval ) {
 intern -> u . file . current_line_num ++ ;
 }
 spl_filesystem_file_free_line ( intern TSRMLS_CC ) ;
 if ( Z_TYPE_P ( retval ) == IS_STRING ) {
 intern -> u . file . current_line = estrndup ( Z_STRVAL_P ( retval ) , Z_STRLEN_P ( retval ) ) ;
 intern -> u . file . current_line_len = Z_STRLEN_P ( retval ) ;
 }
 else {
 MAKE_STD_ZVAL ( intern -> u . file . current_zval ) ;
 ZVAL_ZVAL ( intern -> u . file . current_zval , retval , 1 , 0 ) ;
 }
 zval_ptr_dtor ( & retval ) ;
 return SUCCESS ;
 }
 else {
 return FAILURE ;
 }
 }
 else {
 return spl_filesystem_file_read ( intern , silent TSRMLS_CC ) ;
 }
 }