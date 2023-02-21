int base64_decode ( char * bufplain , const char * bufcoded ) {
 int nbytesdecoded ;
 register const unsigned char * bufin ;
 register unsigned char * bufout ;
 register int nprbytes ;
 bufin = ( const unsigned char * ) bufcoded ;
 while ( pr2six [ * ( bufin ++ ) ] <= 63 ) ;
 nprbytes = ( bufin - ( const unsigned char * ) bufcoded ) - 1 ;
 nbytesdecoded = ( ( nprbytes + 3 ) / 4 ) * 3 ;
 bufout = ( unsigned char * ) bufplain ;
 bufin = ( const unsigned char * ) bufcoded ;
 while ( nprbytes > 4 ) {
 * ( bufout ++ ) = ( unsigned char ) ( pr2six [ * bufin ] << 2 | pr2six [ bufin [ 1 ] ] >> 4 ) ;
 * ( bufout ++ ) = ( unsigned char ) ( pr2six [ bufin [ 1 ] ] << 4 | pr2six [ bufin [ 2 ] ] >> 2 ) ;
 * ( bufout ++ ) = ( unsigned char ) ( pr2six [ bufin [ 2 ] ] << 6 | pr2six [ bufin [ 3 ] ] ) ;
 bufin += 4 ;
 nprbytes -= 4 ;
 }
 if ( nprbytes > 1 ) * ( bufout ++ ) = ( unsigned char ) ( pr2six [ * bufin ] << 2 | pr2six [ bufin [ 1 ] ] >> 4 ) ;
 if ( nprbytes > 2 ) * ( bufout ++ ) = ( unsigned char ) ( pr2six [ bufin [ 1 ] ] << 4 | pr2six [ bufin [ 2 ] ] >> 2 ) ;
 if ( nprbytes > 3 ) * ( bufout ++ ) = ( unsigned char ) ( pr2six [ bufin [ 2 ] ] << 6 | pr2six [ bufin [ 3 ] ] ) ;
 nbytesdecoded -= ( 4 - nprbytes ) & 3 ;
 bufplain [ nbytesdecoded ] = '\0' ;
 return nbytesdecoded ;
 }