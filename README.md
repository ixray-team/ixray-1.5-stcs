# IX-Ray

<!-- markdownlint-disable MD033 -->
<div align="center">
  <p>
    <a href="https://github.com/ixray-team">
      <img src="https://github.com/ixray-team/ixray-docs/blob/default/logo.png" alt="IX-Ray Team" width="150" height="150" />
    </a>
  </p>

  <p>
    <a href="https://github.com/ixray-team/ixray-1.5-stcs/releases/tag/r0.1">
      <img src="https://img.shields.io/github/v/release/ixray-team/ixray-1.5-stcs?include_prereleases" alt="Latest release" />
    </a>
  </p>
</div>
<!-- markdownlint-enable MD033 -->

Stable repository of the modernized *X-Ray* 1.5 game engine

## Requirements

* Visual Studio 2015 Community Edition
* Borland C++ Builder 6

## Building

Download the repository:

```console
git clone https://github.com/ixray-team/ixray-1.5-stcs.git
```

Download needed components:

* DirectX SDK March 2009

Unpack a contents to the root agreeing to merge

If need to build editors run `InstallElpack.bat` and `InstallOther.bat` with administrator permissions

### Engine

#### Debug

* Build `XRay.Dependencies.VS2015.sln` on `Debug`
* Build `XRay.Engine.VS2015.sln` on `Debug` except projects from `utils`

#### Mixed or Release

* Build `XRay.Dependencies.VS2015.sln` on `Release`
* Build `XRay.Engine.VS2015.sln` on `Mixed` or `Release` except projects from `utils`

### Editors

* Build `XRay.Dependencies.VS2015.sln` on `Release`
* Build `XRay.Engine.VS2015.sln` on `Mixed` except projects from `engine_game`
* Run `ConvertLibraries.bat`
* Build `XRay.Editors.BCB6.bpg`

## License

See [this](LICENSE.md) file for details
