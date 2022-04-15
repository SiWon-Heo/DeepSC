int jbig2_complete_page ( Jbig2Ctx * ctx ) {
 int code = 0 ;
 if ( ctx -> segment_index != ctx -> n_segments ) {
 Jbig2Segment * segment = ctx -> segments [ ctx -> segment_index ] ;
 if ( ( segment -> data_length & 0xffffffff ) == 0xffffffff ) {
 jbig2_error ( ctx , JBIG2_SEVERITY_WARNING , segment -> number , "File has an invalid segment data length!" " Trying to decode using the available data." ) ;
 segment -> data_length = ctx -> buf_wr_ix - ctx -> buf_rd_ix ;
 code = jbig2_parse_segment ( ctx , segment , ctx -> buf + ctx -> buf_rd_ix ) ;
 ctx -> buf_rd_ix += segment -> data_length ;
 ctx -> segment_index ++ ;
 }
 }
 if ( ctx -> pages [ ctx -> current_page ] . image != NULL ) {
 ctx -> pages [ ctx -> current_page ] . state = JBIG2_PAGE_COMPLETE ;
 }
 return code ;
 }