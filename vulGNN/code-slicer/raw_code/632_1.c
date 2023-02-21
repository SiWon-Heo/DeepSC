static int mss3_decode_frame ( AVCodecContext * avctx , void * data , int * got_frame , AVPacket * avpkt ) {
 const uint8_t * buf = avpkt -> data ;
 int buf_size = avpkt -> size ;
 MSS3Context * c = avctx -> priv_data ;
 RangeCoder * acoder = & c -> coder ;
 GetByteContext gb ;
 uint8_t * dst [ 3 ] ;
 int dec_width , dec_height , dec_x , dec_y , quality , keyframe ;
 int x , y , i , mb_width , mb_height , blk_size , btype ;
 int ret ;
 if ( buf_size < HEADER_SIZE ) {
 av_log ( avctx , AV_LOG_ERROR , "Frame should have at least %d bytes, got %d instead\n" , HEADER_SIZE , buf_size ) ;
 return AVERROR_INVALIDDATA ;
 }
 bytestream2_init ( & gb , buf , buf_size ) ;
 keyframe = bytestream2_get_be32 ( & gb ) ;
 if ( keyframe & ~ 0x301 ) {
 av_log ( avctx , AV_LOG_ERROR , "Invalid frame type %X\n" , keyframe ) ;
 return AVERROR_INVALIDDATA ;
 }
 keyframe = ! ( keyframe & 1 ) ;
 bytestream2_skip ( & gb , 6 ) ;
 dec_x = bytestream2_get_be16 ( & gb ) ;
 dec_y = bytestream2_get_be16 ( & gb ) ;
 dec_width = bytestream2_get_be16 ( & gb ) ;
 dec_height = bytestream2_get_be16 ( & gb ) ;
 if ( dec_x + dec_width > avctx -> width || dec_y + dec_height > avctx -> height || ( dec_width | dec_height ) & 0xF ) {
 av_log ( avctx , AV_LOG_ERROR , "Invalid frame dimensions %dx%d +%d,%d\n" , dec_width , dec_height , dec_x , dec_y ) ;
 return AVERROR_INVALIDDATA ;
 }
 bytestream2_skip ( & gb , 4 ) ;
 quality = bytestream2_get_byte ( & gb ) ;
 if ( quality < 1 || quality > 100 ) {
 av_log ( avctx , AV_LOG_ERROR , "Invalid quality setting %d\n" , quality ) ;
 return AVERROR_INVALIDDATA ;
 }
 bytestream2_skip ( & gb , 4 ) ;
 if ( keyframe && ! bytestream2_get_bytes_left ( & gb ) ) {
 av_log ( avctx , AV_LOG_ERROR , "Keyframe without data found\n" ) ;
 return AVERROR_INVALIDDATA ;
 }
 if ( ! keyframe && c -> got_error ) return buf_size ;
 c -> got_error = 0 ;
 c -> pic . reference = 3 ;
 c -> pic . buffer_hints = FF_BUFFER_HINTS_VALID | FF_BUFFER_HINTS_PRESERVE | FF_BUFFER_HINTS_REUSABLE ;
 if ( ( ret = avctx -> reget_buffer ( avctx , & c -> pic ) ) < 0 ) {
 av_log ( avctx , AV_LOG_ERROR , "reget_buffer() failed\n" ) ;
 return ret ;
 }
 c -> pic . key_frame = keyframe ;
 c -> pic . pict_type = keyframe ? AV_PICTURE_TYPE_I : AV_PICTURE_TYPE_P ;
 if ( ! bytestream2_get_bytes_left ( & gb ) ) {
 * got_frame = 1 ;
 * ( AVFrame * ) data = c -> pic ;
 return buf_size ;
 }
 reset_coders ( c , quality ) ;
 rac_init ( acoder , buf + HEADER_SIZE , buf_size - HEADER_SIZE ) ;
 mb_width = dec_width >> 4 ;
 mb_height = dec_height >> 4 ;
 dst [ 0 ] = c -> pic . data [ 0 ] + dec_x + dec_y * c -> pic . linesize [ 0 ] ;
 dst [ 1 ] = c -> pic . data [ 1 ] + dec_x / 2 + ( dec_y / 2 ) * c -> pic . linesize [ 1 ] ;
 dst [ 2 ] = c -> pic . data [ 2 ] + dec_x / 2 + ( dec_y / 2 ) * c -> pic . linesize [ 2 ] ;
 for ( y = 0 ;
 y < mb_height ;
 y ++ ) {
 for ( x = 0 ;
 x < mb_width ;
 x ++ ) {
 for ( i = 0 ;
 i < 3 ;
 i ++ ) {
 blk_size = 8 << ! i ;
 btype = decode_block_type ( acoder , c -> btype + i ) ;
 switch ( btype ) {
 case FILL_BLOCK : decode_fill_block ( acoder , c -> fill_coder + i , dst [ i ] + x * blk_size , c -> pic . linesize [ i ] , blk_size ) ;
 break ;
 case IMAGE_BLOCK : decode_image_block ( acoder , c -> image_coder + i , dst [ i ] + x * blk_size , c -> pic . linesize [ i ] , blk_size ) ;
 break ;
 case DCT_BLOCK : decode_dct_block ( acoder , c -> dct_coder + i , dst [ i ] + x * blk_size , c -> pic . linesize [ i ] , blk_size , c -> dctblock , x , y ) ;
 break ;
 case HAAR_BLOCK : decode_haar_block ( acoder , c -> haar_coder + i , dst [ i ] + x * blk_size , c -> pic . linesize [ i ] , blk_size , c -> hblock ) ;
 break ;
 }
 if ( c -> got_error || acoder -> got_error ) {
 av_log ( avctx , AV_LOG_ERROR , "Error decoding block %d,%d\n" , x , y ) ;
 c -> got_error = 1 ;
 return AVERROR_INVALIDDATA ;
 }
 }
 }
 dst [ 0 ] += c -> pic . linesize [ 0 ] * 16 ;
 dst [ 1 ] += c -> pic . linesize [ 1 ] * 8 ;
 dst [ 2 ] += c -> pic . linesize [ 2 ] * 8 ;
 }
 * got_frame = 1 ;
 * ( AVFrame * ) data = c -> pic ;
 return buf_size ;
 }