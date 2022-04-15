static void join_print ( u_char * text , size_t len , struct ip_addr * L3_src ) {
 int ret ;
 if ( GBL_OPTIONS -> regex && regexec ( GBL_OPTIONS -> regex , text , 0 , NULL , 0 ) != 0 ) {
 return ;
 }
 SAFE_REALLOC ( dispbuf , hex_len ( len ) * sizeof ( u_char ) + 1 ) ;
 ret = GBL_FORMAT ( text , len , dispbuf ) ;
 dispbuf [ ret ] = 0 ;
 if ( ! ip_addr_cmp ( L3_src , & curr_conn -> L3_addr1 ) ) gtkui_data_print ( 3 , dispbuf , 1 ) ;
 else gtkui_data_print ( 3 , dispbuf , 2 ) ;
 }