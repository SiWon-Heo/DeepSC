static void vble_restore_plane ( VBLEContext * ctx , int plane , int offset , int width , int height ) {
 AVFrame * pic = ctx -> avctx -> coded_frame ;
 uint8_t * dst = pic -> data [ plane ] ;
 uint8_t * val = ctx -> val + offset ;
 int stride = pic -> linesize [ plane ] ;
 int i , j , left , left_top ;
 for ( i = 0 ;
 i < height ;
 i ++ ) {
 for ( j = 0 ;
 j < width ;
 j ++ ) val [ j ] = ( val [ j ] >> 1 ) ^ - ( val [ j ] & 1 ) ;
 if ( i ) {
 left = 0 ;
 left_top = dst [ - stride ] ;
 ctx -> dsp . add_hfyu_median_prediction ( dst , dst - stride , val , width , & left , & left_top ) ;
 }
 else {
 dst [ 0 ] = val [ 0 ] ;
 for ( j = 1 ;
 j < width ;
 j ++ ) dst [ j ] = val [ j ] + dst [ j - 1 ] ;
 }
 dst += stride ;
 val += width ;
 }
 }