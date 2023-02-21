static int ipvideo_decode_block_opcode_0x4 ( IpvideoContext * s ) {
 int x , y ;
 unsigned char B , BL , BH ;
 if ( ! s -> is_16bpp ) {
 B = bytestream2_get_byte ( & s -> stream_ptr ) ;
 }
 else {
 B = bytestream2_get_byte ( & s -> mv_ptr ) ;
 }
 BL = B & 0x0F ;
 BH = ( B >> 4 ) & 0x0F ;
 x = - 8 + BL ;
 y = - 8 + BH ;
 av_dlog ( NULL , " motion byte = %d, (x, y) = (%d, %d)\n" , B , x , y ) ;
 return copy_from ( s , & s -> last_frame , x , y ) ;
 }