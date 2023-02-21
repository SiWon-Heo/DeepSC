HTTPValTE * http_parse_te ( const char * buf , int len , Arena * arena ) {
 HTTPValTE * val ;
 const char * s ;
 http_skip_ws ( buf , len ) ;
 s = buf ;
 while ( len > 0 && * buf && ( * buf != ';
' ) ) {
 buf += 1 ;
 len -= 1 ;
 }
 val = ( HTTPValTE * ) arena -> alloc ( sizeof ( HTTPValTE ) ) ;
 val -> encoding = http_str_store ( arena , s , ( int ) ( buf - s ) ) ;
 val -> qvalue = http_parse_qvalue ( buf , len ) ;
 return val ;
 }