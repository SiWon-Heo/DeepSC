static int get_buffer ( AVCodecContext * avctx , Picture * pic ) {
 SVQ3Context * s = avctx -> priv_data ;
 H264Context * h = & s -> h ;
 const int big_mb_num = h -> mb_stride * ( h -> mb_height + 1 ) + 1 ;
 const int mb_array_size = h -> mb_stride * h -> mb_height ;
 const int b4_stride = h -> mb_width * 4 + 1 ;
 const int b4_array_size = b4_stride * h -> mb_height * 4 ;
 int ret ;
 if ( ! pic -> motion_val_base [ 0 ] ) {
 int i ;
 pic -> mb_type_base = av_mallocz ( ( big_mb_num + h -> mb_stride ) * sizeof ( uint32_t ) ) ;
 if ( ! pic -> mb_type_base ) return AVERROR ( ENOMEM ) ;
 pic -> f . mb_type = pic -> mb_type_base + 2 * h -> mb_stride + 1 ;
 for ( i = 0 ;
 i < 2 ;
 i ++ ) {
 pic -> motion_val_base [ i ] = av_mallocz ( 2 * ( b4_array_size + 4 ) * sizeof ( int16_t ) ) ;
 pic -> f . ref_index [ i ] = av_mallocz ( 4 * mb_array_size ) ;
 if ( ! pic -> motion_val_base [ i ] || ! pic -> f . ref_index [ i ] ) return AVERROR ( ENOMEM ) ;
 pic -> f . motion_val [ i ] = pic -> motion_val_base [ i ] + 4 ;
 }
 }
 pic -> f . motion_subsample_log2 = 2 ;
 pic -> f . reference = ! ( h -> pict_type == AV_PICTURE_TYPE_B ) ;
 ret = ff_get_buffer ( avctx , & pic -> f ) ;
 if ( ! h -> edge_emu_buffer ) {
 h -> edge_emu_buffer = av_mallocz ( pic -> f . linesize [ 0 ] * 17 ) ;
 if ( ! h -> edge_emu_buffer ) return AVERROR ( ENOMEM ) ;
 }
 h -> linesize = pic -> f . linesize [ 0 ] ;
 h -> uvlinesize = pic -> f . linesize [ 1 ] ;
 return ret ;
 }