int http_hdr_print ( HdrHeap * heap , HTTPHdrImpl * hdr , char * buf , int bufsize , int * bufindex , int * dumpoffset ) {
 # define TRY ( x ) if ( ! x ) return 0 int tmplen , hdrstat ;
 char tmpbuf [ 32 ] ;
 char * p ;
 ink_assert ( ( hdr -> m_polarity == HTTP_TYPE_REQUEST ) || ( hdr -> m_polarity == HTTP_TYPE_RESPONSE ) ) ;
 if ( hdr -> m_polarity == HTTP_TYPE_REQUEST ) {
 if ( hdr -> u . req . m_ptr_method == nullptr ) {
 return 1 ;
 }
 if ( ( buf != nullptr ) && ( * dumpoffset == 0 ) && ( bufsize - * bufindex >= hdr -> u . req . m_len_method + 1 ) ) {
 p = buf + * bufindex ;
 memcpy ( p , hdr -> u . req . m_ptr_method , hdr -> u . req . m_len_method ) ;
 p += hdr -> u . req . m_len_method ;
 * p ++ = ' ' ;
 * bufindex += hdr -> u . req . m_len_method + 1 ;
 if ( hdr -> u . req . m_url_impl ) {
 TRY ( url_print ( hdr -> u . req . m_url_impl , buf , bufsize , bufindex , dumpoffset ) ) ;
 if ( bufsize - * bufindex >= 1 ) {
 if ( hdr -> u . req . m_method_wks_idx == HTTP_WKSIDX_CONNECT ) {
 * bufindex -= 1 ;
 }
 p = buf + * bufindex ;
 * p ++ = ' ' ;
 * bufindex += 1 ;
 }
 else {
 return 0 ;
 }
 }
 if ( bufsize - * bufindex >= 9 ) {
 http_hdr_version_to_string ( hdr -> m_version , p ) ;
 * bufindex += 9 - 1 ;
 }
 else {
 TRY ( http_version_print ( hdr -> m_version , buf , bufsize , bufindex , dumpoffset ) ) ;
 }
 if ( bufsize - * bufindex >= 2 ) {
 p = buf + * bufindex ;
 * p ++ = '\r' ;
 * p ++ = '\n' ;
 * bufindex += 2 ;
 }
 else {
 TRY ( mime_mem_print ( "\r\n" , 2 , buf , bufsize , bufindex , dumpoffset ) ) ;
 }
 TRY ( mime_hdr_print ( heap , hdr -> m_fields_impl , buf , bufsize , bufindex , dumpoffset ) ) ;
 }
 else {
 TRY ( mime_mem_print ( hdr -> u . req . m_ptr_method , hdr -> u . req . m_len_method , buf , bufsize , bufindex , dumpoffset ) ) ;
 TRY ( mime_mem_print ( " " , 1 , buf , bufsize , bufindex , dumpoffset ) ) ;
 if ( hdr -> u . req . m_url_impl ) {
 TRY ( url_print ( hdr -> u . req . m_url_impl , buf , bufsize , bufindex , dumpoffset ) ) ;
 TRY ( mime_mem_print ( " " , 1 , buf , bufsize , bufindex , dumpoffset ) ) ;
 }
 TRY ( http_version_print ( hdr -> m_version , buf , bufsize , bufindex , dumpoffset ) ) ;
 TRY ( mime_mem_print ( "\r\n" , 2 , buf , bufsize , bufindex , dumpoffset ) ) ;
 TRY ( mime_hdr_print ( heap , hdr -> m_fields_impl , buf , bufsize , bufindex , dumpoffset ) ) ;
 }
 }
 else {
 if ( ( buf != nullptr ) && ( * dumpoffset == 0 ) && ( bufsize - * bufindex >= 9 + 6 + 1 ) ) {
 p = buf + * bufindex ;
 http_hdr_version_to_string ( hdr -> m_version , p ) ;
 p += 8 ;
 * p ++ = ' ' ;
 * bufindex += 9 ;
 hdrstat = http_hdr_status_get ( hdr ) ;
 if ( hdrstat == 200 ) {
 * p ++ = '2' ;
 * p ++ = '0' ;
 * p ++ = '0' ;
 tmplen = 3 ;
 }
 else {
 tmplen = mime_format_int ( p , hdrstat , ( bufsize - ( p - buf ) ) ) ;
 ink_assert ( tmplen <= 6 ) ;
 p += tmplen ;
 }
 * p ++ = ' ' ;
 * bufindex += tmplen + 1 ;
 if ( hdr -> u . resp . m_ptr_reason ) {
 TRY ( mime_mem_print ( hdr -> u . resp . m_ptr_reason , hdr -> u . resp . m_len_reason , buf , bufsize , bufindex , dumpoffset ) ) ;
 }
 if ( bufsize - * bufindex >= 2 ) {
 p = buf + * bufindex ;
 * p ++ = '\r' ;
 * p ++ = '\n' ;
 * bufindex += 2 ;
 }
 else {
 TRY ( mime_mem_print ( "\r\n" , 2 , buf , bufsize , bufindex , dumpoffset ) ) ;
 }
 TRY ( mime_hdr_print ( heap , hdr -> m_fields_impl , buf , bufsize , bufindex , dumpoffset ) ) ;
 }
 else {
 TRY ( http_version_print ( hdr -> m_version , buf , bufsize , bufindex , dumpoffset ) ) ;
 TRY ( mime_mem_print ( " " , 1 , buf , bufsize , bufindex , dumpoffset ) ) ;
 tmplen = mime_format_int ( tmpbuf , http_hdr_status_get ( hdr ) , sizeof ( tmpbuf ) ) ;
 TRY ( mime_mem_print ( tmpbuf , tmplen , buf , bufsize , bufindex , dumpoffset ) ) ;
 TRY ( mime_mem_print ( " " , 1 , buf , bufsize , bufindex , dumpoffset ) ) ;
 if ( hdr -> u . resp . m_ptr_reason ) {
 TRY ( mime_mem_print ( hdr -> u . resp . m_ptr_reason , hdr -> u . resp . m_len_reason , buf , bufsize , bufindex , dumpoffset ) ) ;
 }
 TRY ( mime_mem_print ( "\r\n" , 2 , buf , bufsize , bufindex , dumpoffset ) ) ;
 TRY ( mime_hdr_print ( heap , hdr -> m_fields_impl , buf , bufsize , bufindex , dumpoffset ) ) ;
 }
 }
 return 1 ;
 # undef TRY }