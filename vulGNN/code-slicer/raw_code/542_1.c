static int ipvideo_decode_frame ( AVCodecContext * avctx , void * data , int * got_frame , AVPacket * avpkt ) {
 const uint8_t * buf = avpkt -> data ;
 int buf_size = avpkt -> size ;
 IpvideoContext * s = avctx -> priv_data ;
 int ret ;
 s -> decoding_map_size = avctx -> width * avctx -> height / ( 8 * 8 * 2 ) ;
 if ( buf_size < s -> decoding_map_size ) return buf_size ;
 s -> decoding_map = buf ;
 bytestream2_init ( & s -> stream_ptr , buf + s -> decoding_map_size , buf_size - s -> decoding_map_size ) ;
 s -> current_frame . reference = 3 ;
 if ( ( ret = ff_get_buffer ( avctx , & s -> current_frame ) ) < 0 ) {
 av_log ( avctx , AV_LOG_ERROR , " Interplay Video: get_buffer() failed\n" ) ;
 return ret ;
 }
 if ( ! s -> is_16bpp ) {
 const uint8_t * pal = av_packet_get_side_data ( avpkt , AV_PKT_DATA_PALETTE , NULL ) ;
 if ( pal ) {
 s -> current_frame . palette_has_changed = 1 ;
 memcpy ( s -> pal , pal , AVPALETTE_SIZE ) ;
 }
 }
 ipvideo_decode_opcodes ( s ) ;
 * got_frame = 1 ;
 * ( AVFrame * ) data = s -> current_frame ;
 if ( s -> second_last_frame . data [ 0 ] ) avctx -> release_buffer ( avctx , & s -> second_last_frame ) ;
 s -> second_last_frame = s -> last_frame ;
 s -> last_frame = s -> current_frame ;
 s -> current_frame . data [ 0 ] = NULL ;
 return buf_size ;
 }