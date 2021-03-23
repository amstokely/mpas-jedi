/*
 * (C) Copyright 2017-2019 UCAR
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 */

#include <oops/runs/ConvertState.h>
#include <oops/runs/Run.h>

#include <saber/oops/instantiateVariableChangeFactory.h>

#include <ufo/instantiateObsFilterFactory.h>

#include "MPASTraits.h"
#include "mpasjedi/instantiateMPASVarChangeFactory.h"

int main(int argc,  char ** argv) {
  oops::Run run(argc, argv);
  mpas::instantiateMPASVarChangeFactory();
  saber::instantiateVariableChangeFactory<mpas::MPASTraits>();
  oops::ConvertState<mpas::MPASTraits> cs;
  return run.execute(cs);
}