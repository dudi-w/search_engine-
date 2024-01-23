#include "updater.hpp"
#include "configuration.hpp"
#include <iostream>

  
se::Updater::Updater(Publisher& publisher, db::GraphData& graph, db::WordLinks& words)
: m_buffer()
, m_mount(Config::getLinksMountForPagerank())
, m_publisher(publisher)    
, m_mtx()
, m_graphData(graph)
, m_wordsData(words)
{}

 
// void se::Updater::fill(std::pair<Map, Map>& resCrewl ,const std::string& url)
// {  
//     std::unique_lock<std::shared_mutex> locker(m_mtx);
     
//     m_buffer.insert(url, resCrewl);
//     if(m_buffer.size() >= m_mount){        
//         SafeUnorderedMap<std::string, std::pair<Map, Map>> tempBuffer(std::move(m_buffer));
//         locker.unlock();
//         m_graphData.insert(tempBuffer);
//         m_publisher.notify();
//         m_wordsData.insert(tempBuffer);
//     }    
// }

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
        std::cout << "graph" << '\n';
        
        // std::unique_lock<std::shared_mutex> notLocker(m_notMtx);

        m_publisher.notify();
        // notLocker.unlock();
         
        for(auto key : keys){              
            m_wordsData.insert(tempBuffer[key].second, key);
        }  
        std::cout << "wordliks" << '\n';
            
    }    
}
void se::Updater::bufferFlush()
{    
    if(m_buffer.size() > 0){ 
        m_graphData.insert(m_buffer);
        m_publisher.notify();
        m_wordsData.insert(m_buffer);
    }  
}
