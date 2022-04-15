void ff_fix_long_p_mvs ( MpegEncContext * s ) {
 MotionEstContext * const c = & s -> me ;
 const int f_code = s -> f_code ;
 int y , range ;
 assert ( s -> pict_type == AV_PICTURE_TYPE_P ) ;
 range = ( ( ( s -> out_format == FMT_MPEG1 || s -> msmpeg4_version ) ? 8 : 16 ) << f_code ) ;
 assert ( range <= 16 || ! s -> msmpeg4_version ) ;
 assert ( range <= 256 || ! ( s -> codec_id == AV_CODEC_ID_MPEG2VIDEO && s -> avctx -> strict_std_compliance >= FF_COMPLIANCE_NORMAL ) ) ;
 if ( c -> avctx -> me_range && range > c -> avctx -> me_range ) range = c -> avctx -> me_range ;
 if ( s -> flags & CODEC_FLAG_4MV ) {
 const int wrap = s -> b8_stride ;
 for ( y = 0 ;
 y < s -> mb_height ;
 y ++ ) {
 int xy = y * 2 * wrap ;
 int i = y * s -> mb_stride ;
 int x ;
 for ( x = 0 ;
 x < s -> mb_width ;
 x ++ ) {
 if ( s -> mb_type [ i ] & CANDIDATE_MB_TYPE_INTER4V ) {
 int block ;
 for ( block = 0 ;
 block < 4 ;
 block ++ ) {
 int off = ( block & 1 ) + ( block >> 1 ) * wrap ;
 int mx = s -> current_picture . f . motion_val [ 0 ] [ xy + off ] [ 0 ] ;
 int my = s -> current_picture . f . motion_val [ 0 ] [ xy + off ] [ 1 ] ;
 if ( mx >= range || mx < - range || my >= range || my < - range ) {
 s -> mb_type [ i ] &= ~ CANDIDATE_MB_TYPE_INTER4V ;
 s -> mb_type [ i ] |= CANDIDATE_MB_TYPE_INTRA ;
 s -> current_picture . f . mb_type [ i ] = CANDIDATE_MB_TYPE_INTRA ;
 }
 }
 }
 xy += 2 ;
 i ++ ;
 }
 }
 }
 }