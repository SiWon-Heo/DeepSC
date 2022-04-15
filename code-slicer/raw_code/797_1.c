static int dissect_segment_ofstable ( tvbuff_t * tvb , int offset , packet_info * pinfo _U_ , proto_tree * tree , lbmpdm_offset_table_t * * offset_table , int encoding ) {
 proto_item * subtree_item = NULL ;
 proto_tree * subtree = NULL ;
 int datalen = 0 ;
 int seglen = 0 ;
 int datalen_remaining = 0 ;
 int ofs = 0 ;
 int field_count = 0 ;
 int idx ;
 gint32 * id_list = NULL ;
 gint32 * ofs_list = NULL ;
 gint32 max_index = - 1 ;
 gint32 min_offset = G_MAXINT32 ;
 lbmpdm_offset_table_t * ofs_table = NULL ;
 seglen = lbmpdm_get_segment_length ( tvb , offset , encoding , & datalen ) ;
 subtree_item = proto_tree_add_none_format ( tree , hf_lbmpdm_segment , tvb , offset , seglen , "Offset Table Segment" ) ;
 subtree = proto_item_add_subtree ( subtree_item , ett_lbmpdm_segment ) ;
 proto_tree_add_item ( subtree , hf_lbmpdm_segment_next_hdr , tvb , offset + O_LBMPDM_SEG_HDR_T_NEXT_HDR , L_LBMPDM_SEG_HDR_T_NEXT_HDR , encoding ) ;
 proto_tree_add_item ( subtree , hf_lbmpdm_segment_flags , tvb , offset + O_LBMPDM_SEG_HDR_T_FLAGS , L_LBMPDM_SEG_HDR_T_FLAGS , encoding ) ;
 proto_tree_add_item ( subtree , hf_lbmpdm_segment_res , tvb , offset + O_LBMPDM_SEG_HDR_T_RES , L_LBMPDM_SEG_HDR_T_RES , encoding ) ;
 proto_tree_add_item ( subtree , hf_lbmpdm_segment_len , tvb , offset + O_LBMPDM_SEG_HDR_T_LEN , L_LBMPDM_SEG_HDR_T_LEN , encoding ) ;
 field_count = datalen / L_LBMPDM_OFFSET_ENTRY_T ;
 id_list = wmem_alloc_array ( wmem_packet_scope ( ) , gint32 , field_count ) ;
 ofs_list = wmem_alloc_array ( wmem_packet_scope ( ) , gint32 , field_count ) ;
 for ( idx = 0 ;
 idx < field_count ;
 ++ idx ) {
 id_list [ idx ] = - 1 ;
 ofs_list [ idx ] = - 1 ;
 }
 datalen_remaining = datalen ;
 ofs = offset + L_LBMPDM_SEG_HDR_T ;
 for ( idx = 0 ;
 ( idx < field_count ) && ( datalen_remaining >= L_LBMPDM_OFFSET_ENTRY_T ) ;
 idx ++ , ofs += L_LBMPDM_OFFSET_ENTRY_T ) {
 proto_item * offset_item = NULL ;
 proto_tree * offset_tree = NULL ;
 offset_item = proto_tree_add_item ( subtree , hf_lbmpdm_offset_entry , tvb , ofs , L_LBMPDM_OFFSET_ENTRY_T , ENC_NA ) ;
 offset_tree = proto_item_add_subtree ( offset_item , ett_lbmpdm_offset_entry ) ;
 proto_tree_add_item ( offset_tree , hf_lbmpdm_offset_entry_id , tvb , ofs + O_LBMPDM_OFFSET_ENTRY_T_ID , L_LBMPDM_OFFSET_ENTRY_T_ID , encoding ) ;
 id_list [ idx ] = ( gint32 ) lbmpdm_fetch_uint32_encoded ( tvb , ofs + O_LBMPDM_OFFSET_ENTRY_T_ID , encoding ) ;
 proto_tree_add_item ( offset_tree , hf_lbmpdm_offset_entry_offset , tvb , ofs + O_LBMPDM_OFFSET_ENTRY_T_OFFSET , L_LBMPDM_OFFSET_ENTRY_T_OFFSET , encoding ) ;
 ofs_list [ idx ] = ( gint32 ) lbmpdm_fetch_uint32_encoded ( tvb , ofs + O_LBMPDM_OFFSET_ENTRY_T_OFFSET , encoding ) ;
 if ( id_list [ idx ] > max_index ) {
 max_index = id_list [ idx ] ;
 }
 if ( ofs_list [ idx ] < min_offset ) {
 min_offset = ofs_list [ idx ] ;
 }
 }
 ofs_table = wmem_new ( wmem_packet_scope ( ) , lbmpdm_offset_table_t ) ;
 ofs_table -> num_flds = max_index + 1 ;
 ofs_table -> min_set_offset = NULL ;
 ofs_table -> offset_list = wmem_alloc_array ( wmem_packet_scope ( ) , gint32 , ofs_table -> num_flds ) ;
 for ( idx = 0 ;
 idx < ( int ) ofs_table -> num_flds ;
 ++ idx ) {
 ofs_table -> offset_list [ idx ] = - 1 ;
 }
 for ( idx = 0 ;
 idx < field_count ;
 ++ idx ) {
 ofs_table -> offset_list [ id_list [ idx ] ] = ofs_list [ idx ] ;
 if ( ofs_list [ idx ] == min_offset ) {
 ofs_table -> min_set_offset = & ( ofs_table -> offset_list [ id_list [ idx ] ] ) ;
 }
 }
 if ( offset_table != NULL ) {
 * offset_table = ofs_table ;
 }
 return ( seglen ) ;
 }