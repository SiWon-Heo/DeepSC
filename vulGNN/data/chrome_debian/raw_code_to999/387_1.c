static int idcin_decode_frame ( AVCodecContext * avctx , void * data , int * got_frame , AVPacket * avpkt ) {
 const uint8_t * buf = avpkt -> data ;
 int buf_size = avpkt -> size ;
 IdcinContext * s = avctx -> priv_data ;
 const uint8_t * pal = av_packet_get_side_data ( avpkt , AV_PKT_DATA_PALETTE , NULL ) ;
 s -> buf = buf ;
 s -> size = buf_size ;
 if ( s -> frame . data [ 0 ] ) avctx -> release_buffer ( avctx , & s -> frame ) ;
 if ( ff_get_buffer ( avctx , & s -> frame ) ) {
 av_log ( avctx , AV_LOG_ERROR , " id CIN Video: get_buffer() failed\n" ) ;
 return - 1 ;
 }
 idcin_decode_vlcs ( s ) ;
 if ( pal ) {
 s -> frame . palette_has_changed = 1 ;
 memcpy ( s -> pal , pal , AVPALETTE_SIZE ) ;
 }
 memcpy ( s -> frame . data [ 1 ] , s -> pal , AVPALETTE_SIZE ) ;
 * got_frame = 1 ;
 * ( AVFrame * ) data = s -> frame ;
 return buf_size ;
 }