int jbig2_decode_generic_mmr ( Jbig2Ctx * ctx , Jbig2Segment * segment , const Jbig2GenericRegionParams * params , const byte * data , size_t size , Jbig2Image * image ) {
 Jbig2MmrCtx mmr ;
 const uint32_t rowstride = image -> stride ;
 byte * dst = image -> data ;
 byte * ref = NULL ;
 uint32_t y ;
 int code = 0 ;
 jbig2_decode_mmr_init ( & mmr , image -> width , image -> height , data , size ) ;
 for ( y = 0 ;
 y < image -> height ;
 y ++ ) {
 memset ( dst , 0 , rowstride ) ;
 code = jbig2_decode_mmr_line ( & mmr , ref , dst ) ;
 if ( code < 0 ) return code ;
 ref = dst ;
 dst += rowstride ;
 }
 return code ;
 }