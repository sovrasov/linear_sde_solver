#include <iostream>
#include <fstream>
#include <chrono>
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

  auto start = std::chrono::system_clock::now();
  auto integrate_f = integrate_linear_sde<HeunScheme>;
  if (parser.get<std::string>("method") == "euler")
    integrate_f = integrate_linear_sde<EulerScheme>;

  auto solutions = integrate_f(parser.get<unsigned>("numThreads"),
                               parser.get<unsigned>("n_impls"),
                               parser.get<double>("step"),
                               parser.get<unsigned>("n_steps"),
                               parser.get<double>("x_0"),
                               parser.get<double>("d"),
                               [](double x){return cos(x);},
                               parser.get<unsigned>("seed")
                               );

  auto end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end - start;
  std::cout << "Time elapsed: " << elapsed_seconds.count() << "s\n";

  auto fileName = parser.get<std::string>("outFile");
  if (!fileName.empty())
  {
    std::ofstream fout;
    fout.open(fileName, std::ios_base::out);
    if (!fout.is_open())
    {
      std::cerr << "Failed to create file " << fileName << std::endl;
      return 0;
    }
    nlohmann::json j;
    j["num_threads"] = parser.get<unsigned>("numThreads");
    j["time_elapsed"] = elapsed_seconds.count();
    j["t_0"] = 0;
    j["step"] = parser.get<double>("step");
    j["n_steps"] = parser.get<unsigned>("n_steps");
    j["solutions"] = solutions;
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
  parser.add<double>("d", 'd', "", false, 1);
  parser.add<unsigned>("seed", 0, "seed for random engines", false, 123);
  parser.add<unsigned>("numThreads", 'p', "number of OMP threads", false, 1);
  parser.add<std::string>("method", 'm', "Name of the used integration scheme", false,
    "euler", cmdline::oneof<std::string>("euler", "heun"));
  parser.add<std::string>("outFile", 'f', "Name of the output .json file", false, "");
}
