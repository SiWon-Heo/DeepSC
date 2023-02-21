static int yop_paint_block ( YopDecContext * s , int tag ) {
 if ( s -> src_end - s -> srcptr < paint_lut [ tag ] [ 3 ] ) {
 av_log ( s -> avctx , AV_LOG_ERROR , "Packet too small.\n" ) ;
 return AVERROR_INVALIDDATA ;
 }
 s -> dstptr [ 0 ] = s -> srcptr [ 0 ] ;
 s -> dstptr [ 1 ] = s -> srcptr [ paint_lut [ tag ] [ 0 ] ] ;
 s -> dstptr [ s -> frame . linesize [ 0 ] ] = s -> srcptr [ paint_lut [ tag ] [ 1 ] ] ;
 s -> dstptr [ s -> frame . linesize [ 0 ] + 1 ] = s -> srcptr [ paint_lut [ tag ] [ 2 ] ] ;
 s -> srcptr += paint_lut [ tag ] [ 3 ] ;
 return 0 ;
 }