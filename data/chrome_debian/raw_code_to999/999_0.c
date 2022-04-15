static void join_print_po ( struct packet_object * po ) {
 int ret ;
 if ( wdg_conndata == NULL || wdg_join == NULL ) return ;
 if ( ! ( wdg_conndata -> flags & WDG_OBJ_FOCUSED ) ) return ;
 if ( GBL_OPTIONS -> regex && regexec ( GBL_OPTIONS -> regex , ( const char * ) po -> DATA . disp_data , 0 , NULL , 0 ) != 0 ) {
 return ;
 }
 SAFE_REALLOC ( dispbuf , hex_len ( po -> DATA . disp_len ) * sizeof ( u_char ) + 1 ) ;
 ret = GBL_FORMAT ( po -> DATA . disp_data , po -> DATA . disp_len , dispbuf ) ;
 dispbuf [ ret ] = 0 ;
 if ( ! ip_addr_cmp ( & po -> L3 . src , & curr_conn -> L3_addr1 ) ) wdg_scroll_print ( wdg_join , EC_COLOR_JOIN1 , "%s" , dispbuf ) ;
 else wdg_scroll_print ( wdg_join , EC_COLOR_JOIN2 , "%s" , dispbuf ) ;
 }