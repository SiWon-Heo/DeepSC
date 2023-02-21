SPL_METHOD ( SplFileInfo , openFile ) {
 spl_filesystem_object * intern = ( spl_filesystem_object * ) zend_object_store_get_object ( getThis ( ) TSRMLS_CC ) ;
 spl_filesystem_object_create_type ( ht , intern , SPL_FS_FILE , NULL , return_value TSRMLS_CC ) ;
 }