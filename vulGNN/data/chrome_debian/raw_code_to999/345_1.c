static inline void idct_put ( MDECContext * a , int mb_x , int mb_y ) {
 int16_t ( * block ) [ 64 ] = a -> block ;
 int linesize = a -> picture . linesize [ 0 ] ;
 uint8_t * dest_y = a -> picture . data [ 0 ] + ( mb_y * 16 * linesize ) + mb_x * 16 ;
 uint8_t * dest_cb = a -> picture . data [ 1 ] + ( mb_y * 8 * a -> picture . linesize [ 1 ] ) + mb_x * 8 ;
 uint8_t * dest_cr = a -> picture . data [ 2 ] + ( mb_y * 8 * a -> picture . linesize [ 2 ] ) + mb_x * 8 ;
 a -> dsp . idct_put ( dest_y , linesize , block [ 0 ] ) ;
 a -> dsp . idct_put ( dest_y + 8 , linesize , block [ 1 ] ) ;
 a -> dsp . idct_put ( dest_y + 8 * linesize , linesize , block [ 2 ] ) ;
 a -> dsp . idct_put ( dest_y + 8 * linesize + 8 , linesize , block [ 3 ] ) ;
 if ( ! ( a -> avctx -> flags & CODEC_FLAG_GRAY ) ) {
 a -> dsp . idct_put ( dest_cb , a -> picture . linesize [ 1 ] , block [ 4 ] ) ;
 a -> dsp . idct_put ( dest_cr , a -> picture . linesize [ 2 ] , block [ 5 ] ) ;
 }
 }