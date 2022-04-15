static guint32 dissect_minipacket ( tvbuff_t * tvb , guint32 offset , guint16 scallno , packet_info * pinfo , proto_tree * iax2_tree , proto_tree * main_tree ) {
 guint32 ts ;
 iax_packet_data * iax_packet ;
 proto_item * item ;
 ts = tvb_get_ntohs ( tvb , offset ) ;
 iax_packet = iax2_get_packet_data_for_minipacket ( pinfo , scallno , FALSE ) ;
 if ( iax2_tree ) {
 if ( iax_packet -> call_data ) {
 item = proto_tree_add_uint ( iax2_tree , hf_iax2_callno , tvb , 0 , 4 , iax_packet -> call_data -> forward_circuit_ids [ 0 ] ) ;
 PROTO_ITEM_SET_GENERATED ( item ) ;
 }
 proto_tree_add_uint ( iax2_tree , hf_iax2_minits , tvb , offset , 2 , ts ) ;
 iax2_add_ts_fields ( pinfo , iax2_tree , iax_packet , ( guint16 ) ts ) ;
 }
 else {
 iax2_add_ts_fields ( pinfo , iax2_tree , iax_packet , ( guint16 ) ts ) ;
 }
 offset += 2 ;
 col_add_fstr ( pinfo -> cinfo , COL_INFO , "Mini packet, source call# %d, timestamp %ums" , scallno , ts ) ;
 dissect_payload ( tvb , offset , pinfo , iax2_tree , main_tree , ts , FALSE , iax_packet ) ;
 iax_packet -> first_time = FALSE ;
 return offset ;
 }