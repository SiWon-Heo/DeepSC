static void dovec ( struct vars * v , struct cvec * cv , struct state * lp , struct state * rp ) {
 chr ch , from , to ;
 const chr * p ;
 int i ;
 for ( p = cv -> chrs , i = cv -> nchrs ;
 i > 0 ;
 p ++ , i -- ) {
 ch = * p ;
 newarc ( v -> nfa , PLAIN , subcolor ( v -> cm , ch ) , lp , rp ) ;
 }
 for ( p = cv -> ranges , i = cv -> nranges ;
 i > 0 ;
 p += 2 , i -- ) {
 from = * p ;
 to = * ( p + 1 ) ;
 if ( from <= to ) subrange ( v , from , to , lp , rp ) ;
 }
 }