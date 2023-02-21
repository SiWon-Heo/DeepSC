static void tvb_raw_text_add ( tvbuff_t * tvb , proto_tree * tree ) {
 proto_tree * raw_tree = NULL ;
 proto_item * ti = NULL ;
 int offset , next_offset , linelen ;
 if ( tree ) {
 ti = proto_tree_add_item ( tree , proto_raw_sigcomp , tvb , 0 , - 1 , ENC_NA ) ;
 raw_tree = proto_item_add_subtree ( ti , ett_raw_text ) ;
 }
 offset = 0 ;
 while ( tvb_offset_exists ( tvb , offset ) ) {
 tvb_find_line_end ( tvb , offset , - 1 , & next_offset , FALSE ) ;
 linelen = next_offset - offset ;
 proto_tree_add_format_text ( raw_tree , tvb , offset , linelen ) ;
 offset = next_offset ;
 }
 }