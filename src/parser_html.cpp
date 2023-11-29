#include "gumbo.h"
#include <ostream>
#include <regex>
 
#include "parser_html.hpp"
#include "configuration.hpp"
#include "string_split.hpp"


namespace se{

ParserHtml::ParserHtml(const std::string& html , LinksMaintenance& algorithem)
: m_html(html)
, m_linksMaintenance_algorithem(algorithem)
{}

std::pair<std::unordered_map<std::string, int>, WordsMap> ParserHtml::result_parser(const std::string& url)
{
    std::vector<std::string> links = get_links();
    links_handling(url,links ,Config::getBounded());
    std::string words = get_text();
    
    StringSplit spliter(words);
    WordsMap textMap = spliter.parser();
     
    std::unordered_map<std::string, int> linksMap = convertToMap(links);

    std::pair<WordsMap, WordsMap> result = {linksMap, textMap};

    return result;
}

std::string ParserHtml::cleantext(GumboNode* node)const
{
    if (node->type == GUMBO_NODE_TEXT){
        return std::string(node->v.text.text);
    } else if (node->type == GUMBO_NODE_ELEMENT && node->v.element.tag != GUMBO_TAG_SCRIPT && node->v.element.tag != GUMBO_TAG_STYLE){
        std::string contents = "";
        GumboVector* children = &node->v.element.children;
        for (unsigned int i = 0; i < children->length; ++i){
            const std::string text = cleantext((GumboNode*) children->data[i]);
            
            if (i != 0 && !text.empty()){
                contents.append(" ");
            }
            
            contents.append(text);
        }

        return contents;

    } else {
        return "";
    }
}
 
std::string ParserHtml::get_text()const
{
    GumboOutput* output = gumbo_parse(m_html.c_str());
    std::string text = cleantext(output->root);
    gumbo_destroy_output(&kGumboDefaultOptions, output);
    
    return text;
}

std::vector<std::string> ParserHtml::get_links()
{
    std::ostringstream htmlContent;
    std::regex linkRegex("<a\\s+(?:[^>]*?\\s+)?href=([\"'])(.*?)\\1", std::regex_constants::icase);
    std::smatch match;

    std::vector<std::string> links;
    std::string html = m_html; 
    
    while (std::regex_search(html, match, linkRegex)){
        std::string link = match[2].str();
        links.push_back(link);
        html = match.suffix().str();
    }

    return links;
}

void ParserHtml::links_handling(const std::string& url,std::vector<std::string>& linksList ,bool bounded)
{
  m_linksMaintenance_algorithem.setListLinks(linksList);
  m_linksMaintenance_algorithem.fixLinks(url);
  if(bounded){
    std::string website_adrress = url.substr(0,36);
    m_linksMaintenance_algorithem.remove_Unbounded_links(website_adrress);
  }
}

std::unordered_map<std::string, int> ParserHtml::convertToMap(const std::vector<std::string>& links)const
{
    std::unordered_map<std::string, int> instancees;

    for (auto& link : links){
        ++instancees[link];
    }  

    return instancees; 
}

} // namespace se
