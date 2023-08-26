# Changelog

Full changelog of _IX-Ray_ 1.5 project

## Release 0.6 (August 2023)

### Common

- Fixed most part of warnings
- Fixed ignoring temp and metadata files by locator
- Fixed error skipping process troubles
- Added __Visual Studio__ solution filters
- Added __Visual Studio Install__ config
- Enabled NuGet packages getting and caching in __GitHub Actions__
- Set latest __Windows SDK__ version
- Splitted project configuration paths
- Refactored color math module
- Replaced `D3DCOLOR_RGBA`, `D3DCOLOR_ARGB` and `D3DCOLOR_XRGB` macroses
- Replaced legacy __DirectX Math__ with __DirectXMath__ analog
- Replaced `__uuidof` with `IID_PPV_ARGS`
- Replaced `__interface` keyword with `class`
- Replaced `STATIC_CHECK` with `static_assert`
- Replaced `__asm int 3` with `__debugbreak`
- Replaced `GetCLK` function with unified analog
- Replaced `GetTickCount` with `GetTickCount64` function
- Replaced `_snprintf` with `_snprintf_s` function
- Replaced `std::auto_ptr` pointers with `std::unique_ptr`
- Replaced `std::bind1st` and `std::bind2nd` with lambda functions
- Replaced linker directives with project references
- Renamed `xrDebugNew` module to `xrDebug`
- Deleted `get_ref_count()` function
- Deleted __ATI MGPU__ library with related code
- Deleted __NVAPI__ library with related code
- Deleted redundant `DEBUG_INVOKE`
- Deleted `boost::noncopyable` use cases from editor and xrPhysics
- Deleted `std::binary_function` use cases
- Deleted `std::unary_function` use cases

### Packages

- Replaced __Flobbster.Windows.Forms__ with NuGet package
- Replaced __DockPanelSuite__ with NuGet package
- Replaced __DirectX SDK__ with NuGet package and __Windows SDK__
- Replaced __DirectXTex__ with NuGet package
- Replaced __DirectXMesh__ with NuGet package
- Replaced __Xiph__ libraries with NuGet packages
- Replaced __zlib__ with NuGet package

### Engine

- Fixed crash after switching from singleplayer to multiplayer
- Fixed `cam_inert` console command
- Fixed bones synchronization
- Fixed OGG related code of xrSound
- Deleted `xrTheora_Surface_mmx` module
- Deleted __SecuROM__ related code
- Deleted unused modules from xrSound
- Deleted `mailSlot` module
- Deleted launcher related code
- Deleted `no_single` module
- Deleted `dedicated_server_only` module and `PROTECT_API` macros
- Implemented `hud_fov` item parameter
- Implemented `g_info` and `d_info` console commands
- Implemented `g_money` console command
- Implement `g_spawn` and `g_spawn_inv` console command
- Integrated __OpenAL Soft__ with __EFX__ extensions
- Unified xrEngine refactoring with __IX-Ray 1.6__
- Splitted engine and server applications

### Editors

- Fixed floating types converting in weather editor
- Deleted unused resource files from weather editor

### Render

- Fixed wrong attenuation of far sun shadows
- Fixed mipmap loading for textures in __Direct3D 10__
- Fixed some memory leaks
- Added `r2_use_bump` command to disable bumps on R2+ renderers
- Fixed indentation of texture memory message
- Activated `R2FLAG_USE_BUMP` flag by default
- Set `DXGI_ENUM_MODES_INTERLACED` flag
- Deleted unused mipped noise
- Deleted unused `sunfilter` option
- Deleted unused `accum_direct` methods
- Deleted `bug` option in all renders
- Deleted `sjitter` option in all renders
- Deleted `depth16` option in all renders
- Improved depth buffer format handling
- Implemented sub-space rendering with specified view frustum
- Implemented disabling of anisotropic filtering in __Direct3D 10__
- Implemented `mipLodBias` property setter for __Direct3D 10__
- Implemented textures reloading directly in game
- Implemented __FXAA__ support
- Implemented __SMAA__ support
- Implemented shader-based fog on static lightning
- Unlocked __MSAA x8__
- Refactored some render modules

### Gameplay

- Fixed select point crash in patrol path manager
- Fixed offset for answer numbers
- Fixed wrong place of strike stat
- Fixed stuttering after reloading grenade launcher
- Fixed detector animation playing
- Fixed animation playing of item throwing
- Fixed hiding information about item after double click
- Fixed enabled nighvision on upgrading outfit
- Fixed rows and columns of artifact belt
- Fixed reload animation playing on active detector
- Fixed playing idle animation on empty state
- Fixed animation playing on attached grenade launcher
- Fixed switch animation playing
- Fixed crashing in adjust hud mode
- Fixed flickering after game item usage
- Fixed loading ammo in grenade mode
- Fixed flight grenade
- Fixed zero buck cost in trade with Sakharov
- Enabled inertia control from HUD section
- Implemented 100x100 icons support
- Implemented delay before reloading on active detector
- Implemented custom autoreloading
- Implemented custom autoreloading for grenade launcher
- Implemented full and partial weapon reloading
- Implemented misfire of weapon
- Implemented scripted key blocking
- Implemented blocking actor movement
- Backported progress bars for items from X-Ray 1.6
- Backported `fit_in_rect` function from X-Ray 1.6
- Backported `buy_item_condition_factor` function from X-Ray 1.6
- Backported `CUIItemInfo` class from X-Ray 1.6
- Backported `CUIBoosterInfo` class partially from X-Ray 1.6
- Backported properties box behaviour with checks from X-Ray 1.6
- Backported centering cursor fix on `CUIMessageBoxEx` drawing from X-Ray 1.6
- Backported `use_ai_locations` method from X-Ray 1.6
- Backported discounts feature for trade from X-Ray 1.6
- Decomposed `CUIActorMenu` class
- Unified `OnMouse` and `OnKeyboard` methods with X-Ray 1.6
- Refactored some weapons and detectors methods

### Resources

- Fixed big artefact icons on belt
- Fixed talk window on all resolutions
- Fixed community name in talk window
- Fixed text line alignment in talk window
- Fixed availability of traders money
- Fixed displaying `check_autoreload` parameter in gameplay options
- Fixed using quest icons without `_icon` suffix
- Fixed non-tradable items
- Applied `virtual_cells` for slots
- Refactored parallax bump shaders
- Implemented __SMAA__ support in assets
- Implemented __FXAA__ support in assets
- Updated shaders to Shader Model 3.0

## Release 0.5 (March 2023)

### Common

- Enabled assets packing
- Replaced `dxerr` with Windows SDK analog
- Replaced `stricmp` with POSIX analog
- Simplificated `get-dependencies` script launching
- Incompletely integrated __DirectXMesh__

### Engine

- Enabled screenshots capturing in windowed mode on __Direct3D 9__
- Enabled autocompletion for ingame console
- Fixed language layout switching in ingame console
- Fixed damage accounting on playing NPC animations
- Fixed playing animation of getting weapons
- Fixed playing animation of running
- Fixed pistol dropping with detector in left hand
- Fixed bloodsucker solid state
- Replaced crashing with warning when there is no sound

### Render

- Fixed skybox displaying on __Direct3D 10__
- Fixed tree lighting on static render
- Fixed water displaying on static lightning
- Fixed `sload` for correct nearest bumps displaying
- Enabled light accounting for water foam
- Enabled fog accounting for grass on static lightning
- Enabled fog accounting for wallmarks on static lightning
- Enabled fog accounting for particles
- Enabled static sun shadows on disabled cascades
- Enabled `Ldynamic_dir` counting for `accum_direct_volumetric()` on __Direct3D 9__
- Increased gloss for rain
- Implemented actor shadow
- Prevented writing alpha-blended geometry to depth buffer
- Replaced TSM shadows with sun cascades
- Restored grass shadow

### User interface

- Fixed bleeding icon on widescreen resolutions
- Fixed offset of weapon addon icons
- Fixed icon stretching on displaying in center of screen
- Fixed character info for deadbody color
- Fixed progress bar for optional using of `middle_color`
- Fixed scaling in for ammo icon
- Fixed displaying of current bullet resistance
- Fixed crashing on missing task markers
- Fixed displaying of real items cost
- Fixed crashing on dropping item from container
- Fixed crashing when there are more than 9 dialogs
- Fixed map spots
- Fixed texture offset in dialog window
- Fixed upgrade icons on widescreen resolutions
- Enabled changing items condition by Num7 and Num8
- Implemented dialog menu selection by number
- Implemented item highlighters
- Implement `ui_reload` command
- Allowed to change upgrade icon color by config
- Allowed to use min and max colors for double progress bar
- Disabled stats by class and not by section

### Resources

- Exported time change methods to scripts
- Implemented `use_condition` parameter
- Implemented notification restarting for `r2_sun` command
- Disabled condition drawing for binoculars
- Removed temporary icons for quests

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
