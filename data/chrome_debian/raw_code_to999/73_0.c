int jbig2_end_of_page ( Jbig2Ctx * ctx , Jbig2Segment * segment , const uint8_t * segment_data ) {
 uint32_t page_number = ctx -> pages [ ctx -> current_page ] . number ;
 if ( segment -> page_association != page_number ) {
 jbig2_error ( ctx , JBIG2_SEVERITY_WARNING , segment -> number , "end of page marker for page %d doesn't match current page number %d" , segment -> page_association , page_number ) ;
 }
 jbig2_error ( ctx , JBIG2_SEVERITY_INFO , segment -> number , "end of page %d" , page_number ) ;
 jbig2_complete_page ( ctx ) ;
 # ifdef OUTPUT_PBM jbig2_image_write_pbm ( ctx -> pages [ ctx -> current_page ] . image , stdout ) ;
 # endif return 0 ;
 }