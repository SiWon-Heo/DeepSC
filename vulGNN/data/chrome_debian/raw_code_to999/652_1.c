static void cdxl_decode_rgb ( CDXLVideoContext * c ) {
 uint32_t * new_palette = ( uint32_t * ) c -> frame . data [ 1 ] ;
 import_palette ( c , new_palette ) ;
 import_format ( c , c -> frame . linesize [ 0 ] , c -> frame . data [ 0 ] ) ;
 }