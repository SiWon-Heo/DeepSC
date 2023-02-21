SPL_METHOD ( SplFileInfo , getExtension ) {
 spl_filesystem_object * intern = ( spl_filesystem_object * ) zend_object_store_get_object ( getThis ( ) TSRMLS_CC ) ;
 char * fname = NULL ;
 const char * p ;
 size_t flen ;
 int path_len , idx ;
 if ( zend_parse_parameters_none ( ) == FAILURE ) {
 return ;
 }
 spl_filesystem_object_get_path ( intern , & path_len TSRMLS_CC ) ;
 if ( path_len && path_len < intern -> file_name_len ) {
 fname = intern -> file_name + path_len + 1 ;
 flen = intern -> file_name_len - ( path_len + 1 ) ;
 }
 else {
 fname = intern -> file_name ;
 flen = intern -> file_name_len ;
 }
 php_basename ( fname , flen , NULL , 0 , & fname , & flen TSRMLS_CC ) ;
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