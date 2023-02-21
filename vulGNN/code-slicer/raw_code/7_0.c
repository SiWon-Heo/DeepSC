int curl_msprintf ( char * buffer , const char * format , ... ) {
 va_list ap_save ;
 int retcode ;
 va_start ( ap_save , format ) ;
 retcode = dprintf_formatf ( & buffer , storebuffer , format , ap_save ) ;
 va_end ( ap_save ) ;
 * buffer = 0 ;
 return retcode ;
 }