static void sigcomp_init_udvm ( void ) {
 gchar * partial_state_str ;
 guint8 * sip_sdp_buff , * presence_buff ;
 state_buffer_table = g_hash_table_new_full ( g_str_hash , g_str_equal , g_free , g_free ) ;
 sip_sdp_buff = ( guint8 * ) g_malloc ( SIP_SDP_STATE_LENGTH + 8 ) ;
 partial_state_str = bytes_to_str ( NULL , sip_sdp_state_identifier , 6 ) ;
 memset ( sip_sdp_buff , 0 , 8 ) ;
 sip_sdp_buff [ 0 ] = SIP_SDP_STATE_LENGTH >> 8 ;
 sip_sdp_buff [ 1 ] = SIP_SDP_STATE_LENGTH & 0xff ;
 memcpy ( sip_sdp_buff + 8 , sip_sdp_static_dictionaty_for_sigcomp , SIP_SDP_STATE_LENGTH ) ;
 g_hash_table_insert ( state_buffer_table , g_strdup ( partial_state_str ) , sip_sdp_buff ) ;
 wmem_free ( NULL , partial_state_str ) ;
 presence_buff = ( guint8 * ) g_malloc ( PRESENCE_STATE_LENGTH + 8 ) ;
 partial_state_str = bytes_to_str ( NULL , presence_state_identifier , 6 ) ;
 memset ( presence_buff , 0 , 8 ) ;
 presence_buff [ 0 ] = PRESENCE_STATE_LENGTH >> 8 ;
 presence_buff [ 1 ] = PRESENCE_STATE_LENGTH & 0xff ;
 memcpy ( presence_buff + 8 , presence_static_dictionary_for_sigcomp , PRESENCE_STATE_LENGTH ) ;
 g_hash_table_insert ( state_buffer_table , g_strdup ( partial_state_str ) , presence_buff ) ;
 wmem_free ( NULL , partial_state_str ) ;
 }