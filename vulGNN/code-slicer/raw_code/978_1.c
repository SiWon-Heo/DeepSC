extern int as_mysql_job_complete ( mysql_conn_t * mysql_conn , struct job_record * job_ptr ) {
 char * query = NULL ;
 int rc = SLURM_SUCCESS , job_state ;
 time_t submit_time , end_time ;
 uint32_t exit_code = 0 ;
 if ( ! job_ptr -> db_index && ( ( ! job_ptr -> details || ! job_ptr -> details -> submit_time ) && ! job_ptr -> resize_time ) ) {
 error ( "as_mysql_job_complete: " "Not inputing this job, it has no submit time." ) ;
 return SLURM_ERROR ;
 }
 if ( check_connection ( mysql_conn ) != SLURM_SUCCESS ) return ESLURM_DB_CONNECTION ;
 debug2 ( "as_mysql_slurmdb_job_complete() called" ) ;
 if ( job_ptr -> resize_time ) submit_time = job_ptr -> resize_time ;
 else submit_time = job_ptr -> details -> submit_time ;
 if ( IS_JOB_RESIZING ( job_ptr ) ) {
 end_time = job_ptr -> resize_time ;
 job_state = JOB_RESIZING ;
 }
 else {
 if ( job_ptr -> end_time == 0 ) {
 if ( job_ptr -> start_time ) {
 error ( "%s: We are trying to end a job (%u) with no end time, setting it to the start time (%ld) of the job." , __func__ , job_ptr -> job_id , job_ptr -> start_time ) ;
 job_ptr -> end_time = job_ptr -> start_time ;
 }
 else {
 error ( "%s: job %u never started" , __func__ , job_ptr -> job_id ) ;
 return SLURM_SUCCESS ;
 }
 }
 end_time = job_ptr -> end_time ;
 if ( IS_JOB_REQUEUED ( job_ptr ) ) job_state = JOB_REQUEUE ;
 else if ( IS_JOB_REVOKED ( job_ptr ) ) job_state = JOB_REVOKED ;
 else job_state = job_ptr -> job_state & JOB_STATE_BASE ;
 }
 slurm_mutex_lock ( & rollup_lock ) ;
 if ( end_time < global_last_rollup ) {
 global_last_rollup = job_ptr -> end_time ;
 slurm_mutex_unlock ( & rollup_lock ) ;
 query = xstrdup_printf ( "update \"%s_%s\" set " "hourly_rollup=%ld, " "daily_rollup=%ld, monthly_rollup=%ld" , mysql_conn -> cluster_name , last_ran_table , end_time , end_time , end_time ) ;
 if ( debug_flags & DEBUG_FLAG_DB_JOB ) DB_DEBUG ( mysql_conn -> conn , "query\n%s" , query ) ;
 ( void ) mysql_db_query ( mysql_conn , query ) ;
 xfree ( query ) ;
 }
 else slurm_mutex_unlock ( & rollup_lock ) ;
 if ( ! job_ptr -> db_index ) {
 if ( ! ( job_ptr -> db_index = _get_db_index ( mysql_conn , submit_time , job_ptr -> job_id , job_ptr -> assoc_id ) ) ) {
 char * comment = job_ptr -> comment ;
 job_ptr -> comment = NULL ;
 if ( as_mysql_job_start ( mysql_conn , job_ptr ) == SLURM_ERROR ) {
 job_ptr -> comment = comment ;
 error ( "couldn't add job %u at job completion" , job_ptr -> job_id ) ;
 return SLURM_SUCCESS ;
 }
 job_ptr -> comment = comment ;
 }
 }
 query = xstrdup_printf ( "update \"%s_%s\" set " "mod_time=UNIX_TIMESTAMP(), " "time_end=%ld, state=%d" , mysql_conn -> cluster_name , job_table , end_time , job_state ) ;
 if ( job_ptr -> derived_ec != NO_VAL ) xstrfmtcat ( query , ", derived_ec=%u" , job_ptr -> derived_ec ) ;
 if ( job_ptr -> comment ) {
 char * comment = slurm_add_slash_to_quotes ( job_ptr -> comment ) ;
 xstrfmtcat ( query , ", derived_es='%s'" , comment ) ;
 xfree ( comment ) ;
 }
 if ( job_ptr -> admin_comment ) {
 char * comment = slurm_add_slash_to_quotes ( job_ptr -> admin_comment ) ;
 xstrfmtcat ( query , ", admin_comment='%s'" , comment ) ;
 xfree ( comment ) ;
 }
 exit_code = job_ptr -> exit_code ;
 if ( exit_code == 1 ) {
 exit_code = 256 ;
 }
 xstrfmtcat ( query , ", exit_code=%d, kill_requid=%d where job_db_inx=%" PRIu64 ";
" , exit_code , job_ptr -> requid , job_ptr -> db_index ) ;
 if ( debug_flags & DEBUG_FLAG_DB_JOB ) DB_DEBUG ( mysql_conn -> conn , "query\n%s" , query ) ;
 rc = mysql_db_query ( mysql_conn , query ) ;
 xfree ( query ) ;
 return rc ;
 }