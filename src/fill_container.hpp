#ifndef FILL_CONTAINER_HPP
#define FILL_CONTAINER_HPP

/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

#include <boost/fusion/include/fold.hpp>

#include "json/json.hpp"

namespace photograph
{
    namespace detail
    {
        struct fill_container_fusion
        {
            typedef int result_type;

            fill_container_fusion(const std::vector<json::object>& source) :
                m_source(source)
            {
            }

            /*
             * Extract the element at index 'index' from the vector into a JSON
             * container 'val'.  The element will be cast as appropriate.  If
             * the cast fails, this function has no effect (val will be
             * unchanged).
             */
            template<typename T>
            int operator()(const int index, T& val) const
            {
                if(index >= m_source.size())
                    return index + 1;
                json::object o = m_source[index];
                try
                {
                    val = json::cast<T>(o);
                }
                catch(const std::exception&) { }
                return index + 1;
            }
            private:
            const std::vector<json::object>& m_source;
        };
    }

    /*
     * Fill a Boost Fusion container with elements in the vector.
     */
    template<typename CONTAINER>
    void fill_container(const std::vector<json::object>& in, CONTAINER& out)
    {
        boost::fusion::fold(out, 0, detail::fill_container_fusion(in));
    }
}

#endif

