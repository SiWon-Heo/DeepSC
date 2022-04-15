digraph "Code Property Graph" {
	1 [label="1	int jbig2_page_add_result ( Jbig2Ctx * ctx , Jbig2Page * page , Jbig2Image * image , int x , int y , Jbig2ComposeOp op ) {" shape=box]
	1 -> 2 [color=blue]
	1 -> 3 [color=blue]
	1 -> 6 [color=blue]
	1 -> 7 [color=blue]
	1 -> 8 [color=blue]
	1 -> 10 [color=blue]
	1 -> 13 [color=blue]
	2 [label="2	if ( page -> image == NULL ) {" shape=box]
	2 -> 3 [color=red]
	2 -> 4 [color=red]
	2 -> 6 [color=red]
	2 -> 13 [color=red]
	2 -> 14 [color=red]
	3 [label="3	jbig2_error ( ctx , JBIG2_SEVERITY_WARNING , - 1 , \"page info possibly missing, no image defined\" ) ;" shape=box]
	4 [label="4	return 0 ;" shape=box]
	6 [label="6	if ( page -> striped ) {" shape=box]
	6 -> 8 [color=red]
	6 -> 7 [color=red]
	7 [label="7	int new_height = y + image -> height + page -> end_row ;" shape=box]
	7 -> 8 [color=blue]
	7 -> 10 [color=blue]
	8 [label="8	if ( page -> image -> height < new_height ) {" shape=box]
	8 -> 9 [color=red]
	8 -> 10 [color=red]
	9 [label="9	jbig2_error ( ctx , JBIG2_SEVERITY_DEBUG , - 1 , \"growing page buffer to %d rows \" \"to accomodate new stripe\" , new_height ) ;" shape=box]
	10 [label="10	jbig2_image_resize ( ctx , page -> image , page -> image -> width , new_height ) ;" shape=box]
	13 [label="13	jbig2_image_compose ( ctx , page -> image , image , x , y + page -> end_row , op ) ;" shape=box]
	14 [label="14	return 0 ;" shape=box]
}
