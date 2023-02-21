int curl_mvsnprintf ( char * buffer , size_t maxlength , const char * format , va_list ap_save ) {
 int retcode ;
 struct nsprintf info ;
 info . buffer = buffer ;
 info . length = 0 ;
 info . max = maxlength ;
 retcode = dprintf_formatf ( & info , addbyter , format , ap_save ) ;
 if ( ( retcode != - 1 ) && info . max ) {
 if ( info . max == info . length ) info . buffer [ - 1 ] = 0 ;
 else info . buffer [ 0 ] = 0 ;
 }
 return retcode ;
 }