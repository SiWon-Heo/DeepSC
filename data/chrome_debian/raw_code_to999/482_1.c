static void cond_release_buffer ( AVFrame * pic ) {
 if ( pic -> data [ 0 ] ) {
 av_freep ( & pic -> data [ 0 ] ) ;
 av_free ( pic -> data [ 1 ] ) ;
 }
 }