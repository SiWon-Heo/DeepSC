HTTPHdrImpl * http_hdr_clone ( HTTPHdrImpl * s_hh , HdrHeap * s_heap , HdrHeap * d_heap ) {
 HTTPHdrImpl * d_hh ;
 d_hh = http_hdr_create ( d_heap , s_hh -> m_polarity ) ;
 http_hdr_copy_onto ( s_hh , s_heap , d_hh , d_heap , ( ( s_heap != d_heap ) ? true : false ) ) ;
 return ( d_hh ) ;
 }