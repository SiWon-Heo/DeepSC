static int rv34_decode_inter_mb_header ( RV34DecContext * r , int8_t * intra_types ) {
 MpegEncContext * s = & r -> s ;
 GetBitContext * gb = & s -> gb ;
 int mb_pos = s -> mb_x + s -> mb_y * s -> mb_stride ;
 int i , t ;
 r -> block_type = r -> decode_mb_info ( r ) ;
 if ( r -> block_type == - 1 ) return - 1 ;
 s -> current_picture_ptr -> f . mb_type [ mb_pos ] = rv34_mb_type_to_lavc [ r -> block_type ] ;
 r -> mb_type [ mb_pos ] = r -> block_type ;
 if ( r -> block_type == RV34_MB_SKIP ) {
 if ( s -> pict_type == AV_PICTURE_TYPE_P ) r -> mb_type [ mb_pos ] = RV34_MB_P_16x16 ;
 if ( s -> pict_type == AV_PICTURE_TYPE_B ) r -> mb_type [ mb_pos ] = RV34_MB_B_DIRECT ;
 }
 r -> is16 = ! ! IS_INTRA16x16 ( s -> current_picture_ptr -> f . mb_type [ mb_pos ] ) ;
 rv34_decode_mv ( r , r -> block_type ) ;
 if ( r -> block_type == RV34_MB_SKIP ) {
 fill_rectangle ( intra_types , 4 , 4 , r -> intra_types_stride , 0 , sizeof ( intra_types [ 0 ] ) ) ;
 return 0 ;
 }
 r -> chroma_vlc = 1 ;
 r -> luma_vlc = 0 ;
 if ( IS_INTRA ( s -> current_picture_ptr -> f . mb_type [ mb_pos ] ) ) {
 if ( r -> is16 ) {
 t = get_bits ( gb , 2 ) ;
 fill_rectangle ( intra_types , 4 , 4 , r -> intra_types_stride , t , sizeof ( intra_types [ 0 ] ) ) ;
 r -> luma_vlc = 2 ;
 }
 else {
 if ( r -> decode_intra_types ( r , gb , intra_types ) < 0 ) return - 1 ;
 r -> luma_vlc = 1 ;
 }
 r -> chroma_vlc = 0 ;
 r -> cur_vlcs = choose_vlc_set ( r -> si . quant , r -> si . vlc_set , 0 ) ;
 }
 else {
 for ( i = 0 ;
 i < 16 ;
 i ++ ) intra_types [ ( i & 3 ) + ( i >> 2 ) * r -> intra_types_stride ] = 0 ;
 r -> cur_vlcs = choose_vlc_set ( r -> si . quant , r -> si . vlc_set , 1 ) ;
 if ( r -> mb_type [ mb_pos ] == RV34_MB_P_MIX16x16 ) {
 r -> is16 = 1 ;
 r -> chroma_vlc = 1 ;
 r -> luma_vlc = 2 ;
 r -> cur_vlcs = choose_vlc_set ( r -> si . quant , r -> si . vlc_set , 0 ) ;
 }
 }
 return rv34_decode_cbp ( gb , r -> cur_vlcs , r -> is16 ) ;
 }