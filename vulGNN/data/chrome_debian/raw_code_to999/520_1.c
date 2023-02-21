static int ipvideo_decode_block_opcode_0x3 ( IpvideoContext * s ) {
 unsigned char B ;
 int x , y ;
 if ( ! s -> is_16bpp ) {
 B = bytestream2_get_byte ( & s -> stream_ptr ) ;
 }
 else {
 B = bytestream2_get_byte ( & s -> mv_ptr ) ;
 }
 if ( B < 56 ) {
 x = - ( 8 + ( B % 7 ) ) ;
 y = - ( B / 7 ) ;
 }
 else {
 x = - ( - 14 + ( ( B - 56 ) % 29 ) ) ;
 y = - ( 8 + ( ( B - 56 ) / 29 ) ) ;
 }
 av_dlog ( NULL , " motion byte = %d, (x, y) = (%d, %d)\n" , B , x , y ) ;
 return copy_from ( s , & s -> current_frame , x , y ) ;
 }