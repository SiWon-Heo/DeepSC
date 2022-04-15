int ff_alloc_picture ( MpegEncContext * s , Picture * pic , int shared ) {
 const int big_mb_num = s -> mb_stride * ( s -> mb_height + 1 ) + 1 ;
 const int mb_array_size = s -> mb_stride * s -> mb_height ;
 const int b8_array_size = s -> b8_stride * s -> mb_height * 2 ;
 const int b4_array_size = s -> b4_stride * s -> mb_height * 4 ;
 int i ;
 int r = - 1 ;
 if ( shared ) {
 assert ( pic -> f . data [ 0 ] ) ;
 assert ( pic -> f . type == 0 || pic -> f . type == FF_BUFFER_TYPE_SHARED ) ;
 pic -> f . type = FF_BUFFER_TYPE_SHARED ;
 }
 else {
 assert ( ! pic -> f . data [ 0 ] ) ;
 if ( alloc_frame_buffer ( s , pic ) < 0 ) return - 1 ;
 s -> linesize = pic -> f . linesize [ 0 ] ;
 s -> uvlinesize = pic -> f . linesize [ 1 ] ;
 }
 if ( pic -> f . qscale_table == NULL ) {
 if ( s -> encoding ) {
 FF_ALLOCZ_OR_GOTO ( s -> avctx , pic -> mb_var , mb_array_size * sizeof ( int16_t ) , fail ) FF_ALLOCZ_OR_GOTO ( s -> avctx , pic -> mc_mb_var , mb_array_size * sizeof ( int16_t ) , fail ) FF_ALLOCZ_OR_GOTO ( s -> avctx , pic -> mb_mean , mb_array_size * sizeof ( int8_t ) , fail ) }
 FF_ALLOCZ_OR_GOTO ( s -> avctx , pic -> f . mbskip_table , mb_array_size * sizeof ( uint8_t ) + 2 , fail ) FF_ALLOCZ_OR_GOTO ( s -> avctx , pic -> qscale_table_base , ( big_mb_num + s -> mb_stride ) * sizeof ( uint8_t ) , fail ) FF_ALLOCZ_OR_GOTO ( s -> avctx , pic -> mb_type_base , ( big_mb_num + s -> mb_stride ) * sizeof ( uint32_t ) , fail ) pic -> f . mb_type = pic -> mb_type_base + 2 * s -> mb_stride + 1 ;
 pic -> f . qscale_table = pic -> qscale_table_base + 2 * s -> mb_stride + 1 ;
 if ( s -> out_format == FMT_H264 ) {
 for ( i = 0 ;
 i < 2 ;
 i ++ ) {
 FF_ALLOCZ_OR_GOTO ( s -> avctx , pic -> motion_val_base [ i ] , 2 * ( b4_array_size + 4 ) * sizeof ( int16_t ) , fail ) pic -> f . motion_val [ i ] = pic -> motion_val_base [ i ] + 4 ;
 FF_ALLOCZ_OR_GOTO ( s -> avctx , pic -> f . ref_index [ i ] , 4 * mb_array_size * sizeof ( uint8_t ) , fail ) }
 pic -> f . motion_subsample_log2 = 2 ;
 }
 else if ( s -> out_format == FMT_H263 || s -> encoding || ( s -> avctx -> debug & FF_DEBUG_MV ) || s -> avctx -> debug_mv ) {
 for ( i = 0 ;
 i < 2 ;
 i ++ ) {
 FF_ALLOCZ_OR_GOTO ( s -> avctx , pic -> motion_val_base [ i ] , 2 * ( b8_array_size + 4 ) * sizeof ( int16_t ) , fail ) pic -> f . motion_val [ i ] = pic -> motion_val_base [ i ] + 4 ;
 FF_ALLOCZ_OR_GOTO ( s -> avctx , pic -> f . ref_index [ i ] , 4 * mb_array_size * sizeof ( uint8_t ) , fail ) }
 pic -> f . motion_subsample_log2 = 3 ;
 }
 if ( s -> avctx -> debug & FF_DEBUG_DCT_COEFF ) {
 FF_ALLOCZ_OR_GOTO ( s -> avctx , pic -> f . dct_coeff , 64 * mb_array_size * sizeof ( int16_t ) * 6 , fail ) }
 pic -> f . qstride = s -> mb_stride ;
 FF_ALLOCZ_OR_GOTO ( s -> avctx , pic -> f . pan_scan , 1 * sizeof ( AVPanScan ) , fail ) }
 pic -> owner2 = s ;
 return 0 ;
 fail : if ( r >= 0 ) free_frame_buffer ( s , pic ) ;
 return - 1 ;
 }