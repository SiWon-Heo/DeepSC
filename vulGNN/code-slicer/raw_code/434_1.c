static int gsm_decode_frame ( AVCodecContext * avctx , void * data , int * got_frame_ptr , AVPacket * avpkt ) {
 AVFrame * frame = data ;
 int res ;
 GetBitContext gb ;
 const uint8_t * buf = avpkt -> data ;
 int buf_size = avpkt -> size ;
 int16_t * samples ;
 if ( buf_size < avctx -> block_align ) {
 av_log ( avctx , AV_LOG_ERROR , "Packet is too small\n" ) ;
 return AVERROR_INVALIDDATA ;
 }
 frame -> nb_samples = avctx -> frame_size ;
 if ( ( res = ff_get_buffer ( avctx , frame ) ) < 0 ) {
 av_log ( avctx , AV_LOG_ERROR , "get_buffer() failed\n" ) ;
 return res ;
 }
 samples = ( int16_t * ) frame -> data [ 0 ] ;
 switch ( avctx -> codec_id ) {
 case AV_CODEC_ID_GSM : init_get_bits ( & gb , buf , buf_size * 8 ) ;
 if ( get_bits ( & gb , 4 ) != 0xd ) av_log ( avctx , AV_LOG_WARNING , "Missing GSM magic!\n" ) ;
 res = gsm_decode_block ( avctx , samples , & gb ) ;
 if ( res < 0 ) return res ;
 break ;
 case AV_CODEC_ID_GSM_MS : res = ff_msgsm_decode_block ( avctx , samples , buf ) ;
 if ( res < 0 ) return res ;
 }
 * got_frame_ptr = 1 ;
 return avctx -> block_align ;
 }