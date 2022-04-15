static int escape124_decode_frame ( AVCodecContext * avctx , void * data , int * got_frame , AVPacket * avpkt ) {
 const uint8_t * buf = avpkt -> data ;
 int buf_size = avpkt -> size ;
 Escape124Context * s = avctx -> priv_data ;
 GetBitContext gb ;
 unsigned frame_flags , frame_size ;
 unsigned i ;
 unsigned superblock_index , cb_index = 1 , superblock_col_index = 0 , superblocks_per_row = avctx -> width / 8 , skip = - 1 ;
 uint16_t * old_frame_data , * new_frame_data ;
 unsigned old_stride , new_stride ;
 AVFrame new_frame = {
 {
 0 }
 }
 ;
 init_get_bits ( & gb , buf , buf_size * 8 ) ;
 if ( ! can_safely_read ( & gb , 64 ) ) return - 1 ;
 frame_flags = get_bits_long ( & gb , 32 ) ;
 frame_size = get_bits_long ( & gb , 32 ) ;
 if ( ! ( frame_flags & 0x114 ) || ! ( frame_flags & 0x7800000 ) ) {
 av_log ( NULL , AV_LOG_DEBUG , "Skipping frame\n" ) ;
 * got_frame = 1 ;
 * ( AVFrame * ) data = s -> frame ;
 return frame_size ;
 }
 for ( i = 0 ;
 i < 3 ;
 i ++ ) {
 if ( frame_flags & ( 1 << ( 17 + i ) ) ) {
 unsigned cb_depth , cb_size ;
 if ( i == 2 ) {
 cb_size = get_bits_long ( & gb , 20 ) ;
 cb_depth = av_log2 ( cb_size - 1 ) + 1 ;
 }
 else {
 cb_depth = get_bits ( & gb , 4 ) ;
 if ( i == 0 ) {
 cb_size = 1 << cb_depth ;
 }
 else {
 cb_size = s -> num_superblocks << cb_depth ;
 }
 }
 av_free ( s -> codebooks [ i ] . blocks ) ;
 s -> codebooks [ i ] = unpack_codebook ( & gb , cb_depth , cb_size ) ;
 if ( ! s -> codebooks [ i ] . blocks ) return - 1 ;
 }
 }
 new_frame . reference = 3 ;
 if ( ff_get_buffer ( avctx , & new_frame ) ) {
 av_log ( avctx , AV_LOG_ERROR , "get_buffer() failed\n" ) ;
 return - 1 ;
 }
 new_frame_data = ( uint16_t * ) new_frame . data [ 0 ] ;
 new_stride = new_frame . linesize [ 0 ] / 2 ;
 old_frame_data = ( uint16_t * ) s -> frame . data [ 0 ] ;
 old_stride = s -> frame . linesize [ 0 ] / 2 ;
 for ( superblock_index = 0 ;
 superblock_index < s -> num_superblocks ;
 superblock_index ++ ) {
 MacroBlock mb ;
 SuperBlock sb ;
 unsigned multi_mask = 0 ;
 if ( skip == - 1 ) {
 skip = decode_skip_count ( & gb ) ;
 }
 if ( skip ) {
 copy_superblock ( new_frame_data , new_stride , old_frame_data , old_stride ) ;
 }
 else {
 copy_superblock ( sb . pixels , 8 , old_frame_data , old_stride ) ;
 while ( can_safely_read ( & gb , 1 ) && ! get_bits1 ( & gb ) ) {
 unsigned mask ;
 mb = decode_macroblock ( s , & gb , & cb_index , superblock_index ) ;
 mask = get_bits ( & gb , 16 ) ;
 multi_mask |= mask ;
 for ( i = 0 ;
 i < 16 ;
 i ++ ) {
 if ( mask & mask_matrix [ i ] ) {
 insert_mb_into_sb ( & sb , mb , i ) ;
 }
 }
 }
 if ( can_safely_read ( & gb , 1 ) && ! get_bits1 ( & gb ) ) {
 unsigned inv_mask = get_bits ( & gb , 4 ) ;
 for ( i = 0 ;
 i < 4 ;
 i ++ ) {
 if ( inv_mask & ( 1 << i ) ) {
 multi_mask ^= 0xF << i * 4 ;
 }
 else {
 multi_mask ^= get_bits ( & gb , 4 ) << i * 4 ;
 }
 }
 for ( i = 0 ;
 i < 16 ;
 i ++ ) {
 if ( multi_mask & mask_matrix [ i ] ) {
 if ( ! can_safely_read ( & gb , 1 ) ) break ;
 mb = decode_macroblock ( s , & gb , & cb_index , superblock_index ) ;
 insert_mb_into_sb ( & sb , mb , i ) ;
 }
 }
 }
 else if ( frame_flags & ( 1 << 16 ) ) {
 while ( can_safely_read ( & gb , 1 ) && ! get_bits1 ( & gb ) ) {
 mb = decode_macroblock ( s , & gb , & cb_index , superblock_index ) ;
 insert_mb_into_sb ( & sb , mb , get_bits ( & gb , 4 ) ) ;
 }
 }
 copy_superblock ( new_frame_data , new_stride , sb . pixels , 8 ) ;
 }
 superblock_col_index ++ ;
 new_frame_data += 8 ;
 if ( old_frame_data ) old_frame_data += 8 ;
 if ( superblock_col_index == superblocks_per_row ) {
 new_frame_data += new_stride * 8 - superblocks_per_row * 8 ;
 if ( old_frame_data ) old_frame_data += old_stride * 8 - superblocks_per_row * 8 ;
 superblock_col_index = 0 ;
 }
 skip -- ;
 }
 av_log ( NULL , AV_LOG_DEBUG , "Escape sizes: %i, %i, %i\n" , frame_size , buf_size , get_bits_count ( & gb ) / 8 ) ;
 if ( s -> frame . data [ 0 ] ) avctx -> release_buffer ( avctx , & s -> frame ) ;
 * ( AVFrame * ) data = s -> frame = new_frame ;
 * got_frame = 1 ;
 return frame_size ;
 }