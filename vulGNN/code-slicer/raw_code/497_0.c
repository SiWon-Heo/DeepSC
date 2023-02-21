MIMEHdrImpl * mime_hdr_clone ( MIMEHdrImpl * s_mh , HdrHeap * s_heap , HdrHeap * d_heap , bool inherit_strs ) {
 MIMEHdrImpl * d_mh ;
 d_mh = mime_hdr_create ( d_heap ) ;
 mime_hdr_copy_onto ( s_mh , s_heap , d_mh , d_heap , inherit_strs ) ;
 return d_mh ;
 }