digraph "Code Property Graph" {
	1 [label="1	uint8_t * * jbig2_decode_gray_scale_image ( Jbig2Ctx * ctx , Jbig2Segment * segment , const byte * data , const size_t size , bool GSMMR , uint32_t GSW , uint32_t GSH , uint32_t GSBPP , bool GSUSESKIP , Jbig2Image * GSKIP , int GSTEMPLATE , Jbig2ArithCx * GB_stats ) {" shape=box]
	1 -> 12 [color=blue]
	1 -> 16 [color=blue]
	1 -> 18 [color=blue]
	1 -> 20 [color=blue]
	1 -> 24 [color=blue]
	1 -> 26 [color=blue]
	1 -> 30 [color=blue]
	1 -> 31 [color=blue]
	1 -> 33 [color=blue]
	1 -> 34 [color=blue]
	1 -> 42 [color=blue]
	1 -> 43 [color=blue]
	1 -> 46 [color=blue]
	1 -> 48 [color=blue]
	1 -> 51 [color=blue]
	1 -> 53 [color=blue]
	1 -> 56 [color=blue]
	1 -> 59 [color=blue]
	1 -> 62 [color=blue]
	1 -> 64 [color=blue]
	1 -> 65 [color=blue]
	1 -> 68 [color=blue]
	1 -> 71 [color=blue]
	1 -> 76 [color=blue]
	1 -> 82 [color=blue]
	1 -> 86 [color=blue]
	1 -> 88 [color=blue]
	1 -> 90 [color=blue]
	1 -> 94 [color=blue]
	1 -> 96 [color=blue]
	1 -> 102 [color=blue]
	1 -> 105 [color=blue]
	1 -> 109 [color=blue]
	1 -> 113 [color=blue]
	1 -> 114 [color=blue]
	1 -> 115 [color=blue]
	1 -> 118 [color=blue]
	1 -> 119 [color=blue]
	1 -> 120 [color=blue]
	2 [label="2	uint8_t * * GSVALS = NULL ;" shape=box]
	2 -> 96 [color=blue]
	2 -> 121 [color=blue]
	2 -> 8 [color=blue]
	2 -> 107 [color=blue]
	2 -> 110 [color=blue]
	2 -> 81 [color=blue]
	2 -> 88 [color=blue]
	2 -> 89 [color=blue]
	2 -> 94 [color=blue]
	3 [label="3	size_t consumed_bytes = 0 ;" shape=box]
	3 -> 65 [color=blue]
	4 [label="4	int i , j , code , stride ;" shape=box]
	5 [label="5	int x , y ;" shape=box]
	6 [label="6	Jbig2Image * * GSPLANES ;" shape=box]
	6 -> 65 [color=blue]
	6 -> 56 [color=blue]
	6 -> 68 [color=blue]
	6 -> 74 [color=blue]
	6 -> 11 [color=blue]
	6 -> 43 [color=blue]
	6 -> 77 [color=blue]
	6 -> 110 [color=blue]
	6 -> 18 [color=blue]
	6 -> 19 [color=blue]
	6 -> 119 [color=blue]
	6 -> 24 [color=blue]
	6 -> 26 [color=blue]
	6 -> 120 [color=blue]
	7 [label="7	Jbig2GenericRegionParams rparams ;" shape=box]
	7 -> 30 [color=blue]
	8 [label="8	Jbig2WordStream * ws = NULL ;" shape=box]
	8 -> 9 [color=blue]
	8 -> 115 [color=blue]
	9 [label="9	Jbig2ArithState * as = NULL ;" shape=box]
	9 -> 97 [color=blue]
	9 -> 68 [color=blue]
	9 -> 11 [color=blue]
	9 -> 13 [color=blue]
	9 -> 47 [color=blue]
	9 -> 81 [color=blue]
	9 -> 114 [color=blue]
	9 -> 19 [color=blue]
	9 -> 52 [color=blue]
	9 -> 89 [color=blue]
	9 -> 27 [color=blue]
	10 [label="10	GSPLANES = jbig2_new ( ctx , Jbig2Image * , GSBPP ) ;" shape=box]
	11 [label="11	if ( GSPLANES == NULL ) {" shape=box]
	11 -> 16 [color=red]
	11 -> 12 [color=red]
	11 -> 13 [color=red]
	11 -> 15 [color=red]
	12 [label="12	jbig2_error ( ctx , JBIG2_SEVERITY_FATAL , segment -> number , \"failed to allocate %d bytes for GSPLANES\" , GSBPP ) ;" shape=box]
	13 [label="13	return NULL ;" shape=box]
	15 [label="15	for ( i = 0 ;" shape=box]
	15 -> 16 [color=blue]
	15 -> 18 [color=blue]
	15 -> 19 [color=blue]
	15 -> 21 [color=blue]
	16 [label="16	i < GSBPP ;" shape=box]
	16 -> 18 [color=red]
	16 -> 19 [color=red]
	16 -> 30 [color=red]
	16 -> 31 [color=red]
	16 -> 32 [color=red]
	16 -> 33 [color=red]
	16 -> 34 [color=red]
	16 -> 35 [color=red]
	16 -> 36 [color=red]
	16 -> 37 [color=red]
	16 -> 38 [color=red]
	16 -> 39 [color=red]
	16 -> 40 [color=red]
	16 -> 41 [color=red]
	16 -> 42 [color=red]
	16 -> 113 [color=red]
	16 -> 117 [color=red]
	16 -> 118 [color=red]
	16 -> 120 [color=red]
	16 -> 121 [color=red]
	17 [label="17	++ i ) {" shape=box]
	18 [label="18	GSPLANES [ i ] = jbig2_image_new ( ctx , GSW , GSH ) ;" shape=box]
	18 -> 65 [color=blue]
	18 -> 68 [color=blue]
	18 -> 74 [color=blue]
	18 -> 43 [color=blue]
	18 -> 77 [color=blue]
	18 -> 110 [color=blue]
	18 -> 19 [color=blue]
	18 -> 119 [color=blue]
	18 -> 24 [color=blue]
	18 -> 56 [color=blue]
	19 [label="19	if ( GSPLANES [ i ] == NULL ) {" shape=box]
	19 -> 16 [color=red]
	19 -> 17 [color=red]
	19 -> 20 [color=red]
	19 -> 21 [color=red]
	19 -> 22 [color=red]
	19 -> 26 [color=red]
	19 -> 27 [color=red]
	20 [label="20	jbig2_error ( ctx , JBIG2_SEVERITY_FATAL , segment -> number , \"failed to allocate %dx%d image for GSPLANES\" , GSW , GSH ) ;" shape=box]
	21 [label="21	for ( j = i - 1 ;" shape=box]
	21 -> 24 [color=blue]
	21 -> 22 [color=blue]
	22 [label="22	j >= 0 ;" shape=box]
	22 -> 24 [color=red]
	22 -> 23 [color=red]
	23 [label="23	-- j ) {" shape=box]
	24 [label="24	jbig2_image_release ( ctx , GSPLANES [ j ] ) ;" shape=box]
	26 [label="26	jbig2_free ( ctx -> allocator , GSPLANES ) ;" shape=box]
	27 [label="27	return NULL ;" shape=box]
	30 [label="30	rparams . MMR = GSMMR ;" shape=box]
	30 -> 31 [color=blue]
	31 [label="31	rparams . GBTEMPLATE = GSTEMPLATE ;" shape=box]
	31 -> 32 [color=blue]
	32 [label="32	rparams . TPGDON = 0 ;" shape=box]
	32 -> 33 [color=blue]
	33 [label="33	rparams . USESKIP = GSUSESKIP ;" shape=box]
	33 -> 34 [color=blue]
	33 -> 35 [color=blue]
	33 -> 36 [color=blue]
	33 -> 37 [color=blue]
	33 -> 38 [color=blue]
	33 -> 39 [color=blue]
	33 -> 40 [color=blue]
	33 -> 41 [color=blue]
	34 [label="34	rparams . gbat [ 0 ] = ( GSTEMPLATE <= 1 ? 3 : 2 ) ;" shape=box]
	35 [label="35	rparams . gbat [ 1 ] = - 1 ;" shape=box]
	36 [label="36	rparams . gbat [ 2 ] = - 3 ;" shape=box]
	37 [label="37	rparams . gbat [ 3 ] = - 1 ;" shape=box]
	38 [label="38	rparams . gbat [ 4 ] = 2 ;" shape=box]
	39 [label="39	rparams . gbat [ 5 ] = - 2 ;" shape=box]
	40 [label="40	rparams . gbat [ 6 ] = - 2 ;" shape=box]
	41 [label="41	rparams . gbat [ 7 ] = - 2 ;" shape=box]
	42 [label="42	if ( GSMMR ) {" shape=box]
	42 -> 58 [color=red]
	42 -> 43 [color=red]
	42 -> 46 [color=red]
	42 -> 47 [color=red]
	43 [label="43	code = jbig2_decode_halftone_mmr ( ctx , & rparams , data , size , GSPLANES [ GSBPP - 1 ] , & consumed_bytes ) ;" shape=box]
	43 -> 58 [color=blue]
	45 [label="45	else {" shape=box]
	46 [label="46	ws = jbig2_word_stream_buf_new ( ctx , data , size ) ;" shape=box]
	46 -> 115 [color=blue]
	46 -> 51 [color=blue]
	46 -> 47 [color=blue]
	47 [label="47	if ( ws == NULL ) {" shape=box]
	47 -> 48 [color=red]
	47 -> 49 [color=red]
	47 -> 51 [color=red]
	47 -> 52 [color=red]
	48 [label="48	jbig2_error ( ctx , JBIG2_SEVERITY_FATAL , segment -> number , \"failed to allocate ws in jbig2_decode_gray_scale_image\" ) ;" shape=box]
	49 [label="49	goto cleanup ;" shape=box]
	51 [label="51	as = jbig2_arith_new ( ctx , ws ) ;" shape=box]
	51 -> 56 [color=blue]
	51 -> 114 [color=blue]
	51 -> 52 [color=blue]
	51 -> 68 [color=blue]
	52 [label="52	if ( as == NULL ) {" shape=box]
	52 -> 56 [color=red]
	52 -> 58 [color=red]
	52 -> 53 [color=red]
	52 -> 54 [color=red]
	53 [label="53	jbig2_error ( ctx , JBIG2_SEVERITY_FATAL , segment -> number , \"failed to allocate as in jbig2_decode_gray_scale_image\" ) ;" shape=box]
	54 [label="54	goto cleanup ;" shape=box]
	56 [label="56	code = jbig2_decode_generic_region ( ctx , segment , & rparams , as , GSPLANES [ GSBPP - 1 ] , GB_stats ) ;" shape=box]
	56 -> 58 [color=blue]
	58 [label="58	if ( code != 0 ) {" shape=box]
	58 -> 59 [color=red]
	58 -> 60 [color=red]
	58 -> 62 [color=red]
	58 -> 63 [color=red]
	59 [label="59	jbig2_error ( ctx , JBIG2_SEVERITY_FATAL , segment -> number , \"error decoding GSPLANES for halftone image\" ) ;" shape=box]
	60 [label="60	goto cleanup ;" shape=box]
	62 [label="62	j = GSBPP - 2 ;" shape=box]
	62 -> 65 [color=blue]
	62 -> 68 [color=blue]
	62 -> 77 [color=blue]
	62 -> 63 [color=blue]
	63 [label="63	while ( j >= 0 ) {" shape=box]
	63 -> 64 [color=red]
	63 -> 81 [color=red]
	63 -> 80 [color=red]
	63 -> 70 [color=red]
	64 [label="64	if ( GSMMR ) {" shape=box]
	64 -> 65 [color=red]
	64 -> 68 [color=red]
	65 [label="65	code = jbig2_decode_halftone_mmr ( ctx , & rparams , data + consumed_bytes , size - consumed_bytes , GSPLANES [ j ] , & consumed_bytes ) ;" shape=box]
	65 -> 70 [color=blue]
	67 [label="67	else {" shape=box]
	68 [label="68	code = jbig2_decode_generic_region ( ctx , segment , & rparams , as , GSPLANES [ j ] , GB_stats ) ;" shape=box]
	68 -> 70 [color=blue]
	70 [label="70	if ( code != 0 ) {" shape=box]
	70 -> 71 [color=red]
	70 -> 72 [color=red]
	70 -> 74 [color=red]
	70 -> 75 [color=red]
	70 -> 76 [color=red]
	70 -> 78 [color=red]
	70 -> 63 [color=red]
	71 [label="71	jbig2_error ( ctx , JBIG2_SEVERITY_FATAL , segment -> number , \"error decoding GSPLANES for halftone image\" ) ;" shape=box]
	72 [label="72	goto cleanup ;" shape=box]
	74 [label="74	stride = GSPLANES [ 0 ] -> stride ;" shape=box]
	74 -> 76 [color=blue]
	75 [label="75	for ( i = 0 ;" shape=box]
	75 -> 76 [color=blue]
	75 -> 77 [color=blue]
	76 [label="76	i < stride * GSH ;" shape=box]
	76 -> 77 [color=red]
	77 [label="77	++ i ) GSPLANES [ j ] -> data [ i ] ^= GSPLANES [ j + 1 ] -> data [ i ] ;" shape=box]
	77 -> 77 [color=blue]
	77 -> 119 [color=blue]
	78 [label="78	-- j ;" shape=box]
	80 [label="80	GSVALS = jbig2_new ( ctx , uint8_t * , GSW ) ;" shape=box]
	81 [label="81	if ( GSVALS == NULL ) {" shape=box]
	81 -> 82 [color=red]
	81 -> 83 [color=red]
	81 -> 85 [color=red]
	81 -> 86 [color=red]
	82 [label="82	jbig2_error ( ctx , JBIG2_SEVERITY_FATAL , segment -> number , \"failed to allocate GSVALS: %d bytes\" , GSW ) ;" shape=box]
	83 [label="83	goto cleanup ;" shape=box]
	85 [label="85	for ( i = 0 ;" shape=box]
	85 -> 88 [color=blue]
	85 -> 89 [color=blue]
	85 -> 91 [color=blue]
	85 -> 86 [color=blue]
	86 [label="86	i < GSW ;" shape=box]
	86 -> 88 [color=red]
	86 -> 89 [color=red]
	86 -> 101 [color=red]
	86 -> 102 [color=red]
	87 [label="87	++ i ) {" shape=box]
	88 [label="88	GSVALS [ i ] = jbig2_new ( ctx , uint8_t , GSH ) ;" shape=box]
	88 -> 107 [color=blue]
	88 -> 110 [color=blue]
	88 -> 119 [color=blue]
	88 -> 89 [color=blue]
	88 -> 94 [color=blue]
	89 [label="89	if ( GSVALS [ i ] == NULL ) {" shape=box]
	89 -> 96 [color=red]
	89 -> 97 [color=red]
	89 -> 98 [color=red]
	89 -> 86 [color=red]
	89 -> 87 [color=red]
	89 -> 90 [color=red]
	89 -> 91 [color=red]
	89 -> 92 [color=red]
	90 [label="90	jbig2_error ( ctx , JBIG2_SEVERITY_FATAL , segment -> number , \"failed to allocate GSVALS: %d bytes\" , GSH * GSW ) ;" shape=box]
	91 [label="91	for ( j = i - 1 ;" shape=box]
	91 -> 92 [color=blue]
	91 -> 94 [color=blue]
	92 [label="92	j >= 0 ;" shape=box]
	92 -> 93 [color=red]
	92 -> 94 [color=red]
	93 [label="93	-- j ) {" shape=box]
	94 [label="94	jbig2_free ( ctx -> allocator , GSVALS [ j ] ) ;" shape=box]
	96 [label="96	jbig2_free ( ctx -> allocator , GSVALS ) ;" shape=box]
	97 [label="97	GSVALS = NULL ;" shape=box]
	97 -> 121 [color=blue]
	98 [label="98	goto cleanup ;" shape=box]
	101 [label="101	for ( x = 0 ;" shape=box]
	101 -> 110 [color=blue]
	101 -> 107 [color=blue]
	101 -> 102 [color=blue]
	102 [label="102	x < GSW ;" shape=box]
	102 -> 104 [color=red]
	102 -> 105 [color=red]
	102 -> 103 [color=red]
	103 [label="103	++ x ) {" shape=box]
	104 [label="104	for ( y = 0 ;" shape=box]
	104 -> 105 [color=blue]
	104 -> 107 [color=blue]
	104 -> 110 [color=blue]
	105 [label="105	y < GSH ;" shape=box]
	105 -> 106 [color=red]
	105 -> 107 [color=red]
	105 -> 108 [color=red]
	105 -> 109 [color=red]
	106 [label="106	++ y ) {" shape=box]
	107 [label="107	GSVALS [ x ] [ y ] = 0 ;" shape=box]
	107 -> 110 [color=blue]
	108 [label="108	for ( j = 0 ;" shape=box]
	108 -> 109 [color=blue]
	108 -> 110 [color=blue]
	109 [label="109	j < GSBPP ;" shape=box]
	109 -> 110 [color=red]
	110 [label="110	++ j ) GSVALS [ x ] [ y ] += jbig2_image_get_pixel ( GSPLANES [ j ] , x , y ) << j ;" shape=box]
	110 -> 110 [color=blue]
	113 [label="113	cleanup : if ( ! GSMMR ) {" shape=box]
	113 -> 114 [color=red]
	113 -> 115 [color=red]
	114 [label="114	jbig2_free ( ctx -> allocator , as ) ;" shape=box]
	115 [label="115	jbig2_word_stream_buf_free ( ctx , ws ) ;" shape=box]
	117 [label="117	for ( i = 0 ;" shape=box]
	117 -> 118 [color=blue]
	117 -> 119 [color=blue]
	118 [label="118	i < GSBPP ;" shape=box]
	118 -> 119 [color=red]
	119 [label="119	++ i ) jbig2_image_release ( ctx , GSPLANES [ i ] ) ;" shape=box]
	120 [label="120	jbig2_free ( ctx -> allocator , GSPLANES ) ;" shape=box]
	121 [label="121	return GSVALS ;" shape=box]
}
