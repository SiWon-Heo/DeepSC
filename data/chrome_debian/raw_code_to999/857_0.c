static unsigned char * extra_next_record ( struct ctl_extr_rec * ctl , int length ) {
 int cur_len = ctl -> cur_len ;
 extra_close_record ( ctl , RR_CE_SIZE ) ;
 ctl -> use_extr = 1 ;
 if ( ctl -> bp != NULL ) {
 unsigned char * p ;
 ctl -> ce_ptr = & ctl -> bp [ cur_len + 1 ] ;
 p = extra_get_record ( ctl -> isoent , & ctl -> limit , & ctl -> extr_off , & ctl -> extr_loc ) ;
 ctl -> bp = p - 1 ;
 }
 else ( void ) extra_get_record ( ctl -> isoent , & ctl -> limit , NULL , NULL ) ;
 ctl -> cur_len = 0 ;
 if ( extra_space ( ctl ) < length ) ( void ) extra_next_record ( ctl , length ) ;
 return ( ctl -> bp ) ;
 }