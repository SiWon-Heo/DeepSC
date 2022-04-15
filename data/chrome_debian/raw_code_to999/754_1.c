static int parse_CInGroupSortAggregSet ( tvbuff_t * tvb , int offset , proto_tree * parent_tree , proto_tree * pad_tree , const char * fmt , ... ) {
 proto_item * item ;
 proto_tree * tree ;
 va_list ap ;
 enum CInGroupSortAggregSet_type type ;
 const char * txt ;
 va_start ( ap , fmt ) ;
 txt = wmem_strdup_vprintf ( wmem_packet_scope ( ) , fmt , ap ) ;
 va_end ( ap ) ;
 tree = proto_tree_add_subtree ( parent_tree , tvb , offset , 0 , ett_CInGroupSortAggregSet , & item , txt ) ;
 offset = parse_CInGroupSortAggregSet_type ( tvb , offset , tree , & type ) ;
 offset = parse_padding ( tvb , offset , 4 , pad_tree , "CInGroupSortAggregSet" ) ;
 if ( type == GroupIdValue ) {
 struct CBaseStorageVariant id ;
 offset = parse_CBaseStorageVariant ( tvb , offset , tree , pad_tree , & id , "inGroupId" ) ;
 }
 offset = parse_CSortSet ( tvb , offset , tree , pad_tree , "SortSet" ) ;
 proto_item_set_end ( item , tvb , offset ) ;
 return offset ;
 }