/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

#include "statistics.hpp"

#include <boost/fusion/include/vector.hpp>
#include <boost/optional.hpp>

#include "sqlite/devoid.hpp"
#include "sqlite/get_by_id.hpp"
#include "sqlite/get_list.hpp"
#include "sqlite/insert.hpp"
#include "sqlite/select.hpp"
#include "sqlite/step.hpp"
#include "sqlite/update.hpp"

void photograph::db::get(sqlite::connection& db, const statistics& stats)
{
    json::list stats_list;
    sqlite::select<statistics>(
        db,
        "SELECT COUNT(photograph_id) AS total_count FROM photograph ",
        boost::fusion::vector<>(),
        stats_list
        );
    stats.get_object() = stats_list[0];
}

