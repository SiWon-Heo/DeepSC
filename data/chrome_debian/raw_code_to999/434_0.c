wtap_open_return_val ascend_open ( wtap * wth , int * err , gchar * * err_info ) {
 gint64 offset ;
 ws_statb64 statbuf ;
 ascend_t * ascend ;
 wth -> priv = NULL ;
 offset = ascend_seek ( wth , err , err_info ) ;
 if ( offset == - 1 ) {
 if ( * err != 0 && * err != WTAP_ERR_SHORT_READ ) return WTAP_OPEN_ERROR ;
 return WTAP_OPEN_NOT_MINE ;
 }
 init_parse_ascend ( ) ;
 if ( ! check_ascend ( wth -> fh , & wth -> phdr ) ) {
 return WTAP_OPEN_NOT_MINE ;
 }
 wth -> file_type_subtype = WTAP_FILE_TYPE_SUBTYPE_ASCEND ;
 switch ( wth -> phdr . pseudo_header . ascend . type ) {
 case ASCEND_PFX_ISDN_X : case ASCEND_PFX_ISDN_R : wth -> file_encap = WTAP_ENCAP_ISDN ;
 break ;
 case ASCEND_PFX_ETHER : wth -> file_encap = WTAP_ENCAP_ETHERNET ;
 break ;
 default : wth -> file_encap = WTAP_ENCAP_ASCEND ;
 }
 wth -> snapshot_length = ASCEND_MAX_PKT_LEN ;
 wth -> subtype_read = ascend_read ;
 wth -> subtype_seek_read = ascend_seek_read ;
 ascend = ( ascend_t * ) g_malloc ( sizeof ( ascend_t ) ) ;
 wth -> priv = ( void * ) ascend ;
 ascend -> next_packet_seek_start = offset ;
 if ( wtap_fstat ( wth , & statbuf , err ) == - 1 ) {
 return WTAP_OPEN_ERROR ;
 }
 ascend -> inittime = statbuf . st_ctime ;
 ascend -> adjusted = FALSE ;
 wth -> file_tsprec = WTAP_TSPREC_USEC ;
 init_parse_ascend ( ) ;
 return WTAP_OPEN_MINE ;
 }