static int ipvideo_decode_block_opcode_0x1 ( IpvideoContext * s ) {
 return copy_from ( s , & s -> second_last_frame , 0 , 0 ) ;
 }