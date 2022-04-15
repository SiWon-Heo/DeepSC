static int ipvideo_decode_block_opcode_0xE ( IpvideoContext * s ) {
 int y ;
 unsigned char pix ;
 pix = bytestream2_get_byte ( & s -> stream_ptr ) ;
 for ( y = 0 ;
 y < 8 ;
 y ++ ) {
 memset ( s -> pixel_ptr , pix , 8 ) ;
 s -> pixel_ptr += s -> stride ;
 }
 return 0 ;
 }