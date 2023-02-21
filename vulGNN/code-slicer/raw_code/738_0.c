SPL_METHOD ( DirectoryIterator , current ) {
 if ( zend_parse_parameters_none ( ) == FAILURE ) {
 return ;
 }
 RETURN_ZVAL ( getThis ( ) , 1 , 0 ) ;
 }