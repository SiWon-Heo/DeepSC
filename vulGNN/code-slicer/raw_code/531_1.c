static int ipvideo_decode_block_opcode_0x5 ( IpvideoContext * s ) {
 signed char x , y ;
 x = bytestream2_get_byte ( & s -> stream_ptr ) ;
 y = bytestream2_get_byte ( & s -> stream_ptr ) ;
 av_dlog ( NULL , " motion bytes = %d, %d\n" , x , y ) ;
 return copy_from ( s , & s -> last_frame , x , y ) ;
 }