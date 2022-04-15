static inline MIMEField * rebase ( MIMEField * dest_ptr , void * dest_base , void * src_base ) {
 return reinterpret_cast < MIMEField * > ( reinterpret_cast < char * > ( dest_ptr ) + ( static_cast < char * > ( dest_base ) - static_cast < char * > ( src_base ) ) ) ;
 }