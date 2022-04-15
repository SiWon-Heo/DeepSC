void rtp_free_hash_dyn_payload ( GHashTable * rtp_dyn_payload ) {
 if ( rtp_dyn_payload == NULL ) return ;
 g_hash_table_destroy ( rtp_dyn_payload ) ;
 rtp_dyn_payload = NULL ;
 }