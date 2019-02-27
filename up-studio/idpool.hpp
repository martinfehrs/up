#pragma once


#include <vector>
#include <algorithm>


template <typename id_t, id_t START, id_t STEP>
struct IdPool
{
    auto RequestSessionId()
    {
        if(reuseable_ids.empty())
        {
            const auto id = nextId;

            nextId++;

            return id;
        }
        else
        {
            const auto id = reuseable_ids.back();

            reuseable_ids.pop_back();

            return id;
        }
    }

    void ReleaseSessionId(id_t id)
    {
        reuseable_ids.push_back(id);

        std::sort(std::rbegin(reuseable_ids), std::rend(reuseable_ids));
    }

  private:

    std::vector<id_t> reuseable_ids;
    id_t nextId = START;
};
