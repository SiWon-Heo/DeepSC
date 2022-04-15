static void iax2_add_ts_fields ( packet_info * pinfo , proto_tree * iax2_tree , iax_packet_data * iax_packet , guint16 shortts ) {
 guint32 longts = shortts ;
 nstime_t ts ;
 proto_item * item ;
 if ( iax_packet -> call_data == NULL ) {
 return ;
 }
 if ( iax_packet -> abstime . secs == - 1 ) {
 time_t start_secs = iax_packet -> call_data -> start_time . secs ;
 time_t abs_secs = start_secs + longts / 1000 ;
 while ( abs_secs < pinfo -> fd -> abs_ts . secs - 16 ) {
 longts += 32768 ;
 abs_secs = start_secs + longts / 1000 ;
 }
 iax_packet -> abstime . secs = abs_secs ;
 iax_packet -> abstime . nsecs = iax_packet -> call_data -> start_time . nsecs + ( longts % 1000 ) * 1000000 ;
 if ( iax_packet -> abstime . nsecs >= 1000000000 ) {
 iax_packet -> abstime . nsecs -= 1000000000 ;
 iax_packet -> abstime . secs ++ ;
 }
 }
 iax2_info -> timestamp = longts ;
 if ( iax2_tree ) {
 item = proto_tree_add_time ( iax2_tree , hf_iax2_absts , NULL , 0 , 0 , & iax_packet -> abstime ) ;
 PROTO_ITEM_SET_GENERATED ( item ) ;
 ts = pinfo -> fd -> abs_ts ;
 nstime_delta ( & ts , & ts , & iax_packet -> abstime ) ;
 item = proto_tree_add_time ( iax2_tree , hf_iax2_lateness , NULL , 0 , 0 , & ts ) ;
 PROTO_ITEM_SET_GENERATED ( item ) ;
 }
 }