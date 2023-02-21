static void split_print ( u_char * text , size_t len , struct ip_addr * L3_src ) {
 int ret ;
 if ( GBL_OPTIONS -> regex && regexec ( GBL_OPTIONS -> regex , ( const char * ) text , 0 , NULL , 0 ) != 0 ) {
 return ;
 }
 SAFE_REALLOC ( dispbuf , hex_len ( len ) * sizeof ( u_char ) + 1 ) ;
 ret = GBL_FORMAT ( text , len , dispbuf ) ;
 dispbuf [ ret ] = 0 ;
 if ( ! ip_addr_cmp ( L3_src , & curr_conn -> L3_addr1 ) ) wdg_scroll_print ( wdg_c1 , EC_COLOR , "%s" , dispbuf ) ;
 else wdg_scroll_print ( wdg_c2 , EC_COLOR , "%s" , dispbuf ) ;
 }