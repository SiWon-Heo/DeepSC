static int h261_decode_mb ( H261Context * h ) {
 MpegEncContext * const s = & h -> s ;
 int i , cbp , xy ;
 cbp = 63 ;
 do {
 h -> mba_diff = get_vlc2 ( & s -> gb , h261_mba_vlc . table , H261_MBA_VLC_BITS , 2 ) ;
 if ( h -> mba_diff == MBA_STARTCODE ) {
 h -> gob_start_code_skipped = 1 ;
 return SLICE_END ;
 }
 }
 while ( h -> mba_diff == MBA_STUFFING ) ;
 if ( h -> mba_diff < 0 ) {
 if ( get_bits_left ( & s -> gb ) <= 7 ) return SLICE_END ;
 av_log ( s -> avctx , AV_LOG_ERROR , "illegal mba at %d %d\n" , s -> mb_x , s -> mb_y ) ;
 return SLICE_ERROR ;
 }
 h -> mba_diff += 1 ;
 h -> current_mba += h -> mba_diff ;
 if ( h -> current_mba > MBA_STUFFING ) return SLICE_ERROR ;
 s -> mb_x = ( ( h -> gob_number - 1 ) % 2 ) * 11 + ( ( h -> current_mba - 1 ) % 11 ) ;
 s -> mb_y = ( ( h -> gob_number - 1 ) / 2 ) * 3 + ( ( h -> current_mba - 1 ) / 11 ) ;
 xy = s -> mb_x + s -> mb_y * s -> mb_stride ;
 ff_init_block_index ( s ) ;
 ff_update_block_index ( s ) ;
 h -> mtype = get_vlc2 ( & s -> gb , h261_mtype_vlc . table , H261_MTYPE_VLC_BITS , 2 ) ;
 h -> mtype = h261_mtype_map [ h -> mtype ] ;
 if ( IS_QUANT ( h -> mtype ) ) {
 ff_set_qscale ( s , get_bits ( & s -> gb , 5 ) ) ;
 }
 s -> mb_intra = IS_INTRA4x4 ( h -> mtype ) ;
 if ( IS_16X16 ( h -> mtype ) ) {
 if ( ( h -> current_mba == 1 ) || ( h -> current_mba == 12 ) || ( h -> current_mba == 23 ) || ( h -> mba_diff != 1 ) ) {
 h -> current_mv_x = 0 ;
 h -> current_mv_y = 0 ;
 }
 h -> current_mv_x = decode_mv_component ( & s -> gb , h -> current_mv_x ) ;
 h -> current_mv_y = decode_mv_component ( & s -> gb , h -> current_mv_y ) ;
 }
 else {
 h -> current_mv_x = 0 ;
 h -> current_mv_y = 0 ;
 }
 if ( HAS_CBP ( h -> mtype ) ) {
 cbp = get_vlc2 ( & s -> gb , h261_cbp_vlc . table , H261_CBP_VLC_BITS , 2 ) + 1 ;
 }
 if ( s -> mb_intra ) {
 s -> current_picture . f . mb_type [ xy ] = MB_TYPE_INTRA ;
 goto intra ;
 }
 s -> mv_dir = MV_DIR_FORWARD ;
 s -> mv_type = MV_TYPE_16X16 ;
 s -> current_picture . f . mb_type [ xy ] = MB_TYPE_16x16 | MB_TYPE_L0 ;
 s -> mv [ 0 ] [ 0 ] [ 0 ] = h -> current_mv_x * 2 ;
 s -> mv [ 0 ] [ 0 ] [ 1 ] = h -> current_mv_y * 2 ;
 intra : if ( s -> mb_intra || HAS_CBP ( h -> mtype ) ) {
 s -> dsp . clear_blocks ( s -> block [ 0 ] ) ;
 for ( i = 0 ;
 i < 6 ;
 i ++ ) {
 if ( h261_decode_block ( h , s -> block [ i ] , i , cbp & 32 ) < 0 ) {
 return SLICE_ERROR ;
 }
 cbp += cbp ;
 }
 }
 else {
 for ( i = 0 ;
 i < 6 ;
 i ++ ) s -> block_last_index [ i ] = - 1 ;
 }
 ff_MPV_decode_mb ( s , s -> block ) ;
 return SLICE_OK ;
 }