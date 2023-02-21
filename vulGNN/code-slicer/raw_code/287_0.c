int X509_REQ_extension_nid ( int req_nid ) {
 int i , nid ;
 for ( i = 0 ;
 ;
 i ++ ) {
 nid = ext_nids [ i ] ;
 if ( nid == NID_undef ) return 0 ;
 else if ( req_nid == nid ) return 1 ;
 }
 }