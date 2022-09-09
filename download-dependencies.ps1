# Set title of the window
$Host.UI.RawUI.WindowTitle = "IX-Ray"

# Set path to 7-Zip
$path = Join-Path -Path ${env:ProgramFiles} -ChildPath "7-Zip\7z.exe"

# Getting DirectX SDK March 2009
If (!(Test-Path "sdk\dxsdk_mar2009")) {
    Invoke-WebRequest -Uri "https://github.com/ixray-team/ixray-1.5-stcs/releases/download/r0.2/sdk-directxsdk-mar2009.7z" `
                      -OutFile "sdk-directxsdk-mar2009.7z"
    Start-Process -FilePath $path `
                  -ArgumentList "x sdk-directxsdk-mar2009.7z" `
                  -NoNewWindow -Wait
    Remove-Item "sdk-directxsdk-mar2009.7z"
}

# Getting another dependencies from Git
git clone --branch aug2021 --depth 1 https://github.com/microsoft/DirectXTex.git dep/DirectXTex
git clone --branch Release_3.1.0 --depth 1 https://github.com/dockpanelsuite/dockpanelsuite.git dep/dockpanelsuite

# Pause
Write-Host "Press any key to continue..."
$Host.UI.RawUI.ReadKey("NoEcho, IncludeKeyDown") | Out-Null
