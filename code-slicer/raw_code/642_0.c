SPL_METHOD ( SplFileObject , fpassthru ) {
 spl_filesystem_object * intern = ( spl_filesystem_object * ) zend_object_store_get_object ( getThis ( ) TSRMLS_CC ) ;
 RETURN_LONG ( php_stream_passthru ( intern -> u . file . stream ) ) ;
 }