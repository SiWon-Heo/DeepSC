static int decode_frame ( AVCodecContext * avctx , void * data , int * got_frame , AVPacket * avpkt ) {
 VBDecContext * const c = avctx -> priv_data ;
 uint8_t * outptr , * srcptr ;
 int i , j , ret ;
 int flags ;
 uint32_t size ;
 int offset = 0 ;
 bytestream2_init ( & c -> stream , avpkt -> data , avpkt -> size ) ;
 if ( c -> pic . data [ 0 ] ) avctx -> release_buffer ( avctx , & c -> pic ) ;
 c -> pic . reference = 1 ;
 if ( ( ret = ff_get_buffer ( avctx , & c -> pic ) ) < 0 ) {
 av_log ( avctx , AV_LOG_ERROR , "get_buffer() failed\n" ) ;
 return ret ;
 }
 flags = bytestream2_get_le16 ( & c -> stream ) ;
 if ( flags & VB_HAS_GMC ) {
 i = ( int16_t ) bytestream2_get_le16 ( & c -> stream ) ;
 j = ( int16_t ) bytestream2_get_le16 ( & c -> stream ) ;
 offset = i + j * avctx -> width ;
 }
 if ( flags & VB_HAS_VIDEO ) {
 size = bytestream2_get_le32 ( & c -> stream ) ;
 vb_decode_framedata ( c , offset ) ;
 bytestream2_skip ( & c -> stream , size - 4 ) ;
 }
 if ( flags & VB_HAS_PALETTE ) {
 size = bytestream2_get_le32 ( & c -> stream ) ;
 vb_decode_palette ( c , size ) ;
 }
 memcpy ( c -> pic . data [ 1 ] , c -> pal , AVPALETTE_SIZE ) ;
 c -> pic . palette_has_changed = flags & VB_HAS_PALETTE ;
 outptr = c -> pic . data [ 0 ] ;
 srcptr = c -> frame ;
 for ( i = 0 ;
 i < avctx -> height ;
 i ++ ) {
 memcpy ( outptr , srcptr , avctx -> width ) ;
 srcptr += avctx -> width ;
 outptr += c -> pic . linesize [ 0 ] ;
 }
 FFSWAP ( uint8_t * , c -> frame , c -> prev_frame ) ;
 * got_frame = 1 ;
 * ( AVFrame * ) data = c -> pic ;
 return avpkt -> size ;
 }