static int yop_copy_previous_block ( YopDecContext * s , int copy_tag ) {
 uint8_t * bufptr ;
 bufptr = s -> dstptr + motion_vector [ copy_tag ] [ 0 ] + s -> frame . linesize [ 0 ] * motion_vector [ copy_tag ] [ 1 ] ;
 if ( bufptr < s -> dstbuf ) {
 av_log ( s -> avctx , AV_LOG_ERROR , "YOP: cannot decode, file probably corrupt\n" ) ;
 return AVERROR_INVALIDDATA ;
 }
 s -> dstptr [ 0 ] = bufptr [ 0 ] ;
 s -> dstptr [ 1 ] = bufptr [ 1 ] ;
 s -> dstptr [ s -> frame . linesize [ 0 ] ] = bufptr [ s -> frame . linesize [ 0 ] ] ;
 s -> dstptr [ s -> frame . linesize [ 0 ] + 1 ] = bufptr [ s -> frame . linesize [ 0 ] + 1 ] ;
 return 0 ;
 }