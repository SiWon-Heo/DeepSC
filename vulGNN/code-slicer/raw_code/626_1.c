static int pcm_bluray_decode_frame ( AVCodecContext * avctx , void * data , int * got_frame_ptr , AVPacket * avpkt ) {
 AVFrame * frame = data ;
 const uint8_t * src = avpkt -> data ;
 int buf_size = avpkt -> size ;
 GetByteContext gb ;
 int num_source_channels , channel , retval ;
 int sample_size , samples ;
 int16_t * dst16 ;
 int32_t * dst32 ;
 if ( buf_size < 4 ) {
 av_log ( avctx , AV_LOG_ERROR , "PCM packet too small\n" ) ;
 return - 1 ;
 }
 if ( pcm_bluray_parse_header ( avctx , src ) ) return - 1 ;
 src += 4 ;
 buf_size -= 4 ;
 bytestream2_init ( & gb , src , buf_size ) ;
 num_source_channels = FFALIGN ( avctx -> channels , 2 ) ;
 sample_size = ( num_source_channels * ( avctx -> sample_fmt == AV_SAMPLE_FMT_S16 ? 16 : 24 ) ) >> 3 ;
 samples = buf_size / sample_size ;
 frame -> nb_samples = samples ;
 if ( ( retval = ff_get_buffer ( avctx , frame ) ) < 0 ) {
 av_log ( avctx , AV_LOG_ERROR , "get_buffer() failed\n" ) ;
 return retval ;
 }
 dst16 = ( int16_t * ) frame -> data [ 0 ] ;
 dst32 = ( int32_t * ) frame -> data [ 0 ] ;
 if ( samples ) {
 switch ( avctx -> channel_layout ) {
 case AV_CH_LAYOUT_STEREO : case AV_CH_LAYOUT_4POINT0 : case AV_CH_LAYOUT_2_2 : samples *= num_source_channels ;
 if ( AV_SAMPLE_FMT_S16 == avctx -> sample_fmt ) {
 # if HAVE_BIGENDIAN bytestream2_get_buffer ( & gb , dst16 , buf_size ) ;
 # else do {
 * dst16 ++ = bytestream2_get_be16u ( & gb ) ;
 }
 while ( -- samples ) ;
 # endif }
 else {
 do {
 * dst32 ++ = bytestream2_get_be24u ( & gb ) << 8 ;
 }
 while ( -- samples ) ;
 }
 break ;
 case AV_CH_LAYOUT_MONO : case AV_CH_LAYOUT_SURROUND : case AV_CH_LAYOUT_2_1 : case AV_CH_LAYOUT_5POINT0 : if ( AV_SAMPLE_FMT_S16 == avctx -> sample_fmt ) {
 do {
 # if HAVE_BIGENDIAN bytestream2_get_buffer ( & gb , dst16 , avctx -> channels * 2 ) ;
 dst16 += avctx -> channels ;
 # else channel = avctx -> channels ;
 do {
 * dst16 ++ = bytestream2_get_be16u ( & gb ) ;
 }
 while ( -- channel ) ;
 # endif bytestream2_skip ( & gb , 2 ) ;
 }
 while ( -- samples ) ;
 }
 else {
 do {
 channel = avctx -> channels ;
 do {
 * dst32 ++ = bytestream2_get_be24u ( & gb ) << 8 ;
 }
 while ( -- channel ) ;
 bytestream2_skip ( & gb , 3 ) ;
 }
 while ( -- samples ) ;
 }
 break ;
 case AV_CH_LAYOUT_5POINT1 : if ( AV_SAMPLE_FMT_S16 == avctx -> sample_fmt ) {
 do {
 dst16 [ 0 ] = bytestream2_get_be16u ( & gb ) ;
 dst16 [ 1 ] = bytestream2_get_be16u ( & gb ) ;
 dst16 [ 2 ] = bytestream2_get_be16u ( & gb ) ;
 dst16 [ 4 ] = bytestream2_get_be16u ( & gb ) ;
 dst16 [ 5 ] = bytestream2_get_be16u ( & gb ) ;
 dst16 [ 3 ] = bytestream2_get_be16u ( & gb ) ;
 dst16 += 6 ;
 }
 while ( -- samples ) ;
 }
 else {
 do {
 dst32 [ 0 ] = bytestream2_get_be24u ( & gb ) << 8 ;
 dst32 [ 1 ] = bytestream2_get_be24u ( & gb ) << 8 ;
 dst32 [ 2 ] = bytestream2_get_be24u ( & gb ) << 8 ;
 dst32 [ 4 ] = bytestream2_get_be24u ( & gb ) << 8 ;
 dst32 [ 5 ] = bytestream2_get_be24u ( & gb ) << 8 ;
 dst32 [ 3 ] = bytestream2_get_be24u ( & gb ) << 8 ;
 dst32 += 6 ;
 }
 while ( -- samples ) ;
 }
 break ;
 case AV_CH_LAYOUT_7POINT0 : if ( AV_SAMPLE_FMT_S16 == avctx -> sample_fmt ) {
 do {
 dst16 [ 0 ] = bytestream2_get_be16u ( & gb ) ;
 dst16 [ 1 ] = bytestream2_get_be16u ( & gb ) ;
 dst16 [ 2 ] = bytestream2_get_be16u ( & gb ) ;
 dst16 [ 5 ] = bytestream2_get_be16u ( & gb ) ;
 dst16 [ 3 ] = bytestream2_get_be16u ( & gb ) ;
 dst16 [ 4 ] = bytestream2_get_be16u ( & gb ) ;
 dst16 [ 6 ] = bytestream2_get_be16u ( & gb ) ;
 dst16 += 7 ;
 bytestream2_skip ( & gb , 2 ) ;
 }
 while ( -- samples ) ;
 }
 else {
 do {
 dst32 [ 0 ] = bytestream2_get_be24u ( & gb ) << 8 ;
 dst32 [ 1 ] = bytestream2_get_be24u ( & gb ) << 8 ;
 dst32 [ 2 ] = bytestream2_get_be24u ( & gb ) << 8 ;
 dst32 [ 5 ] = bytestream2_get_be24u ( & gb ) << 8 ;
 dst32 [ 3 ] = bytestream2_get_be24u ( & gb ) << 8 ;
 dst32 [ 4 ] = bytestream2_get_be24u ( & gb ) << 8 ;
 dst32 [ 6 ] = bytestream2_get_be24u ( & gb ) << 8 ;
 dst32 += 7 ;
 bytestream2_skip ( & gb , 3 ) ;
 }
 while ( -- samples ) ;
 }
 break ;
 case AV_CH_LAYOUT_7POINT1 : if ( AV_SAMPLE_FMT_S16 == avctx -> sample_fmt ) {
 do {
 dst16 [ 0 ] = bytestream2_get_be16u ( & gb ) ;
 dst16 [ 1 ] = bytestream2_get_be16u ( & gb ) ;
 dst16 [ 2 ] = bytestream2_get_be16u ( & gb ) ;
 dst16 [ 6 ] = bytestream2_get_be16u ( & gb ) ;
 dst16 [ 4 ] = bytestream2_get_be16u ( & gb ) ;
 dst16 [ 5 ] = bytestream2_get_be16u ( & gb ) ;
 dst16 [ 7 ] = bytestream2_get_be16u ( & gb ) ;
 dst16 [ 3 ] = bytestream2_get_be16u ( & gb ) ;
 dst16 += 8 ;
 }
 while ( -- samples ) ;
 }
 else {
 do {
 dst32 [ 0 ] = bytestream2_get_be24u ( & gb ) << 8 ;
 dst32 [ 1 ] = bytestream2_get_be24u ( & gb ) << 8 ;
 dst32 [ 2 ] = bytestream2_get_be24u ( & gb ) << 8 ;
 dst32 [ 6 ] = bytestream2_get_be24u ( & gb ) << 8 ;
 dst32 [ 4 ] = bytestream2_get_be24u ( & gb ) << 8 ;
 dst32 [ 5 ] = bytestream2_get_be24u ( & gb ) << 8 ;
 dst32 [ 7 ] = bytestream2_get_be24u ( & gb ) << 8 ;
 dst32 [ 3 ] = bytestream2_get_be24u ( & gb ) << 8 ;
 dst32 += 8 ;
 }
 while ( -- samples ) ;
 }
 break ;
 }
 }
 * got_frame_ptr = 1 ;
 retval = bytestream2_tell ( & gb ) ;
 if ( avctx -> debug & FF_DEBUG_BITSTREAM ) av_dlog ( avctx , "pcm_bluray_decode_frame: decoded %d -> %d bytes\n" , retval , buf_size ) ;
 return retval + 4 ;
 }