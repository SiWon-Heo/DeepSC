static gint64 ascend_seek ( wtap * wth , int * err , gchar * * err_info ) {
 int byte ;
 gint64 date_off = - 1 , cur_off , packet_off ;
 size_t string_level [ ASCEND_MAGIC_STRINGS ] ;
 guint string_i = 0 , type = 0 ;
 guint excessive_read_count = 262144 ;
 memset ( & string_level , 0 , sizeof ( string_level ) ) ;
 while ( ( ( byte = file_getc ( wth -> fh ) ) != EOF ) ) {
 excessive_read_count -- ;
 if ( ! excessive_read_count ) {
 * err = 0 ;
 return - 1 ;
 }
 for ( string_i = 0 ;
 string_i < ASCEND_MAGIC_STRINGS ;
 string_i ++ ) {
 const gchar * strptr = ascend_magic [ string_i ] . strptr ;
 size_t len = strlen ( strptr ) ;
 if ( byte == * ( strptr + string_level [ string_i ] ) ) {
 string_level [ string_i ] ++ ;
 if ( string_level [ string_i ] >= len ) {
 cur_off = file_tell ( wth -> fh ) ;
 if ( cur_off == - 1 ) {
 * err = file_error ( wth -> fh , err_info ) ;
 return - 1 ;
 }
 if ( strcmp ( strptr , ASCEND_DATE ) == 0 ) {
 date_off = cur_off - len ;
 string_level [ string_i ] = 0 ;
 }
 else {
 if ( date_off == - 1 ) {
 packet_off = cur_off - len ;
 }
 else {
 packet_off = date_off ;
 }
 type = ascend_magic [ string_i ] . type ;
 goto found ;
 }
 }
 }
 else {
 string_level [ string_i ] = 0 ;
 }
 }
 }
 * err = file_error ( wth -> fh , err_info ) ;
 return - 1 ;
 found : if ( file_seek ( wth -> fh , packet_off , SEEK_SET , err ) == - 1 ) return - 1 ;
 wth -> phdr . pseudo_header . ascend . type = type ;
 return packet_off ;
 }