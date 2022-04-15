SPL_METHOD ( SplFileObject , fgetss ) {
 spl_filesystem_object * intern = ( spl_filesystem_object * ) zend_object_store_get_object ( getThis ( ) TSRMLS_CC ) ;
 zval * arg2 = NULL ;
 MAKE_STD_ZVAL ( arg2 ) ;
 if ( intern -> u . file . max_line_len > 0 ) {
 ZVAL_LONG ( arg2 , intern -> u . file . max_line_len ) ;
 }
 else {
 ZVAL_LONG ( arg2 , 1024 ) ;
 }
 spl_filesystem_file_free_line ( intern TSRMLS_CC ) ;
 intern -> u . file . current_line_num ++ ;
 FileFunctionCall ( fgetss , ZEND_NUM_ARGS ( ) , arg2 ) ;
 zval_ptr_dtor ( & arg2 ) ;
 }