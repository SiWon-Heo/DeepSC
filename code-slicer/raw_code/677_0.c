SPL_METHOD ( SplFileObject , fgetc ) {
 spl_filesystem_object * intern = ( spl_filesystem_object * ) zend_object_store_get_object ( getThis ( ) TSRMLS_CC ) ;
 char buf [ 2 ] ;
 int result ;
 spl_filesystem_file_free_line ( intern TSRMLS_CC ) ;
 result = php_stream_getc ( intern -> u . file . stream ) ;
 if ( result == EOF ) {
 RETVAL_FALSE ;
 }
 else {
 if ( result == '\n' ) {
 intern -> u . file . current_line_num ++ ;
 }
 buf [ 0 ] = result ;
 buf [ 1 ] = '\0' ;
 RETURN_STRINGL ( buf , 1 , 1 ) ;
 }
 }