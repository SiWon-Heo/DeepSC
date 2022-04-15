static int zerocodec_decode_frame ( AVCodecContext * avctx , void * data , int * got_frame , AVPacket * avpkt ) {
 ZeroCodecContext * zc = avctx -> priv_data ;
 AVFrame * pic = avctx -> coded_frame ;
 AVFrame * prev_pic = & zc -> previous_frame ;
 z_stream * zstream = & zc -> zstream ;
 uint8_t * prev = prev_pic -> data [ 0 ] ;
 uint8_t * dst ;
 int i , j , zret ;
 pic -> reference = 3 ;
 if ( avpkt -> flags & AV_PKT_FLAG_KEY ) {
 pic -> key_frame = 1 ;
 pic -> pict_type = AV_PICTURE_TYPE_I ;
 }
 else {
 if ( ! prev ) {
 av_log ( avctx , AV_LOG_ERROR , "Missing reference frame.\n" ) ;
 return AVERROR_INVALIDDATA ;
 }
 prev += ( avctx -> height - 1 ) * prev_pic -> linesize [ 0 ] ;
 pic -> key_frame = 0 ;
 pic -> pict_type = AV_PICTURE_TYPE_P ;
 }
 zret = inflateReset ( zstream ) ;
 if ( zret != Z_OK ) {
 av_log ( avctx , AV_LOG_ERROR , "Could not reset inflate: %d.\n" , zret ) ;
 return AVERROR_INVALIDDATA ;
 }
 if ( ff_get_buffer ( avctx , pic ) < 0 ) {
 av_log ( avctx , AV_LOG_ERROR , "Could not allocate buffer.\n" ) ;
 return AVERROR ( ENOMEM ) ;
 }
 zstream -> next_in = avpkt -> data ;
 zstream -> avail_in = avpkt -> size ;
 dst = pic -> data [ 0 ] + ( avctx -> height - 1 ) * pic -> linesize [ 0 ] ;
 for ( i = 0 ;
 i < avctx -> height ;
 i ++ ) {
 zstream -> next_out = dst ;
 zstream -> avail_out = avctx -> width << 1 ;
 zret = inflate ( zstream , Z_SYNC_FLUSH ) ;
 if ( zret != Z_OK && zret != Z_STREAM_END ) {
 avctx -> release_buffer ( avctx , pic ) ;
 av_log ( avctx , AV_LOG_ERROR , "Inflate failed with return code: %d.\n" , zret ) ;
 return AVERROR_INVALIDDATA ;
 }
 if ( ! ( avpkt -> flags & AV_PKT_FLAG_KEY ) ) for ( j = 0 ;
 j < avctx -> width << 1 ;
 j ++ ) dst [ j ] += prev [ j ] & - ! dst [ j ] ;
 prev -= prev_pic -> linesize [ 0 ] ;
 dst -= pic -> linesize [ 0 ] ;
 }
 if ( prev_pic -> data [ 0 ] ) avctx -> release_buffer ( avctx , prev_pic ) ;
 * got_frame = 1 ;
 * ( AVFrame * ) data = * pic ;
 FFSWAP ( AVFrame , * pic , * prev_pic ) ;
 return avpkt -> size ;
 }