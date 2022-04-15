static void tgq_decode_mb ( TgqContext * s , int mb_y , int mb_x ) {
 int mode ;
 int i ;
 int8_t dc [ 6 ] ;
 mode = bytestream2_get_byte ( & s -> gb ) ;
 if ( mode > 12 ) {
 GetBitContext gb ;
 init_get_bits ( & gb , s -> gb . buffer , FFMIN ( s -> gb . buffer_end - s -> gb . buffer , mode ) * 8 ) ;
 for ( i = 0 ;
 i < 6 ;
 i ++ ) tgq_decode_block ( s , s -> block [ i ] , & gb ) ;
 tgq_idct_put_mb ( s , s -> block , mb_x , mb_y ) ;
 bytestream2_skip ( & s -> gb , mode ) ;
 }
 else {
 if ( mode == 3 ) {
 memset ( dc , bytestream2_get_byte ( & s -> gb ) , 4 ) ;
 dc [ 4 ] = bytestream2_get_byte ( & s -> gb ) ;
 dc [ 5 ] = bytestream2_get_byte ( & s -> gb ) ;
 }
 else if ( mode == 6 ) {
 bytestream2_get_buffer ( & s -> gb , dc , 6 ) ;
 }
 else if ( mode == 12 ) {
 for ( i = 0 ;
 i < 6 ;
 i ++ ) {
 dc [ i ] = bytestream2_get_byte ( & s -> gb ) ;
 bytestream2_skip ( & s -> gb , 1 ) ;
 }
 }
 else {
 av_log ( s -> avctx , AV_LOG_ERROR , "unsupported mb mode %i\n" , mode ) ;
 }
 tgq_idct_put_mb_dconly ( s , mb_x , mb_y , dc ) ;
 }
 }