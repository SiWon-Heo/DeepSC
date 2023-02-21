static int dprintf_formatf ( void * data , int ( * stream ) ( int , FILE * ) , const char * format , va_list ap_save ) {
 const char * digits = lower_digits ;
 char * f ;
 int done = 0 ;
 long param ;
 long param_num = 0 ;
 va_stack_t vto [ MAX_PARAMETERS ] ;
 char * endpos [ MAX_PARAMETERS ] ;
 char * * end ;
 char work [ BUFFSIZE ] ;
 va_stack_t * p ;
 char * workend = & work [ sizeof ( work ) - 2 ] ;
 if ( dprintf_Pass1 ( format , vto , endpos , ap_save ) ) return - 1 ;
 end = & endpos [ 0 ] ;
 f = ( char * ) format ;
 while ( * f != '\0' ) {
 int is_alt ;
 long width ;
 long prec ;
 int is_neg ;
 long base ;
 mp_uintmax_t num ;
 mp_intmax_t signed_num ;
 char * w ;
 if ( * f != '%' ) {
 do {
 OUTCHAR ( * f ) ;
 }
 while ( * ++ f && ( '%' != * f ) ) ;
 continue ;
 }
 ++ f ;
 if ( * f == '%' ) {
 ++ f ;
 OUTCHAR ( '%' ) ;
 continue ;
 }
 param = dprintf_DollarString ( f , & f ) ;
 if ( ! param ) param = param_num ;
 else -- param ;
 param_num ++ ;
 p = & vto [ param ] ;
 if ( p -> flags & FLAGS_WIDTHPARAM ) {
 width = ( long ) vto [ p -> width ] . data . num . as_signed ;
 param_num ++ ;
 if ( width < 0 ) {
 width = - width ;
 p -> flags |= FLAGS_LEFT ;
 p -> flags &= ~ FLAGS_PAD_NIL ;
 }
 }
 else width = p -> width ;
 if ( p -> flags & FLAGS_PRECPARAM ) {
 prec = ( long ) vto [ p -> precision ] . data . num . as_signed ;
 param_num ++ ;
 if ( prec < 0 ) prec = - 1 ;
 }
 else if ( p -> flags & FLAGS_PREC ) prec = p -> precision ;
 else prec = - 1 ;
 is_alt = ( p -> flags & FLAGS_ALT ) ? 1 : 0 ;
 switch ( p -> type ) {
 case FORMAT_INT : num = p -> data . num . as_unsigned ;
 if ( p -> flags & FLAGS_CHAR ) {
 if ( ! ( p -> flags & FLAGS_LEFT ) ) while ( -- width > 0 ) OUTCHAR ( ' ' ) ;
 OUTCHAR ( ( char ) num ) ;
 if ( p -> flags & FLAGS_LEFT ) while ( -- width > 0 ) OUTCHAR ( ' ' ) ;
 break ;
 }
 if ( p -> flags & FLAGS_OCTAL ) {
 base = 8 ;
 goto unsigned_number ;
 }
 else if ( p -> flags & FLAGS_HEX ) {
 digits = ( p -> flags & FLAGS_UPPER ) ? upper_digits : lower_digits ;
 base = 16 ;
 goto unsigned_number ;
 }
 else if ( p -> flags & FLAGS_UNSIGNED ) {
 base = 10 ;
 goto unsigned_number ;
 }
 base = 10 ;
 is_neg = ( p -> data . num . as_signed < ( mp_intmax_t ) 0 ) ? 1 : 0 ;
 if ( is_neg ) {
 signed_num = p -> data . num . as_signed + ( mp_intmax_t ) 1 ;
 signed_num = - signed_num ;
 num = ( mp_uintmax_t ) signed_num ;
 num += ( mp_uintmax_t ) 1 ;
 }
 goto number ;
 unsigned_number : is_neg = 0 ;
 number : if ( prec == - 1 ) prec = 1 ;
 w = workend ;
 while ( num > 0 ) {
 * w -- = digits [ num % base ] ;
 num /= base ;
 }
 width -= ( long ) ( workend - w ) ;
 prec -= ( long ) ( workend - w ) ;
 if ( is_alt && base == 8 && prec <= 0 ) {
 * w -- = '0' ;
 -- width ;
 }
 if ( prec > 0 ) {
 width -= prec ;
 while ( prec -- > 0 ) * w -- = '0' ;
 }
 if ( is_alt && base == 16 ) width -= 2 ;
 if ( is_neg || ( p -> flags & FLAGS_SHOWSIGN ) || ( p -> flags & FLAGS_SPACE ) ) -- width ;
 if ( ! ( p -> flags & FLAGS_LEFT ) && ! ( p -> flags & FLAGS_PAD_NIL ) ) while ( width -- > 0 ) OUTCHAR ( ' ' ) ;
 if ( is_neg ) OUTCHAR ( '-' ) ;
 else if ( p -> flags & FLAGS_SHOWSIGN ) OUTCHAR ( '+' ) ;
 else if ( p -> flags & FLAGS_SPACE ) OUTCHAR ( ' ' ) ;
 if ( is_alt && base == 16 ) {
 OUTCHAR ( '0' ) ;
 if ( p -> flags & FLAGS_UPPER ) OUTCHAR ( 'X' ) ;
 else OUTCHAR ( 'x' ) ;
 }
 if ( ! ( p -> flags & FLAGS_LEFT ) && ( p -> flags & FLAGS_PAD_NIL ) ) while ( width -- > 0 ) OUTCHAR ( '0' ) ;
 while ( ++ w <= workend ) {
 OUTCHAR ( * w ) ;
 }
 if ( p -> flags & FLAGS_LEFT ) while ( width -- > 0 ) OUTCHAR ( ' ' ) ;
 break ;
 case FORMAT_STRING : {
 static const char null [ ] = "(nil)" ;
 const char * str ;
 size_t len ;
 str = ( char * ) p -> data . str ;
 if ( str == NULL ) {
 if ( prec == - 1 || prec >= ( long ) sizeof ( null ) - 1 ) {
 str = null ;
 len = sizeof ( null ) - 1 ;
 p -> flags &= ( ~ FLAGS_ALT ) ;
 }
 else {
 str = "" ;
 len = 0 ;
 }
 }
 else if ( prec != - 1 ) len = ( size_t ) prec ;
 else len = strlen ( str ) ;
 width -= ( len > LONG_MAX ) ? LONG_MAX : ( long ) len ;
 if ( p -> flags & FLAGS_ALT ) OUTCHAR ( '"' ) ;
 if ( ! ( p -> flags & FLAGS_LEFT ) ) while ( width -- > 0 ) OUTCHAR ( ' ' ) ;
 while ( ( len -- > 0 ) && * str ) OUTCHAR ( * str ++ ) ;
 if ( p -> flags & FLAGS_LEFT ) while ( width -- > 0 ) OUTCHAR ( ' ' ) ;
 if ( p -> flags & FLAGS_ALT ) OUTCHAR ( '"' ) ;
 }
 break ;
 case FORMAT_PTR : {
 void * ptr ;
 ptr = ( void * ) p -> data . ptr ;
 if ( ptr != NULL ) {
 base = 16 ;
 digits = ( p -> flags & FLAGS_UPPER ) ? upper_digits : lower_digits ;
 is_alt = 1 ;
 num = ( size_t ) ptr ;
 is_neg = 0 ;
 goto number ;
 }
 else {
 static const char strnil [ ] = "(nil)" ;
 const char * point ;
 width -= ( long ) ( sizeof ( strnil ) - 1 ) ;
 if ( p -> flags & FLAGS_LEFT ) while ( width -- > 0 ) OUTCHAR ( ' ' ) ;
 for ( point = strnil ;
 * point != '\0' ;
 ++ point ) OUTCHAR ( * point ) ;
 if ( ! ( p -> flags & FLAGS_LEFT ) ) while ( width -- > 0 ) OUTCHAR ( ' ' ) ;
 }
 }
 break ;
 case FORMAT_DOUBLE : {
 char formatbuf [ 32 ] = "%" ;
 char * fptr = & formatbuf [ 1 ] ;
 size_t left = sizeof ( formatbuf ) - strlen ( formatbuf ) ;
 int len ;
 width = - 1 ;
 if ( p -> flags & FLAGS_WIDTH ) width = p -> width ;
 else if ( p -> flags & FLAGS_WIDTHPARAM ) width = ( long ) vto [ p -> width ] . data . num . as_signed ;
 prec = - 1 ;
 if ( p -> flags & FLAGS_PREC ) prec = p -> precision ;
 else if ( p -> flags & FLAGS_PRECPARAM ) prec = ( long ) vto [ p -> precision ] . data . num . as_signed ;
 if ( p -> flags & FLAGS_LEFT ) * fptr ++ = '-' ;
 if ( p -> flags & FLAGS_SHOWSIGN ) * fptr ++ = '+' ;
 if ( p -> flags & FLAGS_SPACE ) * fptr ++ = ' ' ;
 if ( p -> flags & FLAGS_ALT ) * fptr ++ = '#' ;
 * fptr = 0 ;
 if ( width >= 0 ) {
 len = curl_msnprintf ( fptr , left , "%ld" , width ) ;
 fptr += len ;
 left -= len ;
 }
 if ( prec >= 0 ) {
 len = curl_msnprintf ( fptr , left , ".%ld" , prec ) ;
 fptr += len ;
 }
 if ( p -> flags & FLAGS_LONG ) * fptr ++ = 'l' ;
 if ( p -> flags & FLAGS_FLOATE ) * fptr ++ = ( char ) ( ( p -> flags & FLAGS_UPPER ) ? 'E' : 'e' ) ;
 else if ( p -> flags & FLAGS_FLOATG ) * fptr ++ = ( char ) ( ( p -> flags & FLAGS_UPPER ) ? 'G' : 'g' ) ;
 else * fptr ++ = 'f' ;
 * fptr = 0 ;
 ( sprintf ) ( work , formatbuf , p -> data . dnum ) ;
 for ( fptr = work ;
 * fptr ;
 fptr ++ ) OUTCHAR ( * fptr ) ;
 }
 break ;
 case FORMAT_INTPTR : # ifdef HAVE_LONG_LONG_TYPE if ( p -> flags & FLAGS_LONGLONG ) * ( LONG_LONG_TYPE * ) p -> data . ptr = ( LONG_LONG_TYPE ) done ;
 else # endif if ( p -> flags & FLAGS_LONG ) * ( long * ) p -> data . ptr = ( long ) done ;
 else if ( ! ( p -> flags & FLAGS_SHORT ) ) * ( int * ) p -> data . ptr = ( int ) done ;
 else * ( short * ) p -> data . ptr = ( short ) done ;
 break ;
 default : break ;
 }
 f = * end ++ ;
 }
 return done ;
 }