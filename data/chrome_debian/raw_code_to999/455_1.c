static void fill_decode_neighbors ( H264Context * h , int mb_type ) {
 const int mb_xy = h -> mb_xy ;
 int topleft_xy , top_xy , topright_xy , left_xy [ LEFT_MBS ] ;
 static const uint8_t left_block_options [ 4 ] [ 32 ] = {
 {
 0 , 1 , 2 , 3 , 7 , 10 , 8 , 11 , 3 + 0 * 4 , 3 + 1 * 4 , 3 + 2 * 4 , 3 + 3 * 4 , 1 + 4 * 4 , 1 + 8 * 4 , 1 + 5 * 4 , 1 + 9 * 4 }
 , {
 2 , 2 , 3 , 3 , 8 , 11 , 8 , 11 , 3 + 2 * 4 , 3 + 2 * 4 , 3 + 3 * 4 , 3 + 3 * 4 , 1 + 5 * 4 , 1 + 9 * 4 , 1 + 5 * 4 , 1 + 9 * 4 }
 , {
 0 , 0 , 1 , 1 , 7 , 10 , 7 , 10 , 3 + 0 * 4 , 3 + 0 * 4 , 3 + 1 * 4 , 3 + 1 * 4 , 1 + 4 * 4 , 1 + 8 * 4 , 1 + 4 * 4 , 1 + 8 * 4 }
 , {
 0 , 2 , 0 , 2 , 7 , 10 , 7 , 10 , 3 + 0 * 4 , 3 + 2 * 4 , 3 + 0 * 4 , 3 + 2 * 4 , 1 + 4 * 4 , 1 + 8 * 4 , 1 + 4 * 4 , 1 + 8 * 4 }
 }
 ;
 h -> topleft_partition = - 1 ;
 top_xy = mb_xy - ( h -> mb_stride << MB_FIELD ) ;
 topleft_xy = top_xy - 1 ;
 topright_xy = top_xy + 1 ;
 left_xy [ LBOT ] = left_xy [ LTOP ] = mb_xy - 1 ;
 h -> left_block = left_block_options [ 0 ] ;
 if ( FRAME_MBAFF ) {
 const int left_mb_field_flag = IS_INTERLACED ( h -> cur_pic . f . mb_type [ mb_xy - 1 ] ) ;
 const int curr_mb_field_flag = IS_INTERLACED ( mb_type ) ;
 if ( h -> mb_y & 1 ) {
 if ( left_mb_field_flag != curr_mb_field_flag ) {
 left_xy [ LBOT ] = left_xy [ LTOP ] = mb_xy - h -> mb_stride - 1 ;
 if ( curr_mb_field_flag ) {
 left_xy [ LBOT ] += h -> mb_stride ;
 h -> left_block = left_block_options [ 3 ] ;
 }
 else {
 topleft_xy += h -> mb_stride ;
 h -> topleft_partition = 0 ;
 h -> left_block = left_block_options [ 1 ] ;
 }
 }
 }
 else {
 if ( curr_mb_field_flag ) {
 topleft_xy += h -> mb_stride & ( ( ( h -> cur_pic . f . mb_type [ top_xy - 1 ] >> 7 ) & 1 ) - 1 ) ;
 topright_xy += h -> mb_stride & ( ( ( h -> cur_pic . f . mb_type [ top_xy + 1 ] >> 7 ) & 1 ) - 1 ) ;
 top_xy += h -> mb_stride & ( ( ( h -> cur_pic . f . mb_type [ top_xy ] >> 7 ) & 1 ) - 1 ) ;
 }
 if ( left_mb_field_flag != curr_mb_field_flag ) {
 if ( curr_mb_field_flag ) {
 left_xy [ LBOT ] += h -> mb_stride ;
 h -> left_block = left_block_options [ 3 ] ;
 }
 else {
 h -> left_block = left_block_options [ 2 ] ;
 }
 }
 }
 }
 h -> topleft_mb_xy = topleft_xy ;
 h -> top_mb_xy = top_xy ;
 h -> topright_mb_xy = topright_xy ;
 h -> left_mb_xy [ LTOP ] = left_xy [ LTOP ] ;
 h -> left_mb_xy [ LBOT ] = left_xy [ LBOT ] ;
 h -> topleft_type = h -> cur_pic . f . mb_type [ topleft_xy ] ;
 h -> top_type = h -> cur_pic . f . mb_type [ top_xy ] ;
 h -> topright_type = h -> cur_pic . f . mb_type [ topright_xy ] ;
 h -> left_type [ LTOP ] = h -> cur_pic . f . mb_type [ left_xy [ LTOP ] ] ;
 h -> left_type [ LBOT ] = h -> cur_pic . f . mb_type [ left_xy [ LBOT ] ] ;
 if ( FMO ) {
 if ( h -> slice_table [ topleft_xy ] != h -> slice_num ) h -> topleft_type = 0 ;
 if ( h -> slice_table [ top_xy ] != h -> slice_num ) h -> top_type = 0 ;
 if ( h -> slice_table [ left_xy [ LTOP ] ] != h -> slice_num ) h -> left_type [ LTOP ] = h -> left_type [ LBOT ] = 0 ;
 }
 else {
 if ( h -> slice_table [ topleft_xy ] != h -> slice_num ) {
 h -> topleft_type = 0 ;
 if ( h -> slice_table [ top_xy ] != h -> slice_num ) h -> top_type = 0 ;
 if ( h -> slice_table [ left_xy [ LTOP ] ] != h -> slice_num ) h -> left_type [ LTOP ] = h -> left_type [ LBOT ] = 0 ;
 }
 }
 if ( h -> slice_table [ topright_xy ] != h -> slice_num ) h -> topright_type = 0 ;
 }