static inline void xan_wc3_output_pixel_run ( XanContext * s , const unsigned char * pixel_buffer , int x , int y , int pixel_count ) {
 int stride ;
 int line_inc ;
 int index ;
 int current_x ;
 int width = s -> avctx -> width ;
 unsigned char * palette_plane ;
 palette_plane = s -> current_frame . data [ 0 ] ;
 stride = s -> current_frame . linesize [ 0 ] ;
 line_inc = stride - width ;
 index = y * stride + x ;
 current_x = x ;
 while ( pixel_count && index < s -> frame_size ) {
 int count = FFMIN ( pixel_count , width - current_x ) ;
 memcpy ( palette_plane + index , pixel_buffer , count ) ;
 pixel_count -= count ;
 index += count ;
 pixel_buffer += count ;
 current_x += count ;
 if ( current_x >= width ) {
 index += line_inc ;
 current_x = 0 ;
 }
 }
 }