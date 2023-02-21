static int decode_frame ( AVCodecContext * avctx , void * data , int * got_frame , AVPacket * avpkt ) {
 const uint8_t * buf = avpkt -> data ;
 int buf_size = avpkt -> size ;
 CamtasiaContext * const c = avctx -> priv_data ;
 const unsigned char * encoded = buf ;
 int zret ;
 int ret , len = buf_size ;
 if ( c -> pic . data [ 0 ] ) avctx -> release_buffer ( avctx , & c -> pic ) ;
 c -> pic . reference = 1 ;
 c -> pic . buffer_hints = FF_BUFFER_HINTS_VALID ;
 if ( ( ret = ff_get_buffer ( avctx , & c -> pic ) ) < 0 ) {
 av_log ( avctx , AV_LOG_ERROR , "get_buffer() failed\n" ) ;
 return ret ;
 }
 zret = inflateReset ( & c -> zstream ) ;
 if ( zret != Z_OK ) {
 av_log ( avctx , AV_LOG_ERROR , "Inflate reset error: %d\n" , zret ) ;
 return AVERROR_UNKNOWN ;
 }
 c -> zstream . next_in = encoded ;
 c -> zstream . avail_in = len ;
 c -> zstream . next_out = c -> decomp_buf ;
 c -> zstream . avail_out = c -> decomp_size ;
 zret = inflate ( & c -> zstream , Z_FINISH ) ;
 if ( ( zret != Z_OK ) && ( zret != Z_STREAM_END ) && ( zret != Z_DATA_ERROR ) ) {
 av_log ( avctx , AV_LOG_ERROR , "Inflate error: %d\n" , zret ) ;
 return AVERROR_UNKNOWN ;
 }
 if ( zret != Z_DATA_ERROR ) {
 bytestream2_init ( & c -> gb , c -> decomp_buf , c -> decomp_size - c -> zstream . avail_out ) ;
 ff_msrle_decode ( avctx , ( AVPicture * ) & c -> pic , c -> bpp , & c -> gb ) ;
 }
 if ( c -> avctx -> pix_fmt == AV_PIX_FMT_PAL8 ) {
 const uint8_t * pal = av_packet_get_side_data ( avpkt , AV_PKT_DATA_PALETTE , NULL ) ;
 if ( pal ) {
 c -> pic . palette_has_changed = 1 ;
 memcpy ( c -> pal , pal , AVPALETTE_SIZE ) ;
 }
 memcpy ( c -> pic . data [ 1 ] , c -> pal , AVPALETTE_SIZE ) ;
 }
 * got_frame = 1 ;
 * ( AVFrame * ) data = c -> pic ;
 return buf_size ;
 }