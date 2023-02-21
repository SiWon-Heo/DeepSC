SPL_METHOD ( SplFileObject , fseek ) {
 spl_filesystem_object * intern = ( spl_filesystem_object * ) zend_object_store_get_object ( getThis ( ) TSRMLS_CC ) ;
 long pos , whence = SEEK_SET ;
 if ( zend_parse_parameters ( ZEND_NUM_ARGS ( ) TSRMLS_CC , "l|l" , & pos , & whence ) == FAILURE ) {
 return ;
 }
 spl_filesystem_file_free_line ( intern TSRMLS_CC ) ;
 RETURN_LONG ( php_stream_seek ( intern -> u . file . stream , pos , whence ) ) ;
 }