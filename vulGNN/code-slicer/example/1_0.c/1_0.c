digraph "Code Property Graph" {
	1 [label="1	char * curl_mvaprintf ( const char * format , va_list ap_save ) {" shape=box]
	1 -> 8 [color=blue]
	2 [label="2	int retcode ;" shape=box]
	3 [label="3	struct asprintf info ;" shape=box]
	3 -> 4 [color=blue]
	4 [label="4	info . buffer = NULL ;" shape=box]
	4 -> 10 [color=blue]
	4 -> 5 [color=blue]
	4 -> 14 [color=blue]
	4 -> 15 [color=blue]
	5 [label="5	info . len = 0 ;" shape=box]
	5 -> 6 [color=blue]
	5 -> 14 [color=blue]
	6 [label="6	info . alloc = 0 ;" shape=box]
	6 -> 10 [color=blue]
	6 -> 13 [color=blue]
	6 -> 7 [color=blue]
	7 [label="7	info . fail = 0 ;" shape=box]
	7 -> 9 [color=blue]
	7 -> 10 [color=blue]
	7 -> 13 [color=blue]
	7 -> 14 [color=blue]
	7 -> 15 [color=blue]
	8 [label="8	retcode = dprintf_formatf ( & info , alloc_addbyter , format , ap_save ) ;" shape=box]
	8 -> 9 [color=blue]
	9 [label="9	if ( ( - 1 == retcode ) || info . fail ) {" shape=box]
	9 -> 10 [color=red]
	9 -> 11 [color=red]
	9 -> 13 [color=red]
	10 [label="10	if ( info . alloc ) free ( info . buffer ) ;" shape=box]
	10 -> 10 [color=red]
	11 [label="11	return NULL ;" shape=box]
	13 [label="13	if ( info . alloc ) {" shape=box]
	13 -> 17 [color=red]
	13 -> 14 [color=red]
	13 -> 15 [color=red]
	14 [label="14	info . buffer [ info . len ] = 0 ;" shape=box]
	15 [label="15	return info . buffer ;" shape=box]
	17 [label="17	else return strdup ( \"\" ) ;" shape=box]
}