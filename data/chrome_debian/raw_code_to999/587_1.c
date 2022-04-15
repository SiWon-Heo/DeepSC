void ff_copy_picture ( Picture * dst , Picture * src ) {
 * dst = * src ;
 dst -> f . type = FF_BUFFER_TYPE_COPY ;
 }