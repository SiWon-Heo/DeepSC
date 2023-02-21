int curl_mprintf ( const char * format , ... ) {
 int retcode ;
 va_list ap_save ;
 va_start ( ap_save , format ) ;
 retcode = dprintf_formatf ( stdout , fputc , format , ap_save ) ;
 va_end ( ap_save ) ;
 return retcode ;
 }