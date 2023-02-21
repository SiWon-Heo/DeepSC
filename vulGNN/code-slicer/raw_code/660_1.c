static gboolean dissect_applemidi_heur ( tvbuff_t * tvb , packet_info * pinfo , proto_tree * tree , void * data _U_ ) {
 guint16 command ;
 conversation_t * p_conv ;
 encoding_name_and_rate_t * encoding_name_and_rate = NULL ;
 GHashTable * rtp_dyn_payload = NULL ;
 gint * key ;
 if ( tvb_length ( tvb ) < 4 ) return FALSE ;
 if ( ! test_applemidi ( tvb , & command , FALSE ) ) {
 return FALSE ;
 }
 encoding_name_and_rate = wmem_new ( wmem_file_scope ( ) , encoding_name_and_rate_t ) ;
 rtp_dyn_payload = g_hash_table_new ( g_int_hash , g_int_equal ) ;
 encoding_name_and_rate -> encoding_name = wmem_strdup ( wmem_file_scope ( ) , "rtp-midi" ) ;
 encoding_name_and_rate -> sample_rate = 10000 ;
 key = wmem_new ( wmem_file_scope ( ) , gint ) ;
 * key = 97 ;
 g_hash_table_insert ( rtp_dyn_payload , key , encoding_name_and_rate ) ;
 rtp_add_address ( pinfo , & pinfo -> src , pinfo -> srcport , 0 , APPLEMIDI_DISSECTOR_SHORTNAME , pinfo -> fd -> num , FALSE , rtp_dyn_payload ) ;
 p_conv = find_or_create_conversation ( pinfo ) ;
 conversation_set_dissector ( p_conv , applemidi_handle ) ;
 dissect_applemidi_common ( tvb , pinfo , tree , command ) ;
 return TRUE ;
 }