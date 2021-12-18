//
// Created by Tyler Gilbert on 12/15/21.
//

#ifndef DESIGNLAB_ASSETMANAGER_HPP
#define DESIGNLAB_ASSETMANAGER_HPP

#include <sys/Cli.hpp>

#include "Manager.hpp"

class AssetManager : public Manager {
public:
  struct Construct {
    API_PMAZ(input_path,Construct,var::PathString,{});
    API_PMAZ(output_path,Construct,var::PathString,{});
    API_PMAZ(project_path,Construct,var::PathString,{});
  };

  explicit AssetManager(const sys::Cli & cli);
  explicit AssetManager(const Construct & cli){
    construct(cli);
  }

  fs::PathList get_source_list(var::StringView project_path, const Settings & settings) override;

private:
  CodePrinter m_code_printer;


  void construct(const Construct & options);
};

#endif // DESIGNLAB_ASSETMANAGER_HPP
