static void test_rfc822_parse_content_param ( void ) {
 const char * input = ";
 key1=value1#$!%&'*+-.^_`{
|}
~" ";
 key2=\" \\\"(),/:;
<=>?@[\\\\]\"" ;
 const struct {
 const char * key , * value ;
 }
 output [ ] = {
 {
 "key1" , "value1#$!%&'*+-.^_`{
|}
~" }
 , {
 "key2" , " \"(),/:;
<=>?@[\\]" }
 }
 ;
 struct rfc822_parser_context parser ;
 const char * key , * value ;
 unsigned int i = 0 ;
 int ret ;
 test_begin ( "rfc822 parse content param" ) ;
 rfc822_parser_init ( & parser , ( const void * ) input , strlen ( input ) , NULL ) ;
 while ( ( ret = rfc822_parse_content_param ( & parser , & key , & value ) ) > 0 && i < N_ELEMENTS ( output ) ) {
 test_assert_idx ( strcmp ( output [ i ] . key , key ) == 0 , i ) ;
 test_assert_idx ( strcmp ( output [ i ] . value , value ) == 0 , i ) ;
 i ++ ;
 }
 test_assert ( ret == 0 ) ;
 test_assert ( i == N_ELEMENTS ( output ) ) ;
 test_end ( ) ;
 }