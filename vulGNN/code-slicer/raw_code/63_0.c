int jbig2_halftone_region ( Jbig2Ctx * ctx , Jbig2Segment * segment , const byte * segment_data ) {
 int offset = 0 ;
 Jbig2RegionSegmentInfo region_info ;
 Jbig2HalftoneRegionParams params ;
 Jbig2Image * image = NULL ;
 Jbig2ArithCx * GB_stats = NULL ;
 int code = 0 ;
 if ( segment -> data_length < 17 ) goto too_short ;
 jbig2_get_region_segment_info ( & region_info , segment_data ) ;
 offset += 17 ;
 if ( segment -> data_length < 18 ) goto too_short ;
 params . flags = segment_data [ offset ] ;
 params . HMMR = params . flags & 1 ;
 params . HTEMPLATE = ( params . flags & 6 ) >> 1 ;
 params . HENABLESKIP = ( params . flags & 8 ) >> 3 ;
 params . op = ( Jbig2ComposeOp ) ( ( params . flags & 0x70 ) >> 4 ) ;
 params . HDEFPIXEL = ( params . flags & 0x80 ) >> 7 ;
 offset += 1 ;
 jbig2_error ( ctx , JBIG2_SEVERITY_INFO , segment -> number , "halftone region: %d x %d @ (%x,%d) flags=%02x" , region_info . width , region_info . height , region_info . x , region_info . y , params . flags ) ;
 if ( params . HMMR && params . HTEMPLATE ) {
 jbig2_error ( ctx , JBIG2_SEVERITY_WARNING , segment -> number , "HTEMPLATE is %d when HMMR is %d, contrary to spec" , params . HTEMPLATE , params . HMMR ) ;
 }
 if ( params . HMMR && params . HENABLESKIP ) {
 jbig2_error ( ctx , JBIG2_SEVERITY_WARNING , segment -> number , "HENABLESKIP is %d when HMMR is %d, contrary to spec" , params . HENABLESKIP , params . HMMR ) ;
 }
 if ( segment -> data_length - offset < 16 ) goto too_short ;
 params . HGW = jbig2_get_uint32 ( segment_data + offset ) ;
 params . HGH = jbig2_get_uint32 ( segment_data + offset + 4 ) ;
 params . HGX = jbig2_get_int32 ( segment_data + offset + 8 ) ;
 params . HGY = jbig2_get_int32 ( segment_data + offset + 12 ) ;
 offset += 16 ;
 if ( segment -> data_length - offset < 4 ) goto too_short ;
 params . HRX = jbig2_get_uint16 ( segment_data + offset ) ;
 params . HRY = jbig2_get_uint16 ( segment_data + offset + 2 ) ;
 offset += 4 ;
 jbig2_error ( ctx , JBIG2_SEVERITY_INFO , segment -> number , " grid %d x %d @ (%d.%d,%d.%d) vector (%d.%d,%d.%d)" , params . HGW , params . HGH , params . HGX >> 8 , params . HGX & 0xff , params . HGY >> 8 , params . HGY & 0xff , params . HRX >> 8 , params . HRX & 0xff , params . HRY >> 8 , params . HRY & 0xff ) ;
 if ( ! params . HMMR ) {
 int stats_size = jbig2_generic_stats_size ( ctx , params . HTEMPLATE ) ;
 GB_stats = jbig2_new ( ctx , Jbig2ArithCx , stats_size ) ;
 if ( GB_stats == NULL ) {
 return jbig2_error ( ctx , JBIG2_SEVERITY_FATAL , segment -> number , "failed to allocate GB_stats in halftone region" ) ;
 }
 memset ( GB_stats , 0 , stats_size ) ;
 }
 image = jbig2_image_new ( ctx , region_info . width , region_info . height ) ;
 if ( image == NULL ) {
 jbig2_free ( ctx -> allocator , GB_stats ) ;
 return jbig2_error ( ctx , JBIG2_SEVERITY_WARNING , segment -> number , "unable to allocate halftone image" ) ;
 }
 code = jbig2_decode_halftone_region ( ctx , segment , & params , segment_data + offset , segment -> data_length - offset , image , GB_stats ) ;
 if ( ! params . HMMR ) {
 jbig2_free ( ctx -> allocator , GB_stats ) ;
 }
 jbig2_page_add_result ( ctx , & ctx -> pages [ ctx -> current_page ] , image , region_info . x , region_info . y , region_info . op ) ;
 jbig2_image_release ( ctx , image ) ;
 return code ;
 too_short : return jbig2_error ( ctx , JBIG2_SEVERITY_FATAL , segment -> number , "Segment too short" ) ;
 }