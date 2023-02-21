SPL_METHOD ( SplFileObject , fflush ) {
 spl_filesystem_object * intern = ( spl_filesystem_object * ) zend_object_store_get_object ( getThis ( ) TSRMLS_CC ) ;
 RETURN_BOOL ( ! php_stream_flush ( intern -> u . file . stream ) ) ;
 }