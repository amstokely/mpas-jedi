/*
 * (C) Copyright 2017 UCAR
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 */

#include "MPASTraits.h"
#include "RunMPAS.h"
#include "test/base/ModelIncrement.h"

int main(const int argc, const char ** argv) {
  mpas::RunMPAS run(argc, argv);
  test::ModelIncrement<mpas::MPASTraits> tests;
  run.execute(tests);
  return 0;
};
