static void _slurm_rpc_submit_batch_job ( slurm_msg_t * msg ) {
 static int active_rpc_cnt = 0 ;
 int error_code = SLURM_SUCCESS ;
 DEF_TIMERS ;
 uint32_t job_id = 0 , priority = 0 ;
 struct job_record * job_ptr = NULL ;
 slurm_msg_t response_msg ;
 submit_response_msg_t submit_msg ;
 job_desc_msg_t * job_desc_msg = ( job_desc_msg_t * ) msg -> data ;
 slurmctld_lock_t job_read_lock = {
 READ_LOCK , READ_LOCK , READ_LOCK , READ_LOCK , READ_LOCK }
 ;
 slurmctld_lock_t job_write_lock = {
 READ_LOCK , WRITE_LOCK , WRITE_LOCK , READ_LOCK , READ_LOCK }
 ;
 uid_t uid = g_slurm_auth_get_uid ( msg -> auth_cred , slurmctld_config . auth_info ) ;
 char * err_msg = NULL , * job_submit_user_msg = NULL ;
 bool reject_job = false ;
 START_TIMER ;
 debug2 ( "Processing RPC: REQUEST_SUBMIT_BATCH_JOB from uid=%d" , uid ) ;
 if ( slurmctld_config . submissions_disabled ) {
 info ( "Submissions disabled on system" ) ;
 error_code = ESLURM_SUBMISSIONS_DISABLED ;
 reject_job = true ;
 goto send_msg ;
 }
 if ( ( uid != job_desc_msg -> user_id ) && ( ! validate_super_user ( uid ) ) ) {
 error_code = ESLURM_USER_ID_MISSING ;
 error ( "Security violation, REQUEST_SUBMIT_BATCH_JOB from uid=%d" , uid ) ;
 }
 if ( ( job_desc_msg -> alloc_node == NULL ) || ( job_desc_msg -> alloc_node [ 0 ] == '\0' ) ) {
 error_code = ESLURM_INVALID_NODE_NAME ;
 error ( "REQUEST_SUBMIT_BATCH_JOB lacks alloc_node from uid=%d" , uid ) ;
 }
 dump_job_desc ( job_desc_msg ) ;
 if ( error_code == SLURM_SUCCESS ) {
 lock_slurmctld ( job_read_lock ) ;
 job_desc_msg -> pack_job_offset = NO_VAL ;
 error_code = validate_job_create_req ( job_desc_msg , uid , & err_msg ) ;
 unlock_slurmctld ( job_read_lock ) ;
 }
 if ( err_msg ) job_submit_user_msg = xstrdup ( err_msg ) ;
 if ( error_code ) {
 reject_job = true ;
 goto send_msg ;
 }
 _throttle_start ( & active_rpc_cnt ) ;
 lock_slurmctld ( job_write_lock ) ;
 START_TIMER ;
 if ( fed_mgr_fed_rec ) {
 if ( fed_mgr_job_allocate ( msg , job_desc_msg , false , uid , msg -> protocol_version , & job_id , & error_code , & err_msg ) ) reject_job = true ;
 }
 else {
 job_desc_msg -> pack_job_offset = NO_VAL ;
 error_code = job_allocate ( job_desc_msg , job_desc_msg -> immediate , false , NULL , 0 , uid , & job_ptr , & err_msg , msg -> protocol_version ) ;
 if ( ! job_ptr || ( error_code && job_ptr -> job_state == JOB_FAILED ) ) reject_job = true ;
 else {
 job_id = job_ptr -> job_id ;
 priority = job_ptr -> priority ;
 }
 if ( job_desc_msg -> immediate && ( error_code != SLURM_SUCCESS ) ) {
 error_code = ESLURM_CAN_NOT_START_IMMEDIATELY ;
 reject_job = true ;
 }
 }
 unlock_slurmctld ( job_write_lock ) ;
 _throttle_fini ( & active_rpc_cnt ) ;
 send_msg : END_TIMER2 ( "_slurm_rpc_submit_batch_job" ) ;
 if ( reject_job ) {
 info ( "%s: %s" , __func__ , slurm_strerror ( error_code ) ) ;
 if ( err_msg ) slurm_send_rc_err_msg ( msg , error_code , err_msg ) ;
 else slurm_send_rc_msg ( msg , error_code ) ;
 }
 else {
 info ( "%s: JobId=%u InitPrio=%u %s" , __func__ , job_id , priority , TIME_STR ) ;
 submit_msg . job_id = job_id ;
 submit_msg . step_id = SLURM_BATCH_SCRIPT ;
 submit_msg . error_code = error_code ;
 submit_msg . job_submit_user_msg = job_submit_user_msg ;
 slurm_msg_t_init ( & response_msg ) ;
 response_msg . flags = msg -> flags ;
 response_msg . protocol_version = msg -> protocol_version ;
 response_msg . conn = msg -> conn ;
 response_msg . msg_type = RESPONSE_SUBMIT_BATCH_JOB ;
 response_msg . data = & submit_msg ;
 slurm_send_node_msg ( msg -> conn_fd , & response_msg ) ;
 schedule_job_save ( ) ;
 schedule_node_save ( ) ;
 queue_job_scheduler ( ) ;
 }
 xfree ( err_msg ) ;
 xfree ( job_submit_user_msg ) ;
 }