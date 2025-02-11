workspace "Pman"
architecture "x64"
configurations
{
	"Debug",
	"Release"
}

project "Pman"
kind "consoleapp"
language "c++"
cppdialect "c++23"
staticruntime "off"

targetdir ("bin/".."%{cfg.buildcfg}")
objdir("bin-int/".."%{cfg.buildcfg}")

files
{
	"Source/*.h",
	"Source/*.cpp"
}

includedirs
{
	"%{wks.location}/Dependencies/Raylib/include"
}

libdirs
{
	"%{wks.location}/Dependencies/Raylib/lib"
}

links
{
	"raylib.lib"
}

filter "system:windows"
systemversion "latest"
links
{
	"winmm.lib"
}
filter "configurations:Debug"
runtime "Debug"
symbols "on"
defines "DEBUG"
filter "configurations:Release"
runtime "Release"
symbols "off"
optimize "on"
