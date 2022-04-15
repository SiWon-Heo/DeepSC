static gboolean ascend_read ( wtap * wth , int * err , gchar * * err_info , gint64 * data_offset ) {
 ascend_t * ascend = ( ascend_t * ) wth -> priv ;
 gint64 offset ;
 if ( file_seek ( wth -> fh , ascend -> next_packet_seek_start , SEEK_SET , err ) == - 1 ) return FALSE ;
 offset = ascend_seek ( wth , err , err_info ) ;
 if ( offset == - 1 ) return FALSE ;
 if ( parse_ascend ( ascend , wth -> fh , & wth -> phdr , wth -> frame_buffer , wth -> snapshot_length ) != PARSED_RECORD ) {
 * err = WTAP_ERR_BAD_FILE ;
 * err_info = g_strdup ( ( ascend_parse_error != NULL ) ? ascend_parse_error : "parse error" ) ;
 return FALSE ;
 }
 * data_offset = offset ;
 return TRUE ;
 }