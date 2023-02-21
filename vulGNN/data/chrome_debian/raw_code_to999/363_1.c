static inline void vc1_pred_mv ( VC1Context * v , int n , int dmv_x , int dmv_y , int mv1 , int r_x , int r_y , uint8_t * is_intra , int pred_flag , int dir ) {
 MpegEncContext * s = & v -> s ;
 int xy , wrap , off = 0 ;
 int16_t * A , * B , * C ;
 int px , py ;
 int sum ;
 int mixedmv_pic , num_samefield = 0 , num_oppfield = 0 ;
 int opposite , a_f , b_f , c_f ;
 int16_t field_predA [ 2 ] ;
 int16_t field_predB [ 2 ] ;
 int16_t field_predC [ 2 ] ;
 int a_valid , b_valid , c_valid ;
 int hybridmv_thresh , y_bias = 0 ;
 if ( v -> mv_mode == MV_PMODE_MIXED_MV || ( ( v -> mv_mode == MV_PMODE_INTENSITY_COMP ) && ( v -> mv_mode2 == MV_PMODE_MIXED_MV ) ) ) mixedmv_pic = 1 ;
 else mixedmv_pic = 0 ;
 dmv_x <<= 1 - s -> quarter_sample ;
 dmv_y <<= 1 - s -> quarter_sample ;
 wrap = s -> b8_stride ;
 xy = s -> block_index [ n ] ;
 if ( s -> mb_intra ) {
 s -> mv [ 0 ] [ n ] [ 0 ] = s -> current_picture . f . motion_val [ 0 ] [ xy + v -> blocks_off ] [ 0 ] = 0 ;
 s -> mv [ 0 ] [ n ] [ 1 ] = s -> current_picture . f . motion_val [ 0 ] [ xy + v -> blocks_off ] [ 1 ] = 0 ;
 s -> current_picture . f . motion_val [ 1 ] [ xy + v -> blocks_off ] [ 0 ] = 0 ;
 s -> current_picture . f . motion_val [ 1 ] [ xy + v -> blocks_off ] [ 1 ] = 0 ;
 if ( mv1 ) {
 s -> current_picture . f . motion_val [ 0 ] [ xy + 1 + v -> blocks_off ] [ 0 ] = 0 ;
 s -> current_picture . f . motion_val [ 0 ] [ xy + 1 + v -> blocks_off ] [ 1 ] = 0 ;
 s -> current_picture . f . motion_val [ 0 ] [ xy + wrap + v -> blocks_off ] [ 0 ] = 0 ;
 s -> current_picture . f . motion_val [ 0 ] [ xy + wrap + v -> blocks_off ] [ 1 ] = 0 ;
 s -> current_picture . f . motion_val [ 0 ] [ xy + wrap + 1 + v -> blocks_off ] [ 0 ] = 0 ;
 s -> current_picture . f . motion_val [ 0 ] [ xy + wrap + 1 + v -> blocks_off ] [ 1 ] = 0 ;
 v -> luma_mv [ s -> mb_x ] [ 0 ] = v -> luma_mv [ s -> mb_x ] [ 1 ] = 0 ;
 s -> current_picture . f . motion_val [ 1 ] [ xy + 1 + v -> blocks_off ] [ 0 ] = 0 ;
 s -> current_picture . f . motion_val [ 1 ] [ xy + 1 + v -> blocks_off ] [ 1 ] = 0 ;
 s -> current_picture . f . motion_val [ 1 ] [ xy + wrap ] [ 0 ] = 0 ;
 s -> current_picture . f . motion_val [ 1 ] [ xy + wrap + v -> blocks_off ] [ 1 ] = 0 ;
 s -> current_picture . f . motion_val [ 1 ] [ xy + wrap + 1 + v -> blocks_off ] [ 0 ] = 0 ;
 s -> current_picture . f . motion_val [ 1 ] [ xy + wrap + 1 + v -> blocks_off ] [ 1 ] = 0 ;
 }
 return ;
 }
 C = s -> current_picture . f . motion_val [ dir ] [ xy - 1 + v -> blocks_off ] ;
 A = s -> current_picture . f . motion_val [ dir ] [ xy - wrap + v -> blocks_off ] ;
 if ( mv1 ) {
 if ( v -> field_mode && mixedmv_pic ) off = ( s -> mb_x == ( s -> mb_width - 1 ) ) ? - 2 : 2 ;
 else off = ( s -> mb_x == ( s -> mb_width - 1 ) ) ? - 1 : 2 ;
 }
 else {
 switch ( n ) {
 case 0 : off = ( s -> mb_x > 0 ) ? - 1 : 1 ;
 break ;
 case 1 : off = ( s -> mb_x == ( s -> mb_width - 1 ) ) ? - 1 : 1 ;
 break ;
 case 2 : off = 1 ;
 break ;
 case 3 : off = - 1 ;
 }
 }
 B = s -> current_picture . f . motion_val [ dir ] [ xy - wrap + off + v -> blocks_off ] ;
 a_valid = ! s -> first_slice_line || ( n == 2 || n == 3 ) ;
 b_valid = a_valid && ( s -> mb_width > 1 ) ;
 c_valid = s -> mb_x || ( n == 1 || n == 3 ) ;
 if ( v -> field_mode ) {
 a_valid = a_valid && ! is_intra [ xy - wrap ] ;
 b_valid = b_valid && ! is_intra [ xy - wrap + off ] ;
 c_valid = c_valid && ! is_intra [ xy - 1 ] ;
 }
 if ( a_valid ) {
 a_f = v -> mv_f [ dir ] [ xy - wrap + v -> blocks_off ] ;
 num_oppfield += a_f ;
 num_samefield += 1 - a_f ;
 field_predA [ 0 ] = A [ 0 ] ;
 field_predA [ 1 ] = A [ 1 ] ;
 }
 else {
 field_predA [ 0 ] = field_predA [ 1 ] = 0 ;
 a_f = 0 ;
 }
 if ( b_valid ) {
 b_f = v -> mv_f [ dir ] [ xy - wrap + off + v -> blocks_off ] ;
 num_oppfield += b_f ;
 num_samefield += 1 - b_f ;
 field_predB [ 0 ] = B [ 0 ] ;
 field_predB [ 1 ] = B [ 1 ] ;
 }
 else {
 field_predB [ 0 ] = field_predB [ 1 ] = 0 ;
 b_f = 0 ;
 }
 if ( c_valid ) {
 c_f = v -> mv_f [ dir ] [ xy - 1 + v -> blocks_off ] ;
 num_oppfield += c_f ;
 num_samefield += 1 - c_f ;
 field_predC [ 0 ] = C [ 0 ] ;
 field_predC [ 1 ] = C [ 1 ] ;
 }
 else {
 field_predC [ 0 ] = field_predC [ 1 ] = 0 ;
 c_f = 0 ;
 }
 if ( v -> field_mode ) {
 if ( ! v -> numref ) opposite = 1 - v -> reffield ;
 else {
 if ( num_samefield <= num_oppfield ) opposite = 1 - pred_flag ;
 else opposite = pred_flag ;
 }
 }
 else opposite = 0 ;
 if ( opposite ) {
 if ( a_valid && ! a_f ) {
 field_predA [ 0 ] = scaleforopp ( v , field_predA [ 0 ] , 0 , dir ) ;
 field_predA [ 1 ] = scaleforopp ( v , field_predA [ 1 ] , 1 , dir ) ;
 }
 if ( b_valid && ! b_f ) {
 field_predB [ 0 ] = scaleforopp ( v , field_predB [ 0 ] , 0 , dir ) ;
 field_predB [ 1 ] = scaleforopp ( v , field_predB [ 1 ] , 1 , dir ) ;
 }
 if ( c_valid && ! c_f ) {
 field_predC [ 0 ] = scaleforopp ( v , field_predC [ 0 ] , 0 , dir ) ;
 field_predC [ 1 ] = scaleforopp ( v , field_predC [ 1 ] , 1 , dir ) ;
 }
 v -> mv_f [ dir ] [ xy + v -> blocks_off ] = 1 ;
 v -> ref_field_type [ dir ] = ! v -> cur_field_type ;
 }
 else {
 if ( a_valid && a_f ) {
 field_predA [ 0 ] = scaleforsame ( v , n , field_predA [ 0 ] , 0 , dir ) ;
 field_predA [ 1 ] = scaleforsame ( v , n , field_predA [ 1 ] , 1 , dir ) ;
 }
 if ( b_valid && b_f ) {
 field_predB [ 0 ] = scaleforsame ( v , n , field_predB [ 0 ] , 0 , dir ) ;
 field_predB [ 1 ] = scaleforsame ( v , n , field_predB [ 1 ] , 1 , dir ) ;
 }
 if ( c_valid && c_f ) {
 field_predC [ 0 ] = scaleforsame ( v , n , field_predC [ 0 ] , 0 , dir ) ;
 field_predC [ 1 ] = scaleforsame ( v , n , field_predC [ 1 ] , 1 , dir ) ;
 }
 v -> mv_f [ dir ] [ xy + v -> blocks_off ] = 0 ;
 v -> ref_field_type [ dir ] = v -> cur_field_type ;
 }
 if ( a_valid ) {
 px = field_predA [ 0 ] ;
 py = field_predA [ 1 ] ;
 }
 else if ( c_valid ) {
 px = field_predC [ 0 ] ;
 py = field_predC [ 1 ] ;
 }
 else if ( b_valid ) {
 px = field_predB [ 0 ] ;
 py = field_predB [ 1 ] ;
 }
 else {
 px = 0 ;
 py = 0 ;
 }
 if ( num_samefield + num_oppfield > 1 ) {
 px = mid_pred ( field_predA [ 0 ] , field_predB [ 0 ] , field_predC [ 0 ] ) ;
 py = mid_pred ( field_predA [ 1 ] , field_predB [ 1 ] , field_predC [ 1 ] ) ;
 }
 if ( ! v -> field_mode ) {
 int qx , qy , X , Y ;
 qx = ( s -> mb_x << 6 ) + ( ( n == 1 || n == 3 ) ? 32 : 0 ) ;
 qy = ( s -> mb_y << 6 ) + ( ( n == 2 || n == 3 ) ? 32 : 0 ) ;
 X = ( s -> mb_width << 6 ) - 4 ;
 Y = ( s -> mb_height << 6 ) - 4 ;
 if ( mv1 ) {
 if ( qx + px < - 60 ) px = - 60 - qx ;
 if ( qy + py < - 60 ) py = - 60 - qy ;
 }
 else {
 if ( qx + px < - 28 ) px = - 28 - qx ;
 if ( qy + py < - 28 ) py = - 28 - qy ;
 }
 if ( qx + px > X ) px = X - qx ;
 if ( qy + py > Y ) py = Y - qy ;
 }
 if ( ! v -> field_mode || s -> pict_type != AV_PICTURE_TYPE_B ) {
 hybridmv_thresh = 32 ;
 if ( a_valid && c_valid ) {
 if ( is_intra [ xy - wrap ] ) sum = FFABS ( px ) + FFABS ( py ) ;
 else sum = FFABS ( px - field_predA [ 0 ] ) + FFABS ( py - field_predA [ 1 ] ) ;
 if ( sum > hybridmv_thresh ) {
 if ( get_bits1 ( & s -> gb ) ) {
 px = field_predA [ 0 ] ;
 py = field_predA [ 1 ] ;
 }
 else {
 px = field_predC [ 0 ] ;
 py = field_predC [ 1 ] ;
 }
 }
 else {
 if ( is_intra [ xy - 1 ] ) sum = FFABS ( px ) + FFABS ( py ) ;
 else sum = FFABS ( px - field_predC [ 0 ] ) + FFABS ( py - field_predC [ 1 ] ) ;
 if ( sum > hybridmv_thresh ) {
 if ( get_bits1 ( & s -> gb ) ) {
 px = field_predA [ 0 ] ;
 py = field_predA [ 1 ] ;
 }
 else {
 px = field_predC [ 0 ] ;
 py = field_predC [ 1 ] ;
 }
 }
 }
 }
 }
 if ( v -> field_mode && v -> numref ) r_y >>= 1 ;
 if ( v -> field_mode && v -> cur_field_type && v -> ref_field_type [ dir ] == 0 ) y_bias = 1 ;
 s -> mv [ dir ] [ n ] [ 0 ] = s -> current_picture . f . motion_val [ dir ] [ xy + v -> blocks_off ] [ 0 ] = ( ( px + dmv_x + r_x ) & ( ( r_x << 1 ) - 1 ) ) - r_x ;
 s -> mv [ dir ] [ n ] [ 1 ] = s -> current_picture . f . motion_val [ dir ] [ xy + v -> blocks_off ] [ 1 ] = ( ( py + dmv_y + r_y - y_bias ) & ( ( r_y << 1 ) - 1 ) ) - r_y + y_bias ;
 if ( mv1 ) {
 s -> current_picture . f . motion_val [ dir ] [ xy + 1 + v -> blocks_off ] [ 0 ] = s -> current_picture . f . motion_val [ dir ] [ xy + v -> blocks_off ] [ 0 ] ;
 s -> current_picture . f . motion_val [ dir ] [ xy + 1 + v -> blocks_off ] [ 1 ] = s -> current_picture . f . motion_val [ dir ] [ xy + v -> blocks_off ] [ 1 ] ;
 s -> current_picture . f . motion_val [ dir ] [ xy + wrap + v -> blocks_off ] [ 0 ] = s -> current_picture . f . motion_val [ dir ] [ xy + v -> blocks_off ] [ 0 ] ;
 s -> current_picture . f . motion_val [ dir ] [ xy + wrap + v -> blocks_off ] [ 1 ] = s -> current_picture . f . motion_val [ dir ] [ xy + v -> blocks_off ] [ 1 ] ;
 s -> current_picture . f . motion_val [ dir ] [ xy + wrap + 1 + v -> blocks_off ] [ 0 ] = s -> current_picture . f . motion_val [ dir ] [ xy + v -> blocks_off ] [ 0 ] ;
 s -> current_picture . f . motion_val [ dir ] [ xy + wrap + 1 + v -> blocks_off ] [ 1 ] = s -> current_picture . f . motion_val [ dir ] [ xy + v -> blocks_off ] [ 1 ] ;
 v -> mv_f [ dir ] [ xy + 1 + v -> blocks_off ] = v -> mv_f [ dir ] [ xy + v -> blocks_off ] ;
 v -> mv_f [ dir ] [ xy + wrap + v -> blocks_off ] = v -> mv_f [ dir ] [ xy + wrap + 1 + v -> blocks_off ] = v -> mv_f [ dir ] [ xy + v -> blocks_off ] ;
 }
 }