int encode_offset ( char * string , struct filter_op * fop ) {
 char * str , * p , * q , * tok ;
 int ret ;
 memset ( fop , 0 , sizeof ( struct filter_op ) ) ;
 str = strdup ( string ) ;
 p = ec_strtok ( str , "." , & tok ) ;
 q = ec_strtok ( NULL , "." , & tok ) ;
 if ( q == NULL ) return - E_NOTFOUND ;
 ret = get_virtualpointer ( p , q , & fop -> op . test . level , & fop -> op . test . offset , & fop -> op . test . size ) ;
 SAFE_FREE ( str ) ;
 return ret ;
 }