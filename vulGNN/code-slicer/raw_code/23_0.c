void jbig2_word_stream_buf_free ( Jbig2Ctx * ctx , Jbig2WordStream * ws ) {
 jbig2_free ( ctx -> allocator , ws ) ;
 }