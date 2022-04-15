static int split_field_copy ( Picture * dest , Picture * src , int parity , int id_add ) {
 int match = ! ! ( src -> f . reference & parity ) ;
 if ( match ) {
 * dest = * src ;
 if ( parity != PICT_FRAME ) {
 pic_as_field ( dest , parity ) ;
 dest -> pic_id *= 2 ;
 dest -> pic_id += id_add ;
 }
 }
 return match ;
 }