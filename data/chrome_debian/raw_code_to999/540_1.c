static int ipvideo_decode_block_opcode_0xB_16 ( IpvideoContext * s ) {
 int x , y ;
 uint16_t * pixel_ptr = ( uint16_t * ) s -> pixel_ptr ;
 for ( y = 0 ;
 y < 8 ;
 y ++ ) {
 for ( x = 0 ;
 x < 8 ;
 x ++ ) pixel_ptr [ x ] = bytestream2_get_le16 ( & s -> stream_ptr ) ;
 pixel_ptr += s -> stride ;
 }
 return 0 ;
 }