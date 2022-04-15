static int parse_CBaseStorageVariant ( tvbuff_t * tvb , int offset , proto_tree * parent_tree , proto_tree * pad_tree _U_ , struct CBaseStorageVariant * value , const char * text ) {
 int i , len ;
 proto_item * ti , * ti_type , * ti_val ;
 proto_tree * tree , * tr ;
 enum vType highType ;
 ZERO_STRUCT ( * value ) ;
 tree = proto_tree_add_subtree ( parent_tree , tvb , offset , 0 , ett_CBaseStorageVariant , & ti , text ) ;
 parse_vType ( tvb , offset , & value -> vType ) ;
 value -> type = vType_get_type ( value -> vType ) ;
 DISSECTOR_ASSERT ( value -> type != NULL ) ;
 ti_type = proto_tree_add_string ( tree , hf_mswsp_cbasestorvariant_vtype , tvb , offset , 2 , value -> type -> str ) ;
 offset += 2 ;
 value -> vData1 = tvb_get_guint8 ( tvb , offset ) ;
 proto_tree_add_uint ( tree , hf_mswsp_cbasestorvariant_vdata1 , tvb , offset , 1 , value -> vData1 ) ;
 offset += 1 ;
 value -> vData2 = tvb_get_guint8 ( tvb , offset ) ;
 proto_tree_add_uint ( tree , hf_mswsp_cbasestorvariant_vdata2 , tvb , offset , 1 , value -> vData2 ) ;
 offset += 1 ;
 highType = ( enum vType ) ( value -> vType & 0xFF00 ) ;
 ti_val = proto_tree_add_string ( tree , hf_mswsp_cbasestorvariant_vvalue , tvb , offset , 0 , "" ) ;
 switch ( highType ) {
 case VT_EMPTY : DISSECTOR_ASSERT_HINT ( value -> type -> tvb_get != 0 , "type that we don't know yet how to handle, please submit a bug with trace" ) ;
 len = value -> type -> tvb_get ( tvb , offset , & value -> vValue . vt_single ) ;
 offset += len ;
 break ;
 case VT_VECTOR : proto_item_append_text ( ti_type , "|VT_VECTOR" ) ;
 tr = proto_item_add_subtree ( ti_val , ett_CBaseStorageVariant_Vector ) ;
 len = vvalue_tvb_vector ( tvb , offset , & value -> vValue . vt_vector , value -> type ) ;
 proto_tree_add_uint ( tr , hf_mswsp_cbasestorvariant_num , tvb , offset , 4 , value -> vValue . vt_vector . len ) ;
 offset += len ;
 break ;
 case VT_ARRAY : {
 guint16 cDims , fFeatures ;
 guint32 cbElements , cElements , lLbound ;
 int num = 1 ;
 proto_item_append_text ( ti_type , "|VT_ARRAY" ) ;
 tr = proto_item_add_subtree ( ti_val , ett_CBaseStorageVariant_Array ) ;
 cDims = tvb_get_letohs ( tvb , offset ) ;
 proto_tree_add_uint ( tr , hf_mswsp_cbasestorvariant_cdims , tvb , offset , 2 , cDims ) ;
 offset += 2 ;
 fFeatures = tvb_get_letohs ( tvb , offset ) ;
 proto_tree_add_uint ( tr , hf_mswsp_cbasestorvariant_ffeatures , tvb , offset , 2 , fFeatures ) ;
 offset += 2 ;
 cbElements = tvb_get_letohl ( tvb , offset ) ;
 proto_tree_add_uint ( tr , hf_mswsp_cbasestorvariant_cbelements , tvb , offset , 4 , cbElements ) ;
 offset += 4 ;
 for ( i = 0 ;
 i < cDims ;
 i ++ ) {
 cElements = tvb_get_letohl ( tvb , offset ) ;
 lLbound = tvb_get_letohl ( tvb , offset + 4 ) ;
 proto_tree_add_string_format ( tr , hf_mswsp_cbasestorvariant_rgsabound , tvb , offset , 8 , "" , "Rgsabound[%d]: (%d:%d)" , i , cElements , lLbound ) ;
 offset += 8 ;
 num *= cElements ;
 }
 len = vvalue_tvb_vector_internal ( tvb , offset , & value -> vValue . vt_array . vData , value -> type , num ) ;
 offset += len ;
 break ;
 }
 default : proto_item_append_text ( ti_type , "|0x%x" , highType ) ;
 }
 proto_item_set_end ( ti , tvb , offset ) ;
 proto_item_set_end ( ti_val , tvb , offset ) ;
 proto_item_append_text ( ti_val , " %s" , str_CBaseStorageVariant ( value , FALSE ) ) ;
 proto_item_append_text ( ti , " %s" , str_CBaseStorageVariant ( value , TRUE ) ) ;
 return offset ;
 }