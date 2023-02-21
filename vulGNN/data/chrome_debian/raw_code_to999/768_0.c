static unsigned char * extra_get_record ( struct isoent * isoent , int * space , int * off , int * loc ) {
 struct extr_rec * rec ;
 isoent = isoent -> parent ;
 if ( off != NULL ) {
 rec = isoent -> extr_rec_list . current ;
 if ( DR_SAFETY > LOGICAL_BLOCK_SIZE - rec -> offset ) rec = rec -> next ;
 }
 else {
 rec = extra_last_record ( isoent ) ;
 if ( rec == NULL || DR_SAFETY > LOGICAL_BLOCK_SIZE - rec -> offset ) {
 rec = malloc ( sizeof ( * rec ) ) ;
 if ( rec == NULL ) return ( NULL ) ;
 rec -> location = 0 ;
 rec -> offset = 0 ;
 rec -> next = NULL ;
 if ( isoent -> extr_rec_list . last == NULL ) isoent -> extr_rec_list . last = & ( isoent -> extr_rec_list . first ) ;
 * isoent -> extr_rec_list . last = rec ;
 isoent -> extr_rec_list . last = & ( rec -> next ) ;
 }
 }
 * space = LOGICAL_BLOCK_SIZE - rec -> offset - DR_SAFETY ;
 if ( * space & 0x01 ) * space -= 1 ;
 if ( off != NULL ) * off = rec -> offset ;
 if ( loc != NULL ) * loc = rec -> location ;
 isoent -> extr_rec_list . current = rec ;
 return ( & rec -> buf [ rec -> offset ] ) ;
 }