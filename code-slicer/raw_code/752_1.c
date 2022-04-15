static int parse_RowsBufferCol ( tvbuff_t * tvb , int offset , guint32 row , guint32 col , struct CPMSetBindingsIn * bindingsin , struct rows_data * rowsin , gboolean b_is_64bit , proto_tree * parent_tree , const char * fmt , ... ) {
 proto_tree * tree ;
 proto_item * item ;
 guint32 buf_start = offset ;
 guint32 buf_offset = buf_start + ( row * bindingsin -> brow ) ;
 struct CTableColumn * pcol = & bindingsin -> acolumns [ col ] ;
 static const value_string STATUS [ ] = {
 {
 0 , "StoreStatusOk" }
 , {
 1 , "StoreStatusDeferred" }
 , {
 2 , "StoreStatusNull" }
 , {
 0 , NULL }
 }
 ;
 const char * txt ;
 va_list ap ;
 va_start ( ap , fmt ) ;
 txt = wmem_strdup_vprintf ( wmem_packet_scope ( ) , fmt , ap ) ;
 va_end ( ap ) ;
 tree = proto_tree_add_subtree ( parent_tree , tvb , offset , 0 , ett_GetRowsColumn , & item , txt ) ;
 proto_item_append_text ( item , " (%s)" , pcol -> name ) ;
 if ( pcol -> statusused ) {
 int tmp_offset = buf_offset + pcol -> statusoffset ;
 proto_tree_add_string ( tree , hf_mswsp_ctablecolumn_status , tvb , tmp_offset , 1 , val_to_str ( tvb_get_guint8 ( tvb , tmp_offset ) , STATUS , "(Invalid: 0x%x)" ) ) ;
 }
 if ( pcol -> lengthused ) {
 int tmp_offset = buf_offset + pcol -> lengthoffset ;
 proto_tree_add_item ( tree , hf_mswsp_ctablecolumn_length , tvb , tmp_offset , 1 , ENC_LITTLE_ENDIAN ) ;
 }
 if ( pcol -> valueused ) {
 int tmp_offset = buf_offset + pcol -> valueoffset ;
 struct CRowVariant variant ;
 guint32 len = pcol -> valuesize ;
 guint64 base_address = rowsin -> ulclientbase ;
 ZERO_STRUCT ( variant ) ;
 if ( pcol -> lengthused ) {
 len = tvb_get_letohs ( tvb , buf_offset + pcol -> lengthoffset ) - pcol -> valuesize ;
 }
 if ( pcol -> vtype == VT_VARIANT ) {
 parse_VariantCol ( tvb , tmp_offset , tree , base_address , len , b_is_64bit , & variant , "CRowVariant" ) ;
 }
 }
 return offset ;
 }