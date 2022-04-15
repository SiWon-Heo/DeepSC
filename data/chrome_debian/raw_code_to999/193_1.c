SPL_METHOD ( SplFileObject , func_name ) \ {
 \ spl_filesystem_object * intern = ( spl_filesystem_object * ) zend_object_store_get_object ( getThis ( ) TSRMLS_CC ) ;
 \ FileFunctionCall ( func_name , ZEND_NUM_ARGS ( ) , NULL ) ;
 \ }
 SPL_METHOD ( SplFileObject , fgetcsv ) {
 spl_filesystem_object * intern = ( spl_filesystem_object * ) zend_object_store_get_object ( getThis ( ) TSRMLS_CC ) ;
 char delimiter = intern -> u . file . delimiter , enclosure = intern -> u . file . enclosure , escape = intern -> u . file . escape ;
 char * delim = NULL , * enclo = NULL , * esc = NULL ;
 int d_len = 0 , e_len = 0 , esc_len = 0 ;
 if ( zend_parse_parameters ( ZEND_NUM_ARGS ( ) TSRMLS_CC , "|sss" , & delim , & d_len , & enclo , & e_len , & esc , & esc_len ) == SUCCESS ) {
 switch ( ZEND_NUM_ARGS ( ) ) {
 case 3 : if ( esc_len != 1 ) {
 php_error_docref ( NULL TSRMLS_CC , E_WARNING , "escape must be a character" ) ;
 RETURN_FALSE ;
 }
 escape = esc [ 0 ] ;
 case 2 : if ( e_len != 1 ) {
 php_error_docref ( NULL TSRMLS_CC , E_WARNING , "enclosure must be a character" ) ;
 RETURN_FALSE ;
 }
 enclosure = enclo [ 0 ] ;
 case 1 : if ( d_len != 1 ) {
 php_error_docref ( NULL TSRMLS_CC , E_WARNING , "delimiter must be a character" ) ;
 RETURN_FALSE ;
 }
 delimiter = delim [ 0 ] ;
 case 0 : break ;
 }
 spl_filesystem_file_read_csv ( intern , delimiter , enclosure , escape , return_value TSRMLS_CC ) ;
 }
 }
 SPL_METHOD ( SplFileObject , fputcsv ) {
 spl_filesystem_object * intern = ( spl_filesystem_object * ) zend_object_store_get_object ( getThis ( ) TSRMLS_CC ) ;
 char delimiter = intern -> u . file . delimiter , enclosure = intern -> u . file . enclosure , escape = intern -> u . file . escape ;
 char * delim = NULL , * enclo = NULL , * esc = NULL ;
 int d_len = 0 , e_len = 0 , esc_len = 0 , ret ;
 zval * fields = NULL ;
 if ( zend_parse_parameters ( ZEND_NUM_ARGS ( ) TSRMLS_CC , "a|sss" , & fields , & delim , & d_len , & enclo , & e_len , & esc , & esc_len ) == SUCCESS ) {
 switch ( ZEND_NUM_ARGS ( ) ) {
 case 4 : if ( esc_len != 1 ) {
 php_error_docref ( NULL TSRMLS_CC , E_WARNING , "escape must be a character" ) ;
 RETURN_FALSE ;
 }
 escape = esc [ 0 ] ;
 case 3 : if ( e_len != 1 ) {
 php_error_docref ( NULL TSRMLS_CC , E_WARNING , "enclosure must be a character" ) ;
 RETURN_FALSE ;
 }
 enclosure = enclo [ 0 ] ;
 case 2 : if ( d_len != 1 ) {
 php_error_docref ( NULL TSRMLS_CC , E_WARNING , "delimiter must be a character" ) ;
 RETURN_FALSE ;
 }
 delimiter = delim [ 0 ] ;
 case 1 : case 0 : break ;
 }
 ret = php_fputcsv ( intern -> u . file . stream , fields , delimiter , enclosure , escape TSRMLS_CC ) ;
 RETURN_LONG ( ret ) ;
 }
 }
 SPL_METHOD ( SplFileObject , setCsvControl ) {
 spl_filesystem_object * intern = ( spl_filesystem_object * ) zend_object_store_get_object ( getThis ( ) TSRMLS_CC ) ;
 char delimiter = ',' , enclosure = '"' , escape = '\\' ;
 char * delim = NULL , * enclo = NULL , * esc = NULL ;
 int d_len = 0 , e_len = 0 , esc_len = 0 ;
 if ( zend_parse_parameters ( ZEND_NUM_ARGS ( ) TSRMLS_CC , "|sss" , & delim , & d_len , & enclo , & e_len , & esc , & esc_len ) == SUCCESS ) {
 switch ( ZEND_NUM_ARGS ( ) ) {
 case 3 : if ( esc_len != 1 ) {
 php_error_docref ( NULL TSRMLS_CC , E_WARNING , "escape must be a character" ) ;
 RETURN_FALSE ;
 }
 escape = esc [ 0 ] ;
 case 2 : if ( e_len != 1 ) {
 php_error_docref ( NULL TSRMLS_CC , E_WARNING , "enclosure must be a character" ) ;
 RETURN_FALSE ;
 }
 enclosure = enclo [ 0 ] ;
 case 1 : if ( d_len != 1 ) {
 php_error_docref ( NULL TSRMLS_CC , E_WARNING , "delimiter must be a character" ) ;
 RETURN_FALSE ;
 }
 delimiter = delim [ 0 ] ;
 case 0 : break ;
 }
 intern -> u . file . delimiter = delimiter ;
 intern -> u . file . enclosure = enclosure ;
 intern -> u . file . escape = escape ;
 }
 }
 SPL_METHOD ( SplFileObject , getCsvControl ) {
 spl_filesystem_object * intern = ( spl_filesystem_object * ) zend_object_store_get_object ( getThis ( ) TSRMLS_CC ) ;
 char delimiter [ 2 ] , enclosure [ 2 ] ;
 array_init ( return_value ) ;
 delimiter [ 0 ] = intern -> u . file . delimiter ;
 delimiter [ 1 ] = '\0' ;
 enclosure [ 0 ] = intern -> u . file . enclosure ;
 enclosure [ 1 ] = '\0' ;
 add_next_index_string ( return_value , delimiter , 1 ) ;
 add_next_index_string ( return_value , enclosure , 1 ) ;
 }
 FileFunction ( flock )