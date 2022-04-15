extern int as_mysql_step_start ( mysql_conn_t * mysql_conn , struct step_record * step_ptr ) {
 int tasks = 0 , nodes = 0 , task_dist = 0 ;
 int rc = SLURM_SUCCESS ;
 char temp_bit [ BUF_SIZE ] ;
 char node_list [ BUFFER_SIZE ] ;
 char * node_inx = NULL , * step_name = NULL ;
 time_t start_time , submit_time ;
 char * query = NULL ;
 if ( ! step_ptr -> job_ptr -> db_index && ( ( ! step_ptr -> job_ptr -> details || ! step_ptr -> job_ptr -> details -> submit_time ) && ! step_ptr -> job_ptr -> resize_time ) ) {
 error ( "as_mysql_step_start: " "Not inputing this job, it has no submit time." ) ;
 return SLURM_ERROR ;
 }
 if ( step_ptr -> job_ptr -> resize_time ) {
 submit_time = start_time = step_ptr -> job_ptr -> resize_time ;
 if ( step_ptr -> start_time > submit_time ) start_time = step_ptr -> start_time ;
 }
 else {
 start_time = step_ptr -> start_time ;
 submit_time = step_ptr -> job_ptr -> details -> submit_time ;
 }
 if ( check_connection ( mysql_conn ) != SLURM_SUCCESS ) return ESLURM_DB_CONNECTION ;
 if ( slurmdbd_conf ) {
 if ( step_ptr -> job_ptr -> details ) tasks = step_ptr -> job_ptr -> details -> num_tasks ;
 else tasks = step_ptr -> cpu_count ;
 snprintf ( node_list , BUFFER_SIZE , "%s" , step_ptr -> job_ptr -> nodes ) ;
 nodes = step_ptr -> step_layout -> node_cnt ;
 task_dist = step_ptr -> step_layout -> task_dist ;
 node_inx = step_ptr -> network ;
 }
 else if ( step_ptr -> step_id == SLURM_BATCH_SCRIPT ) {
 if ( step_ptr -> step_node_bitmap ) {
 node_inx = bit_fmt ( temp_bit , sizeof ( temp_bit ) , step_ptr -> step_node_bitmap ) ;
 }
 snprintf ( node_list , BUFFER_SIZE , "%s" , step_ptr -> gres ) ;
 nodes = tasks = 1 ;
 if ( ! step_ptr -> tres_alloc_str ) xstrfmtcat ( step_ptr -> tres_alloc_str , "%s%u=%u,%u=%u" , step_ptr -> tres_alloc_str ? "," : "" , TRES_CPU , 1 , TRES_NODE , 1 ) ;
 }
 else {
 char * ionodes = NULL , * temp_nodes = NULL ;
 if ( step_ptr -> step_node_bitmap ) {
 node_inx = bit_fmt ( temp_bit , sizeof ( temp_bit ) , step_ptr -> step_node_bitmap ) ;
 }
 if ( ! step_ptr -> step_layout || ! step_ptr -> step_layout -> task_cnt ) {
 if ( step_ptr -> cpu_count ) tasks = step_ptr -> cpu_count ;
 else {
 if ( ( tasks = slurmdb_find_tres_count_in_string ( step_ptr -> tres_alloc_str , TRES_CPU ) ) == INFINITE64 ) {
 if ( ( tasks = slurmdb_find_tres_count_in_string ( step_ptr -> job_ptr -> tres_alloc_str , TRES_CPU ) ) == INFINITE64 ) tasks = step_ptr -> job_ptr -> total_nodes ;
 }
 }
 nodes = step_ptr -> job_ptr -> total_nodes ;
 temp_nodes = step_ptr -> job_ptr -> nodes ;
 }
 else {
 tasks = step_ptr -> step_layout -> task_cnt ;
 # ifdef HAVE_BGQ select_g_select_jobinfo_get ( step_ptr -> select_jobinfo , SELECT_JOBDATA_NODE_CNT , & nodes ) ;
 # else nodes = step_ptr -> step_layout -> node_cnt ;
 # endif task_dist = step_ptr -> step_layout -> task_dist ;
 temp_nodes = step_ptr -> step_layout -> node_list ;
 }
 select_g_select_jobinfo_get ( step_ptr -> select_jobinfo , SELECT_JOBDATA_IONODES , & ionodes ) ;
 if ( ionodes ) {
 snprintf ( node_list , BUFFER_SIZE , "%s[%s]" , temp_nodes , ionodes ) ;
 xfree ( ionodes ) ;
 }
 else snprintf ( node_list , BUFFER_SIZE , "%s" , temp_nodes ) ;
 }
 if ( ! step_ptr -> job_ptr -> db_index ) {
 if ( ! ( step_ptr -> job_ptr -> db_index = _get_db_index ( mysql_conn , submit_time , step_ptr -> job_ptr -> job_id , step_ptr -> job_ptr -> assoc_id ) ) ) {
 if ( as_mysql_job_start ( mysql_conn , step_ptr -> job_ptr ) == SLURM_ERROR ) {
 error ( "couldn't add job %u at step start" , step_ptr -> job_ptr -> job_id ) ;
 return SLURM_SUCCESS ;
 }
 }
 }
 step_name = slurm_add_slash_to_quotes ( step_ptr -> name ) ;
 query = xstrdup_printf ( "insert into \"%s_%s\" (job_db_inx, id_step, time_start, " "step_name, state, tres_alloc, " "nodes_alloc, task_cnt, nodelist, node_inx, " "task_dist, req_cpufreq, req_cpufreq_min, req_cpufreq_gov) " "values (%" PRIu64 ", %d, %d, '%s', %d, '%s', %d, %d, " "'%s', '%s', %d, %u, %u, %u) " "on duplicate key update " "nodes_alloc=%d, task_cnt=%d, time_end=0, state=%d, " "nodelist='%s', node_inx='%s', task_dist=%d, " "req_cpufreq=%u, req_cpufreq_min=%u, req_cpufreq_gov=%u," "tres_alloc='%s';
" , mysql_conn -> cluster_name , step_table , step_ptr -> job_ptr -> db_index , step_ptr -> step_id , ( int ) start_time , step_name , JOB_RUNNING , step_ptr -> tres_alloc_str , nodes , tasks , node_list , node_inx , task_dist , step_ptr -> cpu_freq_max , step_ptr -> cpu_freq_min , step_ptr -> cpu_freq_gov , nodes , tasks , JOB_RUNNING , node_list , node_inx , task_dist , step_ptr -> cpu_freq_max , step_ptr -> cpu_freq_min , step_ptr -> cpu_freq_gov , step_ptr -> tres_alloc_str ) ;
 if ( debug_flags & DEBUG_FLAG_DB_STEP ) DB_DEBUG ( mysql_conn -> conn , "query\n%s" , query ) ;
 rc = mysql_db_query ( mysql_conn , query ) ;
 xfree ( query ) ;
 xfree ( step_name ) ;
 return rc ;
 }