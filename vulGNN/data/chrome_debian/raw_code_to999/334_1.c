static int decode_cabac_mb_skip ( H264Context * h , int mb_x , int mb_y ) {
 int mba_xy , mbb_xy ;
 int ctx = 0 ;
 if ( FRAME_MBAFF ) {
 int mb_xy = mb_x + ( mb_y & ~ 1 ) * h -> mb_stride ;
 mba_xy = mb_xy - 1 ;
 if ( ( mb_y & 1 ) && h -> slice_table [ mba_xy ] == h -> slice_num && MB_FIELD == ! ! IS_INTERLACED ( h -> cur_pic . f . mb_type [ mba_xy ] ) ) mba_xy += h -> mb_stride ;
 if ( MB_FIELD ) {
 mbb_xy = mb_xy - h -> mb_stride ;
 if ( ! ( mb_y & 1 ) && h -> slice_table [ mbb_xy ] == h -> slice_num && IS_INTERLACED ( h -> cur_pic . f . mb_type [ mbb_xy ] ) ) mbb_xy -= h -> mb_stride ;
 }
 else mbb_xy = mb_x + ( mb_y - 1 ) * h -> mb_stride ;
 }
 else {
 int mb_xy = h -> mb_xy ;
 mba_xy = mb_xy - 1 ;
 mbb_xy = mb_xy - ( h -> mb_stride << FIELD_PICTURE ) ;
 }
 if ( h -> slice_table [ mba_xy ] == h -> slice_num && ! IS_SKIP ( h -> cur_pic . f . mb_type [ mba_xy ] ) ) ctx ++ ;
 if ( h -> slice_table [ mbb_xy ] == h -> slice_num && ! IS_SKIP ( h -> cur_pic . f . mb_type [ mbb_xy ] ) ) ctx ++ ;
 if ( h -> slice_type_nos == AV_PICTURE_TYPE_B ) ctx += 13 ;
 return get_cabac_noinline ( & h -> cabac , & h -> cabac_state [ 11 + ctx ] ) ;
 }