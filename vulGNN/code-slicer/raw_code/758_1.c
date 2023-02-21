static int dissect_CPMCreateQuery ( tvbuff_t * tvb , packet_info * pinfo , proto_tree * parent_tree , gboolean in , void * data _U_ ) {
 gint offset = 16 ;
 proto_item * item ;
 proto_tree * tree ;
 item = proto_tree_add_item ( parent_tree , hf_mswsp_msg , tvb , offset , - 1 , ENC_NA ) ;
 tree = proto_item_add_subtree ( item , ett_mswsp_msg ) ;
 proto_item_set_text ( item , "CPMCreateQuery%s" , in ? "In" : "Out" ) ;
 col_append_str ( pinfo -> cinfo , COL_INFO , "CreateQuery" ) ;
 if ( in ) {
 proto_item * ti ;
 proto_tree * pad_tree = proto_tree_add_subtree ( tree , tvb , offset , 0 , ett_mswsp_pad , & ti , "Padding" ) ;
 guint8 CColumnSetPresent , CRestrictionPresent , CSortSetPresent , CCategorizationSetPresent ;
 guint32 size = tvb_get_letohl ( tvb , offset ) ;
 proto_tree_add_uint ( tree , hf_mswsp_msg_cpmcreatequery_size , tvb , offset , 4 , size ) ;
 offset += 4 ;
 CColumnSetPresent = tvb_get_guint8 ( tvb , offset ) ;
 proto_tree_add_item ( tree , hf_mswsp_msg_cpmcreatequery_ccolumnsetpresent , tvb , offset , 1 , ENC_LITTLE_ENDIAN ) ;
 offset += 1 ;
 if ( CColumnSetPresent ) {
 offset = parse_padding ( tvb , offset , 4 , pad_tree , "paddingCColumnSetPresent" ) ;
 offset = parse_padding ( tvb , offset , 4 , pad_tree , "paddingCColumnSetPresent" ) ;
 offset = parse_CColumnSet ( tvb , offset , tree , "CColumnSet" ) ;
 }
 CRestrictionPresent = tvb_get_guint8 ( tvb , offset ) ;
 proto_tree_add_item ( tree , hf_mswsp_msg_cpmcreatequery_crestrictionpresent , tvb , offset , 1 , ENC_LITTLE_ENDIAN ) ;
 offset += 1 ;
 if ( CRestrictionPresent ) {
 offset = parse_CRestrictionArray ( tvb , offset , tree , pad_tree , "RestrictionArray" ) ;
 }
 CSortSetPresent = tvb_get_guint8 ( tvb , offset ) ;
 proto_tree_add_item ( tree , hf_mswsp_msg_cpmcreatequery_csortpresent , tvb , offset , 1 , ENC_LITTLE_ENDIAN ) ;
 offset += 1 ;
 if ( CSortSetPresent ) {
 offset = parse_padding ( tvb , offset , 4 , tree , "paddingCSortSetPresent" ) ;
 offset = parse_CInGroupSortAggregSets ( tvb , offset , tree , pad_tree , "GroupSortAggregSets" ) ;
 }
 CCategorizationSetPresent = tvb_get_guint8 ( tvb , offset ) ;
 proto_tree_add_item ( tree , hf_mswsp_msg_cpmcreatequery_ccategpresent , tvb , offset , 1 , ENC_LITTLE_ENDIAN ) ;
 offset += 1 ;
 if ( CCategorizationSetPresent ) {
 guint32 count , i ;
 offset = parse_padding ( tvb , offset , 4 , pad_tree , "paddingCCategorizationSetPresent" ) ;
 count = tvb_get_letohl ( tvb , offset ) ;
 proto_tree_add_uint ( tree , hf_mswsp_msg_cpmcreatequery_ccateg_count , tvb , offset , 4 , count ) ;
 offset += 4 ;
 for ( i = 0 ;
 i < count ;
 i ++ ) {
 offset = parse_CCategorizationSpec ( tvb , offset , tree , pad_tree , "categories[%u]" , i ) ;
 }
 }
 offset = parse_padding ( tvb , offset , 4 , tree , "XXXX" ) ;
 offset = parse_CRowsetProperties ( tvb , offset , tree , pad_tree , "RowSetProperties" ) ;
 offset = parse_CPidMapper ( tvb , offset , tree , pad_tree , "PidMapper" ) ;
 parse_CColumnGroupArray ( tvb , offset , tree , pad_tree , "GroupArray" ) ;
 }
 else {
 proto_tree_add_item ( tree , hf_mswsp_msg_cpmcreatequery_trueseq , tvb , offset , 4 , ENC_LITTLE_ENDIAN ) ;
 offset += 4 ;
 proto_tree_add_item ( tree , hf_mswsp_msg_cpmcreatequery_workid , tvb , offset , 4 , ENC_LITTLE_ENDIAN ) ;
 offset += 4 ;
 proto_tree_add_item ( tree , hf_mswsp_msg_cpmcreatequery_cursors , tvb , offset , - 1 , ENC_NA ) ;
 }
 return tvb_reported_length ( tvb ) ;
 }