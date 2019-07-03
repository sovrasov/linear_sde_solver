#define _USE_MATH_DEFINES
#include <iostream>
#include <fstream>
#include <cmath>

#include <cmdline.h>
#include <json.hpp>
#include <linear_sde_integrator.hpp>

void initParser(cmdline::parser& parser);

int main(int argc, char *argv[])
{
  cmdline::parser parser;
  initParser(parser);
  parser.parse_check(argc, argv);

  double a = parser.get<double>("a");
  auto integrate_f = integrate_linear_sde<HeunScheme>;
  if (parser.get<std::string>("method") == "euler")
    integrate_f = integrate_linear_sde<EulerScheme>;

  std::vector<std::vector<double>> solutions;
  double time;
  std::tie(solutions, time) = integrate_f(parser.get<unsigned>("numThreads"),
                                          parser.get<unsigned>("n_impls"),
                                          parser.get<double>("step"),
                                          parser.get<unsigned>("n_steps"),
                                          parser.get<double>("x_0"),
                                          parser.get<double>("d"),
                                          [a](double x){return a - sin(x);},
                                          parser.get<unsigned>("seed")
                                          );

  std::cout << "Time elapsed: " << time << "s\n";

  auto fileName = parser.get<std::string>("outFile");
  if (!fileName.empty())
  {
    std::vector<double> hole_probs(solutions[0].size(), 0.);
    double hole_lb = parser.get<double>("hole_lb");
    double hole_ub = parser.get<double>("hole_ub");
    for(size_t i = 0; i < solutions.size(); i++)
      for(size_t j = 0; j < solutions[i].size(); j++)
        if (solutions[i][j] >= hole_lb && solutions[i][j] <= hole_ub)
          hole_probs[j] += 1.;
    for(auto& prob : hole_probs)
      prob /= solutions.size();

    std::ofstream fout;
    fout.open(fileName, std::ios_base::out);
    if (!fout.is_open())
    {
      std::cerr << "Failed to create file " << fileName << std::endl;
      return 0;
    }
    nlohmann::json j;
    j["num_threads"] = parser.get<unsigned>("numThreads");
    j["time_elapsed"] = time;
    j["t_0"] = 0;
    j["step"] = parser.get<double>("step");
    j["n_steps"] = parser.get<unsigned>("n_steps");
    j["solutions"] = solutions;
    j["hole_probs"] = hole_probs;
    fout << j;
  }

  return 0;
}

void initParser(cmdline::parser& parser)
{
  parser.add<double>("step", 's', "step of integration", false, 0.01);
  parser.add<unsigned>("n_impls", 'i', "number of simulated implementations", false, 1000);
  parser.add<unsigned>("n_steps", 'n', "number of integration steps", false, 1000);
  parser.add<double>("x_0", 0, "initial value of x", false, 0);
  parser.add<double>("d", 'd', "Diffusion coefficient", false, 1);
  parser.add<double>("a", 'a', "Parameter of the equation", false, 0.5);
  parser.add<double>("hole_lb", 'l', "", false, -M_PI);
  parser.add<double>("hole_ub", 'u', "", false, M_PI);
  parser.add<unsigned>("seed", 0, "seed for random engines", false, 123);
  parser.add<unsigned>("numThreads", 'p', "number of OMP threads", false, 1);
  parser.add<std::string>("method", 'm', "Name of the used integration scheme", false,
    "euler", cmdline::oneof<std::string>("euler", "heun"));
  parser.add<std::string>("outFile", 'f', "Name of the output .json file", false, "");
}
