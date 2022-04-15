static int msvideo1_decode_frame ( AVCodecContext * avctx , void * data , int * got_frame , AVPacket * avpkt ) {
 const uint8_t * buf = avpkt -> data ;
 int buf_size = avpkt -> size ;
 Msvideo1Context * s = avctx -> priv_data ;
 s -> buf = buf ;
 s -> size = buf_size ;
 s -> frame . reference = 1 ;
 s -> frame . buffer_hints = FF_BUFFER_HINTS_VALID | FF_BUFFER_HINTS_PRESERVE | FF_BUFFER_HINTS_REUSABLE ;
 if ( avctx -> reget_buffer ( avctx , & s -> frame ) ) {
 av_log ( s -> avctx , AV_LOG_ERROR , "reget_buffer() failed\n" ) ;
 return - 1 ;
 }
 if ( s -> mode_8bit ) {
 const uint8_t * pal = av_packet_get_side_data ( avpkt , AV_PKT_DATA_PALETTE , NULL ) ;
 if ( pal ) {
 memcpy ( s -> pal , pal , AVPALETTE_SIZE ) ;
 s -> frame . palette_has_changed = 1 ;
 }
 }
 if ( s -> mode_8bit ) msvideo1_decode_8bit ( s ) ;
 else msvideo1_decode_16bit ( s ) ;
 * got_frame = 1 ;
 * ( AVFrame * ) data = s -> frame ;
 return buf_size ;
 }