static int atrac1_decode_frame ( AVCodecContext * avctx , void * data , int * got_frame_ptr , AVPacket * avpkt ) {
 AVFrame * frame = data ;
 const uint8_t * buf = avpkt -> data ;
 int buf_size = avpkt -> size ;
 AT1Ctx * q = avctx -> priv_data ;
 int ch , ret ;
 GetBitContext gb ;
 if ( buf_size < 212 * avctx -> channels ) {
 av_log ( avctx , AV_LOG_ERROR , "Not enough data to decode!\n" ) ;
 return AVERROR_INVALIDDATA ;
 }
 frame -> nb_samples = AT1_SU_SAMPLES ;
 if ( ( ret = ff_get_buffer ( avctx , frame ) ) < 0 ) {
 av_log ( avctx , AV_LOG_ERROR , "get_buffer() failed\n" ) ;
 return ret ;
 }
 for ( ch = 0 ;
 ch < avctx -> channels ;
 ch ++ ) {
 AT1SUCtx * su = & q -> SUs [ ch ] ;
 init_get_bits ( & gb , & buf [ 212 * ch ] , 212 * 8 ) ;
 ret = at1_parse_bsm ( & gb , su -> log2_block_count ) ;
 if ( ret < 0 ) return ret ;
 ret = at1_unpack_dequant ( & gb , su , q -> spec ) ;
 if ( ret < 0 ) return ret ;
 ret = at1_imdct_block ( su , q ) ;
 if ( ret < 0 ) return ret ;
 at1_subband_synthesis ( q , su , ( float * ) frame -> extended_data [ ch ] ) ;
 }
 * got_frame_ptr = 1 ;
 return avctx -> block_align ;
 }