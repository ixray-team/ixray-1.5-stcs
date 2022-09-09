# IX-Ray

<!-- markdownlint-disable MD033 -->
<div align="center">
  <p>
    <a href="https://github.com/ixray-team">
      <img src="https://github.com/ixray-team/ixray-docs/blob/default/logo.png" alt="IX-Ray Team" width="150" height="150" />
    </a>
  </p>

  <p>
    <a href="https://github.com/ixray-team/ixray-1.5-stcs/releases/tag/r0.2">
      <img src="https://img.shields.io/github/v/release/ixray-team/ixray-1.5-stcs?include_prereleases" alt="Latest release" />
    </a>
  </p>
</div>
<!-- markdownlint-enable MD033 -->

Stable repository of the modernized *X-Ray* 1.5 game engine

## Requirements

- Visual Studio 2022 Community Edition
  - MFC
  - C++/CLI
  - Windows SDK 10.0.19041.0
- Borland C++ Builder 6

## Building

Download the repository:

```console
git clone https://github.com/ixray-team/ixray-1.5-stcs.git
```

Download needed components:

- DirectX SDK March 2009

Unpack a contents to the root agreeing to merge

If need to build editors run `InstallElpack.bat` and `InstallOther.bat` with administrator permissions

### Engine

#### Debug

- Run `download-dependencies.ps1`
- Build `XRay.Engine.sln` on `Win32` without projects from `utils`

There may be `fatal error C1002` at the linking stage of __xrGame__. In this case try to set [`/Zm`](https://docs.microsoft.com/en-us/cpp/build/reference/zm-specify-precompiled-header-memory-allocation-limit) parameter of compiler and check the swap file of __Windows__

### Editors

- Run `contrib/install-elpack.bat` and `contrib/install-other.bat` with administrator permissions
- Build `XRay.Engine.sln` on `Mixed` and `Win32` without projects from `engine_game`
- Run `contrib/convert-libraries.bat`
- Build `XRay.Editors.BCB6.bpg`

## Changelog

All significant changes to this repository are documented in [this](CHANGELOG.md) file

## License

See [this](LICENSE.md) file for details
