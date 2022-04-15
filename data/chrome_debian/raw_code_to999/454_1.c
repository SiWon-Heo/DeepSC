static void fill_decode_caches ( H264Context * h , int mb_type ) {
 int topleft_xy , top_xy , topright_xy , left_xy [ LEFT_MBS ] ;
 int topleft_type , top_type , topright_type , left_type [ LEFT_MBS ] ;
 const uint8_t * left_block = h -> left_block ;
 int i ;
 uint8_t * nnz ;
 uint8_t * nnz_cache ;
 topleft_xy = h -> topleft_mb_xy ;
 top_xy = h -> top_mb_xy ;
 topright_xy = h -> topright_mb_xy ;
 left_xy [ LTOP ] = h -> left_mb_xy [ LTOP ] ;
 left_xy [ LBOT ] = h -> left_mb_xy [ LBOT ] ;
 topleft_type = h -> topleft_type ;
 top_type = h -> top_type ;
 topright_type = h -> topright_type ;
 left_type [ LTOP ] = h -> left_type [ LTOP ] ;
 left_type [ LBOT ] = h -> left_type [ LBOT ] ;
 if ( ! IS_SKIP ( mb_type ) ) {
 if ( IS_INTRA ( mb_type ) ) {
 int type_mask = h -> pps . constrained_intra_pred ? IS_INTRA ( - 1 ) : - 1 ;
 h -> topleft_samples_available = h -> top_samples_available = h -> left_samples_available = 0xFFFF ;
 h -> topright_samples_available = 0xEEEA ;
 if ( ! ( top_type & type_mask ) ) {
 h -> topleft_samples_available = 0xB3FF ;
 h -> top_samples_available = 0x33FF ;
 h -> topright_samples_available = 0x26EA ;
 }
 if ( IS_INTERLACED ( mb_type ) != IS_INTERLACED ( left_type [ LTOP ] ) ) {
 if ( IS_INTERLACED ( mb_type ) ) {
 if ( ! ( left_type [ LTOP ] & type_mask ) ) {
 h -> topleft_samples_available &= 0xDFFF ;
 h -> left_samples_available &= 0x5FFF ;
 }
 if ( ! ( left_type [ LBOT ] & type_mask ) ) {
 h -> topleft_samples_available &= 0xFF5F ;
 h -> left_samples_available &= 0xFF5F ;
 }
 }
 else {
 int left_typei = h -> cur_pic . f . mb_type [ left_xy [ LTOP ] + h -> mb_stride ] ;
 assert ( left_xy [ LTOP ] == left_xy [ LBOT ] ) ;
 if ( ! ( ( left_typei & type_mask ) && ( left_type [ LTOP ] & type_mask ) ) ) {
 h -> topleft_samples_available &= 0xDF5F ;
 h -> left_samples_available &= 0x5F5F ;
 }
 }
 }
 else {
 if ( ! ( left_type [ LTOP ] & type_mask ) ) {
 h -> topleft_samples_available &= 0xDF5F ;
 h -> left_samples_available &= 0x5F5F ;
 }
 }
 if ( ! ( topleft_type & type_mask ) ) h -> topleft_samples_available &= 0x7FFF ;
 if ( ! ( topright_type & type_mask ) ) h -> topright_samples_available &= 0xFBFF ;
 if ( IS_INTRA4x4 ( mb_type ) ) {
 if ( IS_INTRA4x4 ( top_type ) ) {
 AV_COPY32 ( h -> intra4x4_pred_mode_cache + 4 + 8 * 0 , h -> intra4x4_pred_mode + h -> mb2br_xy [ top_xy ] ) ;
 }
 else {
 h -> intra4x4_pred_mode_cache [ 4 + 8 * 0 ] = h -> intra4x4_pred_mode_cache [ 5 + 8 * 0 ] = h -> intra4x4_pred_mode_cache [ 6 + 8 * 0 ] = h -> intra4x4_pred_mode_cache [ 7 + 8 * 0 ] = 2 - 3 * ! ( top_type & type_mask ) ;
 }
 for ( i = 0 ;
 i < 2 ;
 i ++ ) {
 if ( IS_INTRA4x4 ( left_type [ LEFT ( i ) ] ) ) {
 int8_t * mode = h -> intra4x4_pred_mode + h -> mb2br_xy [ left_xy [ LEFT ( i ) ] ] ;
 h -> intra4x4_pred_mode_cache [ 3 + 8 * 1 + 2 * 8 * i ] = mode [ 6 - left_block [ 0 + 2 * i ] ] ;
 h -> intra4x4_pred_mode_cache [ 3 + 8 * 2 + 2 * 8 * i ] = mode [ 6 - left_block [ 1 + 2 * i ] ] ;
 }
 else {
 h -> intra4x4_pred_mode_cache [ 3 + 8 * 1 + 2 * 8 * i ] = h -> intra4x4_pred_mode_cache [ 3 + 8 * 2 + 2 * 8 * i ] = 2 - 3 * ! ( left_type [ LEFT ( i ) ] & type_mask ) ;
 }
 }
 }
 }
 nnz_cache = h -> non_zero_count_cache ;
 if ( top_type ) {
 nnz = h -> non_zero_count [ top_xy ] ;
 AV_COPY32 ( & nnz_cache [ 4 + 8 * 0 ] , & nnz [ 4 * 3 ] ) ;
 if ( ! h -> chroma_y_shift ) {
 AV_COPY32 ( & nnz_cache [ 4 + 8 * 5 ] , & nnz [ 4 * 7 ] ) ;
 AV_COPY32 ( & nnz_cache [ 4 + 8 * 10 ] , & nnz [ 4 * 11 ] ) ;
 }
 else {
 AV_COPY32 ( & nnz_cache [ 4 + 8 * 5 ] , & nnz [ 4 * 5 ] ) ;
 AV_COPY32 ( & nnz_cache [ 4 + 8 * 10 ] , & nnz [ 4 * 9 ] ) ;
 }
 }
 else {
 uint32_t top_empty = CABAC && ! IS_INTRA ( mb_type ) ? 0 : 0x40404040 ;
 AV_WN32A ( & nnz_cache [ 4 + 8 * 0 ] , top_empty ) ;
 AV_WN32A ( & nnz_cache [ 4 + 8 * 5 ] , top_empty ) ;
 AV_WN32A ( & nnz_cache [ 4 + 8 * 10 ] , top_empty ) ;
 }
 for ( i = 0 ;
 i < 2 ;
 i ++ ) {
 if ( left_type [ LEFT ( i ) ] ) {
 nnz = h -> non_zero_count [ left_xy [ LEFT ( i ) ] ] ;
 nnz_cache [ 3 + 8 * 1 + 2 * 8 * i ] = nnz [ left_block [ 8 + 0 + 2 * i ] ] ;
 nnz_cache [ 3 + 8 * 2 + 2 * 8 * i ] = nnz [ left_block [ 8 + 1 + 2 * i ] ] ;
 if ( CHROMA444 ) {
 nnz_cache [ 3 + 8 * 6 + 2 * 8 * i ] = nnz [ left_block [ 8 + 0 + 2 * i ] + 4 * 4 ] ;
 nnz_cache [ 3 + 8 * 7 + 2 * 8 * i ] = nnz [ left_block [ 8 + 1 + 2 * i ] + 4 * 4 ] ;
 nnz_cache [ 3 + 8 * 11 + 2 * 8 * i ] = nnz [ left_block [ 8 + 0 + 2 * i ] + 8 * 4 ] ;
 nnz_cache [ 3 + 8 * 12 + 2 * 8 * i ] = nnz [ left_block [ 8 + 1 + 2 * i ] + 8 * 4 ] ;
 }
 else if ( CHROMA422 ) {
 nnz_cache [ 3 + 8 * 6 + 2 * 8 * i ] = nnz [ left_block [ 8 + 0 + 2 * i ] - 2 + 4 * 4 ] ;
 nnz_cache [ 3 + 8 * 7 + 2 * 8 * i ] = nnz [ left_block [ 8 + 1 + 2 * i ] - 2 + 4 * 4 ] ;
 nnz_cache [ 3 + 8 * 11 + 2 * 8 * i ] = nnz [ left_block [ 8 + 0 + 2 * i ] - 2 + 8 * 4 ] ;
 nnz_cache [ 3 + 8 * 12 + 2 * 8 * i ] = nnz [ left_block [ 8 + 1 + 2 * i ] - 2 + 8 * 4 ] ;
 }
 else {
 nnz_cache [ 3 + 8 * 6 + 8 * i ] = nnz [ left_block [ 8 + 4 + 2 * i ] ] ;
 nnz_cache [ 3 + 8 * 11 + 8 * i ] = nnz [ left_block [ 8 + 5 + 2 * i ] ] ;
 }
 }
 else {
 nnz_cache [ 3 + 8 * 1 + 2 * 8 * i ] = nnz_cache [ 3 + 8 * 2 + 2 * 8 * i ] = nnz_cache [ 3 + 8 * 6 + 2 * 8 * i ] = nnz_cache [ 3 + 8 * 7 + 2 * 8 * i ] = nnz_cache [ 3 + 8 * 11 + 2 * 8 * i ] = nnz_cache [ 3 + 8 * 12 + 2 * 8 * i ] = CABAC && ! IS_INTRA ( mb_type ) ? 0 : 64 ;
 }
 }
 if ( CABAC ) {
 if ( top_type ) h -> top_cbp = h -> cbp_table [ top_xy ] ;
 else h -> top_cbp = IS_INTRA ( mb_type ) ? 0x7CF : 0x00F ;
 if ( left_type [ LTOP ] ) {
 h -> left_cbp = ( h -> cbp_table [ left_xy [ LTOP ] ] & 0x7F0 ) | ( ( h -> cbp_table [ left_xy [ LTOP ] ] >> ( left_block [ 0 ] & ( ~ 1 ) ) ) & 2 ) | ( ( ( h -> cbp_table [ left_xy [ LBOT ] ] >> ( left_block [ 2 ] & ( ~ 1 ) ) ) & 2 ) << 2 ) ;
 }
 else {
 h -> left_cbp = IS_INTRA ( mb_type ) ? 0x7CF : 0x00F ;
 }
 }
 }
 if ( IS_INTER ( mb_type ) || ( IS_DIRECT ( mb_type ) && h -> direct_spatial_mv_pred ) ) {
 int list ;
 int b_stride = h -> b_stride ;
 for ( list = 0 ;
 list < h -> list_count ;
 list ++ ) {
 int8_t * ref_cache = & h -> ref_cache [ list ] [ scan8 [ 0 ] ] ;
 int8_t * ref = h -> cur_pic . f . ref_index [ list ] ;
 int16_t ( * mv_cache ) [ 2 ] = & h -> mv_cache [ list ] [ scan8 [ 0 ] ] ;
 int16_t ( * mv ) [ 2 ] = h -> cur_pic . f . motion_val [ list ] ;
 if ( ! USES_LIST ( mb_type , list ) ) continue ;
 assert ( ! ( IS_DIRECT ( mb_type ) && ! h -> direct_spatial_mv_pred ) ) ;
 if ( USES_LIST ( top_type , list ) ) {
 const int b_xy = h -> mb2b_xy [ top_xy ] + 3 * b_stride ;
 AV_COPY128 ( mv_cache [ 0 - 1 * 8 ] , mv [ b_xy + 0 ] ) ;
 ref_cache [ 0 - 1 * 8 ] = ref_cache [ 1 - 1 * 8 ] = ref [ 4 * top_xy + 2 ] ;
 ref_cache [ 2 - 1 * 8 ] = ref_cache [ 3 - 1 * 8 ] = ref [ 4 * top_xy + 3 ] ;
 }
 else {
 AV_ZERO128 ( mv_cache [ 0 - 1 * 8 ] ) ;
 AV_WN32A ( & ref_cache [ 0 - 1 * 8 ] , ( ( top_type ? LIST_NOT_USED : PART_NOT_AVAILABLE ) & 0xFF ) * 0x01010101u ) ;
 }
 if ( mb_type & ( MB_TYPE_16x8 | MB_TYPE_8x8 ) ) {
 for ( i = 0 ;
 i < 2 ;
 i ++ ) {
 int cache_idx = - 1 + i * 2 * 8 ;
 if ( USES_LIST ( left_type [ LEFT ( i ) ] , list ) ) {
 const int b_xy = h -> mb2b_xy [ left_xy [ LEFT ( i ) ] ] + 3 ;
 const int b8_xy = 4 * left_xy [ LEFT ( i ) ] + 1 ;
 AV_COPY32 ( mv_cache [ cache_idx ] , mv [ b_xy + b_stride * left_block [ 0 + i * 2 ] ] ) ;
 AV_COPY32 ( mv_cache [ cache_idx + 8 ] , mv [ b_xy + b_stride * left_block [ 1 + i * 2 ] ] ) ;
 ref_cache [ cache_idx ] = ref [ b8_xy + ( left_block [ 0 + i * 2 ] & ~ 1 ) ] ;
 ref_cache [ cache_idx + 8 ] = ref [ b8_xy + ( left_block [ 1 + i * 2 ] & ~ 1 ) ] ;
 }
 else {
 AV_ZERO32 ( mv_cache [ cache_idx ] ) ;
 AV_ZERO32 ( mv_cache [ cache_idx + 8 ] ) ;
 ref_cache [ cache_idx ] = ref_cache [ cache_idx + 8 ] = ( left_type [ LEFT ( i ) ] ) ? LIST_NOT_USED : PART_NOT_AVAILABLE ;
 }
 }
 }
 else {
 if ( USES_LIST ( left_type [ LTOP ] , list ) ) {
 const int b_xy = h -> mb2b_xy [ left_xy [ LTOP ] ] + 3 ;
 const int b8_xy = 4 * left_xy [ LTOP ] + 1 ;
 AV_COPY32 ( mv_cache [ - 1 ] , mv [ b_xy + b_stride * left_block [ 0 ] ] ) ;
 ref_cache [ - 1 ] = ref [ b8_xy + ( left_block [ 0 ] & ~ 1 ) ] ;
 }
 else {
 AV_ZERO32 ( mv_cache [ - 1 ] ) ;
 ref_cache [ - 1 ] = left_type [ LTOP ] ? LIST_NOT_USED : PART_NOT_AVAILABLE ;
 }
 }
 if ( USES_LIST ( topright_type , list ) ) {
 const int b_xy = h -> mb2b_xy [ topright_xy ] + 3 * b_stride ;
 AV_COPY32 ( mv_cache [ 4 - 1 * 8 ] , mv [ b_xy ] ) ;
 ref_cache [ 4 - 1 * 8 ] = ref [ 4 * topright_xy + 2 ] ;
 }
 else {
 AV_ZERO32 ( mv_cache [ 4 - 1 * 8 ] ) ;
 ref_cache [ 4 - 1 * 8 ] = topright_type ? LIST_NOT_USED : PART_NOT_AVAILABLE ;
 }
 if ( ref_cache [ 4 - 1 * 8 ] < 0 ) {
 if ( USES_LIST ( topleft_type , list ) ) {
 const int b_xy = h -> mb2b_xy [ topleft_xy ] + 3 + b_stride + ( h -> topleft_partition & 2 * b_stride ) ;
 const int b8_xy = 4 * topleft_xy + 1 + ( h -> topleft_partition & 2 ) ;
 AV_COPY32 ( mv_cache [ - 1 - 1 * 8 ] , mv [ b_xy ] ) ;
 ref_cache [ - 1 - 1 * 8 ] = ref [ b8_xy ] ;
 }
 else {
 AV_ZERO32 ( mv_cache [ - 1 - 1 * 8 ] ) ;
 ref_cache [ - 1 - 1 * 8 ] = topleft_type ? LIST_NOT_USED : PART_NOT_AVAILABLE ;
 }
 }
 if ( ( mb_type & ( MB_TYPE_SKIP | MB_TYPE_DIRECT2 ) ) && ! FRAME_MBAFF ) continue ;
 if ( ! ( mb_type & ( MB_TYPE_SKIP | MB_TYPE_DIRECT2 ) ) ) {
 uint8_t ( * mvd_cache ) [ 2 ] = & h -> mvd_cache [ list ] [ scan8 [ 0 ] ] ;
 uint8_t ( * mvd ) [ 2 ] = h -> mvd_table [ list ] ;
 ref_cache [ 2 + 8 * 0 ] = ref_cache [ 2 + 8 * 2 ] = PART_NOT_AVAILABLE ;
 AV_ZERO32 ( mv_cache [ 2 + 8 * 0 ] ) ;
 AV_ZERO32 ( mv_cache [ 2 + 8 * 2 ] ) ;
 if ( CABAC ) {
 if ( USES_LIST ( top_type , list ) ) {
 const int b_xy = h -> mb2br_xy [ top_xy ] ;
 AV_COPY64 ( mvd_cache [ 0 - 1 * 8 ] , mvd [ b_xy + 0 ] ) ;
 }
 else {
 AV_ZERO64 ( mvd_cache [ 0 - 1 * 8 ] ) ;
 }
 if ( USES_LIST ( left_type [ LTOP ] , list ) ) {
 const int b_xy = h -> mb2br_xy [ left_xy [ LTOP ] ] + 6 ;
 AV_COPY16 ( mvd_cache [ - 1 + 0 * 8 ] , mvd [ b_xy - left_block [ 0 ] ] ) ;
 AV_COPY16 ( mvd_cache [ - 1 + 1 * 8 ] , mvd [ b_xy - left_block [ 1 ] ] ) ;
 }
 else {
 AV_ZERO16 ( mvd_cache [ - 1 + 0 * 8 ] ) ;
 AV_ZERO16 ( mvd_cache [ - 1 + 1 * 8 ] ) ;
 }
 if ( USES_LIST ( left_type [ LBOT ] , list ) ) {
 const int b_xy = h -> mb2br_xy [ left_xy [ LBOT ] ] + 6 ;
 AV_COPY16 ( mvd_cache [ - 1 + 2 * 8 ] , mvd [ b_xy - left_block [ 2 ] ] ) ;
 AV_COPY16 ( mvd_cache [ - 1 + 3 * 8 ] , mvd [ b_xy - left_block [ 3 ] ] ) ;
 }
 else {
 AV_ZERO16 ( mvd_cache [ - 1 + 2 * 8 ] ) ;
 AV_ZERO16 ( mvd_cache [ - 1 + 3 * 8 ] ) ;
 }
 AV_ZERO16 ( mvd_cache [ 2 + 8 * 0 ] ) ;
 AV_ZERO16 ( mvd_cache [ 2 + 8 * 2 ] ) ;
 if ( h -> slice_type_nos == AV_PICTURE_TYPE_B ) {
 uint8_t * direct_cache = & h -> direct_cache [ scan8 [ 0 ] ] ;
 uint8_t * direct_table = h -> direct_table ;
 fill_rectangle ( direct_cache , 4 , 4 , 8 , MB_TYPE_16x16 >> 1 , 1 ) ;
 if ( IS_DIRECT ( top_type ) ) {
 AV_WN32A ( & direct_cache [ - 1 * 8 ] , 0x01010101u * ( MB_TYPE_DIRECT2 >> 1 ) ) ;
 }
 else if ( IS_8X8 ( top_type ) ) {
 int b8_xy = 4 * top_xy ;
 direct_cache [ 0 - 1 * 8 ] = direct_table [ b8_xy + 2 ] ;
 direct_cache [ 2 - 1 * 8 ] = direct_table [ b8_xy + 3 ] ;
 }
 else {
 AV_WN32A ( & direct_cache [ - 1 * 8 ] , 0x01010101 * ( MB_TYPE_16x16 >> 1 ) ) ;
 }
 if ( IS_DIRECT ( left_type [ LTOP ] ) ) direct_cache [ - 1 + 0 * 8 ] = MB_TYPE_DIRECT2 >> 1 ;
 else if ( IS_8X8 ( left_type [ LTOP ] ) ) direct_cache [ - 1 + 0 * 8 ] = direct_table [ 4 * left_xy [ LTOP ] + 1 + ( left_block [ 0 ] & ~ 1 ) ] ;
 else direct_cache [ - 1 + 0 * 8 ] = MB_TYPE_16x16 >> 1 ;
 if ( IS_DIRECT ( left_type [ LBOT ] ) ) direct_cache [ - 1 + 2 * 8 ] = MB_TYPE_DIRECT2 >> 1 ;
 else if ( IS_8X8 ( left_type [ LBOT ] ) ) direct_cache [ - 1 + 2 * 8 ] = direct_table [ 4 * left_xy [ LBOT ] + 1 + ( left_block [ 2 ] & ~ 1 ) ] ;
 else direct_cache [ - 1 + 2 * 8 ] = MB_TYPE_16x16 >> 1 ;
 }
 }
 }
 # define MAP_MVS MAP_F2F ( scan8 [ 0 ] - 1 - 1 * 8 , topleft_type ) MAP_F2F ( scan8 [ 0 ] + 0 - 1 * 8 , top_type ) MAP_F2F ( scan8 [ 0 ] + 1 - 1 * 8 , top_type ) MAP_F2F ( scan8 [ 0 ] + 2 - 1 * 8 , top_type ) MAP_F2F ( scan8 [ 0 ] + 3 - 1 * 8 , top_type ) MAP_F2F ( scan8 [ 0 ] + 4 - 1 * 8 , topright_type ) MAP_F2F ( scan8 [ 0 ] - 1 + 0 * 8 , left_type [ LTOP ] ) MAP_F2F ( scan8 [ 0 ] - 1 + 1 * 8 , left_type [ LTOP ] ) MAP_F2F ( scan8 [ 0 ] - 1 + 2 * 8 , left_type [ LBOT ] ) MAP_F2F ( scan8 [ 0 ] - 1 + 3 * 8 , left_type [ LBOT ] ) if ( FRAME_MBAFF ) {
 if ( MB_FIELD ) {
 # define MAP_F2F ( idx , mb_type ) if ( ! IS_INTERLACED ( mb_type ) && h -> ref_cache [ list ] [ idx ] >= 0 ) {
 h -> ref_cache [ list ] [ idx ] <<= 1 ;
 h -> mv_cache [ list ] [ idx ] [ 1 ] /= 2 ;
 h -> mvd_cache [ list ] [ idx ] [ 1 ] >>= 1 ;
 }
 MAP_MVS }
 else {
 # undef MAP_F2F # define MAP_F2F ( idx , mb_type ) if ( IS_INTERLACED ( mb_type ) && h -> ref_cache [ list ] [ idx ] >= 0 ) {
 h -> ref_cache [ list ] [ idx ] >>= 1 ;
 h -> mv_cache [ list ] [ idx ] [ 1 ] <<= 1 ;
 h -> mvd_cache [ list ] [ idx ] [ 1 ] <<= 1 ;
 }
 MAP_MVS # undef MAP_F2F }
 }
 }
 }
 h -> neighbor_transform_size = ! ! IS_8x8DCT ( top_type ) + ! ! IS_8x8DCT ( left_type [ LTOP ] ) ;
 }