static int parse_VariantCol ( tvbuff_t * tvb , int offset , proto_tree * parent_tree , guint64 base_address , guint32 length _U_ , gboolean is_64bit , struct CRowVariant * variant , const char * fmt , ... ) {
 proto_tree * tree ;
 proto_item * item ;
 va_list ap ;
 struct vtype_data * vt_type ;
 const char * modifier = "" , * txt ;
 int size ;
 guint16 vtype_high ;
 va_start ( ap , fmt ) ;
 txt = wmem_strdup_vprintf ( wmem_packet_scope ( ) , fmt , ap ) ;
 va_end ( ap ) ;
 tree = proto_tree_add_subtree ( parent_tree , tvb , offset , 0 , ett_CRowVariant , & item , txt ) ;
 variant -> vtype = tvb_get_letohs ( tvb , offset ) ;
 vt_type = vType_get_type ( ( enum vType ) variant -> vtype ) ;
 DISSECTOR_ASSERT ( vt_type != NULL ) ;
 vtype_high = ( variant -> vtype & 0xFF00 ) ;
 if ( vtype_high ) {
 if ( vtype_high == VT_VECTOR ) {
 modifier = "|VT_VECTOR" ;
 }
 else if ( vtype_high == VT_ARRAY ) {
 modifier = "|VT_ARRAY" ;
 }
 else {
 modifier = "|Unknown, possibly error" ;
 }
 }
 proto_tree_add_string_format_value ( tree , hf_mswsp_rowvariant_vtype , tvb , offset , 2 , vt_type -> str , "%s%s" , vt_type -> str , modifier ) ;
 offset += 2 ;
 proto_tree_add_item ( tree , hf_mswsp_rowvariant_reserved1 , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 variant -> reserved1 = tvb_get_letohs ( tvb , offset ) ;
 offset += 2 ;
 proto_tree_add_item ( tree , hf_mswsp_rowvariant_reserved2 , tvb , offset , 4 , ENC_LITTLE_ENDIAN ) ;
 variant -> reserved2 = tvb_get_letohl ( tvb , offset ) ;
 offset += 4 ;
 size = get_fixed_vtype_dataize ( ( enum vType ) ( variant -> vtype & 0x00FF ) ) ;
 if ( vtype_high == VT_VECTOR || vtype_high == VT_ARRAY ) {
 offset = parse_VariantColVector ( tvb , offset , tree , base_address , is_64bit , variant ) ;
 }
 else {
 wmem_strbuf_t * strbuf = wmem_strbuf_new ( wmem_packet_scope ( ) , "" ) ;
 if ( size != - 1 ) {
 const char * desc = vt_type -> str ;
 DISSECTOR_ASSERT_HINT ( vt_type -> tvb_get != 0 , "appears fixed type that we don't know yet how to handle, please submit a bug with trace" ) ;
 vt_type -> tvb_get ( tvb , offset , & variant -> content ) ;
 vt_type -> strbuf_append ( strbuf , & variant -> content ) ;
 proto_tree_add_string_format_value ( tree , hf_mswsp_rowvariant_item_value , tvb , offset , size , desc , "%s: %s" , desc , wmem_strbuf_get_str ( strbuf ) ) ;
 }
 else {
 gint64 value_address ;
 int buf_offset = offset ;
 int len ;
 union vt_single non_fixed_size_val ;
 DISSECTOR_ASSERT_HINT ( vt_type -> tvb_get_value_only != 0 , "appears this is a dynamic type that we don't know yet how to handle, please submit a bug with network trace" ) ;
 if ( is_64bit ) {
 variant -> content . hyperw = tvb_get_letoh64 ( tvb , offset ) ;
 offset += 8 ;
 value_address = variant -> content . hyperw ;
 proto_tree_add_uint64 ( tree , hf_mswsp_rowvariant_item_address64 , tvb , buf_offset , 8 , value_address ) ;
 }
 else {
 variant -> content . longw = tvb_get_letohl ( tvb , offset ) ;
 offset += 4 ;
 value_address = variant -> content . longw ;
 proto_tree_add_uint ( tree , hf_mswsp_rowvariant_item_address32 , tvb , buf_offset , 4 , ( guint32 ) value_address ) ;
 }
 len = vt_type -> tvb_get_value_only ( tvb , ( int ) ( value_address - base_address ) , 0 , & non_fixed_size_val ) ;
 vt_type -> strbuf_append ( strbuf , & non_fixed_size_val ) ;
 proto_tree_add_string ( tree , hf_mswsp_rowvariant_item_value , tvb , ( gint ) ( value_address - base_address ) , len , wmem_strbuf_get_str ( strbuf ) ) ;
 }
 }
 return offset ;
 }