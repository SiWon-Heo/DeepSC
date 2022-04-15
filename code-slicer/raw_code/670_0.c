static void spl_filesystem_tree_it_current_data ( zend_object_iterator * iter , zval * * * data TSRMLS_DC ) {
 spl_filesystem_iterator * iterator = ( spl_filesystem_iterator * ) iter ;
 spl_filesystem_object * object = spl_filesystem_iterator_to_object ( iterator ) ;
 if ( SPL_FILE_DIR_CURRENT ( object , SPL_FILE_DIR_CURRENT_AS_PATHNAME ) ) {
 if ( ! iterator -> current ) {
 ALLOC_INIT_ZVAL ( iterator -> current ) ;
 spl_filesystem_object_get_file_name ( object TSRMLS_CC ) ;
 ZVAL_STRINGL ( iterator -> current , object -> file_name , object -> file_name_len , 1 ) ;
 }
 * data = & iterator -> current ;
 }
 else if ( SPL_FILE_DIR_CURRENT ( object , SPL_FILE_DIR_CURRENT_AS_FILEINFO ) ) {
 if ( ! iterator -> current ) {
 ALLOC_INIT_ZVAL ( iterator -> current ) ;
 spl_filesystem_object_get_file_name ( object TSRMLS_CC ) ;
 spl_filesystem_object_create_type ( 0 , object , SPL_FS_INFO , NULL , iterator -> current TSRMLS_CC ) ;
 }
 * data = & iterator -> current ;
 }
 else {
 * data = ( zval * * ) & iterator -> intern . data ;
 }
 }