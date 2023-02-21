SPL_METHOD ( SplFileInfo , getPathname ) {
 spl_filesystem_object * intern = ( spl_filesystem_object * ) zend_object_store_get_object ( getThis ( ) TSRMLS_CC ) ;
 char * path ;
 int path_len ;
 if ( zend_parse_parameters_none ( ) == FAILURE ) {
 return ;
 }
 path = spl_filesystem_object_get_pathname ( intern , & path_len TSRMLS_CC ) ;
 if ( path != NULL ) {
 RETURN_STRINGL ( path , path_len , 1 ) ;
 }
 else {
 RETURN_FALSE ;
 }
 }