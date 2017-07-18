// $Id: marc.cc,v 1.11 2005/02/08 20:34:25 othman Exp $

#include "parse_scan.h"

#include <MaRC/config.h>
#include <MaRC/Version.h>

#include <iostream>
//#include <fstream>
#include <list>
#include <cstdio>

#include <unistd.h>




extern void yyrestart (FILE * input_file);

int
main (int argc, char *argv[])
{
//   istream * MaRCInput = 0;
//   ParseParameter * ParseParameterPtr = 0;
//   defaults UserDefs;

  std::cout
    << std::endl <<  "MaRC -- "
    << "Built on " << __DATE__ << " at " << __TIME__ << std::endl
    << "        MaRC Binary  Version " PACKAGE_VERSION  << std::endl
    << "        MaRC Library Version " << MaRC::library_version()
    << std::endl << std::endl
    << "Copyright (C) 1996, 1997, 1998, 2003, 2004, 2017  Ossama Othman"
    << std::endl
    << "All Rights Reserved" << std::endl << std::endl
    << "MaRC comes with ABSOLUTELY NO WARRANTY." << std::endl
    // for details type `show w'.
    << "This is free software, and you are welcome to redistribute it"
    << std::endl
    << "under certain conditions." << std::endl << std::endl;
  // type `show c' for details.


  if (argc < 2)
    {
      std::cerr << std::endl << "USAGE:   marc inputfile1 [inputfile2 ...]"
                << std::endl << std::endl;

      return 1;  // Failure
    }

  try
    {
      MaRC::ParseParameter parse_parameters;

      std::string user_defaults (::getenv ("HOME"));
      user_defaults += "/.marc";

      FILE * const defaults = fopen (user_defaults.c_str (), "r");
      if (defaults != 0)
        {
          ::yyrestart (defaults);

          // Parse user defaults/MaRC initialization file.
          if (::yyparse (parse_parameters) == 0)
            {
              // Successful parse
              std::cout << "MaRC user defaults file parsed." << std::endl;
            }
          else
            {
              fclose (defaults);

              std::cerr << std::endl
                        << "Parse error occurred during user "
                        << "defaults file read."
                        << std::endl;

              return 1;  // Failure
            }
        }

      // Parse MaRC input files.
      for (int i = 1; i < argc; ++i)
        {
          FILE * const map_input = fopen (argv[i], "r");
          if (map_input != 0)
            {
              ::yyrestart (map_input);

              // Parse user defaults/MaRC initialization file.
              if (::yyparse (parse_parameters) == 0)
                {
                  fclose (map_input);

                  // Successful parse
                  std::cout << "MaRC input file "
                            << argv[i]
                            << " parsed." << std::endl;
                }
              else
                {
                  fclose (map_input);

                  std::cerr << std::endl
                            << "Parse error occurred while processing "
                            << "MaRC input file \"" << argv[i]
                            << "\"."
                            << std::endl;

                  return 1;  // Failure
                }
            }
          else
            {
              std::cout << "Unable to open MaRC input file \""
                        << argv[i]
                        << "\"." << std::endl;

              return 1;
            }
        }

      // Create the map(s).

      MaRC::ParseParameter::Commands const & commands =
        parse_parameters.commands ();

      MaRC::ParseParameter::Commands::const_iterator end =
        commands.end ();
      for (MaRC::ParseParameter::Commands::const_iterator p =
             commands.begin ();
           p != end;
           ++p)
        (void) (*p)->execute ();
    }
  catch (const std::exception & e)
    {
      std::cerr << "MaRC: " << e.what () << std::endl;
      return -1;
    }

  return 0;  // Success
}
