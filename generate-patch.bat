:: Get assets
if not exist gamedata_origin (
    git clone ./.git ./temp
    cd temp
    git checkout 13cb8f33f0121cadfd2e3be7d0e522d0bdb1fef0
    cd ..
    move temp/gamedata gamedata_origin
)

:: Generate patch
build\bin\Release\xrCompress.exe -diff gamedata gamedata_origin -out patch

:: Pack patch
build\bin\Release\xrCompress.exe patch -ltx datapack.ltx
mkdir patches
move patch.pack_#0 patches\xpatch_11.db
