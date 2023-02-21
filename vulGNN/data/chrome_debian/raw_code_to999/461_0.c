static const char * comp_http_hdr ( HTTPHdr * h1 , HTTPHdr * h2 ) {
 int h1_len , h2_len ;
 int p_index , p_dumpoffset , rval ;
 char * h1_pbuf , * h2_pbuf ;
 h1_len = h1 -> length_get ( ) ;
 h2_len = h2 -> length_get ( ) ;
 if ( h1_len != h2_len ) {
 return "length mismatch" ;
 }
 h1_pbuf = ( char * ) ats_malloc ( h1_len + 1 ) ;
 h2_pbuf = ( char * ) ats_malloc ( h2_len + 1 ) ;
 p_index = p_dumpoffset = 0 ;
 rval = h1 -> print ( h1_pbuf , h1_len , & p_index , & p_dumpoffset ) ;
 if ( rval != 1 ) {
 ats_free ( h1_pbuf ) ;
 ats_free ( h2_pbuf ) ;
 return "hdr print failed" ;
 }
 p_index = p_dumpoffset = 0 ;
 rval = h2 -> print ( h2_pbuf , h2_len , & p_index , & p_dumpoffset ) ;
 if ( rval != 1 ) {
 ats_free ( h1_pbuf ) ;
 ats_free ( h2_pbuf ) ;
 return "hdr print failed" ;
 }
 rval = memcmp ( h1_pbuf , h2_pbuf , h1_len ) ;
 ats_free ( h1_pbuf ) ;
 ats_free ( h2_pbuf ) ;
 if ( rval != 0 ) {
 return "compare failed" ;
 }
 else {
 return nullptr ;
 }
 }