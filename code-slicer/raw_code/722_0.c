static void spl_filesystem_dir_it_dtor ( zend_object_iterator * iter TSRMLS_DC ) {
 spl_filesystem_iterator * iterator = ( spl_filesystem_iterator * ) iter ;
 if ( iterator -> intern . data ) {
 zval * object = iterator -> intern . data ;
 zval_ptr_dtor ( & object ) ;
 }
 }