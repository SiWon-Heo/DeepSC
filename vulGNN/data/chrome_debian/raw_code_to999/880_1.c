METHOD ( asn1_parser_t , iterate , bool , private_asn1_parser_t * this , int * objectID , chunk_t * object ) {
 chunk_t * blob , * blob1 ;
 u_char * start_ptr ;
 u_int level ;
 asn1Object_t obj ;
 * object = chunk_empty ;
 obj = this -> objects [ ++ ( this -> line ) ] ;
 if ( obj . flags & ASN1_EXIT ) {
 return FALSE ;
 }
 if ( obj . flags & ASN1_END ) {
 if ( this -> loopAddr [ obj . level ] && this -> blobs [ obj . level + 1 ] . len > 0 ) {
 this -> line = this -> loopAddr [ obj . level ] ;
 obj = this -> objects [ this -> line ] ;
 }
 else {
 this -> loopAddr [ obj . level ] = 0 ;
 goto end ;
 }
 }
 level = this -> level0 + obj . level ;
 blob = this -> blobs + obj . level ;
 blob1 = blob + 1 ;
 start_ptr = blob -> ptr ;
 if ( ( obj . flags & ASN1_DEF ) && ( blob -> len == 0 || * start_ptr != obj . type ) ) {
 DBG2 ( DBG_ASN , "L%d - %s:" , level , obj . name ) ;
 if ( obj . type & ASN1_CONSTRUCTED ) {
 this -> line ++ ;
 }
 goto end ;
 }
 if ( ( obj . flags & ASN1_OPT ) && ( blob -> len == 0 || * start_ptr != obj . type ) ) {
 do {
 this -> line ++ ;
 }
 while ( ! ( ( this -> objects [ this -> line ] . flags & ASN1_END ) && ( this -> objects [ this -> line ] . level == obj . level ) ) ) ;
 goto end ;
 }
 if ( blob -> len < 2 ) {
 DBG1 ( DBG_ASN , "L%d - %s: ASN.1 object smaller than 2 octets" , level , obj . name ) ;
 this -> success = FALSE ;
 goto end ;
 }
 blob1 -> len = asn1_length ( blob ) ;
 if ( blob1 -> len == ASN1_INVALID_LENGTH ) {
 DBG1 ( DBG_ASN , "L%d - %s: length of ASN.1 object invalid or too large" , level , obj . name ) ;
 this -> success = FALSE ;
 goto end ;
 }
 blob1 -> ptr = blob -> ptr ;
 blob -> ptr += blob1 -> len ;
 blob -> len -= blob1 -> len ;
 if ( obj . flags & ASN1_RAW ) {
 DBG2 ( DBG_ASN , "L%d - %s:" , level , obj . name ) ;
 object -> ptr = start_ptr ;
 object -> len = ( size_t ) ( blob -> ptr - start_ptr ) ;
 goto end ;
 }
 if ( * start_ptr != obj . type && ! ( this -> implicit && this -> line == 0 ) ) {
 DBG2 ( DBG_ASN , "L%d - %s: ASN1 tag 0x%02x expected, but is 0x%02x" , level , obj . name , obj . type , * start_ptr ) ;
 DBG3 ( DBG_ASN , "%b" , start_ptr , ( u_int ) ( blob -> ptr - start_ptr ) ) ;
 this -> success = FALSE ;
 goto end ;
 }
 DBG2 ( DBG_ASN , "L%d - %s:" , level , obj . name ) ;
 if ( obj . flags & ASN1_LOOP ) {
 if ( blob1 -> len > 0 ) {
 this -> loopAddr [ obj . level ] = this -> line + 1 ;
 }
 else {
 do {
 this -> line ++ ;
 }
 while ( ! ( ( this -> objects [ this -> line ] . flags & ASN1_END ) && ( this -> objects [ this -> line ] . level == obj . level ) ) ) ;
 goto end ;
 }
 }
 if ( obj . flags & ASN1_OBJ ) {
 object -> ptr = start_ptr ;
 object -> len = ( size_t ) ( blob -> ptr - start_ptr ) ;
 if ( this -> private ) {
 DBG4 ( DBG_ASN , "%B" , object ) ;
 }
 else {
 DBG3 ( DBG_ASN , "%B" , object ) ;
 }
 }
 else if ( obj . flags & ASN1_BODY ) {
 * object = * blob1 ;
 asn1_debug_simple_object ( * object , obj . type , this -> private ) ;
 }
 end : * objectID = this -> line ;
 return this -> success ;
 }