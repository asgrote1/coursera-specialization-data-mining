/**
 * @file new-judgements.cpp
 * @author Hussein Hazimeh
 * Built based on interactive-search.cpp which is written by Sean Massung
 */

#include <algorithm>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include "meta/corpus/document.h"
#include "meta/index/inverted_index.h"
#include "meta/index/ranker/all.h"
#include "meta/index/ranker/ranker_factory.h"
#include "meta/util/printing.h"
#include "meta/util/time.h"

using namespace meta;

/**
 * @param path The path to the file to open
 * @return the text content of that file
 */
std::string get_content(const std::string& path)
{
    std::ifstream in{path};
    std::string str{(std::istreambuf_iterator<char>(in)),
                    std::istreambuf_iterator<char>()};
    std::replace(str.begin(), str.end(), '\n', ' ');
    return str;
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage:\t" << argv[0] << " configFile" << std::endl;
        return 1;
    }

    // Turn on logging to std::cerr.
    logging::set_cerr_logging();

    // Create an inverted index using a splay cache. The arguments forwarded
    //  to make_index are the config file for the index and any parameters
    //  for the cache. In this case, we set the maximum number of nodes in
    //  the splay_cache to be 30000.
    auto config = cpptoml::parse_file(argv[1]);
    auto idx = index::make_index<index::splay_inverted_index>(*config, 30000);

    // Create a ranking class based on the config file.
    int token;
    try
    {
        std::default_random_engine generator(std::random_device{}());
        std::uniform_int_distribution<int> distribution(1,484);
        token = distribution(generator);
    }
    catch(...){token=1;}

    std::unique_ptr<index::ranker> ranker;
    ranker = make_unique<index::okapi_bm25>();

    // Find the path prefix to each document so we can print out the contents.
    std::string prefix = *config->get_as<std::string>("prefix")
                       + "/" + *config->get_as<std::string>("dataset") + "/";


    // Get the path to the file containing queries
    auto query_path = config->get_as<std::string>("querypath");
    if (!query_path)
        throw std::runtime_error{"config file needs a \"querypath\" parameter"};

    std::ifstream queries{*query_path + *config->get_as<std::string>("dataset")
                          + "-judging-queries.txt"};

    // Open the file to save the relevance jugdements to
    std::ofstream qrels ("Assignment2/new-judgements.txt");
    if (!qrels.is_open())
    {
        std::cout<<"Problem writing the output to the system. Make sure the script has enough writing privileges. Quiting..."<<std::endl;
        return 0;
    }
    std::string agree;
    std::cout<<std::endl<<std::endl;
    std::cout<<printing::make_bold("To facilitate research and education in information retrieval, I do grant permission to the instructor and to the University of Illinois at Urbana-Champaign to include my anonymized relevance judgements in a dataset that may be released to the public for research and educational purposes.")<<std::endl;
    std::cout<<"If you agree with the statement above write 'yes', else write 'no' (without quotes): ";
    std::getline(std::cin,agree);
    while(agree!="yes" && agree!="no")
    {
        std::cout<<"If you agree with the statement above write 'yes', else write 'no' (without quotes): ";
        std::getline(std::cin,agree);
    }
    if(agree == "yes")
        qrels<<"1\n";
    else qrels<<"0\n";
        

    std::string content;
    int i = 1;

    while (i<=token && queries.good())
    {
            std::getline(queries, content);
            if (i == token)
            {
                corpus::document query;
                query.content(content);
                std::cout << std::endl<< "Query: " << printing::make_bold(content) << std::endl << std::endl;
                auto ranking = ranker->score(*idx, query, 20);
                std::cout << "Showing the top 20 results:" << std::endl;
                for (size_t i = 0; i < ranking.size() && i < 20; ++i)
                {
                    auto path = *idx->metadata(ranking[i].d_id).get<std::string>("name");
                    std::cout << std::to_string(i + 1) + ". " + path << std::endl;
                }
                std::cout<<std::endl<<"Enter the numbers of the relevant pages separated by spaces: " <<std::endl;
                int relevant_doc;
                bool success = 1;
                std::vector<int> relevant_docs;
                std::string line;
                std::getline(std::cin,line);
                if(line.find_first_not_of("0123456789 ") != std::string::npos)
                    success = 0;
                std::istringstream linestream(line);
                while(linestream >> relevant_doc)
                {
                    if (relevant_doc<1 || relevant_doc>20 || success==0)
                    {
                        success = 0;
                        break;
                    }
                    else
                        relevant_docs.push_back(relevant_doc);

                }
                if (relevant_docs.size()==0){success=0;}

                if (!success)
                {
                    std::cout<<"Error: The relevance judgements should be valid numbers separated by spaces. Run the program again."<<std::endl;
                }
                else
                {
                    if (relevant_docs.size()!=0) // Do not allow empty judgements
                    {
                        qrels << content+' '+std::to_string(token)+'\n';
                        for (size_t i=0; i<relevant_docs.size(); i++)
                            qrels<<std::to_string(ranking[relevant_docs[i]-1].d_id)+' ';
                        qrels<<'\n';
                    }
         
                }
            }
            i++;

    }

    qrels.close();

    return 0;
}
