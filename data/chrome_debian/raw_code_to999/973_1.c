extern int as_mysql_node_down ( mysql_conn_t * mysql_conn , struct node_record * node_ptr , time_t event_time , char * reason , uint32_t reason_uid ) {
 int rc = SLURM_SUCCESS ;
 char * query = NULL ;
 char * my_reason ;
 MYSQL_RES * result = NULL ;
 MYSQL_ROW row ;
 if ( check_connection ( mysql_conn ) != SLURM_SUCCESS ) return ESLURM_DB_CONNECTION ;
 if ( ! mysql_conn -> cluster_name ) {
 error ( "%s:%d no cluster name" , THIS_FILE , __LINE__ ) ;
 return SLURM_ERROR ;
 }
 if ( ! node_ptr ) {
 error ( "No node_ptr given!" ) ;
 return SLURM_ERROR ;
 }
 if ( ! node_ptr -> tres_str ) {
 error ( "node ptr has no tres_list!" ) ;
 return SLURM_ERROR ;
 }
 query = xstrdup_printf ( "select state, reason from \"%s_%s\" where " "time_end=0 and node_name='%s';
" , mysql_conn -> cluster_name , event_table , node_ptr -> name ) ;
 result = mysql_db_query_ret ( mysql_conn , query , 0 ) ;
 xfree ( query ) ;
 if ( ! result ) return SLURM_ERROR ;
 if ( reason ) my_reason = slurm_add_slash_to_quotes ( reason ) ;
 else my_reason = slurm_add_slash_to_quotes ( node_ptr -> reason ) ;
 row = mysql_fetch_row ( result ) ;
 if ( row && ( node_ptr -> node_state == slurm_atoul ( row [ 0 ] ) ) && my_reason && row [ 1 ] && ! xstrcasecmp ( my_reason , row [ 1 ] ) ) {
 debug ( "as_mysql_node_down: no change needed %u == %s " "and %s == %s" , node_ptr -> node_state , row [ 0 ] , my_reason , row [ 1 ] ) ;
 xfree ( my_reason ) ;
 mysql_free_result ( result ) ;
 return SLURM_SUCCESS ;
 }
 mysql_free_result ( result ) ;
 debug2 ( "inserting %s(%s) with tres of '%s'" , node_ptr -> name , mysql_conn -> cluster_name , node_ptr -> tres_str ) ;
 query = xstrdup_printf ( "update \"%s_%s\" set time_end=%ld where " "time_end=0 and node_name='%s';
" , mysql_conn -> cluster_name , event_table , event_time , node_ptr -> name ) ;
 xstrfmtcat ( query , "insert into \"%s_%s\" " "(node_name, state, tres, time_start, " "reason, reason_uid) " "values ('%s', %u, '%s', %ld, '%s', %u) " "on duplicate key update time_end=0;
" , mysql_conn -> cluster_name , event_table , node_ptr -> name , node_ptr -> node_state , node_ptr -> tres_str , event_time , my_reason , reason_uid ) ;
 debug2 ( "%d(%s:%d) query\n%s" , mysql_conn -> conn , THIS_FILE , __LINE__ , query ) ;
 rc = mysql_db_query ( mysql_conn , query ) ;
 xfree ( query ) ;
 xfree ( my_reason ) ;
 return rc ;
 }