int encode_function ( char * string , struct filter_op * fop ) {
 char * str = strdup ( string ) ;
 int ret = - E_NOTFOUND ;
 char * name , * args ;
 int nargs = 0 , i ;
 char * * dec_args = NULL ;
 char * tok ;
 memset ( fop , 0 , sizeof ( struct filter_op ) ) ;
 name = ec_strtok ( string , "(" , & tok ) ;
 args = name + strlen ( name ) + 1 ;
 dec_args = decode_args ( args , & nargs ) ;
 fop -> opcode = FOP_FUNC ;
 if ( ! strcmp ( name , "search" ) ) {
 if ( nargs == 2 ) {
 if ( encode_offset ( dec_args [ 0 ] , fop ) == E_SUCCESS ) {
 fop -> opcode = FOP_FUNC ;
 fop -> op . func . op = FFUNC_SEARCH ;
 fop -> op . func . string = ( u_char * ) strdup ( dec_args [ 1 ] ) ;
 fop -> op . func . slen = strescape ( ( char * ) fop -> op . func . string , ( char * ) fop -> op . func . string ) ;
 ret = E_SUCCESS ;
 }
 else SCRIPT_ERROR ( "Unknown offset %s " , dec_args [ 0 ] ) ;
 }
 else SCRIPT_ERROR ( "Wrong number of arguments for function \"%s\" " , name ) ;
 }
 else if ( ! strcmp ( name , "regex" ) ) {
 if ( nargs == 2 ) {
 int err ;
 regex_t regex ;
 char errbuf [ 100 ] ;
 if ( encode_offset ( dec_args [ 0 ] , fop ) == E_SUCCESS ) {
 fop -> opcode = FOP_FUNC ;
 fop -> op . func . op = FFUNC_REGEX ;
 fop -> op . func . string = ( u_char * ) strdup ( dec_args [ 1 ] ) ;
 fop -> op . func . slen = strescape ( ( char * ) fop -> op . func . string , ( char * ) fop -> op . func . string ) ;
 ret = E_SUCCESS ;
 }
 else SCRIPT_ERROR ( "Unknown offset %s " , dec_args [ 0 ] ) ;
 err = regcomp ( & regex , ( const char * ) fop -> op . func . string , REG_EXTENDED | REG_NOSUB | REG_ICASE ) ;
 if ( err ) {
 regerror ( err , & regex , errbuf , sizeof ( errbuf ) ) ;
 SCRIPT_ERROR ( "%s" , errbuf ) ;
 }
 regfree ( & regex ) ;
 }
 else SCRIPT_ERROR ( "Wrong number of arguments for function \"%s\" " , name ) ;
 }
 else if ( ! strcmp ( name , "pcre_regex" ) ) {
 # ifndef HAVE_PCRE WARNING ( "The script contains pcre_regex, but you don't have support for it." ) ;
 # else pcre * pregex ;
 const char * errbuf = NULL ;
 int erroff ;
 if ( nargs == 2 ) {
 if ( encode_offset ( dec_args [ 0 ] , fop ) == E_SUCCESS ) {
 fop -> opcode = FOP_FUNC ;
 fop -> op . func . op = FFUNC_PCRE ;
 fop -> op . func . string = strdup ( dec_args [ 1 ] ) ;
 fop -> op . func . slen = strlen ( fop -> op . func . string ) ;
 ret = E_SUCCESS ;
 }
 else SCRIPT_ERROR ( "Unknown offset %s " , dec_args [ 0 ] ) ;
 pregex = pcre_compile ( fop -> op . func . string , 0 , & errbuf , & erroff , NULL ) ;
 if ( pregex == NULL ) SCRIPT_ERROR ( "%s\n" , errbuf ) ;
 pcre_free ( pregex ) ;
 }
 else if ( nargs == 3 ) {
 fop -> opcode = FOP_FUNC ;
 fop -> op . func . op = FFUNC_PCRE ;
 fop -> op . func . level = 5 ;
 fop -> op . func . string = strdup ( dec_args [ 1 ] ) ;
 fop -> op . func . slen = strlen ( fop -> op . func . string ) ;
 fop -> op . func . replace = strdup ( dec_args [ 2 ] ) ;
 fop -> op . func . rlen = strlen ( fop -> op . func . replace ) ;
 ret = E_SUCCESS ;
 pregex = pcre_compile ( fop -> op . func . string , 0 , & errbuf , & erroff , NULL ) ;
 if ( pregex == NULL ) SCRIPT_ERROR ( "%s\n" , errbuf ) ;
 pcre_free ( pregex ) ;
 }
 else SCRIPT_ERROR ( "Wrong number of arguments for function \"%s\" " , name ) ;
 # endif }
 else if ( ! strcmp ( name , "replace" ) ) {
 if ( nargs == 2 ) {
 fop -> op . func . op = FFUNC_REPLACE ;
 fop -> op . func . level = 5 ;
 fop -> op . func . string = ( u_char * ) strdup ( dec_args [ 0 ] ) ;
 fop -> op . func . slen = strescape ( ( char * ) fop -> op . func . string , ( char * ) fop -> op . func . string ) ;
 fop -> op . func . replace = ( u_char * ) strdup ( dec_args [ 1 ] ) ;
 fop -> op . func . rlen = strescape ( ( char * ) fop -> op . func . replace , ( char * ) fop -> op . func . replace ) ;
 ret = E_SUCCESS ;
 }
 else SCRIPT_ERROR ( "Wrong number of arguments for function \"%s\" " , name ) ;
 }
 else if ( ! strcmp ( name , "inject" ) ) {
 if ( nargs == 1 ) {
 fop -> op . func . op = FFUNC_INJECT ;
 fop -> op . func . level = 5 ;
 fop -> op . func . string = ( u_char * ) strdup ( dec_args [ 0 ] ) ;
 fop -> op . func . slen = strlen ( ( const char * ) fop -> op . func . string ) ;
 ret = E_SUCCESS ;
 }
 else SCRIPT_ERROR ( "Wrong number of arguments for function \"%s\" " , name ) ;
 }
 else if ( ! strcmp ( name , "execinject" ) ) {
 if ( nargs == 1 ) {
 fop -> op . func . op = FFUNC_EXECINJECT ;
 fop -> op . func . level = 5 ;
 fop -> op . func . string = ( u_char * ) strdup ( dec_args [ 0 ] ) ;
 fop -> op . func . slen = strlen ( ( const char * ) fop -> op . func . string ) ;
 ret = E_SUCCESS ;
 }
 else SCRIPT_ERROR ( "Wrong number of arguments for function \"%s\" " , name ) ;
 }
 else if ( ! strcmp ( name , "log" ) ) {
 if ( nargs == 2 ) {
 if ( encode_offset ( dec_args [ 0 ] , fop ) == E_SUCCESS ) {
 fop -> opcode = FOP_FUNC ;
 fop -> op . func . op = FFUNC_LOG ;
 fop -> op . func . string = ( u_char * ) strdup ( dec_args [ 1 ] ) ;
 fop -> op . func . slen = strlen ( ( const char * ) fop -> op . func . string ) ;
 ret = E_SUCCESS ;
 }
 else SCRIPT_ERROR ( "Unknown offset %s " , dec_args [ 0 ] ) ;
 }
 else SCRIPT_ERROR ( "Wrong number of arguments for function \"%s\" " , name ) ;
 }
 else if ( ! strcmp ( name , "drop" ) ) {
 if ( nargs == 0 ) {
 fop -> op . func . op = FFUNC_DROP ;
 ret = E_SUCCESS ;
 }
 else SCRIPT_ERROR ( "Wrong number of arguments for function \"%s\" " , name ) ;
 }
 else if ( ! strcmp ( name , "kill" ) ) {
 if ( nargs == 0 ) {
 fop -> op . func . op = FFUNC_KILL ;
 ret = E_SUCCESS ;
 }
 else SCRIPT_ERROR ( "Wrong number of arguments for function \"%s\" " , name ) ;
 }
 else if ( ! strcmp ( name , "msg" ) ) {
 if ( nargs == 1 ) {
 fop -> op . func . op = FFUNC_MSG ;
 fop -> op . func . string = ( u_char * ) strdup ( dec_args [ 0 ] ) ;
 fop -> op . func . slen = strescape ( ( char * ) fop -> op . func . string , ( char * ) fop -> op . func . string ) ;
 ret = E_SUCCESS ;
 }
 else SCRIPT_ERROR ( "Wrong number of arguments for function \"%s\" " , name ) ;
 }
 else if ( ! strcmp ( name , "exec" ) ) {
 if ( nargs == 1 ) {
 fop -> op . func . op = FFUNC_EXEC ;
 fop -> op . func . string = ( u_char * ) strdup ( dec_args [ 0 ] ) ;
 fop -> op . func . slen = strlen ( ( const char * ) fop -> op . func . string ) ;
 ret = E_SUCCESS ;
 }
 else SCRIPT_ERROR ( "Wrong number of arguments for function \"%s\" " , name ) ;
 }
 else if ( ! strcmp ( name , "exit" ) ) {
 if ( nargs == 0 ) {
 fop -> opcode = FOP_EXIT ;
 ret = E_SUCCESS ;
 }
 else SCRIPT_ERROR ( "Wrong number of arguments for function \"%s\" " , name ) ;
 }
 for ( i = 0 ;
 i < nargs ;
 i ++ ) SAFE_FREE ( dec_args [ i ] ) ;
 SAFE_FREE ( dec_args ) ;
 SAFE_FREE ( str ) ;
 return ret ;
 }