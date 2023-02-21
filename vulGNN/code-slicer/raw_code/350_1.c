static inline void vc1_pred_b_mv_intfi ( VC1Context * v , int n , int * dmv_x , int * dmv_y , int mv1 , int * pred_flag ) {
 int dir = ( v -> bmvtype == BMV_TYPE_BACKWARD ) ? 1 : 0 ;
 MpegEncContext * s = & v -> s ;
 int mb_pos = s -> mb_x + s -> mb_y * s -> mb_stride ;
 if ( v -> bmvtype == BMV_TYPE_DIRECT ) {
 int total_opp , k , f ;
 if ( s -> next_picture . f . mb_type [ mb_pos + v -> mb_off ] != MB_TYPE_INTRA ) {
 s -> mv [ 0 ] [ 0 ] [ 0 ] = scale_mv ( s -> next_picture . f . motion_val [ 1 ] [ s -> block_index [ 0 ] + v -> blocks_off ] [ 0 ] , v -> bfraction , 0 , s -> quarter_sample ) ;
 s -> mv [ 0 ] [ 0 ] [ 1 ] = scale_mv ( s -> next_picture . f . motion_val [ 1 ] [ s -> block_index [ 0 ] + v -> blocks_off ] [ 1 ] , v -> bfraction , 0 , s -> quarter_sample ) ;
 s -> mv [ 1 ] [ 0 ] [ 0 ] = scale_mv ( s -> next_picture . f . motion_val [ 1 ] [ s -> block_index [ 0 ] + v -> blocks_off ] [ 0 ] , v -> bfraction , 1 , s -> quarter_sample ) ;
 s -> mv [ 1 ] [ 0 ] [ 1 ] = scale_mv ( s -> next_picture . f . motion_val [ 1 ] [ s -> block_index [ 0 ] + v -> blocks_off ] [ 1 ] , v -> bfraction , 1 , s -> quarter_sample ) ;
 total_opp = v -> mv_f_next [ 0 ] [ s -> block_index [ 0 ] + v -> blocks_off ] + v -> mv_f_next [ 0 ] [ s -> block_index [ 1 ] + v -> blocks_off ] + v -> mv_f_next [ 0 ] [ s -> block_index [ 2 ] + v -> blocks_off ] + v -> mv_f_next [ 0 ] [ s -> block_index [ 3 ] + v -> blocks_off ] ;
 f = ( total_opp > 2 ) ? 1 : 0 ;
 }
 else {
 s -> mv [ 0 ] [ 0 ] [ 0 ] = s -> mv [ 0 ] [ 0 ] [ 1 ] = 0 ;
 s -> mv [ 1 ] [ 0 ] [ 0 ] = s -> mv [ 1 ] [ 0 ] [ 1 ] = 0 ;
 f = 0 ;
 }
 v -> ref_field_type [ 0 ] = v -> ref_field_type [ 1 ] = v -> cur_field_type ^ f ;
 for ( k = 0 ;
 k < 4 ;
 k ++ ) {
 s -> current_picture . f . motion_val [ 0 ] [ s -> block_index [ k ] + v -> blocks_off ] [ 0 ] = s -> mv [ 0 ] [ 0 ] [ 0 ] ;
 s -> current_picture . f . motion_val [ 0 ] [ s -> block_index [ k ] + v -> blocks_off ] [ 1 ] = s -> mv [ 0 ] [ 0 ] [ 1 ] ;
 s -> current_picture . f . motion_val [ 1 ] [ s -> block_index [ k ] + v -> blocks_off ] [ 0 ] = s -> mv [ 1 ] [ 0 ] [ 0 ] ;
 s -> current_picture . f . motion_val [ 1 ] [ s -> block_index [ k ] + v -> blocks_off ] [ 1 ] = s -> mv [ 1 ] [ 0 ] [ 1 ] ;
 v -> mv_f [ 0 ] [ s -> block_index [ k ] + v -> blocks_off ] = f ;
 v -> mv_f [ 1 ] [ s -> block_index [ k ] + v -> blocks_off ] = f ;
 }
 return ;
 }
 if ( v -> bmvtype == BMV_TYPE_INTERPOLATED ) {
 vc1_pred_mv ( v , 0 , dmv_x [ 0 ] , dmv_y [ 0 ] , 1 , v -> range_x , v -> range_y , v -> mb_type [ 0 ] , pred_flag [ 0 ] , 0 ) ;
 vc1_pred_mv ( v , 0 , dmv_x [ 1 ] , dmv_y [ 1 ] , 1 , v -> range_x , v -> range_y , v -> mb_type [ 0 ] , pred_flag [ 1 ] , 1 ) ;
 return ;
 }
 if ( dir ) {
 vc1_pred_mv ( v , n , dmv_x [ 1 ] , dmv_y [ 1 ] , mv1 , v -> range_x , v -> range_y , v -> mb_type [ 0 ] , pred_flag [ 1 ] , 1 ) ;
 if ( n == 3 || mv1 ) {
 vc1_pred_mv ( v , 0 , dmv_x [ 0 ] , dmv_y [ 0 ] , 1 , v -> range_x , v -> range_y , v -> mb_type [ 0 ] , 0 , 0 ) ;
 }
 }
 else {
 vc1_pred_mv ( v , n , dmv_x [ 0 ] , dmv_y [ 0 ] , mv1 , v -> range_x , v -> range_y , v -> mb_type [ 0 ] , pred_flag [ 0 ] , 0 ) ;
 if ( n == 3 || mv1 ) {
 vc1_pred_mv ( v , 0 , dmv_x [ 1 ] , dmv_y [ 1 ] , 1 , v -> range_x , v -> range_y , v -> mb_type [ 0 ] , 0 , 1 ) ;
 }
 }
 }