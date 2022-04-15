static int flac_decode_frame ( AVCodecContext * avctx , void * data , int * got_frame_ptr , AVPacket * avpkt ) {
 AVFrame * frame = data ;
 const uint8_t * buf = avpkt -> data ;
 int buf_size = avpkt -> size ;
 FLACContext * s = avctx -> priv_data ;
 int bytes_read = 0 ;
 int ret ;
 * got_frame_ptr = 0 ;
 if ( s -> max_framesize == 0 ) {
 s -> max_framesize = ff_flac_get_max_frame_size ( s -> max_blocksize ? s -> max_blocksize : FLAC_MAX_BLOCKSIZE , FLAC_MAX_CHANNELS , 32 ) ;
 }
 if ( buf_size < FLAC_MIN_FRAME_SIZE ) return buf_size ;
 if ( AV_RB32 ( buf ) == MKBETAG ( 'f' , 'L' , 'a' , 'C' ) ) {
 if ( ! s -> got_streaminfo && parse_streaminfo ( s , buf , buf_size ) ) {
 av_log ( s -> avctx , AV_LOG_ERROR , "invalid header\n" ) ;
 return - 1 ;
 }
 return get_metadata_size ( buf , buf_size ) ;
 }
 init_get_bits ( & s -> gb , buf , buf_size * 8 ) ;
 if ( decode_frame ( s ) < 0 ) {
 av_log ( s -> avctx , AV_LOG_ERROR , "decode_frame() failed\n" ) ;
 return - 1 ;
 }
 bytes_read = ( get_bits_count ( & s -> gb ) + 7 ) / 8 ;
 frame -> nb_samples = s -> blocksize ;
 if ( ( ret = ff_get_buffer ( avctx , frame ) ) < 0 ) {
 av_log ( avctx , AV_LOG_ERROR , "get_buffer() failed\n" ) ;
 return ret ;
 }
 s -> dsp . decorrelate [ s -> ch_mode ] ( frame -> data , s -> decoded , s -> channels , s -> blocksize , s -> sample_shift ) ;
 if ( bytes_read > buf_size ) {
 av_log ( s -> avctx , AV_LOG_ERROR , "overread: %d\n" , bytes_read - buf_size ) ;
 return - 1 ;
 }
 if ( bytes_read < buf_size ) {
 av_log ( s -> avctx , AV_LOG_DEBUG , "underread: %d orig size: %d\n" , buf_size - bytes_read , buf_size ) ;
 }
 * got_frame_ptr = 1 ;
 return bytes_read ;
 }