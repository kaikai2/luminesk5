project "lumines"
	kind "WindowedApp"
	location ("../build/" .. _ACTION)
	targetname "lumines"
	targetdir "../bin"
	targetextension ".exe"
	links {"curvedani", "glport"}
	files
	{
		"../src/**.cpp",
		"../src/**.h",
		"../include/**.h",
	}
	excludes
	{
		"fsm/**.*",
	}
	includedirs
	{
		"../include",
		"../../curvedani/include",
		"../../glport/include",
	}
	libdirs
	{
		"../lib",
		"../../curvedani/lib/" .. _ACTION,
		"../../glport/lib/" .. _ACTION,
		"../../glport/lib",
	}
	flags
	{
		"NoPCH",
		"StaticRuntime",
		"WinMain",
	}
	configuration "Debug"
		defines { "_DEBUG" }
		targetname "lumines_d"
		flags
		{
			"Symbols", 
		}
	configuration "Release"
		defines { "NDEBUG" }
		targetname "lumines"
		flags
		{
			"OptimizeSpeed",
		}
