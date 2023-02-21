static void _slurm_rpc_allocate_resources ( slurm_msg_t * msg ) {
 static int active_rpc_cnt = 0 ;
 int error_code = SLURM_SUCCESS ;
 slurm_msg_t response_msg ;
 DEF_TIMERS ;
 job_desc_msg_t * job_desc_msg = ( job_desc_msg_t * ) msg -> data ;
 resource_allocation_response_msg_t alloc_msg ;
 slurmctld_lock_t job_read_lock = {
 READ_LOCK , READ_LOCK , READ_LOCK , READ_LOCK , READ_LOCK }
 ;
 slurmctld_lock_t job_write_lock = {
 READ_LOCK , WRITE_LOCK , WRITE_LOCK , READ_LOCK , READ_LOCK }
 ;
 uid_t uid = g_slurm_auth_get_uid ( msg -> auth_cred , slurmctld_config . auth_info ) ;
 int immediate = job_desc_msg -> immediate ;
 bool do_unlock = false ;
 bool reject_job = false ;
 struct job_record * job_ptr = NULL ;
 uint16_t port ;
 slurm_addr_t resp_addr ;
 char * err_msg = NULL , * job_submit_user_msg = NULL ;
 START_TIMER ;
 if ( slurmctld_config . submissions_disabled ) {
 info ( "Submissions disabled on system" ) ;
 error_code = ESLURM_SUBMISSIONS_DISABLED ;
 reject_job = true ;
 goto send_msg ;
 }
 if ( ( uid != job_desc_msg -> user_id ) && ( ! validate_slurm_user ( uid ) ) ) {
 error_code = ESLURM_USER_ID_MISSING ;
 error ( "Security violation, RESOURCE_ALLOCATE from uid=%d" , uid ) ;
 }
 debug2 ( "sched: Processing RPC: REQUEST_RESOURCE_ALLOCATION from uid=%d" , uid ) ;
 if ( ( job_desc_msg -> alloc_node == NULL ) || ( job_desc_msg -> alloc_node [ 0 ] == '\0' ) ) {
 error_code = ESLURM_INVALID_NODE_NAME ;
 error ( "REQUEST_RESOURCE_ALLOCATE lacks alloc_node from uid=%d" , uid ) ;
 }
 if ( error_code == SLURM_SUCCESS ) {
 lock_slurmctld ( job_read_lock ) ;
 job_desc_msg -> pack_job_offset = NO_VAL ;
 error_code = validate_job_create_req ( job_desc_msg , uid , & err_msg ) ;
 unlock_slurmctld ( job_read_lock ) ;
 }
 if ( err_msg ) job_submit_user_msg = xstrdup ( err_msg ) ;
 # if HAVE_ALPS_CRAY if ( allocated_session_in_use ( job_desc_msg ) ) {
 error_code = ESLURM_RESERVATION_BUSY ;
 error ( "attempt to nest ALPS allocation on %s:%d by uid=%d" , job_desc_msg -> alloc_node , job_desc_msg -> alloc_sid , uid ) ;
 }
 # endif if ( error_code ) {
 reject_job = true ;
 }
 else if ( ! slurm_get_peer_addr ( msg -> conn_fd , & resp_addr ) ) {
 if ( ! job_desc_msg -> resp_host ) {
 job_desc_msg -> resp_host = xmalloc ( 16 ) ;
 slurm_get_ip_str ( & resp_addr , & port , job_desc_msg -> resp_host , 16 ) ;
 }
 dump_job_desc ( job_desc_msg ) ;
 do_unlock = true ;
 _throttle_start ( & active_rpc_cnt ) ;
 lock_slurmctld ( job_write_lock ) ;
 if ( fed_mgr_fed_rec ) {
 uint32_t job_id ;
 if ( fed_mgr_job_allocate ( msg , job_desc_msg , true , uid , msg -> protocol_version , & job_id , & error_code , & err_msg ) ) {
 reject_job = true ;
 }
 else if ( ! ( job_ptr = find_job_record ( job_id ) ) ) {
 error ( "%s: can't find fed job that was just created. this should never happen" , __func__ ) ;
 reject_job = true ;
 error_code = SLURM_ERROR ;
 }
 }
 else {
 job_desc_msg -> pack_job_offset = NO_VAL ;
 error_code = job_allocate ( job_desc_msg , immediate , false , NULL , true , uid , & job_ptr , & err_msg , msg -> protocol_version ) ;
 if ( ! job_ptr || ( error_code && job_ptr -> job_state == JOB_FAILED ) ) reject_job = true ;
 }
 END_TIMER2 ( "_slurm_rpc_allocate_resources" ) ;
 }
 else {
 reject_job = true ;
 if ( errno ) error_code = errno ;
 else error_code = SLURM_ERROR ;
 }
 send_msg : if ( ! reject_job ) {
 xassert ( job_ptr ) ;
 info ( "sched: %s JobId=%u NodeList=%s %s" , __func__ , job_ptr -> job_id , job_ptr -> nodes , TIME_STR ) ;
 _build_alloc_msg ( job_ptr , & alloc_msg , error_code , job_submit_user_msg ) ;
 if ( do_unlock ) {
 unlock_slurmctld ( job_write_lock ) ;
 _throttle_fini ( & active_rpc_cnt ) ;
 }
 slurm_msg_t_init ( & response_msg ) ;
 response_msg . conn = msg -> conn ;
 response_msg . flags = msg -> flags ;
 response_msg . protocol_version = msg -> protocol_version ;
 response_msg . msg_type = RESPONSE_RESOURCE_ALLOCATION ;
 response_msg . data = & alloc_msg ;
 if ( slurm_send_node_msg ( msg -> conn_fd , & response_msg ) < 0 ) _kill_job_on_msg_fail ( job_ptr -> job_id ) ;
 schedule_job_save ( ) ;
 schedule_node_save ( ) ;
 if ( ! alloc_msg . node_cnt ) queue_job_scheduler ( ) ;
 alloc_msg . working_cluster_rec = NULL ;
 slurm_free_resource_allocation_response_msg_members ( & alloc_msg ) ;
 }
 else {
 if ( do_unlock ) {
 unlock_slurmctld ( job_write_lock ) ;
 _throttle_fini ( & active_rpc_cnt ) ;
 }
 info ( "%s: %s " , __func__ , slurm_strerror ( error_code ) ) ;
 if ( err_msg ) slurm_send_rc_err_msg ( msg , error_code , err_msg ) ;
 else slurm_send_rc_msg ( msg , error_code ) ;
 }
 xfree ( err_msg ) ;
 xfree ( job_submit_user_msg ) ;
 }