static void parse_content_disposition ( const char * content_disposition , const char * * filename_r ) {
 struct rfc822_parser_context parser ;
 const char * const * results , * filename2 ;
 string_t * str ;
 * filename_r = NULL ;
 if ( content_disposition == NULL ) return ;
 rfc822_parser_init ( & parser , ( const unsigned char * ) content_disposition , strlen ( content_disposition ) , NULL ) ;
 rfc822_skip_lwsp ( & parser ) ;
 str = t_str_new ( 32 ) ;
 if ( rfc822_parse_mime_token ( & parser , str ) < 0 ) return ;
 rfc2231_parse ( & parser , & results ) ;
 filename2 = NULL ;
 for ( ;
 * results != NULL ;
 results += 2 ) {
 if ( strcasecmp ( results [ 0 ] , "filename" ) == 0 ) {
 * filename_r = results [ 1 ] ;
 break ;
 }
 if ( strcasecmp ( results [ 0 ] , "filename*" ) == 0 ) filename2 = results [ 1 ] ;
 }
 if ( * filename_r == NULL ) {
 * filename_r = filename2 ;
 }
 }