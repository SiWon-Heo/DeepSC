int curl_mvfprintf ( FILE * whereto , const char * format , va_list ap_save ) {
 return dprintf_formatf ( whereto , fputc , format , ap_save ) ;
 }