static gboolean ascend_seek_read ( wtap * wth , gint64 seek_off , struct wtap_pkthdr * phdr , Buffer * buf , int * err , gchar * * err_info ) {
 ascend_t * ascend = ( ascend_t * ) wth -> priv ;
 if ( file_seek ( wth -> random_fh , seek_off , SEEK_SET , err ) == - 1 ) return FALSE ;
 if ( parse_ascend ( ascend , wth -> random_fh , phdr , buf , wth -> snapshot_length ) != PARSED_RECORD ) {
 * err = WTAP_ERR_BAD_FILE ;
 * err_info = g_strdup ( ( ascend_parse_error != NULL ) ? ascend_parse_error : "parse error" ) ;
 return FALSE ;
 }
 return TRUE ;
 }