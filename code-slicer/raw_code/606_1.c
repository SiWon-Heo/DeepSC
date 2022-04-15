static void tgq_idct_put_mb ( TgqContext * s , int16_t ( * block ) [ 64 ] , int mb_x , int mb_y ) {
 int linesize = s -> frame . linesize [ 0 ] ;
 uint8_t * dest_y = s -> frame . data [ 0 ] + ( mb_y * 16 * linesize ) + mb_x * 16 ;
 uint8_t * dest_cb = s -> frame . data [ 1 ] + ( mb_y * 8 * s -> frame . linesize [ 1 ] ) + mb_x * 8 ;
 uint8_t * dest_cr = s -> frame . data [ 2 ] + ( mb_y * 8 * s -> frame . linesize [ 2 ] ) + mb_x * 8 ;
 ff_ea_idct_put_c ( dest_y , linesize , block [ 0 ] ) ;
 ff_ea_idct_put_c ( dest_y + 8 , linesize , block [ 1 ] ) ;
 ff_ea_idct_put_c ( dest_y + 8 * linesize , linesize , block [ 2 ] ) ;
 ff_ea_idct_put_c ( dest_y + 8 * linesize + 8 , linesize , block [ 3 ] ) ;
 if ( ! ( s -> avctx -> flags & CODEC_FLAG_GRAY ) ) {
 ff_ea_idct_put_c ( dest_cb , s -> frame . linesize [ 1 ] , block [ 4 ] ) ;
 ff_ea_idct_put_c ( dest_cr , s -> frame . linesize [ 2 ] , block [ 5 ] ) ;
 }
 }