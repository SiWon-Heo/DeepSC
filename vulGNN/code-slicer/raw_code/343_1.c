int ff_wmv2_decode_mb ( MpegEncContext * s , int16_t block [ 6 ] [ 64 ] ) {
 Wmv2Context * const w = ( Wmv2Context * ) s ;
 int cbp , code , i ;
 uint8_t * coded_val ;
 if ( w -> j_type ) return 0 ;
 if ( s -> pict_type == AV_PICTURE_TYPE_P ) {
 if ( IS_SKIP ( s -> current_picture . f . mb_type [ s -> mb_y * s -> mb_stride + s -> mb_x ] ) ) {
 s -> mb_intra = 0 ;
 for ( i = 0 ;
 i < 6 ;
 i ++ ) s -> block_last_index [ i ] = - 1 ;
 s -> mv_dir = MV_DIR_FORWARD ;
 s -> mv_type = MV_TYPE_16X16 ;
 s -> mv [ 0 ] [ 0 ] [ 0 ] = 0 ;
 s -> mv [ 0 ] [ 0 ] [ 1 ] = 0 ;
 s -> mb_skipped = 1 ;
 w -> hshift = 0 ;
 return 0 ;
 }
 code = get_vlc2 ( & s -> gb , ff_mb_non_intra_vlc [ w -> cbp_table_index ] . table , MB_NON_INTRA_VLC_BITS , 3 ) ;
 if ( code < 0 ) return - 1 ;
 s -> mb_intra = ( ~ code & 0x40 ) >> 6 ;
 cbp = code & 0x3f ;
 }
 else {
 s -> mb_intra = 1 ;
 code = get_vlc2 ( & s -> gb , ff_msmp4_mb_i_vlc . table , MB_INTRA_VLC_BITS , 2 ) ;
 if ( code < 0 ) {
 av_log ( s -> avctx , AV_LOG_ERROR , "II-cbp illegal at %d %d\n" , s -> mb_x , s -> mb_y ) ;
 return - 1 ;
 }
 cbp = 0 ;
 for ( i = 0 ;
 i < 6 ;
 i ++ ) {
 int val = ( ( code >> ( 5 - i ) ) & 1 ) ;
 if ( i < 4 ) {
 int pred = ff_msmpeg4_coded_block_pred ( s , i , & coded_val ) ;
 val = val ^ pred ;
 * coded_val = val ;
 }
 cbp |= val << ( 5 - i ) ;
 }
 }
 if ( ! s -> mb_intra ) {
 int mx , my ;
 wmv2_pred_motion ( w , & mx , & my ) ;
 if ( cbp ) {
 s -> dsp . clear_blocks ( s -> block [ 0 ] ) ;
 if ( s -> per_mb_rl_table ) {
 s -> rl_table_index = decode012 ( & s -> gb ) ;
 s -> rl_chroma_table_index = s -> rl_table_index ;
 }
 if ( w -> abt_flag && w -> per_mb_abt ) {
 w -> per_block_abt = get_bits1 ( & s -> gb ) ;
 if ( ! w -> per_block_abt ) w -> abt_type = decode012 ( & s -> gb ) ;
 }
 else w -> per_block_abt = 0 ;
 }
 if ( wmv2_decode_motion ( w , & mx , & my ) < 0 ) return - 1 ;
 s -> mv_dir = MV_DIR_FORWARD ;
 s -> mv_type = MV_TYPE_16X16 ;
 s -> mv [ 0 ] [ 0 ] [ 0 ] = mx ;
 s -> mv [ 0 ] [ 0 ] [ 1 ] = my ;
 for ( i = 0 ;
 i < 6 ;
 i ++ ) {
 if ( wmv2_decode_inter_block ( w , block [ i ] , i , ( cbp >> ( 5 - i ) ) & 1 ) < 0 ) {
 av_log ( s -> avctx , AV_LOG_ERROR , "\nerror while decoding inter block: %d x %d (%d)\n" , s -> mb_x , s -> mb_y , i ) ;
 return - 1 ;
 }
 }
 }
 else {
 if ( s -> pict_type == AV_PICTURE_TYPE_P ) av_dlog ( s -> avctx , "%d%d " , s -> inter_intra_pred , cbp ) ;
 av_dlog ( s -> avctx , "I at %d %d %d %06X\n" , s -> mb_x , s -> mb_y , ( ( cbp & 3 ) ? 1 : 0 ) + ( ( cbp & 0x3C ) ? 2 : 0 ) , show_bits ( & s -> gb , 24 ) ) ;
 s -> ac_pred = get_bits1 ( & s -> gb ) ;
 if ( s -> inter_intra_pred ) {
 s -> h263_aic_dir = get_vlc2 ( & s -> gb , ff_inter_intra_vlc . table , INTER_INTRA_VLC_BITS , 1 ) ;
 av_dlog ( s -> avctx , "%d%d %d %d/" , s -> ac_pred , s -> h263_aic_dir , s -> mb_x , s -> mb_y ) ;
 }
 if ( s -> per_mb_rl_table && cbp ) {
 s -> rl_table_index = decode012 ( & s -> gb ) ;
 s -> rl_chroma_table_index = s -> rl_table_index ;
 }
 s -> dsp . clear_blocks ( s -> block [ 0 ] ) ;
 for ( i = 0 ;
 i < 6 ;
 i ++ ) {
 if ( ff_msmpeg4_decode_block ( s , block [ i ] , i , ( cbp >> ( 5 - i ) ) & 1 , NULL ) < 0 ) {
 av_log ( s -> avctx , AV_LOG_ERROR , "\nerror while decoding intra block: %d x %d (%d)\n" , s -> mb_x , s -> mb_y , i ) ;
 return - 1 ;
 }
 }
 }
 return 0 ;
 }