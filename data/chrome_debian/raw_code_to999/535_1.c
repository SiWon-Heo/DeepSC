static int ipvideo_decode_block_opcode_0xD_16 ( IpvideoContext * s ) {
 int x , y ;
 uint16_t P [ 2 ] ;
 uint16_t * pixel_ptr = ( uint16_t * ) s -> pixel_ptr ;
 for ( y = 0 ;
 y < 8 ;
 y ++ ) {
 if ( ! ( y & 3 ) ) {
 P [ 0 ] = bytestream2_get_le16 ( & s -> stream_ptr ) ;
 P [ 1 ] = bytestream2_get_le16 ( & s -> stream_ptr ) ;
 }
 for ( x = 0 ;
 x < 8 ;
 x ++ ) pixel_ptr [ x ] = P [ x >> 2 ] ;
 pixel_ptr += s -> stride ;
 }
 return 0 ;
 }