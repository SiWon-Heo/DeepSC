static int rv34_decode_inter_macroblock ( RV34DecContext * r , int8_t * intra_types ) {
 MpegEncContext * s = & r -> s ;
 GetBitContext * gb = & s -> gb ;
 uint8_t * dst = s -> dest [ 0 ] ;
 int16_t * ptr = s -> block [ 0 ] ;
 int mb_pos = s -> mb_x + s -> mb_y * s -> mb_stride ;
 int cbp , cbp2 ;
 int q_dc , q_ac , has_ac ;
 int i , j ;
 int dist ;
 memset ( r -> avail_cache , 0 , sizeof ( r -> avail_cache ) ) ;
 fill_rectangle ( r -> avail_cache + 6 , 2 , 2 , 4 , 1 , 4 ) ;
 dist = ( s -> mb_x - s -> resync_mb_x ) + ( s -> mb_y - s -> resync_mb_y ) * s -> mb_width ;
 if ( s -> mb_x && dist ) r -> avail_cache [ 5 ] = r -> avail_cache [ 9 ] = s -> current_picture_ptr -> f . mb_type [ mb_pos - 1 ] ;
 if ( dist >= s -> mb_width ) r -> avail_cache [ 2 ] = r -> avail_cache [ 3 ] = s -> current_picture_ptr -> f . mb_type [ mb_pos - s -> mb_stride ] ;
 if ( ( ( s -> mb_x + 1 ) < s -> mb_width ) && dist >= s -> mb_width - 1 ) r -> avail_cache [ 4 ] = s -> current_picture_ptr -> f . mb_type [ mb_pos - s -> mb_stride + 1 ] ;
 if ( s -> mb_x && dist > s -> mb_width ) r -> avail_cache [ 1 ] = s -> current_picture_ptr -> f . mb_type [ mb_pos - s -> mb_stride - 1 ] ;
 s -> qscale = r -> si . quant ;
 cbp = cbp2 = rv34_decode_inter_mb_header ( r , intra_types ) ;
 r -> cbp_luma [ mb_pos ] = cbp ;
 r -> cbp_chroma [ mb_pos ] = cbp >> 16 ;
 r -> deblock_coefs [ mb_pos ] = rv34_set_deblock_coef ( r ) | r -> cbp_luma [ mb_pos ] ;
 s -> current_picture_ptr -> f . qscale_table [ mb_pos ] = s -> qscale ;
 if ( cbp == - 1 ) return - 1 ;
 if ( IS_INTRA ( s -> current_picture_ptr -> f . mb_type [ mb_pos ] ) ) {
 if ( r -> is16 ) rv34_output_i16x16 ( r , intra_types , cbp ) ;
 else rv34_output_intra ( r , intra_types , cbp ) ;
 return 0 ;
 }
 if ( r -> is16 ) {
 LOCAL_ALIGNED_16 ( int16_t , block16 , [ 16 ] ) ;
 memset ( block16 , 0 , 16 * sizeof ( * block16 ) ) ;
 q_dc = rv34_qscale_tab [ r -> luma_dc_quant_p [ s -> qscale ] ] ;
 q_ac = rv34_qscale_tab [ s -> qscale ] ;
 if ( rv34_decode_block ( block16 , gb , r -> cur_vlcs , 3 , 0 , q_dc , q_dc , q_ac ) ) r -> rdsp . rv34_inv_transform ( block16 ) ;
 else r -> rdsp . rv34_inv_transform_dc ( block16 ) ;
 q_ac = rv34_qscale_tab [ s -> qscale ] ;
 for ( j = 0 ;
 j < 4 ;
 j ++ ) {
 for ( i = 0 ;
 i < 4 ;
 i ++ , cbp >>= 1 ) {
 int dc = block16 [ i + j * 4 ] ;
 if ( cbp & 1 ) {
 has_ac = rv34_decode_block ( ptr , gb , r -> cur_vlcs , r -> luma_vlc , 0 , q_ac , q_ac , q_ac ) ;
 }
 else has_ac = 0 ;
 if ( has_ac ) {
 ptr [ 0 ] = dc ;
 r -> rdsp . rv34_idct_add ( dst + 4 * i , s -> linesize , ptr ) ;
 }
 else r -> rdsp . rv34_idct_dc_add ( dst + 4 * i , s -> linesize , dc ) ;
 }
 dst += 4 * s -> linesize ;
 }
 r -> cur_vlcs = choose_vlc_set ( r -> si . quant , r -> si . vlc_set , 1 ) ;
 }
 else {
 q_ac = rv34_qscale_tab [ s -> qscale ] ;
 for ( j = 0 ;
 j < 4 ;
 j ++ ) {
 for ( i = 0 ;
 i < 4 ;
 i ++ , cbp >>= 1 ) {
 if ( ! ( cbp & 1 ) ) continue ;
 rv34_process_block ( r , dst + 4 * i , s -> linesize , r -> luma_vlc , 0 , q_ac , q_ac ) ;
 }
 dst += 4 * s -> linesize ;
 }
 }
 q_dc = rv34_qscale_tab [ rv34_chroma_quant [ 1 ] [ s -> qscale ] ] ;
 q_ac = rv34_qscale_tab [ rv34_chroma_quant [ 0 ] [ s -> qscale ] ] ;
 for ( j = 1 ;
 j < 3 ;
 j ++ ) {
 dst = s -> dest [ j ] ;
 for ( i = 0 ;
 i < 4 ;
 i ++ , cbp >>= 1 ) {
 uint8_t * pdst ;
 if ( ! ( cbp & 1 ) ) continue ;
 pdst = dst + ( i & 1 ) * 4 + ( i & 2 ) * 2 * s -> uvlinesize ;
 rv34_process_block ( r , pdst , s -> uvlinesize , r -> chroma_vlc , 1 , q_dc , q_ac ) ;
 }
 }
 return 0 ;
 }