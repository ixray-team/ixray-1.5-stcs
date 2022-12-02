# Changelog

Full changelog of _IX-Ray_ 1.5 project

## Release 0.4 (December 2022)

### Common

- Added basic editorconfig
- Normalized line endings for the root files

### Engine

- Disabled use cases of `MSAA_ATEST_DX10_1_NATIVE`
- Disabled `r3_minmax_sm` by default
- Disabled `vid_restart` on level loading
- Fixed initial key binding
- Fixed sequence of initialization of API videocards
- Fixed screen resolution selection on missed `user.ltx`
- Fixed engine closing from taskbar menu
- Fixed displaying of transparent surfaces on HUD
- Fixed displaying of shadows
- Fixed sunshafts
- Fixed transparent on static lightning
- Fixed comparison resulting in endless creation of new objects on __Direct3D 10__
- Fixed camera glitches
- Fixed bloodsucker invisibility
- Fixed shadow frame for lamps
- Fixed flickering detail objects
- Fixed damage from hairs zone
- Fixed calculation of path construction
- Fixed index buffer cache resetting
- Fixed `USE_DX10` macros scope in render backend runtime
- Fixed actor satiety state
- Implemented deletion of breaking screenshot
- Improved comparison algorithm of render resources destination
- Enabled teleport using `demo_record`
- Enabled __EAX__
- Enabled freeing instances for renders
- Added support for capturing cube map and location map on __Direct3D 10__
- Added terrain mask support on static lighting
- Added additional `set_weather` console command
- Added additional `read_sun_config` console command
- Added `trees_amplitude` option to weather settings
- Added satiety indicator
- Setted FPS limit on UI rendering
- Backported `mark_msaa_edges` fixes from _IX-Ray_ 1.6
- Unlocked additional `hud_draw` console command

### Resources

- Normalized encoding of shaders
- Fixed _MSAA_ (redefinition of `Texture2DMS`)
- Fixed dynamic wet surfaces
- Fixed displaying of water foam
- Fixed skycube displaying on water surface
- Fixed bullet tracer effects on __Direct3D 10__
- Fixed escape treasure issuing from quest NPC
- Fixed sudden death of quest NPC on escape level
- Fixed some treasures on escape level
- Fixed dialog checks for weapons
- Fixed dialog phrase for freedom fixer
- Fixed freedom exo outfit upgrade
- Added shader to correct displaying of _LODs_ when _MSAA_ alphatest is enabled in classic __Direct3D 10__
- Added treasure dialog for `esc_military_secret_trader`
- Implemented skyblend accumulation for sunshafts
- Implemented fog accounting for water on __Direct3D 9__
- Implemented fog accounting for portals
- Implemented light accounting for water foam
- Implemented satiety indicator
- Enabled `water_soft` shader for `water_studen` and `water_ryaska`

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
