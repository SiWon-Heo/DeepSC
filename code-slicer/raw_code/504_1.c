static int decode_frame ( WMAProDecodeCtx * s , AVFrame * frame , int * got_frame_ptr ) {
 AVCodecContext * avctx = s -> avctx ;
 GetBitContext * gb = & s -> gb ;
 int more_frames = 0 ;
 int len = 0 ;
 int i , ret ;
 if ( s -> len_prefix ) len = get_bits ( gb , s -> log2_frame_size ) ;
 av_dlog ( s -> avctx , "decoding frame with length %x\n" , len ) ;
 if ( decode_tilehdr ( s ) ) {
 s -> packet_loss = 1 ;
 return 0 ;
 }
 if ( s -> avctx -> channels > 1 && get_bits1 ( gb ) ) {
 if ( get_bits1 ( gb ) ) {
 for ( i = 0 ;
 i < avctx -> channels * avctx -> channels ;
 i ++ ) skip_bits ( gb , 4 ) ;
 }
 }
 if ( s -> dynamic_range_compression ) {
 s -> drc_gain = get_bits ( gb , 8 ) ;
 av_dlog ( s -> avctx , "drc_gain %i\n" , s -> drc_gain ) ;
 }
 if ( get_bits1 ( gb ) ) {
 int av_unused skip ;
 if ( get_bits1 ( gb ) ) {
 skip = get_bits ( gb , av_log2 ( s -> samples_per_frame * 2 ) ) ;
 av_dlog ( s -> avctx , "start skip: %i\n" , skip ) ;
 }
 if ( get_bits1 ( gb ) ) {
 skip = get_bits ( gb , av_log2 ( s -> samples_per_frame * 2 ) ) ;
 av_dlog ( s -> avctx , "end skip: %i\n" , skip ) ;
 }
 }
 av_dlog ( s -> avctx , "BITSTREAM: frame header length was %i\n" , get_bits_count ( gb ) - s -> frame_offset ) ;
 s -> parsed_all_subframes = 0 ;
 for ( i = 0 ;
 i < avctx -> channels ;
 i ++ ) {
 s -> channel [ i ] . decoded_samples = 0 ;
 s -> channel [ i ] . cur_subframe = 0 ;
 s -> channel [ i ] . reuse_sf = 0 ;
 }
 while ( ! s -> parsed_all_subframes ) {
 if ( decode_subframe ( s ) < 0 ) {
 s -> packet_loss = 1 ;
 return 0 ;
 }
 }
 frame -> nb_samples = s -> samples_per_frame ;
 if ( ( ret = ff_get_buffer ( avctx , frame ) ) < 0 ) {
 av_log ( avctx , AV_LOG_ERROR , "get_buffer() failed\n" ) ;
 s -> packet_loss = 1 ;
 return 0 ;
 }
 for ( i = 0 ;
 i < avctx -> channels ;
 i ++ ) memcpy ( frame -> extended_data [ i ] , s -> channel [ i ] . out , s -> samples_per_frame * sizeof ( * s -> channel [ i ] . out ) ) ;
 for ( i = 0 ;
 i < avctx -> channels ;
 i ++ ) {
 memcpy ( & s -> channel [ i ] . out [ 0 ] , & s -> channel [ i ] . out [ s -> samples_per_frame ] , s -> samples_per_frame * sizeof ( * s -> channel [ i ] . out ) >> 1 ) ;
 }
 if ( s -> skip_frame ) {
 s -> skip_frame = 0 ;
 * got_frame_ptr = 0 ;
 }
 else {
 * got_frame_ptr = 1 ;
 }
 if ( s -> len_prefix ) {
 if ( len != ( get_bits_count ( gb ) - s -> frame_offset ) + 2 ) {
 av_log ( s -> avctx , AV_LOG_ERROR , "frame[%i] would have to skip %i bits\n" , s -> frame_num , len - ( get_bits_count ( gb ) - s -> frame_offset ) - 1 ) ;
 s -> packet_loss = 1 ;
 return 0 ;
 }
 skip_bits_long ( gb , len - ( get_bits_count ( gb ) - s -> frame_offset ) - 1 ) ;
 }
 else {
 while ( get_bits_count ( gb ) < s -> num_saved_bits && get_bits1 ( gb ) == 0 ) {
 }
 }
 more_frames = get_bits1 ( gb ) ;
 ++ s -> frame_num ;
 return more_frames ;
 }