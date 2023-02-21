static int jbig2_default_error ( void * data , const char * msg , Jbig2Severity severity , int32_t seg_idx ) {
 if ( severity == JBIG2_SEVERITY_FATAL ) {
 fprintf ( stderr , "jbig2 decoder FATAL ERROR: %s" , msg ) ;
 if ( seg_idx != - 1 ) fprintf ( stderr , " (segment 0x%02x)" , seg_idx ) ;
 fprintf ( stderr , "\n" ) ;
 fflush ( stderr ) ;
 }
 return 0 ;
 }