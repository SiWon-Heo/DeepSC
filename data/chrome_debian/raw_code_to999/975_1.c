extern int as_mysql_job_start ( mysql_conn_t * mysql_conn , struct job_record * job_ptr ) {
 int rc = SLURM_SUCCESS ;
 char * nodes = NULL , * jname = NULL , * node_inx = NULL ;
 int track_steps = 0 ;
 char * block_id = NULL , * partition = NULL ;
 char * gres_req = NULL , * gres_alloc = NULL ;
 char temp_bit [ BUF_SIZE ] ;
 char * query = NULL ;
 int reinit = 0 ;
 time_t begin_time , check_time , start_time , submit_time ;
 uint32_t wckeyid = 0 ;
 uint32_t job_state ;
 int node_cnt = 0 ;
 uint32_t array_task_id = ( job_ptr -> array_job_id ) ? job_ptr -> array_task_id : NO_VAL ;
 uint64_t job_db_inx = job_ptr -> db_index ;
 job_array_struct_t * array_recs = job_ptr -> array_recs ;
 if ( ( ! job_ptr -> details || ! job_ptr -> details -> submit_time ) && ! job_ptr -> resize_time ) {
 error ( "as_mysql_job_start: " "Not inputing this job, it has no submit time." ) ;
 return SLURM_ERROR ;
 }
 if ( check_connection ( mysql_conn ) != SLURM_SUCCESS ) return ESLURM_DB_CONNECTION ;
 debug2 ( "as_mysql_slurmdb_job_start() called" ) ;
 job_state = job_ptr -> job_state ;
 if ( job_ptr -> resize_time ) {
 begin_time = job_ptr -> resize_time ;
 submit_time = job_ptr -> resize_time ;
 start_time = job_ptr -> resize_time ;
 }
 else {
 begin_time = job_ptr -> details -> begin_time ;
 submit_time = job_ptr -> details -> submit_time ;
 start_time = job_ptr -> start_time ;
 }
 if ( job_ptr -> state_reason == WAIT_ARRAY_TASK_LIMIT ) begin_time = INFINITE ;
 if ( IS_JOB_RESIZING ( job_ptr ) ) {
 if ( ! job_ptr -> db_index ) {
 error ( "We don't have a db_index for job %u, " "this should only happen when resizing " "jobs and the database interface was down." , job_ptr -> job_id ) ;
 job_ptr -> db_index = _get_db_index ( mysql_conn , job_ptr -> details -> submit_time , job_ptr -> job_id , job_ptr -> assoc_id ) ;
 }
 if ( job_ptr -> db_index ) as_mysql_job_complete ( mysql_conn , job_ptr ) ;
 job_state &= ( ~ JOB_RESIZING ) ;
 job_ptr -> db_index = 0 ;
 }
 job_state &= JOB_STATE_BASE ;
 if ( start_time ) check_time = start_time ;
 else if ( begin_time ) check_time = begin_time ;
 else check_time = submit_time ;
 slurm_mutex_lock ( & rollup_lock ) ;
 if ( check_time < global_last_rollup ) {
 MYSQL_RES * result = NULL ;
 MYSQL_ROW row ;
 query = xstrdup_printf ( "select job_db_inx " "from \"%s_%s\" where id_job=%u and " "time_submit=%ld and time_eligible=%ld " "and time_start=%ld;
" , mysql_conn -> cluster_name , job_table , job_ptr -> job_id , submit_time , begin_time , start_time ) ;
 if ( debug_flags & DEBUG_FLAG_DB_JOB ) DB_DEBUG ( mysql_conn -> conn , "query\n%s" , query ) ;
 if ( ! ( result = mysql_db_query_ret ( mysql_conn , query , 0 ) ) ) {
 xfree ( query ) ;
 slurm_mutex_unlock ( & rollup_lock ) ;
 return SLURM_ERROR ;
 }
 xfree ( query ) ;
 if ( ( row = mysql_fetch_row ( result ) ) ) {
 mysql_free_result ( result ) ;
 debug4 ( "revieved an update for a " "job (%u) already known about" , job_ptr -> job_id ) ;
 slurm_mutex_unlock ( & rollup_lock ) ;
 goto no_rollup_change ;
 }
 mysql_free_result ( result ) ;
 if ( job_ptr -> start_time ) debug ( "Need to reroll usage from %s Job %u " "from %s started then and we are just " "now hearing about it." , slurm_ctime2 ( & check_time ) , job_ptr -> job_id , mysql_conn -> cluster_name ) ;
 else if ( begin_time ) debug ( "Need to reroll usage from %s Job %u " "from %s became eligible then and we are just " "now hearing about it." , slurm_ctime2 ( & check_time ) , job_ptr -> job_id , mysql_conn -> cluster_name ) ;
 else debug ( "Need to reroll usage from %s Job %u " "from %s was submitted then and we are just " "now hearing about it." , slurm_ctime2 ( & check_time ) , job_ptr -> job_id , mysql_conn -> cluster_name ) ;
 global_last_rollup = check_time ;
 slurm_mutex_unlock ( & rollup_lock ) ;
 query = xstrdup_printf ( "update \"%s_%s\" set " "hourly_rollup=%ld, " "daily_rollup=%ld, monthly_rollup=%ld" , mysql_conn -> cluster_name , last_ran_table , check_time , check_time , check_time ) ;
 if ( debug_flags & DEBUG_FLAG_DB_JOB ) DB_DEBUG ( mysql_conn -> conn , "query\n%s" , query ) ;
 rc = mysql_db_query ( mysql_conn , query ) ;
 xfree ( query ) ;
 }
 else slurm_mutex_unlock ( & rollup_lock ) ;
 no_rollup_change : if ( job_ptr -> name && job_ptr -> name [ 0 ] ) jname = slurm_add_slash_to_quotes ( job_ptr -> name ) ;
 else {
 jname = xstrdup ( "allocation" ) ;
 track_steps = 1 ;
 }
 if ( job_ptr -> nodes && job_ptr -> nodes [ 0 ] ) nodes = job_ptr -> nodes ;
 else nodes = "None assigned" ;
 if ( job_ptr -> batch_flag ) track_steps = 1 ;
 if ( slurmdbd_conf ) {
 block_id = xstrdup ( job_ptr -> comment ) ;
 node_cnt = job_ptr -> total_nodes ;
 node_inx = job_ptr -> network ;
 }
 else {
 if ( job_ptr -> node_bitmap ) {
 node_inx = bit_fmt ( temp_bit , sizeof ( temp_bit ) , job_ptr -> node_bitmap ) ;
 }
 # ifdef HAVE_BG select_g_select_jobinfo_get ( job_ptr -> select_jobinfo , SELECT_JOBDATA_BLOCK_ID , & block_id ) ;
 select_g_select_jobinfo_get ( job_ptr -> select_jobinfo , SELECT_JOBDATA_NODE_CNT , & node_cnt ) ;
 # else node_cnt = job_ptr -> total_nodes ;
 # endif }
 if ( job_ptr -> assoc_id && ( ! job_ptr -> db_index || job_ptr -> wckey ) ) wckeyid = _get_wckeyid ( mysql_conn , & job_ptr -> wckey , job_ptr -> user_id , mysql_conn -> cluster_name , job_ptr -> assoc_id ) ;
 if ( ! IS_JOB_PENDING ( job_ptr ) && job_ptr -> part_ptr ) partition = slurm_add_slash_to_quotes ( job_ptr -> part_ptr -> name ) ;
 else if ( job_ptr -> partition ) partition = slurm_add_slash_to_quotes ( job_ptr -> partition ) ;
 if ( job_ptr -> gres_req ) gres_req = slurm_add_slash_to_quotes ( job_ptr -> gres_req ) ;
 if ( job_ptr -> gres_alloc ) gres_alloc = slurm_add_slash_to_quotes ( job_ptr -> gres_alloc ) ;
 if ( ! job_ptr -> db_index ) {
 query = xstrdup_printf ( "insert into \"%s_%s\" " "(id_job, mod_time, id_array_job, id_array_task, " "id_assoc, id_qos, id_user, " "id_group, nodelist, id_resv, timelimit, " "time_eligible, time_submit, time_start, " "job_name, track_steps, state, priority, cpus_req, " "nodes_alloc, mem_req" , mysql_conn -> cluster_name , job_table ) ;
 if ( wckeyid ) xstrcat ( query , ", id_wckey" ) ;
 if ( job_ptr -> account ) xstrcat ( query , ", account" ) ;
 if ( partition ) xstrcat ( query , ", `partition`" ) ;
 if ( block_id ) xstrcat ( query , ", id_block" ) ;
 if ( job_ptr -> wckey ) xstrcat ( query , ", wckey" ) ;
 if ( node_inx ) xstrcat ( query , ", node_inx" ) ;
 if ( gres_req ) xstrcat ( query , ", gres_req" ) ;
 if ( gres_alloc ) xstrcat ( query , ", gres_alloc" ) ;
 if ( array_recs && array_recs -> task_id_str ) xstrcat ( query , ", array_task_str, array_max_tasks, " "array_task_pending" ) ;
 else xstrcat ( query , ", array_task_str, array_task_pending" ) ;
 if ( job_ptr -> tres_alloc_str ) xstrcat ( query , ", tres_alloc" ) ;
 if ( job_ptr -> tres_req_str ) xstrcat ( query , ", tres_req" ) ;
 xstrfmtcat ( query , ") values (%u, UNIX_TIMESTAMP(), " "%u, %u, %u, %u, %u, %u, " "'%s', %u, %u, %ld, %ld, %ld, " "'%s', %u, %u, %u, %u, %u, %" PRIu64 "" , job_ptr -> job_id , job_ptr -> array_job_id , array_task_id , job_ptr -> assoc_id , job_ptr -> qos_id , job_ptr -> user_id , job_ptr -> group_id , nodes , job_ptr -> resv_id , job_ptr -> time_limit , begin_time , submit_time , start_time , jname , track_steps , job_state , job_ptr -> priority , job_ptr -> details -> min_cpus , node_cnt , job_ptr -> details -> pn_min_memory ) ;
 if ( wckeyid ) xstrfmtcat ( query , ", %u" , wckeyid ) ;
 if ( job_ptr -> account ) xstrfmtcat ( query , ", '%s'" , job_ptr -> account ) ;
 if ( partition ) xstrfmtcat ( query , ", '%s'" , partition ) ;
 if ( block_id ) xstrfmtcat ( query , ", '%s'" , block_id ) ;
 if ( job_ptr -> wckey ) xstrfmtcat ( query , ", '%s'" , job_ptr -> wckey ) ;
 if ( node_inx ) xstrfmtcat ( query , ", '%s'" , node_inx ) ;
 if ( gres_req ) xstrfmtcat ( query , ", '%s'" , gres_req ) ;
 if ( gres_alloc ) xstrfmtcat ( query , ", '%s'" , gres_alloc ) ;
 if ( array_recs && array_recs -> task_id_str ) xstrfmtcat ( query , ", '%s', %u, %u" , array_recs -> task_id_str , array_recs -> max_run_tasks , array_recs -> task_cnt ) ;
 else xstrcat ( query , ", NULL, 0" ) ;
 if ( job_ptr -> tres_alloc_str ) xstrfmtcat ( query , ", '%s'" , job_ptr -> tres_alloc_str ) ;
 if ( job_ptr -> tres_req_str ) xstrfmtcat ( query , ", '%s'" , job_ptr -> tres_req_str ) ;
 xstrfmtcat ( query , ") on duplicate key update " "job_db_inx=LAST_INSERT_ID(job_db_inx), " "id_user=%u, id_group=%u, " "nodelist='%s', id_resv=%u, timelimit=%u, " "time_submit=%ld, time_eligible=%ld, " "time_start=%ld, mod_time=UNIX_TIMESTAMP(), " "job_name='%s', track_steps=%u, id_qos=%u, " "state=greatest(state, %u), priority=%u, " "cpus_req=%u, nodes_alloc=%u, " "mem_req=%" PRIu64 ", id_array_job=%u, id_array_task=%u" , job_ptr -> user_id , job_ptr -> group_id , nodes , job_ptr -> resv_id , job_ptr -> time_limit , submit_time , begin_time , start_time , jname , track_steps , job_ptr -> qos_id , job_state , job_ptr -> priority , job_ptr -> details -> min_cpus , node_cnt , job_ptr -> details -> pn_min_memory , job_ptr -> array_job_id , array_task_id ) ;
 if ( wckeyid ) xstrfmtcat ( query , ", id_wckey=%u" , wckeyid ) ;
 if ( job_ptr -> account ) xstrfmtcat ( query , ", account='%s'" , job_ptr -> account ) ;
 if ( partition ) xstrfmtcat ( query , ", `partition`='%s'" , partition ) ;
 if ( block_id ) xstrfmtcat ( query , ", id_block='%s'" , block_id ) ;
 if ( job_ptr -> wckey ) xstrfmtcat ( query , ", wckey='%s'" , job_ptr -> wckey ) ;
 if ( node_inx ) xstrfmtcat ( query , ", node_inx='%s'" , node_inx ) ;
 if ( gres_req ) xstrfmtcat ( query , ", gres_req='%s'" , gres_req ) ;
 if ( gres_alloc ) xstrfmtcat ( query , ", gres_alloc='%s'" , gres_alloc ) ;
 if ( array_recs && array_recs -> task_id_str ) xstrfmtcat ( query , ", array_task_str='%s', " "array_max_tasks=%u, array_task_pending=%u" , array_recs -> task_id_str , array_recs -> max_run_tasks , array_recs -> task_cnt ) ;
 else xstrfmtcat ( query , ", array_task_str=NULL, " "array_task_pending=0" ) ;
 if ( job_ptr -> tres_alloc_str ) xstrfmtcat ( query , ", tres_alloc='%s'" , job_ptr -> tres_alloc_str ) ;
 if ( job_ptr -> tres_req_str ) xstrfmtcat ( query , ", tres_req='%s'" , job_ptr -> tres_req_str ) ;
 if ( debug_flags & DEBUG_FLAG_DB_JOB ) DB_DEBUG ( mysql_conn -> conn , "query\n%s" , query ) ;
 try_again : if ( ! ( job_ptr -> db_index = mysql_db_insert_ret_id ( mysql_conn , query ) ) ) {
 if ( ! reinit ) {
 error ( "It looks like the storage has gone " "away trying to reconnect" ) ;
 check_connection ( mysql_conn ) ;
 reinit = 1 ;
 goto try_again ;
 }
 else rc = SLURM_ERROR ;
 }
 }
 else {
 query = xstrdup_printf ( "update \"%s_%s\" set nodelist='%s', " , mysql_conn -> cluster_name , job_table , nodes ) ;
 if ( wckeyid ) xstrfmtcat ( query , "id_wckey=%u, " , wckeyid ) ;
 if ( job_ptr -> account ) xstrfmtcat ( query , "account='%s', " , job_ptr -> account ) ;
 if ( partition ) xstrfmtcat ( query , "`partition`='%s', " , partition ) ;
 if ( block_id ) xstrfmtcat ( query , "id_block='%s', " , block_id ) ;
 if ( job_ptr -> wckey ) xstrfmtcat ( query , "wckey='%s', " , job_ptr -> wckey ) ;
 if ( node_inx ) xstrfmtcat ( query , "node_inx='%s', " , node_inx ) ;
 if ( gres_req ) xstrfmtcat ( query , "gres_req='%s', " , gres_req ) ;
 if ( gres_alloc ) xstrfmtcat ( query , "gres_alloc='%s', " , gres_alloc ) ;
 if ( array_recs && array_recs -> task_id_str ) xstrfmtcat ( query , "array_task_str='%s', " "array_max_tasks=%u, " "array_task_pending=%u, " , array_recs -> task_id_str , array_recs -> max_run_tasks , array_recs -> task_cnt ) ;
 else xstrfmtcat ( query , "array_task_str=NULL, " "array_task_pending=0, " ) ;
 if ( job_ptr -> tres_alloc_str ) xstrfmtcat ( query , "tres_alloc='%s', " , job_ptr -> tres_alloc_str ) ;
 if ( job_ptr -> tres_req_str ) xstrfmtcat ( query , "tres_req='%s', " , job_ptr -> tres_req_str ) ;
 xstrfmtcat ( query , "time_start=%ld, job_name='%s', state=%u, " "nodes_alloc=%u, id_qos=%u, " "id_assoc=%u, id_resv=%u, " "timelimit=%u, mem_req=%" PRIu64 ", " "id_array_job=%u, id_array_task=%u, " "time_eligible=%ld, mod_time=UNIX_TIMESTAMP() " "where job_db_inx=%" PRIu64 , start_time , jname , job_state , node_cnt , job_ptr -> qos_id , job_ptr -> assoc_id , job_ptr -> resv_id , job_ptr -> time_limit , job_ptr -> details -> pn_min_memory , job_ptr -> array_job_id , array_task_id , begin_time , job_ptr -> db_index ) ;
 if ( debug_flags & DEBUG_FLAG_DB_JOB ) DB_DEBUG ( mysql_conn -> conn , "query\n%s" , query ) ;
 rc = mysql_db_query ( mysql_conn , query ) ;
 }
 xfree ( block_id ) ;
 xfree ( partition ) ;
 xfree ( gres_req ) ;
 xfree ( gres_alloc ) ;
 xfree ( jname ) ;
 xfree ( query ) ;
 if ( IS_JOB_RESIZING ( job_ptr ) ) {
 if ( IS_JOB_SUSPENDED ( job_ptr ) ) as_mysql_suspend ( mysql_conn , job_db_inx , job_ptr ) ;
 }
 return rc ;
 }