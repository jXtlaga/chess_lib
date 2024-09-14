//
// Created by Jan Tlaga on 9/12/24.
//
void search_depth(int depth_max, Position position) {
    int analysis_count[depth_max];
    int analysis_checkmate = 0;
    for(int i = 0; i < depth_max; i++){
        analysis_count[i] = 0;
    }

    //1. set variables for the search
    //1.1 vertices - array of mask_moves for each depth
    Move vertices[depth_max][17];
    //1.2 count vertices - how many non analysed vertices are in the current depth
    int count_vertices[depth_max];
    for (int i = 0; i < depth_max; i++) {
        count_vertices[i] = 0;
    }
    //1.3 depth_level - current depth level
    int depth_level = 0;
    //1.4 set the thirst mask_moves after input position
    position_analysis(&position, vertices[depth_level], &count_vertices[depth_level]);
    //1.5
    Position last_positions[depth_max];
    last_positions[0] = position;
    //2. creating the depth search
    while (depth_level >= 0) {
        //2.1 if there are no vertices in the current depth level, go to the previous level
        if (count_vertices[depth_level] == 0) {
            depth_level--;
            continue;
        }
        assert(count_vertices[depth_level] > 0);
        //2.3 take the last mask_move
        int ite_ver = count_vertices[depth_level] - 1;
        Move last_mask_move = vertices[depth_level][ite_ver];
        U64 last_move_to = get_LS1B(last_mask_move.to);
        if (last_move_to == 0) {
            count_vertices[depth_level]--;
            continue;
        }
        Position new_position;
        //2.4 if the last mask_move is a pawn move, check if it is a promotion
        if (last_mask_move.type == PAWN_MOVE && (last_move_to & 0xFF000000000000FF)) {
            //2.4.1 create 3 new mask_moves (promotion to rook, bishop, knight)
            vertices[depth_level][ite_ver] = (Move) {last_mask_move.from, last_move_to, PROMOTION_BISHOP};
            vertices[depth_level][ite_ver + 1] = (Move) {last_mask_move.from, last_move_to, PROMOTION_KNIGHT};
            vertices[depth_level][ite_ver + 2] = (Move) {last_mask_move.from, last_move_to, PROMOTION_ROOK};
            count_vertices[depth_level] += 3;
            //2.4.2 play the promotion to queen
            new_position = make_move(last_positions[depth_level], last_mask_move.from, last_move_to,
                                     PROMOTION_QUEEN);
        }
        else if (last_mask_move.type == EN_PASSANT) {
            new_position = make_move(last_positions[depth_level], last_mask_move.from, last_move_to,
                                     last_mask_move.type);
            vertices[depth_level][ite_ver].to ^= last_move_to;
        }
        else{
            new_position = make_move(last_positions[depth_level], last_mask_move.from, last_move_to,
                                     last_mask_move.type);
            vertices[depth_level][ite_ver].to ^= last_move_to;
        }
        //2.5 play
        //2.6 analyse the new position
        analysis_count[depth_level]++;
        depth_level++;

        if (depth_level == depth_max) {
//            Position history_search[depth_max+1];
//            for(int i = 0; i < depth_level; i++){
//                history_search[i] = last_positions[i];
//            }
//            history_search[depth_max] = new_position;
//            print_search_way(history_search, depth_level+1);
            depth_level--;
            continue;
        }
        last_positions[depth_level] = new_position;
        position_analysis(&new_position, vertices[depth_level], &count_vertices[depth_level]);
        if(count_vertices[depth_level] == 0){
            printf("checkmate\n");
            analysis_checkmate++;
        }

    }

    //vertices[0]
    for(int i = 0; i < depth_max; i++){
        printf("______________________\n");
        printf("depth: %d, vertices count: %d \n", i + 1, analysis_count[i]);
    }
    printf("checkmate count: %d\n", analysis_checkmate);

}