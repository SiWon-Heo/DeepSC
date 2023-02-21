static int fill_vaapi_ReferenceFrames ( VAPictureParameterBufferH264 * pic_param , H264Context * h ) {
 DPB dpb ;
 int i ;
 dpb . size = 0 ;
 dpb . max_size = FF_ARRAY_ELEMS ( pic_param -> ReferenceFrames ) ;
 dpb . va_pics = pic_param -> ReferenceFrames ;
 for ( i = 0 ;
 i < dpb . max_size ;
 i ++ ) init_vaapi_pic ( & dpb . va_pics [ i ] ) ;
 for ( i = 0 ;
 i < h -> short_ref_count ;
 i ++ ) {
 Picture * const pic = h -> short_ref [ i ] ;
 if ( pic && pic -> f . reference && dpb_add ( & dpb , pic ) < 0 ) return - 1 ;
 }
 for ( i = 0 ;
 i < 16 ;
 i ++ ) {
 Picture * const pic = h -> long_ref [ i ] ;
 if ( pic && pic -> f . reference && dpb_add ( & dpb , pic ) < 0 ) return - 1 ;
 }
 return 0 ;
 }