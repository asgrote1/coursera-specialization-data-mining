/**
 * @file competition.cpp
 * @author Hussein Hazimeh
 * Built based on query-runner.cpp which is written by Sean Massung
 */

#include <cmath>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "meta/corpus/document.h"
#include "meta/index/eval/ir_eval.h"
#include "meta/index/inverted_index.h"
#include "meta/index/ranker/ranker_factory.h"
#include "meta/index/score_data.h"
#include "meta/parser/analyzers/tree_analyzer.h"
#include "meta/sequence/analyzers/ngram_pos_analyzer.h"
#include "meta/util/time.h"

using namespace meta;

/*
class new_ranker: public index::ranker
{
private: // Change the parameters to suit your ranking function
    float param1_ = 0;
    float param2_ = 0;

public:
    const static util::string_view id;
    new_ranker(); // Default Constructor
    new_ranker(float param1, float param2); // Constructor
    new_ranker(std::istream& in);
    void set_param(float param1, float param2){param1_ = param1; param2_ =
param2;}; // Sets the parameters
    void save(std::ostream& out) const override;
    float score_one(const index::score_data&); // Calculates the score for one
matched term
};

const util::string_view new_ranker::id = "newranker"; // Used to identify the
ranker in config.toml
new_ranker::new_ranker(){}
new_ranker::new_ranker(float param1, float param2) : param1_{param1},
param2_{param2} {}

new_ranker::new_ranker(std::istream& in)
    : param1_{io::packed::read<float>(in)},
      param2_{io::packed::read<float>(in)}
{
    // nothing
}

void new_ranker::save(std::ostream& out) const
{
    io::packed::write(out, id);
    io::packed::write(out, param1_);
    io::packed::write(out, param2_);
}

float new_ranker::score_one(const index::score_data& sd)
{
    // Implement your scoring function here

   return 0;

}


namespace meta{
namespace index{
template <>
std::unique_ptr<ranker>make_ranker<new_ranker>(
        const cpptoml::table & config) // Used by new_ranker to read the
parameters param1 and param2 from config.toml
{
    float param1 = 0; // Change to the default parameter value
    if (auto param1_file = config.get_as<double>("param1"))
        param1 = *param1_file;

    float param2 = 0; // Change to the default parameter value
    if (auto param2_file = config.get_as<double>("param2"))
        param2 = *param2_file;

    return make_unique<new_ranker>(param1, param2);
}

}
}

*/

// pl2_ranker is a class that implements the pl2 ranking function. It is derived
// from the base class ranker.
//XXXclass pl2_ranker : public index::ranking_function {
class pl2_ranker: public index::ranker
{
 private: // c_ and lambda_ are the parameters of pl2
  float c_ = 7;
  float lambda_ = 0.1;

 public:
  const static util::string_view id;
  pl2_ranker(); // Default Constructor
  pl2_ranker(float c, float lambda); // Constructor
  pl2_ranker(std::istream& in);
  void set_param(float c, float lambda) { // Set parameters
    c_ = c;
    lambda_ = lambda;
  };
  // Saves this ranker, if used in a classifier
  void save(std::ostream& out) const override;
  float score_one(const index::score_data&); // Calculates the score for a single matched term
  //float score_one(const index::score_data&) override;
};

const util::string_view pl2_ranker::id = "pl2"; // Used to identify pl2_ranker in config.toml
pl2_ranker::pl2_ranker(){}
pl2_ranker::pl2_ranker(float c, float lambda) : c_{c}, lambda_{lambda} {}

pl2_ranker::pl2_ranker(std::istream& in)
    : c_{io::packed::read<float>(in)},
      lambda_{io::packed::read<float>(in)} {}

void pl2_ranker::save(std::ostream& out) const
{
  io::packed::write(out, id);
  io::packed::write(out, c_);
  io::packed::write(out, lambda_);
}

float pl2_ranker::score_one(const index::score_data& sd) {
  /**
   * This function is called for each matched term between the query and the
   * document. The function's argument is a struct that contains important
   * information about the matched term. For example, sd.doc_term_count gives
   * the # of occurrences of the term in the document.
  **/
  float doc_len = sd.idx.doc_size(sd.d_id);  // Current document length
  float avg_dl = sd.avg_dl;      // Average document length in the corpus
  float tf = sd.doc_term_count;  // Raw term count in the document
  float pi = 3.14;               // Use this for pi - Do NOT use other values
  float lambda = lambda_;        // pl2's parameter
  float c = c_;                  // pl2's parameter

  /**
   * You should implement the ranking function after this comment.
   * Use all the variables provided above (doc_len, avg_dl, tf, pi, lambda,
   * and c)
   * Use the function log2() to implement the logarithm
   * Use exp(1) to implement e
  **/
 float tfn = tf * log2(1 + c * avg_dl / doc_len);

 float score = 1 / (tfn + 1) * (tfn * log2(tfn / lambda) + (lambda + 1 / (12 * tfn) - tfn) * log2(exp(1)) + 0.5 * log2(2 * pi * tfn));

  return score;  // Change 0 to the final score you calculated
}

namespace meta {
namespace index {
template <>
std::unique_ptr<ranker> make_ranker<pl2_ranker>(
    const cpptoml::table & config)  // Used by pl2_ranker to read the parameters
                                   // c and lambda from config.toml - You can
                                   // ignore it
{
    float c = 7.0; // Change to the default parameter value
    if (auto c_file = config.get_as<double>("c"))
        c = *c_file;

    float lambda = 0.1; // Change to the default parameter value
    if (auto lambda_file = config.get_as<double>("lambda"))
        lambda = *lambda_file;

    return make_unique<pl2_ranker>(c, lambda);
 
 /*xxx
  return make_unique<pl2_ranker>(
      config.get_as<double>("c").value_or(7.0f),
      config.get_as<double>("lambda").value_or(0.1f));
 */
}
}
}


int main(int argc, char* argv[]) {
  // index::register_ranker<new_ranker>();
  if (argc != 2 && argc != 3) {
    std::cerr << "Usage:\t" << argv[0] << " configFile" << std::endl;
    return 1;
  }

  // Log to standard error
  logging::set_cerr_logging();

  // Register additional analyzers
  parser::register_analyzers();
  sequence::register_analyzers();

  // Submission-specific - Ignore
  std::ofstream submission;

  submission.open("Assignment2/output.txt");
  if (!submission.is_open()) {
    std::cout << "Problem writing the output to the system. Make sure the "
                 "program has enough writing privileges. Quiting..."
              << std::endl;
    return 0;
  }
  std::string nickname;
  std::cout << "Enter your nickname: ";
  std::getline(std::cin, nickname);
  submission << nickname + '\n';
  // End of the submission-specific code

  //  Create an inverted index using a DBLRU cache.
  auto config = cpptoml::parse_file(argv[1]);
  auto idx = index::make_index<index::dblru_inverted_index>(*config, 30000);

  // Create a ranking class based on the config file.
  auto group = config->get_table("ranker");
  if (!group)
    throw std::runtime_error{"\"ranker\" group needed in config file!"};
  auto ranker = index::make_ranker(*group); //XXX current line of error.

  // Get the path to the file containing queries
  auto query_path = config->get_as<std::string>("querypath");
  if (!query_path)
    throw std::runtime_error{"config file needs a \"querypath\" parameter"};

  std::ifstream queries{*query_path + *config->get_as<std::string>("dataset") +
                        "-queries.txt"};

  // Create an instance of ir_eval to evaluate the MAP and Precision@10 for the
  // training queries
  auto eval = index::ir_eval(*config);

  // Print the precision@10 and the MAP for the training queries
  size_t i = 1;
  std::string content;
  while (i <= 100 && queries.good()) {
    std::getline(
        queries,
        content);  // Read the content of the current training query from file

    corpus::document query{
        doc_id{i - 1}};  // Instantiate the query as an empty document

    query.content(content);  // Set the content of the query

    std::cout << "Ranking query " << i++ << ": " << content << std::endl;

    auto ranking = ranker->score(
        *idx, query,
        50);  // ranking is a vector of pairs of the form <docID,docScore>
    // You can access the ith document's ID using ranking[i].d_id and its score
    // using ranking[i].score

    std::cout << "Precision@10 for this query: "
              << eval.precision(ranking, (query_id) query.id(), 10) << std::endl; // XXX "(query_id)" added

    eval.avg_p(ranking, (query_id) query.id(), 50);  // Store the average precision at 50
                                                     // documents for the current query
                                                     // XXX "(query_id)" added

    std::cout << "Showing top 10 of " << ranking.size() << " results."
              << std::endl;

    for (size_t i = 0; i < ranking.size() && i < 10;
         ++i)  // Loop over the top 10 documents in ranking
      std::cout << (i + 1) << ". "
                << " "
                << *idx->metadata(ranking[i].d_id).get<std::string>("name")
                << " " << ranking[i].score << std::endl;

    std::cout << std::endl;
    
    /*
    std::cout << "i: "
              << i 
              << "\tqueries.good(): " 
              << queries.good() << "\n"
              << "content: "
              << content
              << "\n";
    */
  }

  std::cout << "The MAP for the training queries is: " << eval.map()
            << std::endl;

  // Write the top 50 documents of each test query to the submission file
  while (queries.good()) {
    std::getline(
        queries,
        content);  // Read the content of the current testing query from file

    corpus::document query{doc_id{i - 1}};

    query.content(content);

    auto ranking = ranker->score(*idx, query, 50);

    for (size_t i = 0; i < ranking.size() && i < 50;
         i++)  // Loop over the top 50 documents
    {
      submission
          << std::to_string(ranking[i].d_id) +
                 " ";  // Write the IDs of the top 50 documents to output.txt
    }
    submission << "\n";
  }

  submission.close();
  return 0;
}
