static int testit ( struct test_case_struct * tc ) {
 int retval ;
 wordexp_t we , sav_we ;
 char * dummy ;
 int bzzzt = 0 ;
 int start_offs = 0 ;
 int i ;
 if ( tc -> env ) setenv ( "var" , tc -> env , 1 ) ;
 else unsetenv ( "var" ) ;
 if ( tc -> ifs ) setenv ( "IFS" , tc -> ifs , 1 ) ;
 else unsetenv ( "IFS" ) ;
 sav_we . we_wordc = 99 ;
 sav_we . we_wordv = & dummy ;
 sav_we . we_offs = 3 ;
 we = sav_we ;
 printf ( "Test %d (%s): " , ++ tests , tc -> words ) ;
 if ( tc -> flags & WRDE_APPEND ) {
 if ( wordexp ( "pre1 pre2" , & we , tc -> flags & ~ WRDE_APPEND ) != 0 ) {
 printf ( "FAILED setup\n" ) ;
 return 1 ;
 }
 }
 retval = wordexp ( tc -> words , & we , tc -> flags ) ;
 if ( tc -> flags & WRDE_DOOFFS ) start_offs = sav_we . we_offs ;
 if ( retval != tc -> retval || ( retval == 0 && we . we_wordc != tc -> wordc ) ) bzzzt = 1 ;
 else if ( retval == 0 ) {
 for ( i = 0 ;
 i < start_offs ;
 ++ i ) if ( we . we_wordv [ i ] != NULL ) {
 bzzzt = 1 ;
 break ;
 }
 for ( i = 0 ;
 i < we . we_wordc ;
 ++ i ) if ( we . we_wordv [ i + start_offs ] == NULL || strcmp ( tc -> wordv [ i ] , we . we_wordv [ i + start_offs ] ) != 0 ) {
 bzzzt = 1 ;
 break ;
 }
 }
 if ( bzzzt ) {
 printf ( "FAILED\n" ) ;
 printf ( "Test words: <%s>, need retval %d, wordc %Zd\n" , tc -> words , tc -> retval , tc -> wordc ) ;
 if ( start_offs != 0 ) printf ( "(preceded by %d NULLs)\n" , start_offs ) ;
 printf ( "Got retval %d, wordc %Zd: " , retval , we . we_wordc ) ;
 if ( retval == 0 || retval == WRDE_NOSPACE ) {
 for ( i = 0 ;
 i < we . we_wordc + start_offs ;
 ++ i ) if ( we . we_wordv [ i ] == NULL ) printf ( "NULL " ) ;
 else printf ( "<%s> " , we . we_wordv [ i ] ) ;
 }
 printf ( "\n" ) ;
 }
 else if ( retval != 0 && retval != WRDE_NOSPACE && ( we . we_wordc != sav_we . we_wordc || we . we_wordv != sav_we . we_wordv || we . we_offs != sav_we . we_offs ) ) {
 bzzzt = 1 ;
 printf ( "FAILED to restore wordexp_t members\n" ) ;
 }
 else printf ( "OK\n" ) ;
 if ( retval == 0 || retval == WRDE_NOSPACE ) wordfree ( & we ) ;
 return bzzzt ;
 }