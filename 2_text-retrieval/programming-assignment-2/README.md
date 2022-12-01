In Assignment_2/competition.cpp, change:

    line 169:

        original: << eval.precision(ranking, query.id(), 10) << std::endl;
        refactored: << eval.precision(ranking, (query_id) query.id(), 10) << std::endl;

    line 171:

        original: eval.avg_p(ranking, query.id(), 50);  // Store the average precision at 50
        refactoed: eval.avg_p(ranking, (query_id) query.id(), 50);  // Store the average precision at 50