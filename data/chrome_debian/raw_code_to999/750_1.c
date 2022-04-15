int parse_CCategorizationSpec ( tvbuff_t * tvb , int offset , proto_tree * parent_tree , proto_tree * pad_tree , const char * fmt , ... ) {
 proto_item * item ;
 proto_tree * tree ;
 const char * txt ;
 va_list ap ;
 va_start ( ap , fmt ) ;
 txt = wmem_strdup_vprintf ( wmem_packet_scope ( ) , fmt , ap ) ;
 va_end ( ap ) ;
 tree = proto_tree_add_subtree ( parent_tree , tvb , offset , 0 , ett_CCategorizationSpec , & item , txt ) ;
 offset = parse_CColumnSet ( tvb , offset , tree , "csColumns" ) ;
 offset = parse_CCategSpec ( tvb , offset , tree , pad_tree , "Spec" ) ;
 offset = parse_CAggregSet ( tvb , offset , tree , pad_tree , "AggregSet" ) ;
 offset = parse_CSortAggregSet ( tvb , offset , tree , pad_tree , "SortAggregSet" ) ;
 offset = parse_CInGroupSortAggregSets ( tvb , offset , tree , pad_tree , "InGroupSortAggregSets" ) ;
 proto_tree_add_item ( tree , hf_mswsp_categorizationspec_cmaxres , tvb , offset , 4 , ENC_LITTLE_ENDIAN ) ;
 offset += 4 ;
 proto_item_set_end ( item , tvb , offset ) ;
 return offset ;
 }