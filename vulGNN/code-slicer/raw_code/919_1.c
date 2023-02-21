static void create_new_objects ( void ) {
 int dbnum ;
 prep_status ( "Restoring database schemas in the new cluster\n" ) ;
 for ( dbnum = 0 ;
 dbnum < old_cluster . dbarr . ndbs ;
 dbnum ++ ) {
 char sql_file_name [ MAXPGPATH ] , log_file_name [ MAXPGPATH ] ;
 DbInfo * old_db = & old_cluster . dbarr . dbs [ dbnum ] ;
 pg_log ( PG_STATUS , "%s" , old_db -> db_name ) ;
 snprintf ( sql_file_name , sizeof ( sql_file_name ) , DB_DUMP_FILE_MASK , old_db -> db_oid ) ;
 snprintf ( log_file_name , sizeof ( log_file_name ) , DB_DUMP_LOG_FILE_MASK , old_db -> db_oid ) ;
 parallel_exec_prog ( log_file_name , NULL , "\"%s/pg_restore\" %s --exit-on-error --verbose --dbname \"%s\" \"%s\"" , new_cluster . bindir , cluster_conn_opts ( & new_cluster ) , old_db -> db_name , sql_file_name ) ;
 }
 while ( reap_child ( true ) == true ) ;
 end_progress_output ( ) ;
 check_ok ( ) ;
 if ( GET_MAJOR_VERSION ( old_cluster . major_version ) < 903 ) set_frozenxids ( true ) ;
 get_db_and_rel_infos ( & new_cluster ) ;
 }