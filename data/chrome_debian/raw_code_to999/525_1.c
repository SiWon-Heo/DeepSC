static int ipvideo_decode_block_opcode_0xC_16 ( IpvideoContext * s ) {
 int x , y ;
 uint16_t * pixel_ptr = ( uint16_t * ) s -> pixel_ptr ;
 for ( y = 0 ;
 y < 8 ;
 y += 2 ) {
 for ( x = 0 ;
 x < 8 ;
 x += 2 ) {
 pixel_ptr [ x ] = pixel_ptr [ x + 1 ] = pixel_ptr [ x + s -> stride ] = pixel_ptr [ x + 1 + s -> stride ] = bytestream2_get_le16 ( & s -> stream_ptr ) ;
 }
 pixel_ptr += s -> stride * 2 ;
 }
 return 0 ;
 }