static int ipvideo_decode_block_opcode_0xB ( IpvideoContext * s ) {
 int y ;
 for ( y = 0 ;
 y < 8 ;
 y ++ ) {
 bytestream2_get_buffer ( & s -> stream_ptr , s -> pixel_ptr , 8 ) ;
 s -> pixel_ptr += s -> stride ;
 }
 return 0 ;
 }