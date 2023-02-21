static int decode_frame ( WmallDecodeCtx * s ) {
 GetBitContext * gb = & s -> gb ;
 int more_frames = 0 , len = 0 , i , ret ;
 s -> frame . nb_samples = s -> samples_per_frame ;
 if ( ( ret = ff_get_buffer ( s -> avctx , & s -> frame ) ) < 0 ) {
 av_log ( s -> avctx , AV_LOG_ERROR , "not enough space for the output samples\n" ) ;
 s -> packet_loss = 1 ;
 return ret ;
 }
 for ( i = 0 ;
 i < s -> num_channels ;
 i ++ ) {
 s -> samples_16 [ i ] = ( int16_t * ) s -> frame . extended_data [ i ] ;
 s -> samples_32 [ i ] = ( int32_t * ) s -> frame . extended_data [ i ] ;
 }
 if ( s -> len_prefix ) len = get_bits ( gb , s -> log2_frame_size ) ;
 if ( decode_tilehdr ( s ) ) {
 s -> packet_loss = 1 ;
 return 0 ;
 }
 if ( s -> dynamic_range_compression ) s -> drc_gain = get_bits ( gb , 8 ) ;
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
 s -> parsed_all_subframes = 0 ;
 for ( i = 0 ;
 i < s -> num_channels ;
 i ++ ) {
 s -> channel [ i ] . decoded_samples = 0 ;
 s -> channel [ i ] . cur_subframe = 0 ;
 }
 while ( ! s -> parsed_all_subframes ) {
 if ( decode_subframe ( s ) < 0 ) {
 s -> packet_loss = 1 ;
 return 0 ;
 }
 }
 av_dlog ( s -> avctx , "Frame done\n" ) ;
 if ( s -> skip_frame ) s -> skip_frame = 0 ;
 if ( s -> len_prefix ) {
 if ( len != ( get_bits_count ( gb ) - s -> frame_offset ) + 2 ) {
 av_log ( s -> avctx , AV_LOG_ERROR , "frame[%i] would have to skip %i bits\n" , s -> frame_num , len - ( get_bits_count ( gb ) - s -> frame_offset ) - 1 ) ;
 s -> packet_loss = 1 ;
 return 0 ;
 }
 skip_bits_long ( gb , len - ( get_bits_count ( gb ) - s -> frame_offset ) - 1 ) ;
 }
 more_frames = get_bits1 ( gb ) ;
 ++ s -> frame_num ;
 return more_frames ;
 }