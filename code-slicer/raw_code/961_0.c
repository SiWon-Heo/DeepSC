static void split_print_po ( struct packet_object * po ) {
 int ret ;
 if ( ! data_window ) return ;
 if ( GBL_OPTIONS -> regex && regexec ( GBL_OPTIONS -> regex , po -> DATA . disp_data , 0 , NULL , 0 ) != 0 ) {
 return ;
 }
 SAFE_REALLOC ( dispbuf , hex_len ( po -> DATA . disp_len ) * sizeof ( u_char ) + 1 ) ;
 ret = GBL_FORMAT ( po -> DATA . disp_data , po -> DATA . disp_len , dispbuf ) ;
 dispbuf [ ret ] = 0 ;
 if ( ! ip_addr_cmp ( & po -> L3 . src , & curr_conn -> L3_addr1 ) ) gtkui_data_print ( 1 , dispbuf , 0 ) ;
 else gtkui_data_print ( 2 , dispbuf , 0 ) ;
 }