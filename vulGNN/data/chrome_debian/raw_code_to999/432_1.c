static int decode_pic ( AVSContext * h ) {
 int skip_count = - 1 ;
 enum cavs_mb mb_type ;
 skip_bits ( & h -> gb , 16 ) ;
 if ( h -> stc == PIC_PB_START_CODE ) {
 h -> cur . f -> pict_type = get_bits ( & h -> gb , 2 ) + AV_PICTURE_TYPE_I ;
 if ( h -> cur . f -> pict_type > AV_PICTURE_TYPE_B ) {
 av_log ( h -> avctx , AV_LOG_ERROR , "illegal picture type\n" ) ;
 return - 1 ;
 }
 if ( ! h -> DPB [ 0 ] . f -> data [ 0 ] || ( ! h -> DPB [ 1 ] . f -> data [ 0 ] && h -> cur . f -> pict_type == AV_PICTURE_TYPE_B ) ) return - 1 ;
 }
 else {
 h -> cur . f -> pict_type = AV_PICTURE_TYPE_I ;
 if ( get_bits1 ( & h -> gb ) ) skip_bits ( & h -> gb , 24 ) ;
 if ( h -> low_delay || ! ( show_bits ( & h -> gb , 9 ) & 1 ) ) h -> stream_revision = 1 ;
 else if ( show_bits ( & h -> gb , 11 ) & 3 ) h -> stream_revision = 1 ;
 if ( h -> stream_revision > 0 ) skip_bits ( & h -> gb , 1 ) ;
 }
 if ( h -> cur . f -> data [ 0 ] ) h -> avctx -> release_buffer ( h -> avctx , h -> cur . f ) ;
 ff_get_buffer ( h -> avctx , h -> cur . f ) ;
 if ( ! h -> edge_emu_buffer ) {
 int alloc_size = FFALIGN ( FFABS ( h -> cur . f -> linesize [ 0 ] ) + 32 , 32 ) ;
 h -> edge_emu_buffer = av_mallocz ( alloc_size * 2 * 24 ) ;
 if ( ! h -> edge_emu_buffer ) return AVERROR ( ENOMEM ) ;
 }
 ff_cavs_init_pic ( h ) ;
 h -> cur . poc = get_bits ( & h -> gb , 8 ) * 2 ;
 if ( h -> cur . f -> pict_type != AV_PICTURE_TYPE_B ) {
 h -> dist [ 0 ] = ( h -> cur . poc - h -> DPB [ 0 ] . poc + 512 ) % 512 ;
 }
 else {
 h -> dist [ 0 ] = ( h -> DPB [ 0 ] . poc - h -> cur . poc + 512 ) % 512 ;
 }
 h -> dist [ 1 ] = ( h -> cur . poc - h -> DPB [ 1 ] . poc + 512 ) % 512 ;
 h -> scale_den [ 0 ] = h -> dist [ 0 ] ? 512 / h -> dist [ 0 ] : 0 ;
 h -> scale_den [ 1 ] = h -> dist [ 1 ] ? 512 / h -> dist [ 1 ] : 0 ;
 if ( h -> cur . f -> pict_type == AV_PICTURE_TYPE_B ) {
 h -> sym_factor = h -> dist [ 0 ] * h -> scale_den [ 1 ] ;
 }
 else {
 h -> direct_den [ 0 ] = h -> dist [ 0 ] ? 16384 / h -> dist [ 0 ] : 0 ;
 h -> direct_den [ 1 ] = h -> dist [ 1 ] ? 16384 / h -> dist [ 1 ] : 0 ;
 }
 if ( h -> low_delay ) get_ue_golomb ( & h -> gb ) ;
 h -> progressive = get_bits1 ( & h -> gb ) ;
 h -> pic_structure = 1 ;
 if ( ! h -> progressive ) h -> pic_structure = get_bits1 ( & h -> gb ) ;
 if ( ! h -> pic_structure && h -> stc == PIC_PB_START_CODE ) skip_bits1 ( & h -> gb ) ;
 skip_bits1 ( & h -> gb ) ;
 skip_bits1 ( & h -> gb ) ;
 h -> qp_fixed = get_bits1 ( & h -> gb ) ;
 h -> qp = get_bits ( & h -> gb , 6 ) ;
 if ( h -> cur . f -> pict_type == AV_PICTURE_TYPE_I ) {
 if ( ! h -> progressive && ! h -> pic_structure ) skip_bits1 ( & h -> gb ) ;
 skip_bits ( & h -> gb , 4 ) ;
 }
 else {
 if ( ! ( h -> cur . f -> pict_type == AV_PICTURE_TYPE_B && h -> pic_structure == 1 ) ) h -> ref_flag = get_bits1 ( & h -> gb ) ;
 skip_bits ( & h -> gb , 4 ) ;
 h -> skip_mode_flag = get_bits1 ( & h -> gb ) ;
 }
 h -> loop_filter_disable = get_bits1 ( & h -> gb ) ;
 if ( ! h -> loop_filter_disable && get_bits1 ( & h -> gb ) ) {
 h -> alpha_offset = get_se_golomb ( & h -> gb ) ;
 h -> beta_offset = get_se_golomb ( & h -> gb ) ;
 }
 else {
 h -> alpha_offset = h -> beta_offset = 0 ;
 }
 if ( h -> cur . f -> pict_type == AV_PICTURE_TYPE_I ) {
 do {
 check_for_slice ( h ) ;
 decode_mb_i ( h , 0 ) ;
 }
 while ( ff_cavs_next_mb ( h ) ) ;
 }
 else if ( h -> cur . f -> pict_type == AV_PICTURE_TYPE_P ) {
 do {
 if ( check_for_slice ( h ) ) skip_count = - 1 ;
 if ( h -> skip_mode_flag && ( skip_count < 0 ) ) skip_count = get_ue_golomb ( & h -> gb ) ;
 if ( h -> skip_mode_flag && skip_count -- ) {
 decode_mb_p ( h , P_SKIP ) ;
 }
 else {
 mb_type = get_ue_golomb ( & h -> gb ) + P_SKIP + h -> skip_mode_flag ;
 if ( mb_type > P_8X8 ) decode_mb_i ( h , mb_type - P_8X8 - 1 ) ;
 else decode_mb_p ( h , mb_type ) ;
 }
 }
 while ( ff_cavs_next_mb ( h ) ) ;
 }
 else {
 do {
 if ( check_for_slice ( h ) ) skip_count = - 1 ;
 if ( h -> skip_mode_flag && ( skip_count < 0 ) ) skip_count = get_ue_golomb ( & h -> gb ) ;
 if ( h -> skip_mode_flag && skip_count -- ) {
 decode_mb_b ( h , B_SKIP ) ;
 }
 else {
 mb_type = get_ue_golomb ( & h -> gb ) + B_SKIP + h -> skip_mode_flag ;
 if ( mb_type > B_8X8 ) decode_mb_i ( h , mb_type - B_8X8 - 1 ) ;
 else decode_mb_b ( h , mb_type ) ;
 }
 }
 while ( ff_cavs_next_mb ( h ) ) ;
 }
 if ( h -> cur . f -> pict_type != AV_PICTURE_TYPE_B ) {
 if ( h -> DPB [ 1 ] . f -> data [ 0 ] ) h -> avctx -> release_buffer ( h -> avctx , h -> DPB [ 1 ] . f ) ;
 FFSWAP ( AVSFrame , h -> cur , h -> DPB [ 1 ] ) ;
 FFSWAP ( AVSFrame , h -> DPB [ 0 ] , h -> DPB [ 1 ] ) ;
 }
 return 0 ;
 }