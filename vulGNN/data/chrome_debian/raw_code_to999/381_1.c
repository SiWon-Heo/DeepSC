static int libschroedinger_decode_frame ( AVCodecContext * avctx , void * data , int * got_frame , AVPacket * avpkt ) {
 const uint8_t * buf = avpkt -> data ;
 int buf_size = avpkt -> size ;
 int64_t pts = avpkt -> pts ;
 SchroTag * tag ;
 SchroDecoderParams * p_schro_params = avctx -> priv_data ;
 SchroDecoder * decoder = p_schro_params -> decoder ;
 SchroBuffer * enc_buf ;
 SchroFrame * frame ;
 int state ;
 int go = 1 ;
 int outer = 1 ;
 SchroParseUnitContext parse_ctx ;
 LibSchroFrameContext * framewithpts = NULL ;
 * got_frame = 0 ;
 parse_context_init ( & parse_ctx , buf , buf_size ) ;
 if ( ! buf_size ) {
 if ( ! p_schro_params -> eos_signalled ) {
 state = schro_decoder_push_end_of_stream ( decoder ) ;
 p_schro_params -> eos_signalled = 1 ;
 }
 }
 do {
 if ( ( enc_buf = find_next_parse_unit ( & parse_ctx ) ) ) {
 enc_buf -> tag = schro_tag_new ( av_malloc ( sizeof ( int64_t ) ) , av_free ) ;
 if ( ! enc_buf -> tag -> value ) {
 av_log ( avctx , AV_LOG_ERROR , "Unable to allocate SchroTag\n" ) ;
 return AVERROR ( ENOMEM ) ;
 }
 AV_WN ( 64 , enc_buf -> tag -> value , pts ) ;
 if ( SCHRO_PARSE_CODE_IS_PICTURE ( enc_buf -> data [ 4 ] ) && SCHRO_PARSE_CODE_NUM_REFS ( enc_buf -> data [ 4 ] ) > 0 ) avctx -> has_b_frames = 1 ;
 state = schro_decoder_push ( decoder , enc_buf ) ;
 if ( state == SCHRO_DECODER_FIRST_ACCESS_UNIT ) libschroedinger_handle_first_access_unit ( avctx ) ;
 go = 1 ;
 }
 else outer = 0 ;
 while ( go ) {
 state = schro_decoder_wait ( decoder ) ;
 switch ( state ) {
 case SCHRO_DECODER_FIRST_ACCESS_UNIT : libschroedinger_handle_first_access_unit ( avctx ) ;
 break ;
 case SCHRO_DECODER_NEED_BITS : go = 0 ;
 break ;
 case SCHRO_DECODER_NEED_FRAME : frame = ff_create_schro_frame ( avctx , p_schro_params -> frame_format ) ;
 schro_decoder_add_output_picture ( decoder , frame ) ;
 break ;
 case SCHRO_DECODER_OK : tag = schro_decoder_get_picture_tag ( decoder ) ;
 frame = schro_decoder_pull ( decoder ) ;
 if ( frame ) {
 framewithpts = av_malloc ( sizeof ( LibSchroFrameContext ) ) ;
 if ( ! framewithpts ) {
 av_log ( avctx , AV_LOG_ERROR , "Unable to allocate FrameWithPts\n" ) ;
 return AVERROR ( ENOMEM ) ;
 }
 framewithpts -> frame = frame ;
 framewithpts -> pts = AV_RN64 ( tag -> value ) ;
 ff_schro_queue_push_back ( & p_schro_params -> dec_frame_queue , framewithpts ) ;
 }
 break ;
 case SCHRO_DECODER_EOS : go = 0 ;
 p_schro_params -> eos_pulled = 1 ;
 schro_decoder_reset ( decoder ) ;
 outer = 0 ;
 break ;
 case SCHRO_DECODER_ERROR : return - 1 ;
 break ;
 }
 }
 }
 while ( outer ) ;
 framewithpts = ff_schro_queue_pop ( & p_schro_params -> dec_frame_queue ) ;
 if ( framewithpts && framewithpts -> frame ) {
 if ( p_schro_params -> dec_frame . data [ 0 ] ) avctx -> release_buffer ( avctx , & p_schro_params -> dec_frame ) ;
 if ( ff_get_buffer ( avctx , & p_schro_params -> dec_frame ) < 0 ) {
 av_log ( avctx , AV_LOG_ERROR , "Unable to allocate buffer\n" ) ;
 return AVERROR ( ENOMEM ) ;
 }
 memcpy ( p_schro_params -> dec_frame . data [ 0 ] , framewithpts -> frame -> components [ 0 ] . data , framewithpts -> frame -> components [ 0 ] . length ) ;
 memcpy ( p_schro_params -> dec_frame . data [ 1 ] , framewithpts -> frame -> components [ 1 ] . data , framewithpts -> frame -> components [ 1 ] . length ) ;
 memcpy ( p_schro_params -> dec_frame . data [ 2 ] , framewithpts -> frame -> components [ 2 ] . data , framewithpts -> frame -> components [ 2 ] . length ) ;
 p_schro_params -> dec_frame . format = - 1 ;
 p_schro_params -> dec_frame . width = framewithpts -> frame -> width ;
 p_schro_params -> dec_frame . height = framewithpts -> frame -> height ;
 p_schro_params -> dec_frame . pkt_pts = framewithpts -> pts ;
 p_schro_params -> dec_frame . linesize [ 0 ] = framewithpts -> frame -> components [ 0 ] . stride ;
 p_schro_params -> dec_frame . linesize [ 1 ] = framewithpts -> frame -> components [ 1 ] . stride ;
 p_schro_params -> dec_frame . linesize [ 2 ] = framewithpts -> frame -> components [ 2 ] . stride ;
 * ( AVFrame * ) data = p_schro_params -> dec_frame ;
 * got_frame = 1 ;
 libschroedinger_decode_frame_free ( framewithpts -> frame ) ;
 av_free ( framewithpts ) ;
 }
 else {
 data = NULL ;
 * got_frame = 0 ;
 }
 return buf_size ;
 }