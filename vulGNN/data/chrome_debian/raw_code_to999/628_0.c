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
 FileFunction ( flock ) SPL_METHOD ( SplFileObject , fflush ) {
 spl_filesystem_object * intern = ( spl_filesystem_object * ) zend_object_store_get_object ( getThis ( ) TSRMLS_CC ) ;
 RETURN_BOOL ( ! php_stream_flush ( intern -> u . file . stream ) ) ;
 }
 SPL_METHOD ( SplFileObject , ftell ) {
 spl_filesystem_object * intern = ( spl_filesystem_object * ) zend_object_store_get_object ( getThis ( ) TSRMLS_CC ) ;
 long ret = php_stream_tell ( intern -> u . file . stream ) ;
 if ( ret == - 1 ) {
 RETURN_FALSE ;
 }
 else {
 RETURN_LONG ( ret ) ;
 }
 }
 SPL_METHOD ( SplFileObject , fseek ) {
 spl_filesystem_object * intern = ( spl_filesystem_object * ) zend_object_store_get_object ( getThis ( ) TSRMLS_CC ) ;
 long pos , whence = SEEK_SET ;
 if ( zend_parse_parameters ( ZEND_NUM_ARGS ( ) TSRMLS_CC , "l|l" , & pos , & whence ) == FAILURE ) {
 return ;
 }
 spl_filesystem_file_free_line ( intern TSRMLS_CC ) ;
 RETURN_LONG ( php_stream_seek ( intern -> u . file . stream , pos , whence ) ) ;
 }
 SPL_METHOD ( SplFileObject , fgetc ) {
 spl_filesystem_object * intern = ( spl_filesystem_object * ) zend_object_store_get_object ( getThis ( ) TSRMLS_CC ) ;
 char buf [ 2 ] ;
 int result ;
 spl_filesystem_file_free_line ( intern TSRMLS_CC ) ;
 result = php_stream_getc ( intern -> u . file . stream ) ;
 if ( result == EOF ) {
 RETVAL_FALSE ;
 }
 else {
 if ( result == '\n' ) {
 intern -> u . file . current_line_num ++ ;
 }
 buf [ 0 ] = result ;
 buf [ 1 ] = '\0' ;
 RETURN_STRINGL ( buf , 1 , 1 ) ;
 }
 }
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
 SPL_METHOD ( SplFileObject , fpassthru ) {
 spl_filesystem_object * intern = ( spl_filesystem_object * ) zend_object_store_get_object ( getThis ( ) TSRMLS_CC ) ;
 RETURN_LONG ( php_stream_passthru ( intern -> u . file . stream ) ) ;
 }
 SPL_METHOD ( SplFileObject , fscanf ) {
 spl_filesystem_object * intern = ( spl_filesystem_object * ) zend_object_store_get_object ( getThis ( ) TSRMLS_CC ) ;
 spl_filesystem_file_free_line ( intern TSRMLS_CC ) ;
 intern -> u . file . current_line_num ++ ;
 FileFunctionCall ( fscanf , ZEND_NUM_ARGS ( ) , NULL ) ;
 }
 SPL_METHOD ( SplFileObject , fwrite ) {
 spl_filesystem_object * intern = ( spl_filesystem_object * ) zend_object_store_get_object ( getThis ( ) TSRMLS_CC ) ;
 char * str ;
 int str_len ;
 long length = 0 ;
 if ( zend_parse_parameters ( ZEND_NUM_ARGS ( ) TSRMLS_CC , "s|l" , & str , & str_len , & length ) == FAILURE ) {
 return ;
 }
 if ( ZEND_NUM_ARGS ( ) > 1 ) {
 str_len = MAX ( 0 , MIN ( length , str_len ) ) ;
 }
 if ( ! str_len ) {
 RETURN_LONG ( 0 ) ;
 }
 RETURN_LONG ( php_stream_write ( intern -> u . file . stream , str , str_len ) ) ;
 }
 SPL_METHOD ( SplFileObject , fread ) {
 spl_filesystem_object * intern = ( spl_filesystem_object * ) zend_object_store_get_object ( getThis ( ) TSRMLS_CC ) ;
 long length = 0 ;
 if ( zend_parse_parameters ( ZEND_NUM_ARGS ( ) TSRMLS_CC , "l" , & length ) == FAILURE ) {
 return ;
 }
 if ( length <= 0 ) {
 php_error_docref ( NULL TSRMLS_CC , E_WARNING , "Length parameter must be greater than 0" ) ;
 RETURN_FALSE ;
 }
 if ( length > INT_MAX ) {
 php_error_docref ( NULL TSRMLS_CC , E_WARNING , "Length parameter must be no more than %d" , INT_MAX ) ;
 RETURN_FALSE ;
 }
 Z_STRVAL_P ( return_value ) = emalloc ( length + 1 ) ;
 Z_STRLEN_P ( return_value ) = php_stream_read ( intern -> u . file . stream , Z_STRVAL_P ( return_value ) , length ) ;
 Z_STRVAL_P ( return_value ) [ Z_STRLEN_P ( return_value ) ] = 0 ;
 Z_TYPE_P ( return_value ) = IS_STRING ;
 }
 FileFunction ( fstat )