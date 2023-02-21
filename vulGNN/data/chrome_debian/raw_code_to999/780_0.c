static void set_VD_bp ( unsigned char * bp , enum VD_type type , unsigned char ver ) {
 bp [ 1 ] = ( unsigned char ) type ;
 memcpy ( bp + 2 , "CD001" , 5 ) ;
 bp [ 7 ] = ver ;
 }