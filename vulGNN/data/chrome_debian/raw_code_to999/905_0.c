static void udvm_state_create ( guint8 * state_buff , guint8 * state_identifier , guint16 p_id_length ) {
 char partial_state [ STATE_BUFFER_SIZE ] ;
 guint i ;
 gchar * partial_state_str ;
 gchar * dummy_buff ;
 i = 0 ;
 while ( i < p_id_length && i < STATE_BUFFER_SIZE ) {
 partial_state [ i ] = state_identifier [ i ] ;
 i ++ ;
 }
 partial_state_str = bytes_to_str ( NULL , partial_state , p_id_length ) ;
 dummy_buff = ( gchar * ) g_hash_table_lookup ( state_buffer_table , partial_state_str ) ;
 if ( dummy_buff == NULL ) {
 g_hash_table_insert ( state_buffer_table , g_strdup ( partial_state_str ) , state_buff ) ;
 }
 else {
 g_free ( state_buff ) ;
 }
 wmem_free ( NULL , partial_state_str ) ;
 }