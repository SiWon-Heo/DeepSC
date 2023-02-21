static void fill_vaapi_RefPicList ( VAPictureH264 RefPicList [ 32 ] , Picture * ref_list , unsigned int ref_count ) {
 unsigned int i , n = 0 ;
 for ( i = 0 ;
 i < ref_count ;
 i ++ ) if ( ref_list [ i ] . f . reference ) fill_vaapi_pic ( & RefPicList [ n ++ ] , & ref_list [ i ] , 0 ) ;
 for ( ;
 n < 32 ;
 n ++ ) init_vaapi_pic ( & RefPicList [ n ] ) ;
 }