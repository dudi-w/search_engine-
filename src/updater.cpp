// #include <iostream> 

// #include "updater.hpp"
// #include "mysql_graph_data.hpp" 
// #include "mysql_links_data.hpp"
// #include "mysql_word_links.hpp"
// #include <cppconn/exception.h>
// #include "se_exceptions.hpp"


// se::Updater::Updater(LinksMamInsertion& linkMap, IndexerInsertion& indexer, size_t mount, Publisher& publisher)

// : m_indexer(indexer)
// , m_buffer()
// , m_linkMap(linkMap)
// , m_mount(mount)
// , m_publisher(publisher)
// , m_mtx()
// , m_notMtx()
// {}

// void se::Updater::fill(std::pair<LinksVector, WordsMap>& resCrewl ,const std::string& url)
// {                
//     std::unique_lock<std::shared_mutex> locker(m_mtx);

//     m_buffer.insert(url, resCrewl);
//     if(m_buffer.size() >= m_mount){        
//         SafeUnorderedMap<std::string, std::pair<LinksVector, WordsMap>> tempBuffer(std::move(m_buffer));
        
//         locker.unlock();

//         std::vector<std::string> keys = tempBuffer.getKeys();

//         for(auto key : keys){            
//             m_linkMap.insert(tempBuffer[key].first ,key);
//         }
        
//         std::unique_lock<std::shared_mutex> notLocker(m_notMtx);

//         m_publisher.notify();
//         notLocker.unlock();
        
//         for(auto key : keys){
//             m_indexer.insert(tempBuffer[key].second ,key);
//         }
//     }   
// }


#include <iostream> 

#include "updater.hpp"
#include "mysql_graph_data.hpp" 
#include "mysql_word_links.hpp"
 

se::Updater::Updater(size_t mount, Publisher& publisher, db::GraphData& graph, db::WordLinks& words)

: m_buffer()
, m_mount(mount)
, m_publisher(publisher)
, m_mtx()
, m_notMtx()
, m_graphData(graph)
, m_wordsData(words)
{}

void se::Updater::fill(std::pair<Map, Map>& resCrewl ,const std::string& url)
{                
    std::unique_lock<std::shared_mutex> locker(m_mtx);

    m_buffer.insert(url, resCrewl);
    if(m_buffer.size() >= m_mount){        
        SafeUnorderedMap<std::string, std::pair<Map, Map>> tempBuffer(std::move(m_buffer));
        locker.unlock();

        std::vector<std::string> keys = tempBuffer.getKeys();

        for(auto key : keys){            
            m_graphData.insert(tempBuffer[key].first, key);
        }
        
        std::unique_lock<std::shared_mutex> notLocker(m_notMtx);

        m_publisher.notify();
        notLocker.unlock();
        
        for(auto key : keys){
            m_wordsData.insert(tempBuffer[key].second, key);
        }
    }
}
