static void ps2_common_post_load ( PS2State * s ) {
 PS2Queue * q = & s -> queue ;
 int size ;
 int i ;
 int tmp_data [ PS2_QUEUE_SIZE ] ;
 size = q -> count > PS2_QUEUE_SIZE ? 0 : q -> count ;
 if ( size > 0 ) {
 for ( i = 0 ;
 i < size ;
 i ++ ) {
 tmp_data [ i ] = q -> data [ q -> rptr ] ;
 if ( ++ q -> rptr == 256 ) {
 q -> rptr = 0 ;
 }
 }
 memcpy ( q -> data , tmp_data , size ) ;
 }
 q -> rptr = 0 ;
 q -> wptr = size ;
 q -> count = size ;
 s -> update_irq ( s -> update_arg , q -> count != 0 ) ;
 }