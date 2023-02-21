SPL_METHOD ( DirectoryIterator , getExtension ) {
 spl_filesystem_object * intern = ( spl_filesystem_object * ) zend_object_store_get_object ( getThis ( ) TSRMLS_CC ) ;
 char * fname = NULL ;
 const char * p ;
 size_t flen ;
 int idx ;
 if ( zend_parse_parameters_none ( ) == FAILURE ) {
 return ;
 }
 php_basename ( intern -> u . dir . entry . d_name , strlen ( intern -> u . dir . entry . d_name ) , NULL , 0 , & fname , & flen TSRMLS_CC ) ;
 p = zend_memrchr ( fname , '.' , flen ) ;
 if ( p ) {
 idx = p - fname ;
 RETVAL_STRINGL ( fname + idx + 1 , flen - idx - 1 , 1 ) ;
 efree ( fname ) ;
 return ;
 }
 else {
 if ( fname ) {
 efree ( fname ) ;
 }
 RETURN_EMPTY_STRING ( ) ;
 }
 }