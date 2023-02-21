static int output_data ( MLPDecodeContext * m , unsigned int substr , AVFrame * frame , int * got_frame_ptr ) {
 AVCodecContext * avctx = m -> avctx ;
 SubStream * s = & m -> substream [ substr ] ;
 unsigned int i , out_ch = 0 ;
 int32_t * data_32 ;
 int16_t * data_16 ;
 int ret ;
 int is32 = ( m -> avctx -> sample_fmt == AV_SAMPLE_FMT_S32 ) ;
 if ( m -> avctx -> channels != s -> max_matrix_channel + 1 ) {
 av_log ( m -> avctx , AV_LOG_ERROR , "channel count mismatch\n" ) ;
 return AVERROR_INVALIDDATA ;
 }
 if ( ! s -> blockpos ) {
 av_log ( avctx , AV_LOG_ERROR , "No samples to output.\n" ) ;
 return AVERROR_INVALIDDATA ;
 }
 frame -> nb_samples = s -> blockpos ;
 if ( ( ret = ff_get_buffer ( avctx , frame ) ) < 0 ) {
 av_log ( avctx , AV_LOG_ERROR , "get_buffer() failed\n" ) ;
 return ret ;
 }
 data_32 = ( int32_t * ) frame -> data [ 0 ] ;
 data_16 = ( int16_t * ) frame -> data [ 0 ] ;
 for ( i = 0 ;
 i < s -> blockpos ;
 i ++ ) {
 for ( out_ch = 0 ;
 out_ch <= s -> max_matrix_channel ;
 out_ch ++ ) {
 int mat_ch = s -> ch_assign [ out_ch ] ;
 int32_t sample = m -> sample_buffer [ i ] [ mat_ch ] << s -> output_shift [ mat_ch ] ;
 s -> lossless_check_data ^= ( sample & 0xffffff ) << mat_ch ;
 if ( is32 ) * data_32 ++ = sample << 8 ;
 else * data_16 ++ = sample >> 8 ;
 }
 }
 * got_frame_ptr = 1 ;
 return 0 ;
 }