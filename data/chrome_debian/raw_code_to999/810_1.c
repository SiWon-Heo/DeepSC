static int lexescape ( struct vars * v ) {
 chr c ;
 static const chr alert [ ] = {
 CHR ( 'a' ) , CHR ( 'l' ) , CHR ( 'e' ) , CHR ( 'r' ) , CHR ( 't' ) }
 ;
 static const chr esc [ ] = {
 CHR ( 'E' ) , CHR ( 'S' ) , CHR ( 'C' ) }
 ;
 const chr * save ;
 assert ( v -> cflags & REG_ADVF ) ;
 assert ( ! ATEOS ( ) ) ;
 c = * v -> now ++ ;
 if ( ! iscalnum ( c ) ) RETV ( PLAIN , c ) ;
 NOTE ( REG_UNONPOSIX ) ;
 switch ( c ) {
 case CHR ( 'a' ) : RETV ( PLAIN , chrnamed ( v , alert , ENDOF ( alert ) , CHR ( '\007' ) ) ) ;
 break ;
 case CHR ( 'A' ) : RETV ( SBEGIN , 0 ) ;
 break ;
 case CHR ( 'b' ) : RETV ( PLAIN , CHR ( '\b' ) ) ;
 break ;
 case CHR ( 'B' ) : RETV ( PLAIN , CHR ( '\\' ) ) ;
 break ;
 case CHR ( 'c' ) : NOTE ( REG_UUNPORT ) ;
 if ( ATEOS ( ) ) FAILW ( REG_EESCAPE ) ;
 RETV ( PLAIN , ( chr ) ( * v -> now ++ & 037 ) ) ;
 break ;
 case CHR ( 'd' ) : NOTE ( REG_ULOCALE ) ;
 RETV ( CCLASS , 'd' ) ;
 break ;
 case CHR ( 'D' ) : NOTE ( REG_ULOCALE ) ;
 RETV ( CCLASS , 'D' ) ;
 break ;
 case CHR ( 'e' ) : NOTE ( REG_UUNPORT ) ;
 RETV ( PLAIN , chrnamed ( v , esc , ENDOF ( esc ) , CHR ( '\033' ) ) ) ;
 break ;
 case CHR ( 'f' ) : RETV ( PLAIN , CHR ( '\f' ) ) ;
 break ;
 case CHR ( 'm' ) : RET ( '<' ) ;
 break ;
 case CHR ( 'M' ) : RET ( '>' ) ;
 break ;
 case CHR ( 'n' ) : RETV ( PLAIN , CHR ( '\n' ) ) ;
 break ;
 case CHR ( 'r' ) : RETV ( PLAIN , CHR ( '\r' ) ) ;
 break ;
 case CHR ( 's' ) : NOTE ( REG_ULOCALE ) ;
 RETV ( CCLASS , 's' ) ;
 break ;
 case CHR ( 'S' ) : NOTE ( REG_ULOCALE ) ;
 RETV ( CCLASS , 'S' ) ;
 break ;
 case CHR ( 't' ) : RETV ( PLAIN , CHR ( '\t' ) ) ;
 break ;
 case CHR ( 'u' ) : c = lexdigits ( v , 16 , 4 , 4 ) ;
 if ( ISERR ( ) ) FAILW ( REG_EESCAPE ) ;
 RETV ( PLAIN , c ) ;
 break ;
 case CHR ( 'U' ) : c = lexdigits ( v , 16 , 8 , 8 ) ;
 if ( ISERR ( ) ) FAILW ( REG_EESCAPE ) ;
 RETV ( PLAIN , c ) ;
 break ;
 case CHR ( 'v' ) : RETV ( PLAIN , CHR ( '\v' ) ) ;
 break ;
 case CHR ( 'w' ) : NOTE ( REG_ULOCALE ) ;
 RETV ( CCLASS , 'w' ) ;
 break ;
 case CHR ( 'W' ) : NOTE ( REG_ULOCALE ) ;
 RETV ( CCLASS , 'W' ) ;
 break ;
 case CHR ( 'x' ) : NOTE ( REG_UUNPORT ) ;
 c = lexdigits ( v , 16 , 1 , 255 ) ;
 if ( ISERR ( ) ) FAILW ( REG_EESCAPE ) ;
 RETV ( PLAIN , c ) ;
 break ;
 case CHR ( 'y' ) : NOTE ( REG_ULOCALE ) ;
 RETV ( WBDRY , 0 ) ;
 break ;
 case CHR ( 'Y' ) : NOTE ( REG_ULOCALE ) ;
 RETV ( NWBDRY , 0 ) ;
 break ;
 case CHR ( 'Z' ) : RETV ( SEND , 0 ) ;
 break ;
 case CHR ( '1' ) : case CHR ( '2' ) : case CHR ( '3' ) : case CHR ( '4' ) : case CHR ( '5' ) : case CHR ( '6' ) : case CHR ( '7' ) : case CHR ( '8' ) : case CHR ( '9' ) : save = v -> now ;
 v -> now -- ;
 c = lexdigits ( v , 10 , 1 , 255 ) ;
 if ( ISERR ( ) ) FAILW ( REG_EESCAPE ) ;
 if ( v -> now == save || ( ( int ) c > 0 && ( int ) c <= v -> nsubexp ) ) {
 NOTE ( REG_UBACKREF ) ;
 RETV ( BACKREF , ( chr ) c ) ;
 }
 v -> now = save ;
 case CHR ( '0' ) : NOTE ( REG_UUNPORT ) ;
 v -> now -- ;
 c = lexdigits ( v , 8 , 1 , 3 ) ;
 if ( ISERR ( ) ) FAILW ( REG_EESCAPE ) ;
 if ( c > 0xff ) {
 v -> now -- ;
 c >>= 3 ;
 }
 RETV ( PLAIN , c ) ;
 break ;
 default : assert ( iscalpha ( c ) ) ;
 FAILW ( REG_EESCAPE ) ;
 break ;
 }
 assert ( NOTREACHED ) ;
 }