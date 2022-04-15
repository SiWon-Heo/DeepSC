int curl_mvprintf ( const char * format , va_list ap_save ) {
 return dprintf_formatf ( stdout , fputc , format , ap_save ) ;
 }