static int rv34_decode_intra_macroblock ( RV34DecContext * r , int8_t * intra_types ) {
 MpegEncContext * s = & r -> s ;
 int cbp , dist ;
 int mb_pos = s -> mb_x + s -> mb_y * s -> mb_stride ;
 memset ( r -> avail_cache , 0 , sizeof ( r -> avail_cache ) ) ;
 fill_rectangle ( r -> avail_cache + 6 , 2 , 2 , 4 , 1 , 4 ) ;
 dist = ( s -> mb_x - s -> resync_mb_x ) + ( s -> mb_y - s -> resync_mb_y ) * s -> mb_width ;
 if ( s -> mb_x && dist ) r -> avail_cache [ 5 ] = r -> avail_cache [ 9 ] = s -> current_picture_ptr -> f . mb_type [ mb_pos - 1 ] ;
 if ( dist >= s -> mb_width ) r -> avail_cache [ 2 ] = r -> avail_cache [ 3 ] = s -> current_picture_ptr -> f . mb_type [ mb_pos - s -> mb_stride ] ;
 if ( ( ( s -> mb_x + 1 ) < s -> mb_width ) && dist >= s -> mb_width - 1 ) r -> avail_cache [ 4 ] = s -> current_picture_ptr -> f . mb_type [ mb_pos - s -> mb_stride + 1 ] ;
 if ( s -> mb_x && dist > s -> mb_width ) r -> avail_cache [ 1 ] = s -> current_picture_ptr -> f . mb_type [ mb_pos - s -> mb_stride - 1 ] ;
 s -> qscale = r -> si . quant ;
 cbp = rv34_decode_intra_mb_header ( r , intra_types ) ;
 r -> cbp_luma [ mb_pos ] = cbp ;
 r -> cbp_chroma [ mb_pos ] = cbp >> 16 ;
 r -> deblock_coefs [ mb_pos ] = 0xFFFF ;
 s -> current_picture_ptr -> f . qscale_table [ mb_pos ] = s -> qscale ;
 if ( cbp == - 1 ) return - 1 ;
 if ( r -> is16 ) {
 rv34_output_i16x16 ( r , intra_types , cbp ) ;
 return 0 ;
 }
 rv34_output_intra ( r , intra_types , cbp ) ;
 return 0 ;
 }