static bool on_accept ( private_stroke_socket_t * this , stream_t * stream ) {
 stroke_msg_t * msg ;
 uint16_t len ;
 FILE * out ;
 if ( ! stream -> read_all ( stream , & len , sizeof ( len ) ) ) {
 if ( errno != EWOULDBLOCK ) {
 DBG1 ( DBG_CFG , "reading length of stroke message failed: %s" , strerror ( errno ) ) ;
 }
 return FALSE ;
 }
 msg = malloc ( len + 1 ) ;
 msg -> length = len ;
 if ( ! stream -> read_all ( stream , ( char * ) msg + sizeof ( len ) , len - sizeof ( len ) ) ) {
 if ( errno != EWOULDBLOCK ) {
 DBG1 ( DBG_CFG , "reading stroke message failed: %s" , strerror ( errno ) ) ;
 }
 free ( msg ) ;
 return FALSE ;
 }
 ( ( char * ) msg ) [ len ] = '\0' ;
 DBG3 ( DBG_CFG , "stroke message %b" , ( void * ) msg , len ) ;
 out = stream -> get_file ( stream ) ;
 if ( ! out ) {
 DBG1 ( DBG_CFG , "creating stroke output stream failed" ) ;
 free ( msg ) ;
 return FALSE ;
 }
 switch ( msg -> type ) {
 case STR_INITIATE : stroke_initiate ( this , msg , out ) ;
 break ;
 case STR_ROUTE : stroke_route ( this , msg , out ) ;
 break ;
 case STR_UNROUTE : stroke_unroute ( this , msg , out ) ;
 break ;
 case STR_TERMINATE : stroke_terminate ( this , msg , out ) ;
 break ;
 case STR_TERMINATE_SRCIP : stroke_terminate_srcip ( this , msg , out ) ;
 break ;
 case STR_REKEY : stroke_rekey ( this , msg , out ) ;
 break ;
 case STR_STATUS : stroke_status ( this , msg , out , FALSE , TRUE ) ;
 break ;
 case STR_STATUS_ALL : stroke_status ( this , msg , out , TRUE , TRUE ) ;
 break ;
 case STR_STATUS_ALL_NOBLK : stroke_status ( this , msg , out , TRUE , FALSE ) ;
 break ;
 case STR_ADD_CONN : stroke_add_conn ( this , msg ) ;
 break ;
 case STR_DEL_CONN : stroke_del_conn ( this , msg ) ;
 break ;
 case STR_ADD_CA : stroke_add_ca ( this , msg , out ) ;
 break ;
 case STR_DEL_CA : stroke_del_ca ( this , msg , out ) ;
 break ;
 case STR_LOGLEVEL : stroke_loglevel ( this , msg , out ) ;
 break ;
 case STR_CONFIG : stroke_config ( this , msg , out ) ;
 break ;
 case STR_LIST : stroke_list ( this , msg , out ) ;
 break ;
 case STR_REREAD : stroke_reread ( this , msg , out ) ;
 break ;
 case STR_PURGE : stroke_purge ( this , msg , out ) ;
 break ;
 case STR_EXPORT : stroke_export ( this , msg , out ) ;
 break ;
 case STR_LEASES : stroke_leases ( this , msg , out ) ;
 break ;
 case STR_MEMUSAGE : stroke_memusage ( this , msg , out ) ;
 break ;
 case STR_USER_CREDS : stroke_user_creds ( this , msg , out ) ;
 break ;
 case STR_COUNTERS : stroke_counters ( this , msg , out ) ;
 break ;
 default : DBG1 ( DBG_CFG , "received unknown stroke" ) ;
 break ;
 }
 free ( msg ) ;
 fclose ( out ) ;
 return FALSE ;
 }