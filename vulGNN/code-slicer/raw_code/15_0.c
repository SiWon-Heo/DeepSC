char * curl_maprintf ( const char * format , ... ) {
 va_list ap_save ;
 int retcode ;
 struct asprintf info ;
 info . buffer = NULL ;
 info . len = 0 ;
 info . alloc = 0 ;
 info . fail = 0 ;
 va_start ( ap_save , format ) ;
 retcode = dprintf_formatf ( & info , alloc_addbyter , format , ap_save ) ;
 va_end ( ap_save ) ;
 if ( ( - 1 == retcode ) || info . fail ) {
 if ( info . alloc ) free ( info . buffer ) ;
 return NULL ;
 }
 if ( info . alloc ) {
 info . buffer [ info . len ] = 0 ;
 return info . buffer ;
 }
 else return strdup ( "" ) ;
 }