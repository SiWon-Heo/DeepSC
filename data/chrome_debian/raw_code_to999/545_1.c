static int decode_element ( AVCodecContext * avctx , AVFrame * frame , int ch_index , int channels ) {
 ALACContext * alac = avctx -> priv_data ;
 int has_size , bps , is_compressed , decorr_shift , decorr_left_weight , ret ;
 uint32_t output_samples ;
 int i , ch ;
 skip_bits ( & alac -> gb , 4 ) ;
 skip_bits ( & alac -> gb , 12 ) ;
 has_size = get_bits1 ( & alac -> gb ) ;
 alac -> extra_bits = get_bits ( & alac -> gb , 2 ) << 3 ;
 bps = alac -> sample_size - alac -> extra_bits + channels - 1 ;
 if ( bps > 32 ) {
 av_log ( avctx , AV_LOG_ERROR , "bps is unsupported: %d\n" , bps ) ;
 return AVERROR_PATCHWELCOME ;
 }
 is_compressed = ! get_bits1 ( & alac -> gb ) ;
 if ( has_size ) output_samples = get_bits_long ( & alac -> gb , 32 ) ;
 else output_samples = alac -> max_samples_per_frame ;
 if ( ! output_samples || output_samples > alac -> max_samples_per_frame ) {
 av_log ( avctx , AV_LOG_ERROR , "invalid samples per frame: %d\n" , output_samples ) ;
 return AVERROR_INVALIDDATA ;
 }
 if ( ! alac -> nb_samples ) {
 frame -> nb_samples = output_samples ;
 if ( ( ret = ff_get_buffer ( avctx , frame ) ) < 0 ) {
 av_log ( avctx , AV_LOG_ERROR , "get_buffer() failed\n" ) ;
 return ret ;
 }
 }
 else if ( output_samples != alac -> nb_samples ) {
 av_log ( avctx , AV_LOG_ERROR , "sample count mismatch: %u != %d\n" , output_samples , alac -> nb_samples ) ;
 return AVERROR_INVALIDDATA ;
 }
 alac -> nb_samples = output_samples ;
 if ( alac -> sample_size > 16 ) {
 for ( ch = 0 ;
 ch < channels ;
 ch ++ ) alac -> output_samples_buffer [ ch ] = ( int32_t * ) frame -> extended_data [ ch_index + ch ] ;
 }
 if ( is_compressed ) {
 int16_t lpc_coefs [ 2 ] [ 32 ] ;
 int lpc_order [ 2 ] ;
 int prediction_type [ 2 ] ;
 int lpc_quant [ 2 ] ;
 int rice_history_mult [ 2 ] ;
 decorr_shift = get_bits ( & alac -> gb , 8 ) ;
 decorr_left_weight = get_bits ( & alac -> gb , 8 ) ;
 for ( ch = 0 ;
 ch < channels ;
 ch ++ ) {
 prediction_type [ ch ] = get_bits ( & alac -> gb , 4 ) ;
 lpc_quant [ ch ] = get_bits ( & alac -> gb , 4 ) ;
 rice_history_mult [ ch ] = get_bits ( & alac -> gb , 3 ) ;
 lpc_order [ ch ] = get_bits ( & alac -> gb , 5 ) ;
 for ( i = lpc_order [ ch ] - 1 ;
 i >= 0 ;
 i -- ) lpc_coefs [ ch ] [ i ] = get_sbits ( & alac -> gb , 16 ) ;
 }
 if ( alac -> extra_bits ) {
 for ( i = 0 ;
 i < alac -> nb_samples ;
 i ++ ) {
 for ( ch = 0 ;
 ch < channels ;
 ch ++ ) alac -> extra_bits_buffer [ ch ] [ i ] = get_bits ( & alac -> gb , alac -> extra_bits ) ;
 }
 }
 for ( ch = 0 ;
 ch < channels ;
 ch ++ ) {
 rice_decompress ( alac , alac -> predict_error_buffer [ ch ] , alac -> nb_samples , bps , rice_history_mult [ ch ] * alac -> rice_history_mult / 4 ) ;
 if ( prediction_type [ ch ] == 15 ) {
 lpc_prediction ( alac -> predict_error_buffer [ ch ] , alac -> predict_error_buffer [ ch ] , alac -> nb_samples , bps , NULL , 31 , 0 ) ;
 }
 else if ( prediction_type [ ch ] > 0 ) {
 av_log ( avctx , AV_LOG_WARNING , "unknown prediction type: %i\n" , prediction_type [ ch ] ) ;
 }
 lpc_prediction ( alac -> predict_error_buffer [ ch ] , alac -> output_samples_buffer [ ch ] , alac -> nb_samples , bps , lpc_coefs [ ch ] , lpc_order [ ch ] , lpc_quant [ ch ] ) ;
 }
 }
 else {
 for ( i = 0 ;
 i < alac -> nb_samples ;
 i ++ ) {
 for ( ch = 0 ;
 ch < channels ;
 ch ++ ) {
 alac -> output_samples_buffer [ ch ] [ i ] = get_sbits_long ( & alac -> gb , alac -> sample_size ) ;
 }
 }
 alac -> extra_bits = 0 ;
 decorr_shift = 0 ;
 decorr_left_weight = 0 ;
 }
 if ( channels == 2 && decorr_left_weight ) {
 decorrelate_stereo ( alac -> output_samples_buffer , alac -> nb_samples , decorr_shift , decorr_left_weight ) ;
 }
 if ( alac -> extra_bits ) {
 append_extra_bits ( alac -> output_samples_buffer , alac -> extra_bits_buffer , alac -> extra_bits , channels , alac -> nb_samples ) ;
 }
 switch ( alac -> sample_size ) {
 case 16 : {
 for ( ch = 0 ;
 ch < channels ;
 ch ++ ) {
 int16_t * outbuffer = ( int16_t * ) frame -> extended_data [ ch_index + ch ] ;
 for ( i = 0 ;
 i < alac -> nb_samples ;
 i ++ ) * outbuffer ++ = alac -> output_samples_buffer [ ch ] [ i ] ;
 }
 }
 break ;
 case 24 : {
 for ( ch = 0 ;
 ch < channels ;
 ch ++ ) {
 for ( i = 0 ;
 i < alac -> nb_samples ;
 i ++ ) alac -> output_samples_buffer [ ch ] [ i ] <<= 8 ;
 }
 }
 break ;
 }
 return 0 ;
 }