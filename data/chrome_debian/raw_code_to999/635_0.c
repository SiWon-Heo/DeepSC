SPL_METHOD ( SplFileObject , fscanf ) {
 spl_filesystem_object * intern = ( spl_filesystem_object * ) zend_object_store_get_object ( getThis ( ) TSRMLS_CC ) ;
 spl_filesystem_file_free_line ( intern TSRMLS_CC ) ;
 intern -> u . file . current_line_num ++ ;
 FileFunctionCall ( fscanf , ZEND_NUM_ARGS ( ) , NULL ) ;
 }