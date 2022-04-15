static void cmv_decode_intra ( CmvContext * s , const uint8_t * buf , const uint8_t * buf_end ) {
 unsigned char * dst = s -> frame . data [ 0 ] ;
 int i ;
 for ( i = 0 ;
 i < s -> avctx -> height && buf_end - buf >= s -> avctx -> width ;
 i ++ ) {
 memcpy ( dst , buf , s -> avctx -> width ) ;
 dst += s -> frame . linesize [ 0 ] ;
 buf += s -> avctx -> width ;
 }
 }