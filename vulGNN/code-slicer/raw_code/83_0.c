int jbig2_decode_halftone_mmr ( Jbig2Ctx * ctx , const Jbig2GenericRegionParams * params , const byte * data , size_t size , Jbig2Image * image , size_t * consumed_bytes ) {
 Jbig2MmrCtx mmr ;
 const uint32_t rowstride = image -> stride ;
 byte * dst = image -> data ;
 byte * ref = NULL ;
 uint32_t y ;
 int code = 0 ;
 const uint32_t EOFB = 0x001001 ;
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
 if ( mmr . word >> 8 == EOFB ) {
 mmr . data_index += 3 ;
 }
 * consumed_bytes += mmr . data_index + ( mmr . bit_index >> 3 ) + ( mmr . bit_index > 0 ? 1 : 0 ) ;
 return code ;
 }