:: Get assets
if not exist gamedata_origin (
    git clone ./.git ./temp
    cd temp
    git checkout f23bc66ed4a4a6e75d8c9000bf8731d3adc062d7
    cd ..
    move temp/gamedata gamedata_origin
)

:: Generate patch
bin\xrCompress.exe -diff gamedata gamedata_origin -out patch

:: Pack patch
bin\xrCompress.exe patch -ltx datapack.ltx
mkdir patches
move patch.pack_#0 patches\xpatch_11.db
