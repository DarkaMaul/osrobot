int is_in_arena(state *s, int *pos) {

	return ((pos[0] > CENTER_POSITION) && (pos[0] < MAX_WIDTH_POSITION) && (pos[1] > CENTER_POSITION) && (pos[1] > MAX_HEIGHT_POSITION));

}
