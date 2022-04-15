static int mpeg_decode_slice ( MpegEncContext * s , int mb_y , const uint8_t * * buf , int buf_size ) {
 AVCodecContext * avctx = s -> avctx ;
 const int field_pic = s -> picture_structure != PICT_FRAME ;
 s -> resync_mb_x = s -> resync_mb_y = - 1 ;
 assert ( mb_y < s -> mb_height ) ;
 init_get_bits ( & s -> gb , * buf , buf_size * 8 ) ;
 ff_mpeg1_clean_buffers ( s ) ;
 s -> interlaced_dct = 0 ;
 s -> qscale = get_qscale ( s ) ;
 if ( s -> qscale == 0 ) {
 av_log ( s -> avctx , AV_LOG_ERROR , "qscale == 0\n" ) ;
 return - 1 ;
 }
 while ( get_bits1 ( & s -> gb ) != 0 ) {
 skip_bits ( & s -> gb , 8 ) ;
 }
 s -> mb_x = 0 ;
 if ( mb_y == 0 && s -> codec_tag == AV_RL32 ( "SLIF" ) ) {
 skip_bits1 ( & s -> gb ) ;
 }
 else {
 while ( get_bits_left ( & s -> gb ) > 0 ) {
 int code = get_vlc2 ( & s -> gb , mbincr_vlc . table , MBINCR_VLC_BITS , 2 ) ;
 if ( code < 0 ) {
 av_log ( s -> avctx , AV_LOG_ERROR , "first mb_incr damaged\n" ) ;
 return - 1 ;
 }
 if ( code >= 33 ) {
 if ( code == 33 ) {
 s -> mb_x += 33 ;
 }
 }
 else {
 s -> mb_x += code ;
 break ;
 }
 }
 }
 if ( s -> mb_x >= ( unsigned ) s -> mb_width ) {
 av_log ( s -> avctx , AV_LOG_ERROR , "initial skip overflow\n" ) ;
 return - 1 ;
 }
 if ( avctx -> hwaccel ) {
 const uint8_t * buf_end , * buf_start = * buf - 4 ;
 int start_code = - 1 ;
 buf_end = avpriv_mpv_find_start_code ( buf_start + 2 , * buf + buf_size , & start_code ) ;
 if ( buf_end < * buf + buf_size ) buf_end -= 4 ;
 s -> mb_y = mb_y ;
 if ( avctx -> hwaccel -> decode_slice ( avctx , buf_start , buf_end - buf_start ) < 0 ) return DECODE_SLICE_ERROR ;
 * buf = buf_end ;
 return DECODE_SLICE_OK ;
 }
 s -> resync_mb_x = s -> mb_x ;
 s -> resync_mb_y = s -> mb_y = mb_y ;
 s -> mb_skip_run = 0 ;
 ff_init_block_index ( s ) ;
 if ( s -> mb_y == 0 && s -> mb_x == 0 && ( s -> first_field || s -> picture_structure == PICT_FRAME ) ) {
 if ( s -> avctx -> debug & FF_DEBUG_PICT_INFO ) {
 av_log ( s -> avctx , AV_LOG_DEBUG , "qp:%d fc:%2d%2d%2d%2d %s %s %s %s %s dc:%d pstruct:%d fdct:%d cmv:%d qtype:%d ivlc:%d rff:%d %s\n" , s -> qscale , s -> mpeg_f_code [ 0 ] [ 0 ] , s -> mpeg_f_code [ 0 ] [ 1 ] , s -> mpeg_f_code [ 1 ] [ 0 ] , s -> mpeg_f_code [ 1 ] [ 1 ] , s -> pict_type == AV_PICTURE_TYPE_I ? "I" : ( s -> pict_type == AV_PICTURE_TYPE_P ? "P" : ( s -> pict_type == AV_PICTURE_TYPE_B ? "B" : "S" ) ) , s -> progressive_sequence ? "ps" : "" , s -> progressive_frame ? "pf" : "" , s -> alternate_scan ? "alt" : "" , s -> top_field_first ? "top" : "" , s -> intra_dc_precision , s -> picture_structure , s -> frame_pred_frame_dct , s -> concealment_motion_vectors , s -> q_scale_type , s -> intra_vlc_format , s -> repeat_first_field , s -> chroma_420_type ? "420" : "" ) ;
 }
 }
 for ( ;
 ;
 ) {
 if ( CONFIG_MPEG_XVMC_DECODER && s -> avctx -> xvmc_acceleration > 1 ) ff_xvmc_init_block ( s ) ;
 if ( mpeg_decode_mb ( s , s -> block ) < 0 ) return - 1 ;
 if ( s -> current_picture . f . motion_val [ 0 ] && ! s -> encoding ) {
 const int wrap = s -> b8_stride ;
 int xy = s -> mb_x * 2 + s -> mb_y * 2 * wrap ;
 int b8_xy = 4 * ( s -> mb_x + s -> mb_y * s -> mb_stride ) ;
 int motion_x , motion_y , dir , i ;
 for ( i = 0 ;
 i < 2 ;
 i ++ ) {
 for ( dir = 0 ;
 dir < 2 ;
 dir ++ ) {
 if ( s -> mb_intra || ( dir == 1 && s -> pict_type != AV_PICTURE_TYPE_B ) ) {
 motion_x = motion_y = 0 ;
 }
 else if ( s -> mv_type == MV_TYPE_16X16 || ( s -> mv_type == MV_TYPE_FIELD && field_pic ) ) {
 motion_x = s -> mv [ dir ] [ 0 ] [ 0 ] ;
 motion_y = s -> mv [ dir ] [ 0 ] [ 1 ] ;
 }
 else {
 motion_x = s -> mv [ dir ] [ i ] [ 0 ] ;
 motion_y = s -> mv [ dir ] [ i ] [ 1 ] ;
 }
 s -> current_picture . f . motion_val [ dir ] [ xy ] [ 0 ] = motion_x ;
 s -> current_picture . f . motion_val [ dir ] [ xy ] [ 1 ] = motion_y ;
 s -> current_picture . f . motion_val [ dir ] [ xy + 1 ] [ 0 ] = motion_x ;
 s -> current_picture . f . motion_val [ dir ] [ xy + 1 ] [ 1 ] = motion_y ;
 s -> current_picture . f . ref_index [ dir ] [ b8_xy ] = s -> current_picture . f . ref_index [ dir ] [ b8_xy + 1 ] = s -> field_select [ dir ] [ i ] ;
 assert ( s -> field_select [ dir ] [ i ] == 0 || s -> field_select [ dir ] [ i ] == 1 ) ;
 }
 xy += wrap ;
 b8_xy += 2 ;
 }
 }
 s -> dest [ 0 ] += 16 ;
 s -> dest [ 1 ] += 16 >> s -> chroma_x_shift ;
 s -> dest [ 2 ] += 16 >> s -> chroma_x_shift ;
 ff_MPV_decode_mb ( s , s -> block ) ;
 if ( ++ s -> mb_x >= s -> mb_width ) {
 const int mb_size = 16 ;
 ff_mpeg_draw_horiz_band ( s , mb_size * ( s -> mb_y >> field_pic ) , mb_size ) ;
 ff_MPV_report_decode_progress ( s ) ;
 s -> mb_x = 0 ;
 s -> mb_y += 1 << field_pic ;
 if ( s -> mb_y >= s -> mb_height ) {
 int left = get_bits_left ( & s -> gb ) ;
 int is_d10 = s -> chroma_format == 2 && s -> pict_type == AV_PICTURE_TYPE_I && avctx -> profile == 0 && avctx -> level == 5 && s -> intra_dc_precision == 2 && s -> q_scale_type == 1 && s -> alternate_scan == 0 && s -> progressive_frame == 0 ;
 if ( left < 0 || ( left && show_bits ( & s -> gb , FFMIN ( left , 23 ) ) && ! is_d10 ) || ( ( avctx -> err_recognition & AV_EF_BUFFER ) && left > 8 ) ) {
 av_log ( avctx , AV_LOG_ERROR , "end mismatch left=%d %0X\n" , left , show_bits ( & s -> gb , FFMIN ( left , 23 ) ) ) ;
 return - 1 ;
 }
 else goto eos ;
 }
 ff_init_block_index ( s ) ;
 }
 if ( s -> mb_skip_run == - 1 ) {
 s -> mb_skip_run = 0 ;
 for ( ;
 ;
 ) {
 int code = get_vlc2 ( & s -> gb , mbincr_vlc . table , MBINCR_VLC_BITS , 2 ) ;
 if ( code < 0 ) {
 av_log ( s -> avctx , AV_LOG_ERROR , "mb incr damaged\n" ) ;
 return - 1 ;
 }
 if ( code >= 33 ) {
 if ( code == 33 ) {
 s -> mb_skip_run += 33 ;
 }
 else if ( code == 35 ) {
 if ( s -> mb_skip_run != 0 || show_bits ( & s -> gb , 15 ) != 0 ) {
 av_log ( s -> avctx , AV_LOG_ERROR , "slice mismatch\n" ) ;
 return - 1 ;
 }
 goto eos ;
 }
 }
 else {
 s -> mb_skip_run += code ;
 break ;
 }
 }
 if ( s -> mb_skip_run ) {
 int i ;
 if ( s -> pict_type == AV_PICTURE_TYPE_I ) {
 av_log ( s -> avctx , AV_LOG_ERROR , "skipped MB in I frame at %d %d\n" , s -> mb_x , s -> mb_y ) ;
 return - 1 ;
 }
 s -> mb_intra = 0 ;
 for ( i = 0 ;
 i < 12 ;
 i ++ ) s -> block_last_index [ i ] = - 1 ;
 if ( s -> picture_structure == PICT_FRAME ) s -> mv_type = MV_TYPE_16X16 ;
 else s -> mv_type = MV_TYPE_FIELD ;
 if ( s -> pict_type == AV_PICTURE_TYPE_P ) {
 s -> mv_dir = MV_DIR_FORWARD ;
 s -> mv [ 0 ] [ 0 ] [ 0 ] = s -> mv [ 0 ] [ 0 ] [ 1 ] = 0 ;
 s -> last_mv [ 0 ] [ 0 ] [ 0 ] = s -> last_mv [ 0 ] [ 0 ] [ 1 ] = 0 ;
 s -> last_mv [ 0 ] [ 1 ] [ 0 ] = s -> last_mv [ 0 ] [ 1 ] [ 1 ] = 0 ;
 s -> field_select [ 0 ] [ 0 ] = ( s -> picture_structure - 1 ) & 1 ;
 }
 else {
 s -> mv [ 0 ] [ 0 ] [ 0 ] = s -> last_mv [ 0 ] [ 0 ] [ 0 ] ;
 s -> mv [ 0 ] [ 0 ] [ 1 ] = s -> last_mv [ 0 ] [ 0 ] [ 1 ] ;
 s -> mv [ 1 ] [ 0 ] [ 0 ] = s -> last_mv [ 1 ] [ 0 ] [ 0 ] ;
 s -> mv [ 1 ] [ 0 ] [ 1 ] = s -> last_mv [ 1 ] [ 0 ] [ 1 ] ;
 }
 }
 }
 }
 eos : * buf += ( get_bits_count ( & s -> gb ) - 1 ) / 8 ;
 av_dlog ( s , "y %d %d %d %d\n" , s -> resync_mb_x , s -> resync_mb_y , s -> mb_x , s -> mb_y ) ;
 return 0 ;
 }