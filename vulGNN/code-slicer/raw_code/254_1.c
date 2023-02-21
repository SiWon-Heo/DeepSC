static void parse_content_type ( struct message_decoder_context * ctx , struct message_header_line * hdr ) {
 struct rfc822_parser_context parser ;
 const char * const * results ;
 string_t * str ;
 int ret ;
 if ( ctx -> content_type != NULL ) return ;
 rfc822_parser_init ( & parser , hdr -> full_value , hdr -> full_value_len , NULL ) ;
 rfc822_skip_lwsp ( & parser ) ;
 str = t_str_new ( 64 ) ;
 ret = rfc822_parse_content_type ( & parser , str ) ;
 ctx -> content_type = i_strdup ( str_c ( str ) ) ;
 if ( ret < 0 ) return ;
 rfc2231_parse ( & parser , & results ) ;
 for ( ;
 * results != NULL ;
 results += 2 ) {
 if ( strcasecmp ( results [ 0 ] , "charset" ) == 0 ) {
 ctx -> content_charset = i_strdup ( results [ 1 ] ) ;
 break ;
 }
 }
 }