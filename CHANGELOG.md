# Changelog

Full changelog of *IX-Ray* 1.6 project

<!-- markdownlint-disable MD024 -->

## Release 0.3 (September 2022)

### Common

- Migration to __Visual Studio 2022__
- Fixed compilation errors
- Fixed a lot of issues with linking
- Enabled multicore building for all projects
- Enabled __x86-64__ toolchain for all projects
- Enabled __GitHub Actions__

### Core

- Replaced custom `xr_deque<T>` and `xr_vector<T>` with aliases of `std::deque<T>` and `std::vector<T>`
- Placed `clear_and_reserve()` method of `xr_vector<T>` class in a separate function
- Partially replaced STL extension aliases with `using` analogs
- Deleted `DEF_*` and `DEFINE_*` macroses from STL extensions

### Engine

- Fixed __VSync__ on all renders

### Dependencies

- Replaced `Flobbster.Windows.Forms` binary
- Replaced `dockpanelsuite` and bumped to `3.1`
- Deleted unused __Intel VTune__ functionality
- Deleted unused __OpenAutomate__ functionality
- Bumped `TargetFramework` to __.NET Framework 4.7.2__

### Resources

- Added resources
- Normalized encoding of scripts

## Release 0.2 (July 2022)

### Common

- Migration to __Visual Studio 2015__
- Fixed compilation errors
- Replaced deprecated functions to safe and modern analogs
- Replaced some custom functions and types to standard library analog
- Replaced `debug::make_final<T>` class to __C++11__ `final` specifier

### Core

- Removed __BugTrap__ and __minizip__
- Fixed `Debug` configuration workability
- Fixed window focus error

### Engine

- Unlocked console commands: `hud_fov`, `fov`, `jump_to_level`, `g_god`, `g_unlimitedammo`, `run_script`, `run_string`, `time_factor`

### Utilities

- Incompletely integrated __DirectXTex__

## Release 0.1 (March 2021)

### Common

- Migration to __Visual Studio 2013__
- Fixed compilation errors
- Fixed windows displaying in editor projects
- Configured engine and editor projects building
- Configured __OpenAutomate__ project building

<!-- markdownlint-enable MD024 -->
