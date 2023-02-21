static int rv34_decode_intra_mb_header ( RV34DecContext * r , int8_t * intra_types ) {
 MpegEncContext * s = & r -> s ;
 GetBitContext * gb = & s -> gb ;
 int mb_pos = s -> mb_x + s -> mb_y * s -> mb_stride ;
 int t ;
 r -> is16 = get_bits1 ( gb ) ;
 if ( r -> is16 ) {
 s -> current_picture_ptr -> f . mb_type [ mb_pos ] = MB_TYPE_INTRA16x16 ;
 r -> block_type = RV34_MB_TYPE_INTRA16x16 ;
 t = get_bits ( gb , 2 ) ;
 fill_rectangle ( intra_types , 4 , 4 , r -> intra_types_stride , t , sizeof ( intra_types [ 0 ] ) ) ;
 r -> luma_vlc = 2 ;
 }
 else {
 if ( ! r -> rv30 ) {
 if ( ! get_bits1 ( gb ) ) av_log ( s -> avctx , AV_LOG_ERROR , "Need DQUANT\n" ) ;
 }
 s -> current_picture_ptr -> f . mb_type [ mb_pos ] = MB_TYPE_INTRA ;
 r -> block_type = RV34_MB_TYPE_INTRA ;
 if ( r -> decode_intra_types ( r , gb , intra_types ) < 0 ) return - 1 ;
 r -> luma_vlc = 1 ;
 }
 r -> chroma_vlc = 0 ;
 r -> cur_vlcs = choose_vlc_set ( r -> si . quant , r -> si . vlc_set , 0 ) ;
 return rv34_decode_cbp ( gb , r -> cur_vlcs , r -> is16 ) ;
 }