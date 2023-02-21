static void dump_table ( char * table , char * db ) {
 char ignore_flag ;
 char buf [ 200 ] , table_buff [ NAME_LEN + 3 ] ;
 DYNAMIC_STRING query_string ;
 char table_type [ NAME_LEN ] ;
 char * result_table , table_buff2 [ NAME_LEN * 2 + 3 ] , * opt_quoted_table ;
 int error = 0 ;
 ulong rownr , row_break , total_length , init_length ;
 uint num_fields ;
 MYSQL_RES * res ;
 MYSQL_FIELD * field ;
 MYSQL_ROW row ;
 DBUG_ENTER ( "dump_table" ) ;
 num_fields = get_table_structure ( table , db , table_type , & ignore_flag ) ;
 if ( strcmp ( table_type , "VIEW" ) == 0 ) DBUG_VOID_RETURN ;
 if ( opt_no_data ) {
 verbose_msg ( "-- Skipping dump data for table '%s', --no-data was used\n" , table ) ;
 DBUG_VOID_RETURN ;
 }
 DBUG_PRINT ( "info" , ( "ignore_flag: %x num_fields: %d" , ( int ) ignore_flag , num_fields ) ) ;
 if ( ignore_flag & IGNORE_DATA ) {
 verbose_msg ( "-- Warning: Skipping data for table '%s' because " "it's of type %s\n" , table , table_type ) ;
 DBUG_VOID_RETURN ;
 }
 if ( num_fields == 0 ) {
 verbose_msg ( "-- Skipping dump data for table '%s', it has no fields\n" , table ) ;
 DBUG_VOID_RETURN ;
 }
 if ( ! opt_events && ! my_strcasecmp ( & my_charset_latin1 , db , "mysql" ) && ! my_strcasecmp ( & my_charset_latin1 , table , "event" ) ) {
 verbose_msg ( "-- Skipping data table mysql.event, --skip-events was used\n" ) ;
 DBUG_VOID_RETURN ;
 }
 result_table = quote_name ( table , table_buff , 1 ) ;
 opt_quoted_table = quote_name ( table , table_buff2 , 0 ) ;
 verbose_msg ( "-- Sending SELECT query...\n" ) ;
 init_dynamic_string_checked ( & query_string , "" , 1024 , 1024 ) ;
 if ( path ) {
 char filename [ FN_REFLEN ] , tmp_path [ FN_REFLEN ] ;
 convert_dirname ( tmp_path , path , NullS ) ;
 my_load_path ( tmp_path , tmp_path , NULL ) ;
 fn_format ( filename , table , tmp_path , ".txt" , MYF ( MY_UNPACK_FILENAME ) ) ;
 my_delete ( filename , MYF ( 0 ) ) ;
 to_unix_path ( filename ) ;
 dynstr_append_checked ( & query_string , "SELECT /*!40001 SQL_NO_CACHE */ * INTO OUTFILE '" ) ;
 dynstr_append_checked ( & query_string , filename ) ;
 dynstr_append_checked ( & query_string , "'" ) ;
 dynstr_append_checked ( & query_string , " /*!50138 CHARACTER SET " ) ;
 dynstr_append_checked ( & query_string , default_charset == mysql_universal_client_charset ? my_charset_bin . name : default_charset ) ;
 dynstr_append_checked ( & query_string , " */" ) ;
 if ( fields_terminated || enclosed || opt_enclosed || escaped ) dynstr_append_checked ( & query_string , " FIELDS" ) ;
 add_load_option ( & query_string , " TERMINATED BY " , fields_terminated ) ;
 add_load_option ( & query_string , " ENCLOSED BY " , enclosed ) ;
 add_load_option ( & query_string , " OPTIONALLY ENCLOSED BY " , opt_enclosed ) ;
 add_load_option ( & query_string , " ESCAPED BY " , escaped ) ;
 add_load_option ( & query_string , " LINES TERMINATED BY " , lines_terminated ) ;
 dynstr_append_checked ( & query_string , " FROM " ) ;
 dynstr_append_checked ( & query_string , result_table ) ;
 if ( where ) {
 dynstr_append_checked ( & query_string , " WHERE " ) ;
 dynstr_append_checked ( & query_string , where ) ;
 }
 if ( order_by ) {
 dynstr_append_checked ( & query_string , " ORDER BY " ) ;
 dynstr_append_checked ( & query_string , order_by ) ;
 }
 if ( mysql_real_query ( mysql , query_string . str , query_string . length ) ) {
 DB_error ( mysql , "when executing 'SELECT INTO OUTFILE'" ) ;
 dynstr_free ( & query_string ) ;
 DBUG_VOID_RETURN ;
 }
 }
 else {
 print_comment ( md_result_file , 0 , "\n--\n-- Dumping data for table %s\n--\n" , result_table ) ;
 dynstr_append_checked ( & query_string , "SELECT /*!40001 SQL_NO_CACHE */ * FROM " ) ;
 dynstr_append_checked ( & query_string , result_table ) ;
 if ( where ) {
 print_comment ( md_result_file , 0 , "-- WHERE: %s\n" , where ) ;
 dynstr_append_checked ( & query_string , " WHERE " ) ;
 dynstr_append_checked ( & query_string , where ) ;
 }
 if ( order_by ) {
 print_comment ( md_result_file , 0 , "-- ORDER BY: %s\n" , order_by ) ;
 dynstr_append_checked ( & query_string , " ORDER BY " ) ;
 dynstr_append_checked ( & query_string , order_by ) ;
 }
 if ( ! opt_xml && ! opt_compact ) {
 fputs ( "\n" , md_result_file ) ;
 check_io ( md_result_file ) ;
 }
 if ( mysql_query_with_error_report ( mysql , 0 , query_string . str ) ) {
 DB_error ( mysql , "when retrieving data from server" ) ;
 goto err ;
 }
 if ( quick ) res = mysql_use_result ( mysql ) ;
 else res = mysql_store_result ( mysql ) ;
 if ( ! res ) {
 DB_error ( mysql , "when retrieving data from server" ) ;
 goto err ;
 }
 verbose_msg ( "-- Retrieving rows...\n" ) ;
 if ( mysql_num_fields ( res ) != num_fields ) {
 fprintf ( stderr , "%s: Error in field count for table: %s ! Aborting.\n" , my_progname_short , result_table ) ;
 error = EX_CONSCHECK ;
 goto err ;
 }
 if ( opt_lock ) {
 fprintf ( md_result_file , "LOCK TABLES %s WRITE;
\n" , opt_quoted_table ) ;
 check_io ( md_result_file ) ;
 }
 if ( opt_disable_keys ) {
 fprintf ( md_result_file , "/*!40000 ALTER TABLE %s DISABLE KEYS */;
\n" , opt_quoted_table ) ;
 check_io ( md_result_file ) ;
 }
 total_length = opt_net_buffer_length ;
 row_break = 0 ;
 rownr = 0 ;
 init_length = ( uint ) insert_pat . length + 4 ;
 if ( opt_xml ) print_xml_tag ( md_result_file , "\t" , "\n" , "table_data" , "name=" , table , NullS ) ;
 if ( opt_autocommit ) {
 fprintf ( md_result_file , "set autocommit=0;
\n" ) ;
 check_io ( md_result_file ) ;
 }
 while ( ( row = mysql_fetch_row ( res ) ) ) {
 uint i ;
 ulong * lengths = mysql_fetch_lengths ( res ) ;
 rownr ++ ;
 if ( ! extended_insert && ! opt_xml ) {
 fputs ( insert_pat . str , md_result_file ) ;
 check_io ( md_result_file ) ;
 }
 mysql_field_seek ( res , 0 ) ;
 if ( opt_xml ) {
 fputs ( "\t<row>\n" , md_result_file ) ;
 check_io ( md_result_file ) ;
 }
 for ( i = 0 ;
 i < mysql_num_fields ( res ) ;
 i ++ ) {
 int is_blob ;
 ulong length = lengths [ i ] ;
 if ( ! ( field = mysql_fetch_field ( res ) ) ) die ( EX_CONSCHECK , "Not enough fields from table %s! Aborting.\n" , result_table ) ;
 is_blob = ( opt_hex_blob && field -> charsetnr == 63 && ( field -> type == MYSQL_TYPE_BIT || field -> type == MYSQL_TYPE_STRING || field -> type == MYSQL_TYPE_VAR_STRING || field -> type == MYSQL_TYPE_VARCHAR || field -> type == MYSQL_TYPE_BLOB || field -> type == MYSQL_TYPE_LONG_BLOB || field -> type == MYSQL_TYPE_MEDIUM_BLOB || field -> type == MYSQL_TYPE_TINY_BLOB || field -> type == MYSQL_TYPE_GEOMETRY ) ) ? 1 : 0 ;
 if ( extended_insert && ! opt_xml ) {
 if ( i == 0 ) dynstr_set_checked ( & extended_row , "(" ) ;
 else dynstr_append_checked ( & extended_row , "," ) ;
 if ( row [ i ] ) {
 if ( length ) {
 if ( ! ( field -> flags & NUM_FLAG ) ) {
 dynstr_realloc_checked ( & extended_row , length * 2 + 2 + 1 ) ;
 if ( opt_hex_blob && is_blob ) {
 dynstr_append_checked ( & extended_row , "0x" ) ;
 extended_row . length += mysql_hex_string ( extended_row . str + extended_row . length , row [ i ] , length ) ;
 DBUG_ASSERT ( extended_row . length + 1 <= extended_row . max_length ) ;
 DBUG_ASSERT ( extended_row . str [ extended_row . length ] == '\0' ) ;
 }
 else {
 dynstr_append_checked ( & extended_row , "'" ) ;
 extended_row . length += mysql_real_escape_string ( & mysql_connection , & extended_row . str [ extended_row . length ] , row [ i ] , length ) ;
 extended_row . str [ extended_row . length ] = '\0' ;
 dynstr_append_checked ( & extended_row , "'" ) ;
 }
 }
 else {
 char * ptr = row [ i ] ;
 if ( my_isalpha ( charset_info , * ptr ) || ( * ptr == '-' && my_isalpha ( charset_info , ptr [ 1 ] ) ) ) dynstr_append_checked ( & extended_row , "NULL" ) ;
 else {
 if ( field -> type == MYSQL_TYPE_DECIMAL ) {
 dynstr_append_checked ( & extended_row , "'" ) ;
 dynstr_append_checked ( & extended_row , ptr ) ;
 dynstr_append_checked ( & extended_row , "'" ) ;
 }
 else dynstr_append_checked ( & extended_row , ptr ) ;
 }
 }
 }
 else dynstr_append_checked ( & extended_row , "''" ) ;
 }
 else dynstr_append_checked ( & extended_row , "NULL" ) ;
 }
 else {
 if ( i && ! opt_xml ) {
 fputc ( ',' , md_result_file ) ;
 check_io ( md_result_file ) ;
 }
 if ( row [ i ] ) {
 if ( ! ( field -> flags & NUM_FLAG ) ) {
 if ( opt_xml ) {
 if ( opt_hex_blob && is_blob && length ) {
 print_xml_tag ( md_result_file , "\t\t" , "" , "field" , "name=" , field -> name , "xsi:type=" , "xs:hexBinary" , NullS ) ;
 print_blob_as_hex ( md_result_file , row [ i ] , length ) ;
 }
 else {
 print_xml_tag ( md_result_file , "\t\t" , "" , "field" , "name=" , field -> name , NullS ) ;
 print_quoted_xml ( md_result_file , row [ i ] , length , 0 ) ;
 }
 fputs ( "</field>\n" , md_result_file ) ;
 }
 else if ( opt_hex_blob && is_blob && length ) {
 fputs ( "0x" , md_result_file ) ;
 print_blob_as_hex ( md_result_file , row [ i ] , length ) ;
 }
 else unescape ( md_result_file , row [ i ] , length ) ;
 }
 else {
 char * ptr = row [ i ] ;
 if ( opt_xml ) {
 print_xml_tag ( md_result_file , "\t\t" , "" , "field" , "name=" , field -> name , NullS ) ;
 fputs ( ! my_isalpha ( charset_info , * ptr ) ? ptr : "NULL" , md_result_file ) ;
 fputs ( "</field>\n" , md_result_file ) ;
 }
 else if ( my_isalpha ( charset_info , * ptr ) || ( * ptr == '-' && my_isalpha ( charset_info , ptr [ 1 ] ) ) ) fputs ( "NULL" , md_result_file ) ;
 else if ( field -> type == MYSQL_TYPE_DECIMAL ) {
 fputc ( '\'' , md_result_file ) ;
 fputs ( ptr , md_result_file ) ;
 fputc ( '\'' , md_result_file ) ;
 }
 else fputs ( ptr , md_result_file ) ;
 }
 }
 else {
 if ( ! opt_xml ) fputs ( "NULL" , md_result_file ) ;
 else print_xml_null_tag ( md_result_file , "\t\t" , "field name=" , field -> name , "\n" ) ;
 }
 check_io ( md_result_file ) ;
 }
 }
 if ( opt_xml ) {
 fputs ( "\t</row>\n" , md_result_file ) ;
 check_io ( md_result_file ) ;
 }
 if ( extended_insert ) {
 ulong row_length ;
 dynstr_append_checked ( & extended_row , ")" ) ;
 row_length = 2 + extended_row . length ;
 if ( total_length + row_length < opt_net_buffer_length ) {
 total_length += row_length ;
 fputc ( ',' , md_result_file ) ;
 fputs ( extended_row . str , md_result_file ) ;
 }
 else {
 if ( row_break ) fputs ( ";
\n" , md_result_file ) ;
 row_break = 1 ;
 fputs ( insert_pat . str , md_result_file ) ;
 fputs ( extended_row . str , md_result_file ) ;
 total_length = row_length + init_length ;
 }
 check_io ( md_result_file ) ;
 }
 else if ( ! opt_xml ) {
 fputs ( ");
\n" , md_result_file ) ;
 check_io ( md_result_file ) ;
 }
 }
 if ( opt_xml ) fputs ( "\t</table_data>\n" , md_result_file ) ;
 else if ( extended_insert && row_break ) fputs ( ";
\n" , md_result_file ) ;
 fflush ( md_result_file ) ;
 check_io ( md_result_file ) ;
 if ( mysql_errno ( mysql ) ) {
 my_snprintf ( buf , sizeof ( buf ) , "%s: Error %d: %s when dumping table %s at row: %ld\n" , my_progname_short , mysql_errno ( mysql ) , mysql_error ( mysql ) , result_table , rownr ) ;
 fputs ( buf , stderr ) ;
 error = EX_CONSCHECK ;
 goto err ;
 }
 if ( opt_disable_keys ) {
 fprintf ( md_result_file , "/*!40000 ALTER TABLE %s ENABLE KEYS */;
\n" , opt_quoted_table ) ;
 check_io ( md_result_file ) ;
 }
 if ( opt_lock ) {
 fputs ( "UNLOCK TABLES;
\n" , md_result_file ) ;
 check_io ( md_result_file ) ;
 }
 if ( opt_autocommit ) {
 fprintf ( md_result_file , "commit;
\n" ) ;
 check_io ( md_result_file ) ;
 }
 mysql_free_result ( res ) ;
 }
 dynstr_free ( & query_string ) ;
 DBUG_VOID_RETURN ;
 err : dynstr_free ( & query_string ) ;
 maybe_exit ( error ) ;
 DBUG_VOID_RETURN ;
 }