/*
 * (C) Copyright 2017 UCAR
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 */

#include "model/StateMPAS.h"

#include <algorithm>
#include <string>
#include <vector>

#include "eckit/config/LocalConfiguration.h"
#include "oops/util/Logger.h"
#include "ufo/GeoVaLs.h"
#include "ioda/Locations.h"
#include "ModelBiasMPAS.h"
#include "FieldsMPAS.h"
#include "GeometryMPAS.h"
#include "GetValuesTrajMPAS.h"
#include "IncrementMPAS.h"
#include "ModelMPAS.h"
#include "oops/base/Variables.h"
#include "oops/generic/UnstructuredGrid.h"
#include "oops/util/DateTime.h"
#include "oops/util/Duration.h"

namespace mpas {

// -----------------------------------------------------------------------------
/// Constructor, destructor
// -----------------------------------------------------------------------------
StateMPAS::StateMPAS(const GeometryMPAS & resol, const oops::Variables & vars,
                 const util::DateTime & vt)
  : fields_(new FieldsMPAS(resol, vars, vt)), stash_()
{
  oops::Log::trace() << "StateMPAS::StateMPAS created." << std::endl;
}
// -----------------------------------------------------------------------------
StateMPAS::StateMPAS(const GeometryMPAS & resol, const oops::Variables & vars,
                     const eckit::Configuration & file)
  : fields_(new FieldsMPAS(resol, vars, util::DateTime())), stash_()
{
  if (file.has("analytic_init"))
    fields_->analytic_init(file, resol);
  else
    fields_->read(file);

  ASSERT(fields_);

  oops::Log::trace() << "StateMPAS::StateMPAS created and read in."
                     << std::endl;
}
// -----------------------------------------------------------------------------
StateMPAS::StateMPAS(const GeometryMPAS & resol, const StateMPAS & other)
  : fields_(new FieldsMPAS(*other.fields_, resol)), stash_()
{
  oops::Log::trace() << "StateMPAS::StateMPAS create by interpolation."
                     << std::endl;
  ASSERT(fields_);
  oops::Log::trace() << "StateMPAS::StateMPAS created by interpolation."
                     << std::endl;
}
// -----------------------------------------------------------------------------
StateMPAS::StateMPAS(const StateMPAS & other)
  : fields_(new FieldsMPAS(*other.fields_)), stash_()
{
  oops::Log::trace() << "StateMPAS::StateMPAS before copied." << std::endl;
  ASSERT(fields_);
  oops::Log::trace() << "StateMPAS::StateMPAS copied." << std::endl;
}
// -----------------------------------------------------------------------------
StateMPAS::~StateMPAS() {
  oops::Log::trace() << "StateMPAS::StateMPAS destructed." << std::endl;
}
// -----------------------------------------------------------------------------
/// Basic operators
// -----------------------------------------------------------------------------
StateMPAS & StateMPAS::operator=(const StateMPAS & rhs) {
  ASSERT(fields_);
  *fields_ = *rhs.fields_;
  return *this;
}
// -----------------------------------------------------------------------------
/// Get state values at observation locations
// -----------------------------------------------------------------------------
void StateMPAS::getValues(const ioda::Locations & locs,
                          const oops::Variables & vars,
                          ufo::GeoVaLs & cols) const {
  oops::Log::trace() << "StateMPAS::getValues STANDARD ONE" << std::endl;
  fields_->getValues(locs, vars, cols);
}
// -----------------------------------------------------------------------------
void StateMPAS::getValues(const ioda::Locations & locs,
                          const oops::Variables & vars,
                          ufo::GeoVaLs & cols,
                          const GetValuesTrajMPAS & traj) const {
  oops::Log::trace() << "StateMPAS::getValues PPTRAJ" << std::endl;
  fields_->getValues(locs, vars, cols, traj);
}
// -----------------------------------------------------------------------------
/// Interpolate full fields
// -----------------------------------------------------------------------------
void StateMPAS::changeResolution(const StateMPAS & other) {
  fields_->changeResolution(*other.fields_);
  oops::Log::trace() << "StateMPAS changed resolution" << std::endl;
}
// -----------------------------------------------------------------------------
/// Interactions with Increments
// -----------------------------------------------------------------------------
StateMPAS & StateMPAS::operator+=(const IncrementMPAS & dx) {
  ASSERT(this->validTime() == dx.validTime());
  ASSERT(fields_);
  fields_->add(dx.fields());
  return *this;
}
// -----------------------------------------------------------------------------
/// I/O and diagnostics
// -----------------------------------------------------------------------------
void StateMPAS::read(const eckit::Configuration & files) {
  fields_->read(files);
}
// -----------------------------------------------------------------------------
void StateMPAS::analytic_init(const eckit::Configuration & files,
                                 const GeometryMPAS & resol) {
  fields_->analytic_init(files, resol);
}
// -----------------------------------------------------------------------------
void StateMPAS::write(const eckit::Configuration & files) const {
  fields_->write(files);
}
// -----------------------------------------------------------------------------
void StateMPAS::print(std::ostream & os) const {
  os << std::endl << "  Valid time: " << validTime();
  os << *fields_;
}
// -----------------------------------------------------------------------------
/// For accumulator
// -----------------------------------------------------------------------------
void StateMPAS::zero() {
  fields_->zero();
}
// -----------------------------------------------------------------------------
void StateMPAS::accumul(const double & zz, const StateMPAS & xx) {
  fields_->axpy(zz, *xx.fields_);
}
// -----------------------------------------------------------------------------

}  // namespace mpas
