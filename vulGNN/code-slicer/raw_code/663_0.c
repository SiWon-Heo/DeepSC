static zend_object_value spl_filesystem_object_new ( zend_class_entry * class_type TSRMLS_DC ) {
 return spl_filesystem_object_new_ex ( class_type , NULL TSRMLS_CC ) ;
 }